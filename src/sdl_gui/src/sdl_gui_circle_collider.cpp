#include "sdl_gui_circle_collider.hpp"
#include <utility>
#include "sdl_gui_log.hpp"

namespace sdl_gui
{

//<f> Constructors & operator=
CircleCollider::CircleCollider(const Position& position, float radius, GuiTransform* owner_transform) : Collider(position, owner_transform), m_radius{radius},
    m_squared_radius{ radius * radius} {}

CircleCollider::~CircleCollider() noexcept {}

CircleCollider::CircleCollider(const CircleCollider& other) : Collider(other), m_radius{other.m_radius}, m_squared_radius{other.m_squared_radius} {}

CircleCollider::CircleCollider(CircleCollider&& other) noexcept : Collider(std::move(other)), m_radius{std::move(other.m_radius)},
    m_squared_radius{std::move(other.m_squared_radius)} {}

Collider* CircleCollider::Duplicate()
{
    return new CircleCollider(*this);
}

CircleCollider& CircleCollider::operator=(const CircleCollider& other)
{
    if(this != &other)//not same ref
    {
        auto tmp(other);
        *this = std::move(tmp);
    }

    return *this;
}

CircleCollider& CircleCollider::operator=(CircleCollider&& other) noexcept
{
    if(this != &other)//not same ref
    {
        Collider::operator=(std::move(other));
        m_radius = std::move(other.m_radius);
        m_squared_radius = std::move(other.m_squared_radius);
    }
    return *this;
}
//</f>

//<f> Intersection
bool CircleCollider::IsPointColliding(int mouse_x, int mouse_y)
{
    //get final position (parent-child relationship calculated manually)
    Position final_position = m_owner_transform->GlobalPosition();
    final_position += m_local_position;

    int x = (mouse_x - final_position.x);//x length between mouse and circle centre
    int y = (mouse_y -final_position.y);//y length between mouse and circle centre
    return (x * x + y * y) <= m_squared_radius;//check if inside circle
}
//</f>

//<f> Collider Update
void CircleCollider::UpdateColliderSize(const ColliderSizeUnion& size)
{
    m_radius = size.radius;
    m_squared_radius = size.radius;
}

//</f>

//<f> Debug
void CircleCollider::DebugRender(SDL_Renderer* renderer_ptr, const SDL_Colour& colour)
{
    // SDL_SetRenderDrawColor(renderer_ptr, colour.r, colour.g, colour.b, colour.a);
    Log("Circle debug not implemented\n");
}
//</f>

}//namespace
