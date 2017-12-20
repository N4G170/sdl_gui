#include "sdl_gui_transform.hpp"
#include <utility>
#include <algorithm>
#include "sdl_gui_log.hpp"
#include "sdl_gui_element.hpp"
#include <iostream>
namespace sdl_gui
{
//<f> Constructors & operator=
GuiTransform::GuiTransform(GuiElement* owner): m_owner{owner}, m_local_position{0,0}, m_parent_viewport{nullptr} {}

GuiTransform::~GuiTransform() noexcept {}

GuiTransform::GuiTransform(const GuiTransform& other): m_owner{other.m_owner}, m_local_position{other.m_local_position},
    m_parent_viewport{other.m_parent_viewport} {}

GuiTransform::GuiTransform(GuiTransform&& other) noexcept: m_owner{std::move(other.m_owner)}, m_local_position{std::move(other.m_local_position)},
    m_parent_viewport{std::move(other.m_parent_viewport)} {other.m_owner = nullptr; other.m_parent_viewport = nullptr;}

GuiTransform& GuiTransform::operator=(const GuiTransform& other)
{
    if(this != &other)//not same ref
    {
        GuiTransform tmp(other);
        *this = std::move(tmp);
    }

    return *this;
}

GuiTransform& GuiTransform::operator=(GuiTransform&& other) noexcept
{
    if(this != &other)
    {
        m_owner = std::move(other.m_owner);
        m_local_position = std::move(other.m_local_position);
        m_parent_viewport = std::move(other.m_parent_viewport);
        other.m_owner = nullptr;
        other.m_parent_viewport = nullptr;
    }
    return *this;
}
//</f>

//<f> Getters/Setters
Position GuiTransform::GlobalPositionIgnoreViewport() const
{
    if(m_owner->Parent() != nullptr)
    {
        return m_owner->Parent()->GlobalPositionIgnoreViewport() + m_local_position;
    }
    else
        return m_local_position;
}

Position GuiTransform::GlobalPosition() const
{
    if(m_owner->Parent() != nullptr)
    {
        if(m_parent_viewport != nullptr)
        {
            //We need to remove the position of the viewport because the viewport "sets" a new origin
            return {m_owner->Parent()->GlobalPosition().x + m_local_position.x - m_parent_viewport->GlobalPosition().x,
                    m_owner->Parent()->GlobalPosition().y + m_local_position.y - m_parent_viewport->GlobalPosition().y};
        }
        else
            return m_owner->Parent()->GlobalPosition() + m_local_position;
    }
    else
        return m_local_position;
}

void GuiTransform::GlobalPosition(const Position& new_global_position)
{
    if(m_owner->Parent() != nullptr)//ajust because of parent
    {
        m_local_position = new_global_position;
        m_local_position -=  m_owner->Parent()->GlobalPosition();//ajust to parent position
    }
    else
    {
        m_local_position = new_global_position;
    }
}

void GuiTransform::LocalPosition(const Position& new_local_position)
{
    if(m_local_position != new_local_position)
    {
        m_local_position = new_local_position;
    }
}

void GuiTransform::ParentViewport(sdl_gui::GuiTransform *parent_viewport)
{
    m_parent_viewport = parent_viewport;

    for(auto& pair : *m_owner->Children())
        pair.second->ParentViewport(parent_viewport);
}

// void GuiTransform::Parent(GuiTransform* parent)
// {
//     if(parent != nullptr)//new parent
//     {
//         if(m_parent != nullptr)//we swap parents
//         {
//             //remove US as a child link
//             m_parent->RemoveChild(m_owner->ElementUID());
//             //set new parent
//             m_parent = parent;
//             //set new child link
//             m_parent->AddChild(m_owner->ElementUID(), this);
//         }
//         else//we add new parent
//         {
//             m_parent = parent;//set new parent
//             //set new child link
//             m_parent->AddChild(m_owner->ElementUID(), this);
//         }
//     }
//     else if(m_parent != nullptr)//we remove current parent
//     {
//         //remove previous child link
//         m_parent->RemoveChild(m_owner->ElementUID());
//         //set new parent
//         m_parent = parent;
//     }
//     //else we do nothing (means that m_parent == parent == nullptr)
// }

//</f>


// void GuiTransform::AddChild(UID child_uid, GuiTransform* child)
// {
//     m_children[child_uid] = child;
// }
//
// void GuiTransform::RemoveChild(UID child_uid)
// {
//     //remove child with same id
//     // m_children.erase( std::find_if(begin(m_children), end(m_children), [child_uid](GuiTransform* child)->bool {return child->m_uid == child_uid; } ) );
//     m_children.erase(child_uid);
// }

}//namespace
