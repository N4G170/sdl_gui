#include "sdl_gui_vslider.hpp"
#include <utility>
#include <cmath>
#include "sdl_gui_log.hpp"
#include "sdl_gui_constants.hpp"
#include "sdl_gui_utils.hpp"
#include "sdl_gui_label.hpp"

namespace sdl_gui
{

//<f> Constructors & operator=
VSlider::VSlider(GuiMainPointers main_pointers, const Position& position, const Dimensions& size):
    GuiElement{main_pointers, position, size}, m_min_value{0}, m_max_value{100}, m_value{0}, m_head_button{}, m_drag{false}, m_mouse_y{0}, m_mouse_offset_y{0},
    m_value_changed_callback{}
{
    m_head_button.reset(new BaseButton{main_pointers, position, {size.w + 6,  10}});
    m_head_button->Parent(this);
    m_head_button->LocalPosition({ -3, m_head_button->Size().h / 2 });
    // m_head_button->LocalPosition({ -3, m_head_button->Size().h / 2 + size.h});

    m_head_button->TransitionType(sdl_gui::ButtonTransitionType::COLOUR);
    m_head_button->TransitionColourPtr()->SetStateColour(Colour::Cyan, Colour::Grey, Colour::Magenta, Colour::Black);
    m_head_button->MouseInteractionPtr()->MouseButtonCallback(SDL_BUTTON_LEFT, sdl_gui::InputKeyCallbackType::DOWN, std::bind(&VSlider::SetDragFlag, this, true));
    m_head_button->MouseInteractionPtr()->MouseButtonCallback(SDL_BUTTON_LEFT, sdl_gui::InputKeyCallbackType::UP, std::bind(&VSlider::SetDragFlag, this, false));
}

VSlider::~VSlider() noexcept {}

VSlider::VSlider(const VSlider& other): GuiElement{other}, m_min_value{other.m_min_value},
    m_max_value{other.m_max_value}, m_value{other.m_value}, m_head_button{},
    m_drag{other.m_drag}, m_mouse_y{other.m_mouse_y}, m_mouse_offset_y{other.m_mouse_offset_y},
    m_value_changed_callback{other.m_value_changed_callback}
{
    if(other.m_head_button)
        m_head_button.reset(new BaseButton(*other.m_head_button.get()));
}

VSlider::VSlider(VSlider&& other) noexcept: GuiElement{std::move(other)}, m_min_value{std::move(other.m_min_value)},
    m_max_value{std::move(other.m_max_value)}, m_value{std::move(other.m_value)}, m_head_button{std::move(other.m_head_button)},
    m_drag{std::move(other.m_drag)}, m_mouse_y{std::move(other.m_mouse_y)}, m_mouse_offset_y{std::move(other.m_mouse_offset_y)},
    m_value_changed_callback{std::move(other.m_value_changed_callback)}
{

}

VSlider& VSlider::operator=(const VSlider& other)
{
    if(this != &other)//not same ref
    {
        VSlider tmp(other);
        *this = std::move(tmp);
    }

    return *this;
}

VSlider& VSlider::operator=(VSlider&& other) noexcept
{
    if(this != &other)
    {
        GuiElement::operator=(std::move(other));
        m_min_value = std::move(other.m_min_value);
        m_max_value = std::move(other.m_max_value);
        m_value = std::move(other.m_value);
        m_head_button = std::move(other.m_head_button);
        m_value_changed_callback = std::move(other.m_value_changed_callback);
    }
    return *this;
}
//</f>

//<f> Overrides GUIElement
void VSlider::Input(const SDL_Event& event)
{
    if(!m_active)
        return;

    m_head_button->Input(event);

    if(m_drag)
    {
        SDL_GetMouseState(nullptr, &m_mouse_y);

        //QUICK FIX
        if(event.button.type == SDL_MOUSEBUTTONUP &&  event.button.button == SDL_BUTTON_LEFT)
            SetDragFlag(false);
    }
}

void VSlider::Logic(float delta_time)
{
    Position head_position {m_head_button->LocalPosition()};

    //convert mouse_ movement to value change
    if(m_drag)
    {
        float slider_y = GlobalPosition().y;//for readability
        //keep new_x inside slider area
        float new_y{ KeepInInterval(static_cast<float>(m_mouse_y), slider_y, slider_y + Size().h) };
        float new_ratio{ (new_y - slider_y) / Size().h };//convert new_x to a ratio inside the slider
        Value( std::fmin(m_max_value * new_ratio, m_max_value) );//apply ratio to max value to get current value. Called fuction to cheack we value changed
    }
    head_position.y = std::fmin(Size().h * m_value/m_max_value - m_head_button->Size().h/2, Size().h + m_head_button->Size().h/2);

    m_head_button->LocalPosition(head_position);
    m_head_button->Logic(delta_time);
}

void VSlider::Render(float delta_time)
{
    Render(delta_time, m_main_pointers.main_camera_ptr);
}

void VSlider::Render(float delta_time, Camera* camera)
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
        m_head_button->Render(delta_time, camera);

        SDL_SetRenderDrawColor(m_main_pointers.main_renderer_ptr, 255,255,255,255);
        SDL_RenderDrawRect(m_main_pointers.main_renderer_ptr, &dst);
    }

}
//</f>

void VSlider::SetDragFlag(bool drag)
{
    if(drag)
    {
        SDL_GetMouseState(nullptr, &m_mouse_y);
        m_mouse_offset_y = m_head_button->GlobalPosition().y - m_mouse_y;
    }
    m_drag = drag;
}

}//namespace
