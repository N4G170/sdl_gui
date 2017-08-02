#include "sdl_gui_box_collider.hpp"
#include <utility>
#include "sdl_gui_utils.hpp"
#include "sdl_gui_element.hpp"
#include "sdl_gui_log.hpp"

namespace sdl_gui
{

//<f> Constructors & operator=
BoxCollider::BoxCollider(const Position& position, const Dimensions& size, GuiTransform* owner_transform) : Collider(position, owner_transform), m_size{size} {}

BoxCollider::~BoxCollider() noexcept {}

BoxCollider::BoxCollider(const BoxCollider& other) : Collider(other), m_size{other.m_size} {}

BoxCollider::BoxCollider(BoxCollider&& other) noexcept : Collider(std::move(other)), m_size{std::move(other.m_size)} {}

Collider* BoxCollider::Duplicate()
{
    return new BoxCollider(*this);
}

BoxCollider& BoxCollider::operator=(const BoxCollider& other)
{
    if(this != &other)//not same ref
    {
        auto tmp(other);
        *this = std::move(tmp);
    }

    return *this;
}

BoxCollider& BoxCollider::operator=(BoxCollider&& other) noexcept
{
    if(this != &other)//not same ref
    {
        Collider::operator=(std::move(other));
        m_size = std::move(other.m_size);
        //move here
    }
    return *this;
}
//</f>

//<f> Intersection
bool BoxCollider::IsPointColliding(int mouse_x, int mouse_y)
{
    // //get camera
    // auto camera_position {m_owner_transform->Owner()->GuiCamera()->CameraPosition()};
    // //correct mouse position
    // mouse_x -= camera_position.x;
    // mouse_y -= camera_position.y;

    Position final_position = m_owner_transform->GlobalPositionIgnoreViewport();
    final_position.x += m_local_position.x;
    final_position.y += m_local_position.y;

    return (mouse_x >= final_position.x && mouse_x < (m_size.w + final_position.x)) && (mouse_y >= final_position.y && mouse_y < (m_size.h + final_position.y));
}
//</f>

//<f> Collider Update
void BoxCollider::UpdateColliderSize(const ColliderSizeUnion& size)
{
    m_size = size.size;
}
//</f>

//<f> Debug
void BoxCollider::DebugRender(SDL_Renderer* renderer_ptr, const SDL_Colour& colour)
{
    SDL_SetRenderDrawColor(renderer_ptr, colour.r, colour.g, colour.b, colour.a);
    SDL_Rect rect = RectFromStructs(m_local_position + m_owner_transform->GlobalPosition(), m_size);
    SDL_RenderDrawRect(renderer_ptr, &rect);
}
//</f>

}//namespace
