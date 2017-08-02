#include "sdl_gui_button_transition_none.hpp"
#include <utility>
#include "sdl_gui_constants.hpp"

namespace sdl_gui
{

//<f> Constructors & operator=
ButtonTransitionNone::ButtonTransitionNone(ResourceManager* resource_manager_ptr): m_texture{resource_manager_ptr->GetTexture(c_img_white_dot)} {}

ButtonTransitionNone::~ButtonTransitionNone() noexcept {}

ButtonTransitionNone::ButtonTransitionNone(const ButtonTransitionNone& other) : m_texture{other.m_texture} {}

ButtonTransitionNone::ButtonTransitionNone(ButtonTransitionNone&& other) noexcept : m_texture{std::move(other.m_texture)} {}

ButtonTransitionNone& ButtonTransitionNone::operator=(const ButtonTransitionNone& other)
{
    if(this != &other)//not same ref
    {
        auto tmp(other);
        *this = std::move(tmp);
    }

    return *this;
}

ButtonTransitionNone& ButtonTransitionNone::operator=(ButtonTransitionNone&& other) noexcept
{
    if(this != &other)//not same ref
    {
        m_texture = std::move(other.m_texture);
    }
    return *this;
}
//</f>

//<f> Virtual Methods
void ButtonTransitionNone::Render(const SDL_Rect& dst)
{
    SDL_Rect destination{dst};
    m_texture.Render(nullptr, &destination);
}

void ButtonTransitionNone::ChangeState(ButtonState state) {}
//</f>

}//namespace
