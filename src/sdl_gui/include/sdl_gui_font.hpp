#include <map>
#include "SDL.h"
#include "SDL_ttf.h"
#include <vector>
#include "sdl_gui_deleters.hpp"
#include <memory>

namespace sdl_gui
{

#ifndef SDL_GUI_FONT_HPP
#define SDL_GUI_FONT_HPP

struct GlyphSurfaces
{
    SDL_Surface* normal;
    SDL_Surface* bold;
    SDL_Surface* italic;
    SDL_Surface* italic_bold;

    void FreeSurfaces()
    {
        SDL_FreeSurface(normal);
        SDL_FreeSurface(bold);
        SDL_FreeSurface(italic);
        SDL_FreeSurface(italic_bold);
    }
};

struct GlyphPositions
{
    SDL_Rect normal;
    SDL_Rect bold;
    SDL_Rect italic;
    SDL_Rect italic_bold;
};

class Font
{
    public:
        //<f> Constructors & operator=
        /* Default constructor */
        Font(SDL_Renderer* renderer_ptr, const std::string& font_path, int font_size = 12);
        /* Default destructor */
        virtual ~Font() noexcept;

        /* Copy constructor */
        Font(const Font& other) = delete;
        /* Move constructor */
        Font(Font&& other) noexcept;

        /* Copy operator */
        Font& operator= (const Font& other) = delete;
        /* Move operator */
        Font& operator= (Font&& other) noexcept;
        //</f>

        //<f> Getters/Setters
        int LineSpacing() const { return m_line_spacing; }
        void LineSpacing(int line_spacing) { m_line_spacing = line_spacing; }
        int FontSize() const { return m_font_size; }
        std::string FontFilePath() const { return m_source_path; }
        //</f>

        // bool CharTexture(const char& character, int font_size, int origin_x, int origin_y, SDL_Colour font_colour, Texture* output_texture);

        /**
         * \brief Renders the text onto the output_texture starting at position origin_x and origin_y.
         * \param font_colour Note: the alpha value can be used
         * \param line_length    0 means no limit. if line_length < letter width, 1 char per line
         */
        bool StringTexture(const std::string& text, int origin_x, int origin_y, SDL_Colour font_colour, SDL_Texture* output_texture, int line_length = 0);

        /**
         * \brief Returns a pointer to internal TTF_font*. NOTE: DO NOT DELETE IT, you are not the owner
         */
        SDL_Texture* FontPtr(){return m_font_texture;};

        /**
         * Uses the font to calculate an apropriate size for the texture to hold the given string
         * @param text        String to be checked
         * @param w           Var to store the calculated width
         * @param h           Var to store the calculated height
         * @param line_length Max lenght of a line of the string (the width will be close to this value). If 0, line_max is not used
         */
        void CalculateTextTextureSize(const std::string& text, int* w, int* h, int line_length = 0);

    private:
        // vars and stuff
        SDL_Renderer* m_renderer_ptr;
        std::string m_source_path;
        std::unique_ptr<TTF_Font, SDLDeleters> m_font_ptr;
        SDL_Texture* m_font_texture;
        std::map<int, GlyphPositions> m_glyphs_positions;

        //font stats
        int m_font_size;
        int m_max_glyph_w;
        int m_font_height;
        /**
         * Space between two lines
         */
        int m_line_spacing;

        void CreateFontGlyphSheet();
        void RenderGlyph(SDL_Surface*& surface, int& w, int& i);

        //TODO: improve function
        /*
        DOES NOT CHECK IF IS IN RANGE
         */
        bool CheckTextTag(bool is_open_tag, const std::vector<int>& letters, int start_index, int end_index, int type);
};

#endif //SDL_GUI_FONT_HPP

}//namespace
