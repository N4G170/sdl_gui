#include "sdl_gui_font.hpp"
#include "sdl_gui_texture_utils.hpp"
#include <utility>
#include <array>
#include <iostream>
#include <algorithm>
#include "sdl_gui_utf8.hpp"
#include "sdl_gui_constants.hpp"

namespace sdl_gui
{

Font::Font(SDL_Renderer* renderer_ptr, const std::string& font_path, int font_size): m_renderer_ptr{renderer_ptr}, m_source_path{font_path}, m_font_size{font_size},
    m_max_glyph_w{0}, m_line_spacing{5}
{
    m_font_ptr.reset(TTF_OpenFont(m_source_path.data(), font_size));
    m_font_height = TTF_FontHeight(m_font_ptr.get());
    m_font_texture = nullptr;
    CreateFontGlyphSheet();
}

Font::~Font() noexcept
{
    SDL_DestroyTexture(m_font_texture);
}

Font::Font(Font&& other) noexcept
{
    m_renderer_ptr = std::move(other.m_renderer_ptr);
    m_source_path = std::move(other.m_source_path);
    m_font_size = std::move(other.m_font_size);
    m_max_glyph_w = std::move(other.m_max_glyph_w);
    m_font_height = std::move(other.m_font_height);
    m_line_spacing = std::move(other.m_line_spacing);
    m_font_texture = std::move(other.m_font_texture);
}

Font& Font::operator=(Font&& other) noexcept
{
    if(this != &other)
    {
        m_renderer_ptr = std::move(other.m_renderer_ptr);
        m_source_path = std::move(other.m_source_path);
        m_font_size = std::move(other.m_font_size);
        m_max_glyph_w = std::move(other.m_max_glyph_w);
        m_font_height = std::move(other.m_font_height);
        m_line_spacing = std::move(other.m_line_spacing);
        m_font_texture = std::move(other.m_font_texture);
    }

    return *this;
}

// bool Font::CharTexture(const char& character, int font_size, int origin_x, int origin_y, SDL_Colour font_colour, Texture* output_texture)
// {
//     if(output_texture->TextureAccess() != SDL_TEXTUREACCESS_TARGET)
//         return false;
//
//     //char on texture
//     return true;
// }

bool Font::StringTexture(const std::string& text, int origin_x, int origin_y, SDL_Colour font_colour, SDL_Texture* output_texture, int line_length)
{
    //Clear to transparency
    ClearSDLTexture(m_renderer_ptr, output_texture);

    //print text
    int x{0};
    int y{0};
    bool bold{false};//use bold font
    bool italic{false};//use italic font

    //apply any color and alpha modulation to the font
    SDL_SetTextureColorMod(m_font_texture, font_colour.r, font_colour.g, font_colour.b);
    SDL_SetTextureAlphaMod(m_font_texture, font_colour.a);

    std::vector<int> text_codes{ DecodeStringUTF8(text) };//decode to int
    int prev_h{0};//used to perform new line with \n
    int prev_w{0};//used to perform tab with \t

    SDL_SetRenderTarget(m_renderer_ptr, output_texture);
    for(int i{0}; i < text_codes.size(); ++i)
    {
        //<f>check \n & \t
        if(text_codes[i] == c_new_line_code)//new line \n
        {
            x = 0;//reset "caret"
            y += prev_h + m_line_spacing;//new_line
            continue;
        }
        else if(text_codes[i] == c_horizontal_tab_code)// \t
        {
            x += prev_w * 4;//four time the space

            //check for new line
            if(x > line_length)
            {
                x = 0;//reset "caret"
                y += prev_h + m_line_spacing;//new_line
            }
            continue;
        }
        //</f>

        //<f> Check newline because of line line_length
        //break line ?
        if(line_length > 0)//line length was set
        {
            if(text_codes[i] == ' ')//current char is a space ' '
            {
                //check if next word fits
                int word_length{0};
                int char_index{i + 1};
                while(char_index < text_codes.size() && text_codes[char_index] > ' ')//space is the first valid char
                {
                    word_length += m_glyphs_positions[text_codes[char_index]].normal.w;//get norma glyph width
                    ++char_index;
                }

                if(x + word_length > line_length)//new line
                {
                    x = 0;
                    y = y + m_font_height + m_line_spacing;
                }
            }
            else if(x > line_length)//new line
            {
                x = 0;
                y = y + m_font_height + m_line_spacing;
            }
        }
        //</f>

        //<f> Check for tags <b> <i>
        if(CheckTextTag(true, text_codes, i, i+2, 1))//check if open tag
        {
            i+=2;//jump to last tag char
            bold = true;
            continue;//jump to next loop cycle (will move to next valid char)
        }
        else if(CheckTextTag(false, text_codes, i, i+3, 1))//close
        {
            i+=3;//jump to last tag char
            bold = false;
            continue;//jump to next code in the array
        }
        else if(CheckTextTag(true, text_codes, i, i+2, 2))//check if open tag italic
        {
            i+=2;//jump to last tag char
            italic = true;
            continue;//jump to next loop cycle (will move to next valid char)
        }
        else if(CheckTextTag(false, text_codes, i, i+3, 2))//close
        {
            i+=3;//jump to last tag char
            italic = false;
            continue;//jump to next code in the array
        }
        //</f>

        //<f> Get correct glyph and render
        SDL_Rect src{};
        SDL_Rect dst{x, y, 0, 0};

        //get stats from correct font style
        if(italic && bold)
            src = m_glyphs_positions[text_codes[i]].italic_bold;
        else if(bold)
            src = m_glyphs_positions[text_codes[i]].bold;
        else if(italic)
            src = m_glyphs_positions[text_codes[i]].italic;
        else
            src = m_glyphs_positions[text_codes[i]].normal;

        dst.w = src.w;
        dst.h = src.h;

        SDL_RenderCopy(m_renderer_ptr, m_font_texture, &src, &dst);
        x += dst.w;//update x

        //update prev vars
        prev_h = dst.h;
        prev_w = dst.w;
        //</f>

    }

    SDL_SetRenderTarget(m_renderer_ptr, nullptr);
    SDL_SetTextureColorMod(m_font_texture, 255, 255, 255);//reset modulation to white
    SDL_SetTextureAlphaMod(m_font_texture, 255);//opaque

    return true;
}

//private methods
void Font::CreateFontGlyphSheet()
{
    //create characters texture
    const std::string characters{" !\"#$%&'()*+,-./0123456789:;<=>?@ABCDEFGHIJKLMNOPQRSTUVWXYZ[\\]^_`abcdefghijklmnopqrstuvwxyz{|}~"};

    //create glyps
    std::vector<int> decoded_chars = DecodeStringUTF8(characters);
    std::map<int, GlyphSurfaces> glyph_surfaces;

    int texture_width{0};
    int font_height{TTF_FontHeight(m_font_ptr.get())};
    //create glyphs surfaces
    for(int code : decoded_chars)
    {
        //normal
        TTF_SetFontStyle(m_font_ptr.get(), TTF_STYLE_NORMAL);
        RenderGlyph(glyph_surfaces[code].normal, texture_width, code);

        //bold
        TTF_SetFontStyle(m_font_ptr.get(), TTF_STYLE_BOLD);
        RenderGlyph(glyph_surfaces[code].bold, texture_width, code);

        //italic
        TTF_SetFontStyle(m_font_ptr.get(), TTF_STYLE_ITALIC);
        RenderGlyph(glyph_surfaces[code].italic, texture_width, code);

        //italic bold
        TTF_SetFontStyle(m_font_ptr.get(), TTF_STYLE_ITALIC | TTF_STYLE_BOLD);
        RenderGlyph(glyph_surfaces[code].italic_bold, texture_width, code);
    }
    TTF_SetFontStyle(m_font_ptr.get(), TTF_STYLE_NORMAL);//reset

    //create font sprite sheet
    if(m_font_texture)
        SDL_DestroyTexture(m_font_texture);

    m_font_texture = SDL_CreateTexture(m_renderer_ptr, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_TARGET, texture_width, font_height*4+4);//4 styles

    //apply transparency
    ClearSDLTexture(m_renderer_ptr, m_font_texture);

    int current_x{0};//current glyph x printing position

    SDL_SetRenderTarget(m_renderer_ptr, m_font_texture);
    for(auto pair : glyph_surfaces)//TODO::join this loop with the one above
    {
        //TODO: recheck s.w bellow
        int adv=0;//glyph advance
        TTF_GlyphMetrics(m_font_ptr.get(), pair.first, nullptr, nullptr, nullptr, nullptr, &adv);
        SDL_Rect dst{ current_x, 0, adv, font_height };
        SDL_Rect src{ 0, 0, adv, font_height };

        m_max_glyph_w = adv > m_max_glyph_w ? adv : m_max_glyph_w;//update max width of all glyphs

        SDL_Texture* text = SDL_CreateTextureFromSurface(m_renderer_ptr, pair.second.normal);
        // s.w = pair.second.normal->w;
        SDL_RenderCopy(m_renderer_ptr, text, &src, &dst);//normal
        m_glyphs_positions[pair.first].normal = dst;

        text = SDL_CreateTextureFromSurface(m_renderer_ptr, pair.second.bold);
        src.w = pair.second.bold->w;
        dst.y = font_height + 1;
        SDL_RenderCopy(m_renderer_ptr, text, &src, &dst);//bold
        m_glyphs_positions[pair.first].bold = dst;

        text = SDL_CreateTextureFromSurface(m_renderer_ptr, pair.second.italic);
        src.w = pair.second.italic->w;
        dst.y = font_height * 2 + 2;
        SDL_RenderCopy(m_renderer_ptr, text, &src, &dst);//italic
        m_glyphs_positions[pair.first].italic = dst;

        text = SDL_CreateTextureFromSurface(m_renderer_ptr, pair.second.italic_bold);
        src.w = pair.second.italic_bold->w;
        dst.y = font_height * 3 + 3;
        SDL_RenderCopy(m_renderer_ptr, text, &src, &dst);//italic bold
        m_glyphs_positions[pair.first].italic_bold = dst;

        // current_x += s.w;
        current_x += adv;

        //Delete the surfaces, as we do not need them anymore
        pair.second.FreeSurfaces();
    }
    SDL_SetRenderTarget(m_renderer_ptr, nullptr);

    glyph_surfaces.clear();
}

void Font::RenderGlyph(SDL_Surface*& surface, int& w, int& char_unicode)
{
    //i = unicode
    SDL_Surface* glyph_surface = TTF_RenderGlyph_Blended(m_font_ptr.get(), char_unicode, {255,255,255,255});

    int ww{0};
    Uint16 text[] = {static_cast<Uint16>(char_unicode),0};
    TTF_SizeUNICODE(m_font_ptr.get(), text, &ww, nullptr);
    // width += glyph_surface->w;
    w += ww;

    surface = std::move(glyph_surface);
}

void Font::CalculateTextTextureSize(const std::string& text, int* w, int* h, int line_length)
{
    //print text
    int max_w{0};//store max x achieved
    int max_h{0};//store max y achieved
    int line_w{0};//width of current line
    int line_h{m_font_height};//current total height, starts with the height of one line

    bool bold{false};//use bold font
    bool italic{false};//use italic font

    std::vector<int> text_codes{ DecodeStringUTF8(text) };//decode to int
    int prev_glyph_w{0};//used to perform tab with \t (will, probably, always store the same value during the loop)

    //run trough all codes
    for(int i{0}; i < text_codes.size(); ++i)
    {
        //<f> Check \n and \t
        if(text_codes[i] == c_new_line_code)//new line \n
        {
            line_w = 0;//reset width
            line_h += m_font_height + m_line_spacing;//new line
            continue;
        }
        else if(text_codes[i] == c_horizontal_tab_code)// \t
        {
            line_w += prev_glyph_w * 4;//four time the space

            //check for new line
            if(line_w > line_length)
            {
                line_w = 0;//reset width
                line_h += m_font_height + m_line_spacing;//new line
            }
            continue;
        }
        //</f>

        //<f> Check for newline with line_length
        //Check we we need a new line because of width limit reached
        if(line_length > 0)
        {
            if(text_codes[i] == ' ')//' '
            {
                //check if next word fits
                int word_length{0};
                int char_index{i + 1};
                while(char_index < text_codes.size() && text_codes[char_index] > ' ')//space is the first valid char
                {
                    word_length += prev_glyph_w;
                    ++char_index;
                }

                if(line_w + word_length > line_length)//new line
                {
                    line_w = 0;
                    line_h += m_font_height + m_line_spacing;
                }
            }
            else if(line_w > line_length)//new line
            {
                line_w = 0;
                line_h += m_font_height + m_line_spacing;
            }
        }
        //</f>

        //<f> Check for tags <b> <i>
        if(CheckTextTag(true, text_codes, i, i+2, 1))//check if open tag
        {
            i+=2;//jump to last tag char
            bold = true;
            continue;//jump to next loop cycle (will move to next valid char)
        }
        else if(CheckTextTag(false, text_codes, i, i+3, 1))//close
        {
            i+=3;//jump to last tag char
            bold = false;
            continue;//jump to next code in the array
        }
        else if(CheckTextTag(true, text_codes, i, i+2, 2))//check if open tag italic
        {
            i+=2;//jump to last tag char
            italic = true;
            continue;//jump to next loop cycle (will move to next valid char)
        }
        else if(CheckTextTag(false, text_codes, i, i+3, 2))//close
        {
            i+=3;//jump to last tag char
            italic = false;
            continue;//jump to next code in the array
        }
        //</f>

        //<f> Get Correct glyph data & update vars
        //get stats from correct font style
        SDL_Rect glyph_rect{0,0,0,0};
        if(italic && bold)
            glyph_rect = m_glyphs_positions[text_codes[i]].italic_bold;
        else if(bold)
            glyph_rect = m_glyphs_positions[text_codes[i]].bold;
        else if(italic)
            glyph_rect = m_glyphs_positions[text_codes[i]].italic;
        else
            glyph_rect = m_glyphs_positions[text_codes[i]].normal;

        //add current glyph width to line width
        line_w += glyph_rect.w;

        //update prev vars
        prev_glyph_w = glyph_rect.w;

        //update max values
        if(line_w > max_w)
            max_w = line_w;
        if(line_h > max_h)
            max_h = line_h;
        //</f>
    }//for all codes

    //return max values using the pointers
    *w = max_w;
    *h = max_h;
}

bool Font::CheckTextTag(bool is_open_tag, const std::vector<int>& letters, int start_index, int end_index, int type)
{
    /*
    <b></b> - bold
    <i></i> - italic
     */
    const int lt_code{60};//'<'
    const int gt_code{62};//'>'
    const int slash_code{47};//'/'
    int tag_code{ type == 1? 98 : 105};// 98 = b | 105 = i

    if(is_open_tag && start_index + 2 == end_index)
    {
        if(letters[start_index] == lt_code && letters[start_index+1] == tag_code && letters[end_index] == gt_code)
            return true;
    }
    else if(start_index + 3 == end_index)//has enough letters for closing tag
    {
        if(letters[start_index] == lt_code && letters[start_index+1] == slash_code && letters[start_index+2] == tag_code && letters[end_index] == gt_code)
            return true;
    }

    return false;
}

}//namespace
