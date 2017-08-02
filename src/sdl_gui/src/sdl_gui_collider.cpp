#include "sdl_gui_collider.hpp"
#include <utility>
#include "sdl_gui_log.hpp"
#include "sdl_gui_element.hpp"
#include "sdl_gui_utils.hpp"

namespace sdl_gui
{

// <f> < GUI_COLLIDER >
/* constructor */
Collider::Collider(const Position& position, GuiTransform* owner_transform): m_local_position{ position },
    m_owner_transform{owner_transform} {}

Collider::~Collider() noexcept {}

Collider::Collider(const Collider& other): m_local_position{other.m_local_position}, m_owner_transform{other.m_owner_transform} {}

Collider::Collider(Collider&& other) noexcept : m_local_position{std::move(other.m_local_position)}, m_owner_transform{std::move(other.m_owner_transform)} {}

Collider& Collider::operator=(const Collider& other)
{
    if(this != &other)//not same ref
    {
        m_local_position = other.m_local_position;
        m_owner_transform = other.m_owner_transform;
    }

    return *this;
}

Collider& Collider::operator=(Collider&& other) noexcept
{
    if(this != &other)//not same ref
    {
        m_local_position = std::move(other.m_local_position);
        m_owner_transform = std::move(other.m_owner_transform);
    }

    return *this;
}

// </f> < GUI_COLLIDER >
}//namespace
