#include "sdl_gui_button_transition_single_image.hpp"
#include <utility>
#include "sdl_gui_constants.hpp"

namespace sdl_gui
{

//<f> Constructors & operator=
ButtonTransitionSingleImage::ButtonTransitionSingleImage(ResourceManager* resource_manager_ptr) : m_resource_manager_ptr{resource_manager_ptr},
    m_texture{resource_manager_ptr->GetTexture(c_button_base)}, m_state_rects{}, m_current_src_rect_ptr{nullptr}
{
    SetStateTextureSrcRect({0,0,1,1}, {0,1,1,1}, {0,2,1,1}, {0,3,1,1});
}

ButtonTransitionSingleImage::~ButtonTransitionSingleImage() noexcept
{

}

ButtonTransitionSingleImage::ButtonTransitionSingleImage(const ButtonTransitionSingleImage& other): m_resource_manager_ptr{other.m_resource_manager_ptr},
    m_texture{other.m_texture}, m_state_rects{},  m_current_src_rect_ptr{other.m_current_src_rect_ptr}
{
    for(auto& rect : other.m_state_rects)
    {
        if(rect.second)
            m_state_rects.emplace(rect.first, new SDL_Rect{*rect.second.get()});//duplicate pointer content
        else
            m_state_rects.emplace(rect.first, nullptr);
    }
}

ButtonTransitionSingleImage::ButtonTransitionSingleImage(ButtonTransitionSingleImage&& other) noexcept: m_resource_manager_ptr{std::move(other.m_resource_manager_ptr)},
    m_texture{std::move(other.m_texture)}, m_state_rects{std::move(other.m_state_rects)}, m_current_src_rect_ptr{std::move(other.m_current_src_rect_ptr)} {}

ButtonTransitionSingleImage& ButtonTransitionSingleImage::operator=(const ButtonTransitionSingleImage& other)
{
    if(this != &other)//not same ref
    {
        auto tmp(other);
        *this = std::move(tmp);
    }

    return *this;
}

ButtonTransitionSingleImage& ButtonTransitionSingleImage::operator=(ButtonTransitionSingleImage&& other) noexcept
{
    if(this != &other)//not same ref
    {
        m_resource_manager_ptr = std::move(other.m_resource_manager_ptr);
        m_texture = std::move(other.m_texture);
        m_state_rects = std::move(other.m_state_rects);
        m_current_src_rect_ptr = std::move(other.m_current_src_rect_ptr);
    }
    return *this;
}
//</f>

//<f> Virtual Methods
void ButtonTransitionSingleImage::Render(const SDL_Rect& dst)
{
    SDL_Rect destination{dst};
    m_texture.Render(m_current_src_rect_ptr, &destination);
}

void ButtonTransitionSingleImage::ChangeState(ButtonState state)
{
    m_current_src_rect_ptr = m_state_rects[state].get();
}
//</f>

//<f> Getters/Setters
void ButtonTransitionSingleImage::ChangeTexture(const std::string& image_path)
{
    m_texture = m_resource_manager_ptr->GetTexture(image_path);
}

void ButtonTransitionSingleImage::SetStateTextureSrcRect(ButtonState state, const SDL_Rect& rect)
{
    m_state_rects[state].reset(new SDL_Rect{rect});
}

void ButtonTransitionSingleImage::SetStateTextureSrcRect(const SDL_Rect& inactive, const SDL_Rect& active, const SDL_Rect& over, const SDL_Rect& pressed)
{
    SetStateTextureSrcRect(ButtonState::INACTIVE, inactive);
    SetStateTextureSrcRect(ButtonState::ACTIVE, active);
    SetStateTextureSrcRect(ButtonState::OVER, over);
    SetStateTextureSrcRect(ButtonState::PRESSED, pressed);
}

void ButtonTransitionSingleImage::ClearStateTextureSrcRect(ButtonState state)
{
    m_state_rects[state].reset();
}

void ButtonTransitionSingleImage::ClearStateTextureSrcRect()
{
    for(auto& rect : m_state_rects)
        rect.second.reset();
}
//</f>

}//namespace
