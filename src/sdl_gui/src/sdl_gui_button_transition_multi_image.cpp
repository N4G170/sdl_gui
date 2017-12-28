#include "sdl_gui_button_transition_multi_image.hpp"
#include "sdl_gui_constants.hpp"
#include <utility>

namespace sdl_gui
{

//<f> Constructors & operator=
ButtonTransitionMultiImage::ButtonTransitionMultiImage(ResourceManager* resource_manager_ptr): m_resource_manager_ptr{resource_manager_ptr}, m_state_textures{},
    m_current_texture_ptr{nullptr}
{
    SetStateTexture(c_img_btn_inactive, c_img_btn_active, c_img_btn_over, c_img_btn_pressed);
    ChangeState(sdl_gui::ButtonState::ACTIVE);
}

ButtonTransitionMultiImage::~ButtonTransitionMultiImage() noexcept {}

ButtonTransitionMultiImage::ButtonTransitionMultiImage(const ButtonTransitionMultiImage& other): m_resource_manager_ptr{other.m_resource_manager_ptr},
    m_state_textures{other.m_state_textures}, m_current_texture_ptr{other.m_current_texture_ptr} {}

ButtonTransitionMultiImage::ButtonTransitionMultiImage(ButtonTransitionMultiImage&& other) noexcept:m_resource_manager_ptr{std::move(other.m_resource_manager_ptr)},
    m_state_textures{std::move(other.m_state_textures)}, m_current_texture_ptr{std::move(other.m_current_texture_ptr)} {}

ButtonTransitionMultiImage& ButtonTransitionMultiImage::operator=(const ButtonTransitionMultiImage& other)
{
    if(this != &other)//not same ref
    {
        auto tmp(other);
        *this = std::move(tmp);
    }

    return *this;
}

ButtonTransitionMultiImage& ButtonTransitionMultiImage::operator=(ButtonTransitionMultiImage&& other) noexcept
{
    if(this != &other)//not same ref
    {
        //move here
        m_resource_manager_ptr = std::move(other.m_resource_manager_ptr);
        m_state_textures = std::move(other.m_state_textures);
        m_current_texture_ptr = std::move(other.m_current_texture_ptr);
    }
    return *this;
}
//</f>

//<f> Virtual Methods
void ButtonTransitionMultiImage::Render(const SDL_Rect& dst)
{
    SDL_Rect destination{dst};
    m_current_texture_ptr->Render(nullptr, &destination);
}

void ButtonTransitionMultiImage::ChangeState(ButtonState state)
{
    m_current_texture_ptr = &m_state_textures[state];
}
//</f>

//<f> Getters/Setters
void ButtonTransitionMultiImage::SetStateTexture(ButtonState state, const std::string& image_path)
{
    m_state_textures[state] = m_resource_manager_ptr->GetTexture(image_path);
}

void ButtonTransitionMultiImage::SetStateTexture(const std::string& inactive, const std::string& active, const std::string& over, const std::string& pressed)
{
    SetStateTexture(ButtonState::INACTIVE, inactive);
    SetStateTexture(ButtonState::ACTIVE, active);
    SetStateTexture(ButtonState::OVER, over);
    SetStateTexture(ButtonState::PRESSED, pressed);
}
//</f>

}//namespace
