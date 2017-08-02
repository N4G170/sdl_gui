#include "SDL.h"
#include <unordered_map>
#include <vector>
#include <functional>
#include <string>
#include "sdl_gui_transform.hpp"
#include "sdl_gui_enums.hpp"
#include "sdl_gui_resource_manager.hpp"
#include "sdl_gui_camera.hpp"

#include "sdl_gui_uid.hpp"
#include "sdl_gui_collider.hpp"
#include <tuple>

namespace sdl_gui
{

#ifndef SDL_GUI_ELEMENT_HPP
#define SDL_GUI_ELEMENT_HPP

/**
 * Definition of the most basic gui element, containing virtual logic functions and the transform
 */
class GuiElement
{
    public:
        //<f> Constructors & operator=
        GuiElement(GuiMainPointers main_pointers, const Position& position, const Dimensions& size);
        /* Default destructor */
        virtual ~GuiElement() noexcept;

        /**
         * \brief Copy constructor. Will generate new uid for the copy
         */
        GuiElement(const GuiElement& other);
        /* Move constructor */
        GuiElement(GuiElement&& other) noexcept;

        GuiElement& operator= (const GuiElement& other);

        GuiElement& operator= (GuiElement&& other);

        bool operator==(const GuiElement& other){ return m_uid == other.m_uid; }
        //</f>

        //<f> Virtual Methods
        virtual void Input(const SDL_Event& event);
        virtual void ClearInput();

        virtual void FixedLogic(float fixed_delta_time);
        virtual void Logic(float delta_time);

        virtual void Render(float delta_time);
        virtual void Render(float delta_time, Camera* camera);
        //</f>

        //<f> Render
        SDL_Rect RenderRect();
        //</f>

        //<f> Getters/Setters
        UID ElementUID() const { return m_uid; }
        GuiTransform* TransformPtr() { return &m_transform; }
        Camera* GuiCamera() const { return m_main_pointers.main_camera_ptr; }

        bool CanRender() const { return m_render; }
        void CanRender(bool render){ m_render = render; }

        bool IsActive() const { return m_active; }
        void IsActive(bool is_active){ m_active = is_active; }
        //</f>

        //<f> Position, Size & Transform interface
        Position GlobalPositionIgnoreViewport() const { return m_transform.GlobalPositionIgnoreViewport(); }
        Position GlobalPosition() const { return m_transform.GlobalPosition(); }
        void GlobalPosition(const Position& new_global_position) { m_transform.GlobalPosition(new_global_position); }
        Position LocalPosition() const { return m_transform.LocalPosition(); }
        void LocalPosition(const Position& new_local_position) { m_transform.LocalPosition(new_local_position); }

        void ParentViewport(GuiTransform* parent_viewport) { m_transform.ParentViewport(parent_viewport); }
        GuiTransform* ParentViewport() const { return m_transform.ParentViewport(); }

        void Size(const Dimensions& new_size) { m_size = new_size; }
        Dimensions Size() const { return m_size; }

        SDL_Rect Bounds();

        /**
         * \brief Align this element specific point to the same point in the parent.
         * \n Ex: top-left child to top-left of parent, ot middle-centre in child to middle-centre in parent
         * @param point_type Point to align with
         * \param offset Offset (x,y) for the element in relation of the point. Default 0 so points match
         */
        void AlignWithParentPoint(AnchorType point_type, Position offset = {0,0});
        //</f>

        //<f> Interaction & Colliders
        /* Add Box collider */
        void AddGuiCollider(const Position& local_position, const Dimensions& size, GuiTransform* owner_transform);
        /* Add Circle collider */
        void AddGuiCollider(const Position& local_position, float circle_radius, GuiTransform* owner_transform);

        Collider* GetColliderPtr() const { return m_collider.get(); }
        // Collider* GetColliderPtr(int index) const { return m_colliders[index].get(); }

        virtual void Focus(){ m_focused = true; }
        virtual void UnFocus(){ m_focused = false; }
        //</f>

        //<f> Parent & Children
        /**
         * Sets this element parent. Will, also, set the parent in the transform
         */
        void Parent(GuiElement* parent);
        GuiElement* Parent() const { return m_parent; }
        std::unordered_map<UID, GuiElement*>* Children() { return &m_children; }
        //</f>

    protected:
        // vars and stuff
        //<f> ID, Position and Size
        UID m_uid;
        GuiTransform m_transform;
        /**
         * Render dimensions of the element
         */
        Dimensions m_size;
        //</f>

        //<f> Render
        /**
         * \brief Pointers for external resources needed. EX: renderer, camera, resource_manager
         */
        GuiMainPointers m_main_pointers;

        /**
         * True - render element
         * False - Do not render
         */
        bool m_render;
        //</f>

        //<f> Interaction
        // std::vector<std::unique_ptr<Collider>> m_colliders;
        std::unique_ptr<Collider> m_collider;
        bool m_active;
        bool m_focused;
        //</f>

        //<f> Parent & Children
        GuiElement* m_parent;
        std::unordered_map<UID, GuiElement*> m_children;

        /**
         * \brief Take ownership of the element. DO NOT CALL THIS METHOD
         */
        void AddChild(GuiElement* element);
        /**
         *  \brief DO NOT CALL THIS METHOD
         */
        void RemoveChild(UID uid);
        //</f>
};

#endif //GUI_ELEMENT_HPP

}//namespace
