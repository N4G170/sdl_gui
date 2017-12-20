#include "sdl_gui_image.hpp"
#include "sdl_gui_log.hpp"
#include "sdl_gui_utils.hpp"
#include "sdl_gui_constants.hpp"
#include <utility>


namespace sdl_gui
{

//<f> Constructors & operator=
Image::Image(GuiMainPointers main_pointers, const Position& position, const Dimensions& size) : GuiElement{main_pointers, position, size},
    m_texture{m_main_pointers.resource_manager_ptr->GetTexture(c_img_white_dot)}{}

Image::~Image() noexcept {}

Image::Image(const Image& other): GuiElement{other}, m_texture{other.m_texture}
{
    if(other.m_src_rect)
        m_src_rect.reset(new SDL_Rect(*other.m_src_rect.get()));
}

Image::Image(Image&& other) noexcept: GuiElement{std::move(other)}, m_texture{std::move(other.m_texture)}, m_src_rect{std::move(other.m_src_rect)}
{}

Image& Image::operator=(const Image& other)
{
    if(this != &other)//not same ref
    {
        Image tmp(other);
        *this = std::move(tmp);
    }

    return *this;
}

Image& Image::operator=(Image&& other) noexcept
{
    if(this != &other)
    {
        GuiElement::operator=(std::move(other));
        m_texture = std::move(other.m_texture);
        m_src_rect = std::move(other.m_src_rect);
    }
    return *this;
}
//</f>

//<f> Overrides GuiElement
void Image::Input(const SDL_Event& event)
{

}

void Image::FixedLogic(float fixed_delta_time)
{

}

void Image::Logic(float delta_time)
{

}

void Image::Render(float delta_time)
{
    Render(delta_time, m_main_pointers.main_camera_ptr);
}

void Image::Render(float delta_time, Camera* camera)
{
    if(!m_render)
        return;

    SDL_Rect dst{RenderRect()};

    //apply camera position
    if(!m_transform.ParentViewport())//if inside viewport we cant add camera position
    {
        dst.x += camera->CameraPosition().x;
        dst.y += camera->CameraPosition().y;
    }

    if(camera->RectInsideCamera(dst))
    {
        m_texture.Render(m_src_rect.get(), &dst);
    }
}
//</f>

//<f> Getters/Setters
void Image::SetSourceRect(const SDL_Rect& src_rect)
{
    m_src_rect.reset(new SDL_Rect(src_rect));
}

void Image::ClearSourceRect()
{
    m_src_rect.reset();
}

void Image::ChangeTexture(const std::string& texture_path)
{
    m_texture = m_main_pointers.resource_manager_ptr->GetTexture(texture_path);;//store new texture
}
//</f>
}//namespace
