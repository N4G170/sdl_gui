#include "sdl_gui_button.hpp"
#include "sdl_gui_constants.hpp"
#include <utility>
#include <string>
#include "SDL_image.h"
#include "sdl_gui_log.hpp"
#include "sdl_gui_utils.hpp"

namespace sdl_gui
{

Button::Button(GuiMainPointers main_pointers, const Position& position, const Dimensions& size): BaseButton{main_pointers, position, size}
{

}

Button::~Button() noexcept
{
    m_label_ptr.reset();
}

Button::Button(const Button& other): BaseButton{other}
{
    m_label_ptr.reset(new Label(*other.m_label_ptr.get()));
}

Button::Button(Button&& other) noexcept : BaseButton{std::move(other)}
{
    m_label_ptr = std::move(other.m_label_ptr);
}

Button& Button::operator=(const Button& other)
{
    if(this != &other)//not same ref
    {
        Button tmp(other);
        *this = std::move(tmp);
    }

    return *this;
}

Button& Button::operator=(Button&& other) noexcept
{
    if(this != &other)
    {
        BaseButton::operator=(std::move(other));
        m_label_ptr = std::move(other.m_label_ptr);
    }
    return *this;
}

//<f> Overrides GUIElement
// void Button::Input(const SDL_Event &event)
// {
//     BaseButton::Input(event);
// }
//
// void Button::Logic(float fixed_delta_time)
// {
//     BaseButton::Logic(fixed_delta_time);
// }
void Button::Render(float delta_time)
{
    Render(delta_time, m_main_pointers.main_camera_ptr);
}

void Button::Render(float delta_time, Camera* camera)
{
    if(!m_render)
        return;

    BaseButton::Render(delta_time, camera);

    if(m_label_ptr)
    {
        m_label_ptr->Render(delta_time, camera);
        
    // SDL_Rect r{RectFromStructs(m_label_ptr->GlobalPosition(), m_label_ptr->Size())};
    // SDL_RenderDrawRect(m_main_pointers.main_renderer_ptr, &r);
    }
}
//</f>

/* < Methods > */
void Button::CreateLabel(const std::string& text, const std::string& font_path, int font_size, const SDL_Colour& text_colour, Position local_position)
{
    m_label_ptr.reset(new Label(m_main_pointers, local_position, Size()));
    m_label_ptr->Parent(this);
    m_label_ptr->TransformPtr()->LocalPosition(local_position);
    m_label_ptr->ConfigLabel(font_path, font_size, text, text_colour);
}

void Button::CentreLabel()
{
    if(m_label_ptr)
        m_label_ptr->AlignWithParentPoint(sdl_gui::AnchorType::MIDDLE_CENTRE);
}
/* </ Virtual Methods > */

}//namespace
