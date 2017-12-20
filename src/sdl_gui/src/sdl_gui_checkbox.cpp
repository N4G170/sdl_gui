#include "sdl_gui_checkbox.hpp"
#include <utility>
#include "sdl_gui_constants.hpp"
#include "sdl_gui_log.hpp"
#include "sdl_gui_utils.hpp"

namespace sdl_gui
{

CheckBox::CheckBox(GuiMainPointers main_pointers, const Position& position, const Dimensions& size) :
    SelectableButton{main_pointers, position, size}, m_mark_element{nullptr}
{
    m_set_selected_state = false;//disable select transition change (we use the mark on this one)
    // AddGuiCollider({0,0}, Size(), &m_transform);//set by parent

    //the other needed callbacks are set by the parent BaseButton
    m_mouse_interaction.MouseButtonCallback(SDL_BUTTON_LEFT, InputKeyCallbackType::CLICK, std::bind(&CheckBox::MouseClick, this));//override parent
}

CheckBox::~CheckBox() noexcept
{

}

CheckBox::CheckBox(const CheckBox& other) : SelectableButton{other}, m_mark_element{other.m_mark_element}
{
    //the this pointer changes so we set the callback again
    m_mouse_interaction.MouseButtonCallback(SDL_BUTTON_LEFT, InputKeyCallbackType::CLICK, std::bind(&CheckBox::MouseClick, this));//override parent
}

CheckBox::CheckBox(CheckBox&& other) noexcept : SelectableButton{std::move(other)}, m_mark_element{std::move(other.m_mark_element)}
{
    //the this pointer changes so we set the callback again
    m_mouse_interaction.MouseButtonCallback(SDL_BUTTON_LEFT, InputKeyCallbackType::CLICK, std::bind(&CheckBox::MouseClick, this));//override parent
}

CheckBox& CheckBox::operator=(const CheckBox& other)
{
    if(this != &other)//not same ref
    {
        CheckBox tmp(other);
        *this = std::move(tmp);
    }

    return *this;
}

CheckBox& CheckBox::operator=(CheckBox&& other) noexcept
{
    if(this != &other)
    {
        SelectableButton::operator=(std::move(other));
        this->m_mark_element = std::move(other.m_mark_element);

        //the this pointer changes so we set the callback again
        m_mouse_interaction.MouseButtonCallback(SDL_BUTTON_LEFT, InputKeyCallbackType::CLICK, std::bind(&CheckBox::MouseClick, this));//override parent
    }

    return *this;
}

//<f> Overrides GUIElement
void CheckBox::Render(float delta_time)
{
    Render(delta_time, m_main_pointers.main_camera_ptr);
}

void CheckBox::Render(float delta_time, Camera* camera)
{
    if(!m_render)
        return;

    SelectableButton::Render(delta_time, camera);
}
//</f>

void CheckBox::MouseClick()
{
    m_is_selected = !m_is_selected;

    if(m_is_selected && m_mark_element != nullptr)
    {
        m_mark_element->CanRender(true);
        m_mark_element->IsActive(true);
    }
    else if(!m_is_selected && m_mark_element != nullptr)
    {
        m_mark_element->CanRender(false);
        m_mark_element->IsActive(false);
    }

    if(StateChanged)
        StateChanged(this);
}

}//namespace
