#include "sdl_gui_base_button.hpp"
#include <utility>
#include "sdl_gui_constants.hpp"
#include "sdl_gui_log.hpp"
#include <memory>
#include "sdl_gui_utils.hpp"

namespace sdl_gui
{

BaseButton::BaseButton(GuiMainPointers main_pointers, const Position& position, const Dimensions& size) : GuiElement{main_pointers, position, size},
    m_transition_type{ButtonTransitionType::COLOUR}, m_transition_none_ptr{}, m_transition_colour_ptr{}, m_transition_single_image_ptr{}, m_transition_multi_image_ptr{},
    m_current_transition{nullptr}, m_mouse_interaction{}
{
    TransitionType(ButtonTransitionType::COLOUR);//will set m_transition_type & m_current_transition
    ButtonTransitionCallback(ButtonState::ACTIVE);//will set vars with current state

    AddGuiCollider({0,0}, Size(), &m_transform);

    //set callbacks
    m_mouse_interaction.MouseButtonCallback(SDL_BUTTON_LEFT, InputKeyCallbackType::HOLD, std::bind(&BaseButton::ButtonTransitionCallback, this, ButtonState::PRESSED));
    m_mouse_interaction.MouseCallback(MouseCallbackType::OVER, std::bind(&BaseButton::ButtonTransitionCallback, this, ButtonState::OVER));
}

BaseButton::~BaseButton() noexcept
{

}

BaseButton::BaseButton(const BaseButton& other): GuiElement{other}, m_transition_type{other.m_transition_type},
    m_transition_none_ptr{}, m_transition_colour_ptr{}, m_transition_single_image_ptr{}, m_transition_multi_image_ptr{},
    m_current_transition{nullptr}, m_mouse_interaction{other.m_mouse_interaction}
{
    if(other.m_transition_none_ptr)
        m_transition_none_ptr.reset(new ButtonTransitionNone(*other.m_transition_none_ptr.get()));
    if(other.m_transition_colour_ptr)
        m_transition_colour_ptr.reset(new ButtonTransitionColour(*other.m_transition_colour_ptr.get()));
    if(other.m_transition_single_image_ptr)
        m_transition_single_image_ptr.reset(new ButtonTransitionSingleImage(*other.m_transition_single_image_ptr.get()));
    if(other.m_transition_multi_image_ptr)
        m_transition_multi_image_ptr.reset(new ButtonTransitionMultiImage(*other.m_transition_multi_image_ptr.get()));

    SelectTransition(other.m_transition_type);

    m_mouse_interaction.MouseButtonCallback(SDL_BUTTON_LEFT, InputKeyCallbackType::HOLD, std::bind(&BaseButton::ButtonTransitionCallback, this, ButtonState::PRESSED));
    m_mouse_interaction.MouseCallback(MouseCallbackType::OVER, std::bind(&BaseButton::ButtonTransitionCallback, this, ButtonState::OVER));
}

BaseButton::BaseButton(BaseButton&& other) noexcept : GuiElement{std::move(other)}, m_transition_type{std::move(other.m_transition_type)},
    m_transition_none_ptr{std::move(other.m_transition_none_ptr)}, m_transition_colour_ptr{std::move(other.m_transition_colour_ptr)},
    m_transition_single_image_ptr{std::move(other.m_transition_single_image_ptr)}, m_transition_multi_image_ptr{std::move(other.m_transition_multi_image_ptr)},
    m_current_transition{other.m_current_transition}, m_mouse_interaction{std::move(other.m_mouse_interaction)}
{
    m_mouse_interaction.MouseButtonCallback(SDL_BUTTON_LEFT, InputKeyCallbackType::HOLD, std::bind(&BaseButton::ButtonTransitionCallback, this, ButtonState::PRESSED));
    m_mouse_interaction.MouseCallback(MouseCallbackType::OVER, std::bind(&BaseButton::ButtonTransitionCallback, this, ButtonState::OVER));
}

BaseButton& BaseButton::operator=(const BaseButton& other)
{
    if(this != &other)//not same ref
    {
        BaseButton tmp(other);
        *this = std::move(tmp);
    }

    return *this;
}

BaseButton& BaseButton::operator=(BaseButton&& other) noexcept
{
    if(&other != this)
    {
        GuiElement::operator=(std::move(other));
        m_transition_type = std::move(other.m_transition_type);
        m_transition_none_ptr = std::move(other.m_transition_none_ptr);
        m_transition_colour_ptr = std::move(other.m_transition_colour_ptr);
        m_transition_single_image_ptr = std::move(other.m_transition_single_image_ptr);
        m_transition_multi_image_ptr = std::move(other.m_transition_multi_image_ptr);
        m_current_transition = other.m_current_transition;
        m_mouse_interaction = std::move(other.m_mouse_interaction);

        m_mouse_interaction.MouseButtonCallback(SDL_BUTTON_LEFT, InputKeyCallbackType::HOLD, std::bind(&BaseButton::ButtonTransitionCallback, this, ButtonState::PRESSED));
        m_mouse_interaction.MouseCallback(MouseCallbackType::OVER, std::bind(&BaseButton::ButtonTransitionCallback, this, ButtonState::OVER));
    }

    return *this;
}

//<f> Overrides GUIElement
void BaseButton::Input(const SDL_Event& event)
{
    if( !m_active )
    return;

    m_mouse_interaction.Input(event, m_collider.get());
}

void BaseButton::ClearInput()
{
    m_mouse_interaction.ClearInput();
}

void BaseButton::Logic(float delta_time)
{
    if(m_active)//btn is active
    {
        ButtonTransitionCallback(ButtonState::ACTIVE);

        m_mouse_interaction.Logic(delta_time);
    }
    else
    {
        ButtonTransitionCallback(ButtonState::INACTIVE);
    }
}

void BaseButton::Render(float delta_time)
{
    Render(delta_time, m_main_pointers.main_camera_ptr);
}

void BaseButton::Render(float delta_time, Camera* camera)
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
        m_current_transition->Render(dst);
    }
}
//</f>

//<f> Getters/Setters
void BaseButton::TransitionType(ButtonTransitionType type)
{
    DeleteTransition(m_transition_type);
    CreateTransition(type);
    SelectTransition(type);
    m_transition_type = type;
}
//</f>

void BaseButton::ButtonTransitionCallback(ButtonState state)
{
    m_current_transition->ChangeState(state);
}

void BaseButton::DeleteTransition(ButtonTransitionType type)
{
    switch(type)
    {
        case ButtonTransitionType::NONE: m_transition_none_ptr.reset(); return;
        case ButtonTransitionType::COLOUR: m_transition_colour_ptr.reset(); return;
        case ButtonTransitionType::SINGLE_IMAGE: m_transition_single_image_ptr.reset(); return;
        case ButtonTransitionType::MULTI_IMAGE: m_transition_multi_image_ptr.reset(); return;
    }
}

void BaseButton::CreateTransition(ButtonTransitionType type)
{
    switch(type)
    {
        case ButtonTransitionType::NONE: m_transition_none_ptr.reset(new ButtonTransitionNone(m_main_pointers.resource_manager_ptr)); return;
        case ButtonTransitionType::COLOUR: m_transition_colour_ptr.reset(new ButtonTransitionColour(m_main_pointers.resource_manager_ptr)); return;
        case ButtonTransitionType::SINGLE_IMAGE: m_transition_single_image_ptr.reset(new ButtonTransitionSingleImage(m_main_pointers.resource_manager_ptr)); return;
        case ButtonTransitionType::MULTI_IMAGE: m_transition_multi_image_ptr.reset(new ButtonTransitionMultiImage(m_main_pointers.resource_manager_ptr)); return;
    }
}


void BaseButton::SelectTransition(ButtonTransitionType type)
{
    switch(type)
    {
        case ButtonTransitionType::NONE: m_current_transition = m_transition_none_ptr.get(); return;
        case ButtonTransitionType::COLOUR: m_current_transition = m_transition_colour_ptr.get(); return;
        case ButtonTransitionType::SINGLE_IMAGE: m_current_transition = m_transition_single_image_ptr.get(); return;
        case ButtonTransitionType::MULTI_IMAGE: m_current_transition = m_transition_multi_image_ptr.get(); return;
    }
}

}//namespace
