#include "sdl_gui_selectable_button.hpp"
#include <utility>
#include "sdl_gui_constants.hpp"
#include "sdl_gui_log.hpp"

namespace sdl_gui
{

SelectableButton::SelectableButton(GuiMainPointers main_pointers, const Position& position, const Dimensions& size) :
    BaseButton{main_pointers, position, size}, StateChanged{}, m_is_selected{false}, m_value{0}, m_set_selected_state{true}
{
    // AddGuiCollider({0,0}, Size(), &m_transform);//set by parent

    //the other needed callbacks are set by the parent BaseButton
    m_mouse_interaction.MouseButtonCallback(SDL_BUTTON_LEFT, InputKeyCallbackType::CLICK, std::bind(&SelectableButton::MouseClick, this));
}

SelectableButton::~SelectableButton() noexcept
{

}

SelectableButton::SelectableButton(const SelectableButton& other) : BaseButton{other}, StateChanged{},
    m_is_selected{other.m_is_selected}, m_value{other.m_value}, m_set_selected_state{other.m_set_selected_state}
{
    //the this pointer changes so we set the callback again
    m_mouse_interaction.MouseButtonCallback(SDL_BUTTON_LEFT, InputKeyCallbackType::CLICK, std::bind(&SelectableButton::MouseClick, this));
}

SelectableButton::SelectableButton(SelectableButton&& other) noexcept : BaseButton{std::move(other)}, StateChanged{}, m_is_selected{std::move(other.m_is_selected)},
    m_value{std::move(other.m_value)}, m_set_selected_state{std::move(other.m_set_selected_state)}
{
    //the this pointer changes so we set the callback again
    m_mouse_interaction.MouseButtonCallback(SDL_BUTTON_LEFT, InputKeyCallbackType::CLICK, std::bind(&SelectableButton::MouseClick, this));
}

SelectableButton& SelectableButton::operator=(const SelectableButton& other)
{
    if(this != &other)//not same ref
    {
        SelectableButton tmp(other);
        *this = std::move(tmp);
    }

    return *this;
}

SelectableButton& SelectableButton::operator=(SelectableButton&& other) noexcept
{
    if(this != &other)
    {
        this->m_is_selected = std::move(other.m_is_selected);
        this->m_value = std::move(other.m_value);
        this->m_set_selected_state = std::move(other.m_set_selected_state);

        //the this pointer changes so we set the callback again
        m_mouse_interaction.MouseButtonCallback(SDL_BUTTON_LEFT, InputKeyCallbackType::CLICK, std::bind(&SelectableButton::MouseClick, this));
    }

    return *this;
}

//<f> Overrides GUIElement

void SelectableButton::Render(float delta_time)
{
    Render(delta_time, m_main_pointers.main_camera_ptr);
}

void SelectableButton::Render(float delta_time, Camera* camera)
{
    if(!m_render)
        return;

    if(m_is_selected && m_set_selected_state)
        m_current_transition->ChangeState(ButtonState::SELECTED);

    BaseButton::Render(delta_time, camera);

}
//</f>


void SelectableButton::MouseClick()
{
    m_is_selected = !m_is_selected;

    if(StateChanged)
        StateChanged(this);
}

}//namespace
