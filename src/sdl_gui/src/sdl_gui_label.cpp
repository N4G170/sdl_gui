#include "sdl_gui_label.hpp"
#include "sdl_gui_texture_utils.hpp"
#include <utility>
#include <iostream>
#include "sdl_gui_constants.hpp"
#include "sdl_gui_log.hpp"
#include "sdl_gui_utils.hpp"

namespace sdl_gui
{
//<f> Constructors & operator=
// Label::Label(): IGuiRender{}, m_text_texture{}, m_text{""}, m_text_colour{0,0,0,255}, m_font_ptr{nullptr}
// {
//     m_renderer_ptr = nullptr;
// }
Label::Label(GuiMainPointers main_pointers, const Position& position, const Dimensions& size):GuiElement{main_pointers, position, {0,0}},
    m_text_texture{m_main_pointers.main_renderer_ptr,0,0}, m_text{"label text"}, m_text_colour{0,0,0,255},
    m_font_ptr{m_main_pointers.resource_manager_ptr->GetFont(c_default_font_path, c_default_font_size)}, m_line_length{0}
{
    //Sets the new text and resizes the texture if needed
    Text(m_text, m_text_colour);
}

// Label::Label(GuiMainPointers main_pointers, Font* font_ptr, const std::string& text, const SDL_Colour& text_colour, Position position):
//     GuiElement{main_pointers, position, {0,0}}
// {
//     m_font_ptr = nullptr;
// }

Label::~Label() noexcept
{

}

Label::Label(const Label& other): GuiElement{other}, m_text_texture{other.m_text_texture}, m_text{other.m_text}, m_text_colour{other.m_text_colour},
    m_font_ptr{other.m_font_ptr}, m_line_length{other.m_line_length}
{
}

Label::Label(Label&& other) noexcept: GuiElement{other}, m_text_texture{std::move(other.m_text_texture)}, m_text{std::move(other.m_text)},
    m_text_colour{std::move(other.m_text_colour)}, m_font_ptr{std::move(other.m_font_ptr)}, m_line_length{std::move(other.m_line_length)} {}

Label& Label::operator=(const Label& other)
{
    if(this != &other)//not same ref
    {
        Label tmp(other);
        *this = std::move(tmp);
    }

    return *this;
}

Label& Label::operator=(Label&& other) noexcept
{
    if(this != &other)
    {
        GuiElement::operator=(other);
        this->m_text_texture = std::move(other.m_text_texture);
        this->m_text = std::move(other.m_text);
        this->m_text_colour = std::move(other.m_text_colour);
        this->m_font_ptr = std::move(other.m_font_ptr);
    }
    return *this;
}
//</f>

//<f> Config
void Label::ConfigLabel(const std::string& font_path, int font_size, const std::string& text, const SDL_Colour& text_colour)
{
    m_font_ptr = m_main_pointers.resource_manager_ptr->GetFont(font_path, font_size);
    m_text = text;
    m_text_colour = text_colour;
    Text(text, text_colour);
}

void Label::ConfigLabel(Font* font, const std::string& text, const SDL_Colour& text_colour)
{
    m_font_ptr = font;
    m_text = text;
    m_text_colour = text_colour;
    Text(text, text_colour);
}
//</f>

//<f> Overrides GuiElement
void Label::Render(float delta_time)
{
    Render(delta_time, m_main_pointers.main_camera_ptr);
}

void Label::Render(float delta_time, Camera* camera)
{
    if(!m_render)
        return;

    std::lock_guard<std::mutex> lock(m_text_mutex);

    SDL_Rect dst{RenderRect()};
    SDL_Rect src{dst};
    src.x = src.y = 0;

    // if(m_line_length > 0)
    // {
    //     src.w = m_line_length;
    //     dst.w = m_line_length;
    // }

    //apply camera position
    if(!m_transform.ParentViewport())//if inside viewport we cant add camera position
    {
        dst.x += camera->CameraPosition().x;
        dst.y += camera->CameraPosition().y;
    }

    if(camera->RectInsideCamera(dst))
        m_text_texture.Render(&src, &dst);

    // SDL_RenderDrawRect(m_main_pointers.main_renderer_ptr, &dst);
}
//</f>

//<f> Getters/Setters
void Label::Text(const std::string& text, const SDL_Colour& text_colour)
{
    std::lock_guard<std::mutex> lock(m_text_mutex);

    m_text_colour = text_colour;
    m_text = text;
    int w{0}, h{0};
    m_font_ptr->CalculateTextTextureSize(text, &w, &h, m_line_length);
    Dimensions size{Size()};

    if(w == size.w && h == size.h)//same size
    {
         m_text_texture.Clear();
    }
    else//resize texture
    {
        Size({static_cast<float>(w),static_cast<float>(h)});
        m_text_texture.TexturePtr(CreateSDLTexture(m_main_pointers.main_renderer_ptr, w,h));
    }

    m_font_ptr->StringTexture(text, 0, 0, text_colour, m_text_texture.TexturePtr(), m_line_length);
}

void Label::FontSize(int size)
{
    //can change font
    if(m_font_ptr != nullptr && size != m_font_ptr->FontSize())
    {
        auto font_path{m_font_ptr->FontFilePath()};
        m_font_ptr = m_main_pointers.resource_manager_ptr->GetFont(font_path, size);
        //rebuild label
        Text(m_text, m_text_colour);
    }
}

void Label::LineLength(int line_length)
{
    m_line_length = line_length;
    //rebuild texture
    Text(m_text, m_text_colour);
}

//</f>
}//namespace
