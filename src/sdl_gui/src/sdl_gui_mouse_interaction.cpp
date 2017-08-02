#include "sdl_gui_mouse_interaction.hpp"
#include <utility>
#include "sdl_gui_log.hpp"
#include "sdl_gui_utils.hpp"

namespace sdl_gui
{

MouseInteraction::MouseInteraction() : m_mouse_flags{ToInnerType(MouseStateFlags::NONE)}, m_fixed_valid_click_timer{0}, m_valid_click_timer{0},
    m_click_time_limit{0.25}
{

}

MouseInteraction::~MouseInteraction() noexcept
{

}

MouseInteraction::MouseInteraction(const MouseInteraction& other): m_mouse_flags{other.m_mouse_flags},
    m_fixed_valid_click_timer{other.m_fixed_valid_click_timer}, m_valid_click_timer{other.m_valid_click_timer},
    m_click_time_limit{other.m_click_time_limit}, m_mouse_callbacks{other.m_mouse_callbacks}, m_mouse_buttons{other.m_mouse_buttons}
{

}

MouseInteraction::MouseInteraction(MouseInteraction&& other) noexcept : m_mouse_flags{std::move(other.m_mouse_flags)},
    m_fixed_valid_click_timer{std::move(other.m_fixed_valid_click_timer)}, m_valid_click_timer{std::move(other.m_valid_click_timer)},
    m_click_time_limit{std::move(other.m_click_time_limit)}, m_mouse_callbacks{std::move(other.m_mouse_callbacks)},
    m_mouse_buttons{std::move(other.m_mouse_buttons)}
{

}

MouseInteraction& MouseInteraction::operator=(const MouseInteraction& other)
{
    if(this != &other)
    {
        MouseInteraction tmp{other};
        *this = std::move(tmp);
    }
    return *this;
}

MouseInteraction& MouseInteraction::operator=(MouseInteraction&& other) noexcept
{
    if(this != &other)
    {
        m_mouse_flags = std::move(other.m_mouse_flags);
        m_fixed_valid_click_timer = std::move(other.m_fixed_valid_click_timer);
        m_valid_click_timer = std::move(other.m_valid_click_timer);
        m_click_time_limit = std::move(other.m_click_time_limit);
        m_mouse_callbacks = std::move(other.m_mouse_callbacks);
        m_mouse_buttons = std::move(other.m_mouse_buttons);
    }
    return *this;
}

void MouseInteraction::Input(const SDL_Event& event, Collider* collider)
{
    bool inside_collider{ MouseInsideCollider(event.motion.x, event.motion.y, collider) };

    if(inside_collider)//mouse inside detection area, we can do some stuff
    {
        //mouse entered detection area
        if(!(m_mouse_flags & ToInnerType(MouseStateFlags::OVER)) && inside_collider)//we enter the button
            m_mouse_flags |= ToInnerType(MouseStateFlags::ENTER);//add
        //over
        m_mouse_flags |= ToInnerType(MouseStateFlags::OVER);//add

        //moved
        if(event.type == SDL_MOUSEMOTION)
            m_mouse_flags |= ToInnerType(MouseStateFlags::MOVE);//add
        else
            m_mouse_flags &= ~ToInnerType(MouseStateFlags::MOVE);//remove

        //Mouse buttons input
        auto mouse_button_event = event.button;//SDL_MouseButtonEvent
        for(auto& btn_pair : m_mouse_buttons)
        {
            auto& btn{btn_pair.second};
            if(mouse_button_event.button == btn.id)
            {
                if(mouse_button_event.type == SDL_MOUSEBUTTONDOWN)
                {
                    btn.state_flags |= ToInnerType(InputKeyStateFlags::DOWN);//add down
                    btn.state_flags |= ToInnerType(InputKeyStateFlags::HOLD);//add hold
                }
                else if(mouse_button_event.type == SDL_MOUSEBUTTONUP)
                {
                    btn.state_flags |= ToInnerType(InputKeyStateFlags::UP);//add up
                    btn.state_flags &= ~ToInnerType(InputKeyStateFlags::HOLD);//remove hold only
                }
            }
        }
    }
    else//outside detection area
    {
        //Flag clear is made after processing exit flag at MouseLogic function
        //m_mouse_flags = ToInnerType(MouseStateFlags::NONE);//clear flags

        if(IsFlagActive(m_mouse_flags, MouseStateFlags::OVER))//we exit the button
        {
            m_mouse_flags |= ToInnerType(MouseStateFlags::EXIT);//add
            m_mouse_flags &= ~ToInnerType(MouseStateFlags::OVER);//add
        }

        //clear all buttons
        for(auto& btn : m_mouse_buttons)
        {
            btn.second.state_flags = ToInnerType(InputKeyStateFlags::NONE);//clear flags
        }
    }
}

void MouseInteraction::FixedLogic(float fixed_delta_time)
{
    MouseLogic(fixed_delta_time, m_fixed_valid_click_timer);
}

void MouseInteraction::Logic(float delta_time)
{
    MouseLogic(delta_time, m_valid_click_timer);
}

//<f> Callbacks
/**
 * \brief Get a pointer to the callback for the given mouse button action type.
 * Possible actions \see InputKeyCallbackType
 */
std::function<void()>* MouseInteraction::MouseButtonCallback(Uint8 mouse_button, InputKeyCallbackType type)
{
    switch (type)
    {
        case InputKeyCallbackType::DOWN: return &m_mouse_buttons[mouse_button].OnDown; break;
        case InputKeyCallbackType::HOLD: return &m_mouse_buttons[mouse_button].OnHold; break;
        case InputKeyCallbackType::UP: return &m_mouse_buttons[mouse_button].OnUp; break;
        case InputKeyCallbackType::CLICK: return &m_mouse_buttons[mouse_button].OnClick; break;
        default: return nullptr;
    }
}
/**
 * \brief Sets the callback fot the given mouse button action type
 * Possible actions \see InputKeyCallbackType
 */
void MouseInteraction::MouseButtonCallback(Uint8 mouse_button, InputKeyCallbackType type, const std::function<void()>& callback)
{
    //register ID
    m_mouse_buttons[mouse_button].id = mouse_button;
    switch (type)
    {
        case InputKeyCallbackType::DOWN: m_mouse_buttons[mouse_button].OnDown = callback; break;
        case InputKeyCallbackType::HOLD: m_mouse_buttons[mouse_button].OnHold = callback; break;
        case InputKeyCallbackType::UP: m_mouse_buttons[mouse_button].OnUp = callback; break;
        case InputKeyCallbackType::CLICK: m_mouse_buttons[mouse_button].OnClick = callback; break;
    }
}
//</f>

//<f> Private Methods
void MouseInteraction::MouseLogic(float delta_time, float& valid_click_timer)
{
    //Process mouse flags
    //update click timer
    valid_click_timer += delta_time;

    //mouse enter
    if(IsFlagActive(m_mouse_flags, MouseStateFlags::ENTER))
    {
        if(m_mouse_callbacks[MouseCallbackType::ENTER])//calback exists
            m_mouse_callbacks[MouseCallbackType::ENTER]();

        //remove flag
        m_mouse_flags &= ~ToInnerType(MouseStateFlags::ENTER);
    }
    //mouse over
    if(IsFlagActive(m_mouse_flags, MouseStateFlags::OVER))
    {
        if(m_mouse_callbacks[MouseCallbackType::OVER])//calback exists
            m_mouse_callbacks[MouseCallbackType::OVER]();
    }
    //mouse move
    if(IsFlagActive(m_mouse_flags, MouseStateFlags::MOVE))
    {
        if(m_mouse_callbacks[MouseCallbackType::MOVE])//calback exists
            m_mouse_callbacks[MouseCallbackType::MOVE]();
        m_mouse_flags &= ~ToInnerType(MouseStateFlags::MOVE);//remove
    }
    //mouse exit
    if(IsFlagActive(m_mouse_flags, MouseStateFlags::EXIT))
    {
        if(m_mouse_callbacks[MouseCallbackType::EXIT])//calback exists
            m_mouse_callbacks[MouseCallbackType::EXIT]();

        //remove flag
        m_mouse_flags = ToInnerType(MouseStateFlags::NONE);//clear flags
    }

    //process buttons flags
    for(auto& btn_pair : m_mouse_buttons)
    {
        auto& btn = btn_pair.second;
        //btn down
        if(IsFlagActive(btn.state_flags, InputKeyStateFlags::DOWN))
        {
            //remove flag
            btn.state_flags &= ~ToInnerType(InputKeyStateFlags::DOWN);
            //reset timer
            valid_click_timer = 0;
            if(btn.OnDown)//calback exists
                btn.OnDown();
        }
        //btn hold
        if(IsFlagActive(btn.state_flags, InputKeyStateFlags::HOLD))
        {
            if(btn.OnHold)//calback exists
                btn.OnHold();
        }
        //btn up
        if(IsFlagActive(btn.state_flags, InputKeyStateFlags::UP))
        {
            //remove flag
            btn.state_flags &= ~ToInnerType(InputKeyStateFlags::UP);

            if(btn.OnUp)//calback exists
                btn.OnUp();
            // Log("BTN: "+std::to_string(btn.state_flags)+"\n");
            //check for valid click
            if(valid_click_timer <= m_click_time_limit)//valic click
            {
                valid_click_timer = m_click_time_limit + 1;
                if(btn.OnClick)
                    btn.OnClick();
            }
        }
    }
}

bool MouseInteraction::MouseInsideCollider(int mouse_x, int mouse_y, Collider* collider)
{
    if(collider != nullptr && collider->IsPointColliding(mouse_x, mouse_y))
        return true;
    return false;
}
//</f>

//<f> Other Methods
void MouseInteraction::ClearInput()
{
    for(auto& btn_pair : m_mouse_buttons)
        btn_pair.second.state_flags = 0;
    m_mouse_flags = 0;
}
//</f>
}//namespace
