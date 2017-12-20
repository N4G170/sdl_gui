#include "sdl_gui_element.hpp"
#include <utility>
#include "sdl_gui_utils.hpp"
#include "sdl_gui_box_collider.hpp"
#include "sdl_gui_circle_collider.hpp"
#include "sdl_gui_log.hpp"

namespace sdl_gui
{
//<f> Constructors & operator=
GuiElement::GuiElement(GuiMainPointers main_pointers, const Position& position, const Dimensions& size): m_enabled{true}, m_uid{GenerateUID()}, m_transform{this},
    m_anchor{AnchorType::TOP_LEFT}, m_size{size}, m_scale{1,1}, m_main_pointers{main_pointers},
    m_render{true}, m_render_index{0}, m_collider{nullptr}, m_active{true}, m_focused{false}, m_parent{nullptr}
{
    m_transform.LocalPosition(position);
}

GuiElement::~GuiElement() noexcept {}

GuiElement::GuiElement(const GuiElement& other) : m_enabled{other.m_enabled}, m_uid{GenerateUID()}, m_transform{other.m_transform}, m_anchor{other.m_anchor},
    m_size{other.m_size}, m_scale{other.m_scale}, m_main_pointers{other.m_main_pointers},
    m_render{other.m_render}, m_render_index{other.m_render_index}, m_active{other.m_active}, m_focused{other.m_focused},
    m_parent{other.m_parent}, m_children{other.m_children}
{
    if(other.m_collider)
        m_collider.reset(other.m_collider.get()->Duplicate());
    else
        m_collider.reset();

    m_transform.Owner(this);
}

GuiElement::GuiElement(GuiElement&& other) noexcept : m_enabled{std::move(other.m_enabled)}, m_uid{std::move(other.m_uid)}, m_transform{std::move(other.m_transform)},
    m_anchor{std::move(other.m_anchor)}, m_size{std::move(other.m_size)}, m_scale{std::move(other.m_scale)},
    m_main_pointers{std::move(other.m_main_pointers)},
    m_render{std::move(other.m_render)}, m_render_index{std::move(other.m_render_index)}, m_collider{std::move(other.m_collider)},
    m_active{std::move(other.m_active)}, m_focused{std::move(other.m_focused)},
    m_parent{std::move(other.m_parent)}, m_children{std::move(other.m_children)} { m_transform.Owner(this); }

GuiElement& GuiElement::operator= (const GuiElement& other)
{
    if(this != &other)
    {
        GuiElement tmp{other};//copy, will generate new uid
        *this = std::move(tmp);
    }
    return *this;
}

GuiElement& GuiElement::operator= (GuiElement&& other)
{
    if(this != &other)
    {
        this->m_enabled = std::move(other.m_enabled);
        this->m_uid = std::move(other.m_uid);
        this->m_transform = std::move(other.m_transform);
        this->m_anchor = std::move(other.m_anchor);
        this->m_size = std::move(other.m_size);
        this->m_scale = std::move(other.m_scale);
        this->m_main_pointers = std::move(other.m_main_pointers);
        this->m_render = std::move(other.m_render);
        this->m_render_index = std::move(other.m_render_index);
        this->m_collider = std::move(other.m_collider);
        this->m_active = std::move(other.m_active);
        this->m_focused = std::move(other.m_focused);
        this->m_parent = std::move(other.m_parent);
        this->m_children =std::move(other.m_children);

        m_transform.Owner(this);
    }
    return *this;
}
//</f>

//<f> Virtual Funtions
void GuiElement::Input(const SDL_Event& event) {}

void GuiElement::ClearInput() {}

void GuiElement::FixedLogic(float fixed_delta_time) {}

void GuiElement::Logic(float delta_time) {}

void GuiElement::Render(float delta_time) { Render(delta_time, m_main_pointers.main_camera_ptr); }

void GuiElement::Render(float delta_time, Camera* camera) {}
//</f>

//<f> Render
SDL_Rect GuiElement::RenderRect()
{
    Position position{GlobalPosition()};
    Dimensions size{ScaledSize()};

    switch(m_anchor)
    {
        case AnchorType::TOP_LEFT : break;//nothing to do

        case AnchorType::TOP_CENTRE :
            position.x -= size.w / 2;
        break;

        case AnchorType::TOP_RIGHT :
            position.x -= size.w;
        break;

        case AnchorType::MIDDLE_LEFT :
            position.y -= size.h / 2;
        break;

        case AnchorType::MIDDLE_CENTRE :
            position.x -= size.w / 2;
            position.y -= size.h / 2;
        break;

        case AnchorType::MIDDLE_RIGHT :
            position.x -= size.w;
            position.y -= size.h / 2;
        break;

        case AnchorType::BOTTOM_LEFT :
            position.y -= size.h;
        break;

        case AnchorType::BOTTOM_CENTRE :
            position.x -= size.w / 2;
            position.y -= size.h;
        break;

        case AnchorType::BOTTOM_RIGHT :
            position.x -= size.w;
            position.y -= size.h;
        break;
    }

    return RectFromFloats(position.x, position.y, size.w, size.h);
}
//</f>

//<f> Position, Size & Transform interface
void GuiElement::AlignWithParentPoint(AnchorType point_type, Position offset)
{
    if(m_parent != nullptr)
    {
        auto parent_size{m_parent->Size()};
        Position local_point{0,0};

        switch(point_type)
        {
            case AnchorType::TOP_LEFT :
                local_point = {0, 0};
                this->LocalPosition({local_point.x + offset.x, local_point.y + offset.y});
            break;

            case AnchorType::TOP_CENTRE :
                local_point = {parent_size.w / 2, 0};
                this->LocalPosition({local_point.x - Size().w / 2 + offset.x, local_point.y + offset.y});
            break;

            case AnchorType::TOP_RIGHT :
                local_point = {parent_size.w, 0};
                this->LocalPosition({local_point.x - Size().w + offset.x, local_point.y + offset.y});
            break;

            case AnchorType::MIDDLE_LEFT :
                local_point = {0, parent_size.h / 2};
                this->LocalPosition({local_point.x + offset.x, local_point.y - Size().h / 2 + offset.y});
            break;

            case AnchorType::MIDDLE_CENTRE :
                local_point = {parent_size.w / 2, parent_size.h / 2};
                this->LocalPosition({local_point.x - Size().w / 2 + offset.x, local_point.y - Size().h / 2 + offset.y});
            break;

            case AnchorType::MIDDLE_RIGHT :
                local_point = {parent_size.w, parent_size.h / 2};
                this->LocalPosition({local_point.x - Size().w + offset.x, local_point.y - Size().h / 2 + offset.y});
            break;

            case AnchorType::BOTTOM_LEFT :
                local_point = {0, parent_size.h};
                this->LocalPosition({local_point.x + offset.x, local_point.y - Size().h + offset.y});
            break;

            case AnchorType::BOTTOM_CENTRE :
                local_point = {parent_size.w / 2, parent_size.h};
                this->LocalPosition({local_point.x - Size().w / 2 + offset.x, local_point.y - Size().h + offset.y});
            break;

            case AnchorType::BOTTOM_RIGHT :
                local_point = {parent_size.w, parent_size.h};
                this->LocalPosition({local_point.x - Size().w + offset.x, local_point.y - Size().h + offset.y});
            break;
        }
    }
}

SDL_Rect GuiElement::Bounds()
{
    if(m_children.empty())//our bounds = our size and position
        return RenderRect();

    auto min_x {static_cast<int>(GlobalPosition().x)};
    auto max_x {min_x + static_cast<int>(Size().w)};
    auto min_y {static_cast<int>(GlobalPosition().y)};
    auto max_y {min_y + static_cast<int>(Size().h)};

    for(auto& child : m_children)
    {
        auto bounds{child.second->Bounds()};
        // LogLine(ToString(bounds));
        if(bounds.x < min_x)
            min_x = bounds.x;
        if(bounds.x + bounds.w > max_x)
            max_x = bounds.x + bounds.w;
        if(bounds.y < min_y)
            min_y = bounds.y;
        if(bounds.y + bounds.h > max_y)
            max_y = bounds.y + bounds.h;

    }
    // LogLine(std::to_string(max_x)+" - "+std::to_string(max_y));

    //max_x - min_x = w
    //max_y - min_y = h
    return {min_x, min_y, max_x - min_x, max_y - min_y};
}

//<f> Scale
Scale GuiElement::GlobalScale()
{
    if(m_parent == nullptr)
        return m_scale;
    return m_parent->GlobalScale() * m_scale;
};
//</f>
//</f>

//<f> Interaction & Colliders
/* Add Box collider */
void GuiElement::AddGuiCollider(const Position& local_position, const Dimensions& size, GuiTransform* owner_transform)
{
    m_collider.reset(new BoxCollider{local_position, size, owner_transform});
    // m_colliders.emplace_back(std::unique_ptr<Collider>(new BoxCollider{local_position, size, owner_transform}));
}
/* Add Circle collider */
void GuiElement::AddGuiCollider(const Position& local_position, float circle_radius, GuiTransform* owner_transform)
{
    m_collider.reset(new CircleCollider{local_position, circle_radius, owner_transform});
    // m_colliders.emplace_back(std::unique_ptr<Collider>(new CircleCollider{local_position, circle_radius, owner_transform}));
}
//</f>

//<f> Parent & Children
/**
 * Sets this element parent. Will, also, set the parent in the transform
 */
void GuiElement::Parent(GuiElement* parent)
{
    //update transform
    // m_transform.Parent(parent->TransformPtr());

    if(parent != nullptr)//new parent
    {
        if(m_parent != nullptr)//we swap parents
        {
            //remove US as a child link
            m_parent->RemoveChild(ElementUID());
            //set new parent
            m_parent = parent;
            //set new child link
            m_parent->AddChild(this);
        }
        else//we add new parent
        {
            m_parent = parent;//set new parent
            //set new child link
            m_parent->AddChild(this);
        }
    }
    else if(m_parent != nullptr)//we remove current parent
    {
        //remove previous child link
        m_parent->RemoveChild(ElementUID());
        //set new parent
        m_parent = parent;
    }
    //else we do nothing (means that m_parent == parent == nullptr)

}
/**
 * \brief Take ownership of the element
 */
void GuiElement::AddChild(GuiElement* element)
{
    auto found = m_children.find(element->ElementUID());
    if(found != std::end(m_children))//already a child
        return;

    //add child
    m_children.emplace(element->ElementUID(), element);//calls GuiElement move constructor and create pointer
}

void GuiElement::RemoveChild(UID uid)
{
    m_children.erase(uid);
}
//</f>

}//namespace
