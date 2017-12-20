#include "sdl_gui_vscrollbar.hpp"
#include <utility>
#include <cmath>
#include "sdl_gui_log.hpp"
#include "sdl_gui_constants.hpp"
#include "sdl_gui_utils.hpp"
#include "sdl_gui_label.hpp"

namespace sdl_gui
{

//<f> Constructors & operator=
VScrollbar::VScrollbar(GuiMainPointers main_pointers, const Position& position, const Dimensions& size):
    GuiElement{main_pointers, position, size}, m_min_value{0}, m_max_value{100}, m_head_button{}, m_head_ratio{1},
    m_drag{false}, m_mouse_y{0}, m_mouse_offset_y{0}, m_previous_head_y{0},
    m_value_changed_callback{}
{
    m_head_button.reset(new BaseButton{main_pointers, position, {size.w,  10}});
    m_head_button->Parent(this);
    m_head_button->LocalPosition({ 0, 0 });

    m_head_button->TransitionType(sdl_gui::ButtonTransitionType::COLOUR);
    m_head_button->TransitionColourPtr()->SetStateColour(Colour::Cyan, Colour::Grey, Colour::Magenta, Colour::Black);
    m_head_button->MouseInteractionPtr()->MouseButtonCallback(SDL_BUTTON_LEFT, sdl_gui::InputKeyCallbackType::DOWN, std::bind(&VScrollbar::SetDragFlag, this, true));
    m_head_button->MouseInteractionPtr()->MouseButtonCallback(SDL_BUTTON_LEFT, sdl_gui::InputKeyCallbackType::UP, std::bind(&VScrollbar::SetDragFlag, this, false));

    m_head_ratio = 0.3;
    UpdateHeadSize();
}

VScrollbar::~VScrollbar() noexcept {}

VScrollbar::VScrollbar(const VScrollbar& other): GuiElement{other}, m_min_value{other.m_min_value},
    m_max_value{other.m_max_value}, m_head_button{}, m_head_ratio{other.m_head_ratio},
    m_drag{other.m_drag}, m_mouse_y{other.m_mouse_y}, m_mouse_offset_y{other.m_mouse_offset_y}, m_previous_head_y{other.m_previous_head_y},
    m_value_changed_callback{other.m_value_changed_callback}
{
    if(other.m_head_button)
        m_head_button.reset(new BaseButton(*other.m_head_button.get()));
}

VScrollbar::VScrollbar(VScrollbar&& other) noexcept: GuiElement{std::move(other)}, m_min_value{std::move(other.m_min_value)},
    m_max_value{std::move(other.m_max_value)}, m_head_button{std::move(other.m_head_button)}, m_head_ratio{std::move(other.m_head_ratio)},
    m_drag{std::move(other.m_drag)}, m_mouse_y{std::move(other.m_mouse_y)},
    m_mouse_offset_y{std::move(other.m_mouse_offset_y)}, m_previous_head_y{std::move(other.m_previous_head_y)},
    m_value_changed_callback{std::move(other.m_value_changed_callback)}
{

}

VScrollbar& VScrollbar::operator=(const VScrollbar& other)
{
    if(this != &other)//not same ref
    {
        VScrollbar tmp(other);
        *this = std::move(tmp);
    }

    return *this;
}

VScrollbar& VScrollbar::operator=(VScrollbar&& other) noexcept
{
    if(this != &other)
    {
        GuiElement::operator=(std::move(other));
        m_min_value = std::move(other.m_min_value);
        m_max_value = std::move(other.m_max_value);
        m_head_button = std::move(other.m_head_button);
        m_head_ratio = std::move(other.m_head_ratio);
        m_drag = std::move(other.m_drag);
        m_mouse_y = std::move(other.m_mouse_y);
        m_mouse_offset_y = std::move(other.m_mouse_offset_y);
        m_previous_head_y = std::move(other.m_previous_head_y);
        m_value_changed_callback = std::move(other.m_value_changed_callback);
    }
    return *this;
}
//</f>

//<f> Overrides GUIElement
void VScrollbar::Input(const SDL_Event& event)
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

void VScrollbar::Logic(float delta_time)
{
    Position head_position {m_head_button->LocalPosition()};

    if(m_drag)
    {
        if(std::abs(head_position.y - m_previous_head_y) >= 0.001f )//moved
        {
            if(m_value_changed_callback)
            {
                // m_value_changed_callback(5);
                m_value_changed_callback(Value());
            }
        }

        m_previous_head_y = head_position.y;
        head_position.y = std::fmin(std::fmax(0, m_mouse_y - m_mouse_offset_y), Size().h - m_head_button->Size().h);
    }
    //move head
    // head_position.y = std::fmin(std::fmax(0, m_mouse_y - m_mouse_offset_y), Size().h - m_head_button->Size().h);

    m_head_button->LocalPosition(head_position);
    m_head_button->Logic(delta_time);
}

void VScrollbar::Render(float delta_time)
{
    Render(delta_time, m_main_pointers.main_camera_ptr);
}

void VScrollbar::Render(float delta_time, Camera* camera)
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

//<f> Getters/Setters
float VScrollbar::Value() const
{
    //value depends on positions
    auto head_position_y = m_head_button->GlobalPosition().y;
    auto min_y = GlobalPosition().y;
    auto max_y = min_y + Size().h - m_head_button->Size().h;

    float position_ratio = (head_position_y - min_y) / (max_y - min_y);

    return MapValueToRange(position_ratio, m_min_value, m_max_value);
}
/**
 * Set the current value for the ProgressBar. If the value > max or value < min, value will be set to the right limit
 */
void VScrollbar::Value(float new_value)
{
    if(new_value != Value())
    {
        //cap value
        new_value = KeepInInterval(new_value, m_min_value, m_max_value);

        //move head to position
        auto min_y = GlobalPosition().y;
        auto max_y = min_y + Size().h - m_head_button->Size().h;

        float new_y = MapValueToRange(new_value, m_min_value, m_max_value, min_y, max_y);

        m_head_button->GlobalPosition({m_head_button->GlobalPosition().x, new_y});
    }
}
//update value if we change the range
void VScrollbar::MaxValue(float max_value)
{
    if(m_max_value != max_value)
    {
        m_max_value = std::fmax(m_min_value, max_value);
    }
}

//update value if we change the range
void VScrollbar::MinValue(float min_value)
{
    if(m_min_value != min_value)
    {
        m_min_value = std::fmin(m_max_value, min_value);
    }
}
//</f>

void VScrollbar::SetDragFlag(bool drag)
{
    if(drag)
    {
        SDL_GetMouseState(nullptr, &m_mouse_y);
        m_previous_head_y = m_head_button->LocalPosition().y;
        m_mouse_offset_y = m_mouse_y - m_previous_head_y;
    }
    else
    {
        m_mouse_offset_y = 0;
    }
    m_drag = drag;
}

void VScrollbar::UpdateHeadSize()
{
    m_head_button->Size( {m_head_button->Size().w, Size().h * m_head_ratio} );
    m_head_button->GetColliderPtr()->UpdateColliderSize({m_head_button->Size()});
    // dynamic_cast<BoxCollider*>( m_head_button->GetCollider(0) )-> ->>UpdateColliderShape(0, {0,0}, m_head_button->Size());
}

}//namespace
