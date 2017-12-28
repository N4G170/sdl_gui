#include "sdl_gui_menu_item.hpp"
#include <utility>
#include "sdl_gui_constants.hpp"
#include "sdl_gui_log.hpp"

namespace sdl_gui
{

//<f> Constructors & operator=
MenuItem::MenuItem(GuiMainPointers main_pointers, const Position& position, const Dimensions& size): GuiElement(main_pointers, position, size),
    m_bg_image{main_pointers, position, size}, m_label{main_pointers, position, size}, m_is_selected{false}, m_state_colours{}
{
    AddGuiCollider({0,0}, size, this->TransformPtr());

    m_bg_image.Parent(this);
    m_bg_image.LocalPosition({0,0});
    m_label.Parent(this);
    m_label.LocalPosition({0,0});

    m_state_colours.insert({ButtonState::ACTIVE, c_active_colour});
    m_state_colours.insert({ButtonState::OVER, c_over_colour});
    m_state_colours.insert({ButtonState::PRESSED, c_pressed_colour});
    m_state_colours.insert({ButtonState::SELECTED, c_selected_colour});
    m_state_colours.insert({ButtonState::INACTIVE, c_inactive_colour});

    m_mouse_interaction.MouseButtonCallback(SDL_BUTTON_LEFT, InputKeyCallbackType::DOWN, std::bind(&MenuItem::ChangeStateColour, this, ButtonState::PRESSED));
    m_mouse_interaction.MouseButtonCallback(SDL_BUTTON_LEFT, InputKeyCallbackType::CLICK, std::bind(&MenuItem::ChangeSelected, this));
    m_mouse_interaction.MouseCallback(MouseCallbackType::OVER, std::bind(&MenuItem::ChangeStateColour, this, ButtonState::OVER));
}

MenuItem::~MenuItem() noexcept
{

}

MenuItem::MenuItem(const MenuItem& other): GuiElement{other}, m_bg_image{other.m_bg_image}, m_label{other.m_label},
    m_mouse_interaction{other.m_mouse_interaction}, m_is_selected{other.m_is_selected}, m_state_colours{other.m_state_colours}
{

}

MenuItem::MenuItem(MenuItem&& other) noexcept: GuiElement{std::move(other)}, m_bg_image{std::move(other.m_bg_image)}, m_label{std::move(other.m_label)},
    m_mouse_interaction{std::move(other.m_mouse_interaction)}, m_is_selected{std::move(other.m_is_selected)}, m_state_colours{std::move(other.m_state_colours)}
{

}

MenuItem& MenuItem::operator=(const MenuItem& other)
{
    if(this != &other)//not same ref
    {
        auto tmp(other);
        *this = std::move(tmp);
    }

    return *this;
}

MenuItem& MenuItem::operator=(MenuItem&& other) noexcept
{
    if(this != &other)//not same ref
    {
        //move here
        GuiElement::operator=(std::move(other));
        m_bg_image = std::move(other.m_bg_image);
        m_label = std::move(other.m_label);
        m_mouse_interaction = std::move(other.m_mouse_interaction);
        m_is_selected = std::move(other.m_is_selected);
        m_state_colours = std::move(other.m_state_colours);
    }
    return *this;
}
//</f>

//<f> Overrides GUIElement
void MenuItem::Input(const SDL_Event& event)
{
    if( !m_active )
        return;

    // m_mouse_interaction.Input(event, m_collider.get());
}

void MenuItem::ClearInput()
{
    m_mouse_interaction.ClearInput();
}

void MenuItem::Logic(float delta_time)
{
    if(m_active)//btn is active
    {
        ChangeStateColour(ButtonState::ACTIVE);

        m_mouse_interaction.Logic(delta_time);

        if(m_is_selected)
            ChangeStateColour(ButtonState::SELECTED);
    }
    else
    {
        ChangeStateColour(ButtonState::INACTIVE);
    }
}

void MenuItem::Render(float delta_time)
{
    Render(delta_time, m_main_pointers.main_camera_ptr);
}

void MenuItem::Render(float delta_time, Camera* camera)
{
    if(!m_render)
        return;

    SDL_Rect dst{RenderRect()};
    // m_collider->DebugRender(m_main_pointers.main_renderer_ptr, {12,255,0,255});
    //apply camera position
    if(!m_transform.ParentViewport())//if inside viewport we cant add camera position
    {
        dst.x += camera->CameraPosition().x;
        dst.y += camera->CameraPosition().y;
    }

    if(camera->RectInsideCamera(dst))
    {
        m_bg_image.Render(delta_time, camera);
        m_label.Render(delta_time, camera);
    }
}
//</f>

//<f> Getters/Setters
void MenuItem::Text(const std::string& text)
{
    m_label.Text(text);
    //ajust item height
    Size({Size().w, m_label.Size().h});
    m_bg_image.Size({Size().w, m_label.Size().h});

    //resize Collider
    if(m_collider)
        m_collider->UpdateColliderSize({Size()});
}
//</f>

void MenuItem::ChangeStateColour(ButtonState state)
{
    switch(state)
    {
        case ButtonState::ACTIVE: m_bg_image.ColourModulation(m_state_colours[ButtonState::ACTIVE]); break;
        case ButtonState::OVER: m_bg_image.ColourModulation(m_state_colours[ButtonState::OVER]); break;
        case ButtonState::PRESSED: m_bg_image.ColourModulation(m_state_colours[ButtonState::PRESSED]); break;
        case ButtonState::SELECTED: m_bg_image.ColourModulation(m_state_colours[ButtonState::SELECTED]); break;
        case ButtonState::INACTIVE: m_bg_image.ColourModulation(m_state_colours[ButtonState::INACTIVE]); break;
    }
}

}//namespace
