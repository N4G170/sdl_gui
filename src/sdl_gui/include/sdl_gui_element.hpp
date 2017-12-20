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
#include "sdl_gui_utils.hpp"
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
        int RenderIndex() const { return m_render_index; }
        void RenderIndex(int index){ m_render_index = index; }

        /**
         * \brief Calculate and return the render index taking into account the render indeces of all parents (the last -1 will for the child to allways be on top of parent).
         * \nEX: parent rindex = 1; this->m_render_index = -1; GlobalRenderIndex will be 1+(-1) - 1 = -1 meaning that this will stay on top of parent
         */
        int GlobalRenderIndex(){ if(m_parent == nullptr) return m_render_index; else return m_render_index + m_parent->GlobalRenderIndex() - 1; }
        //</f>

        //<f> Getters/Setters
        UID ElementUID() const { return m_uid; }
        GuiTransform* TransformPtr() { return &m_transform; }
        Camera* GuiCamera() const { return m_main_pointers.main_camera_ptr; }

        //<f> Render
        bool CanRender() const { return m_render; }
        void CanRender(bool render){ m_render = render; }
        void Show() { m_render = true; }
        void Hide() { m_render = false; }
        //</f>

        //<f> Active
        bool IsActive() const { return m_active; }
        void IsActive(bool is_active){ m_active = is_active; }
        void Activate() { m_active = true; }
        void Deactivate() { m_active = false; }
        //</f>

        //<f> Enable
        bool Enabled() const{ return m_enabled; }
        void Enabled(bool enabled){ m_enabled = enabled; }
        void Enable() { m_enabled = true; }
        void Disable() { m_enabled = false; }

        /**
         * \brief Search element parents for disabled elements. Also check itself
         * \return false if no disabled parents(or self), true when the first disabled parent is found
         */
        bool DisabledSelfOrParent()
        {
            if(m_parent == nullptr)
                return !m_enabled;//return true if disabled

            if(!m_enabled)//is disabled
                return true;
            else
                return m_parent->DisabledSelfOrParent();//keep checking tree
        }
        //</f>
        //</f>

        //<f> Position, Size & Transform interface
        Position GlobalPositionIgnoreViewport() const { return m_transform.GlobalPositionIgnoreViewport(); }
        Position GlobalPosition() const { return m_transform.GlobalPosition(); }
        void GlobalPosition(const Position& new_global_position) { m_transform.GlobalPosition(new_global_position); }
        Position LocalPosition() const { return m_transform.LocalPosition(); }
        void LocalPosition(const Position& new_local_position) { m_transform.LocalPosition(new_local_position); }

        void ParentViewport(GuiTransform* parent_viewport) { m_transform.ParentViewport(parent_viewport); }
        GuiTransform* ParentViewport() const { return m_transform.ParentViewport(); }

        //<f> Size
        Dimensions Size() const { return m_size; }
        void Size(const Dimensions& new_size) { m_size = new_size; }
        void Size(float new_w, float new_h) { m_size.w = new_w; m_size.h = new_h; }
        void SizeW(float new_w) { m_size.w = new_w; }
        void SizeH(float new_h) { m_size.h = new_h; }

        Dimensions ScaledSize() const { return {m_size.w * m_scale.x, m_size.h * m_scale.y}; }

        SDL_Rect Bounds();
        void ResizeToBounds(){ auto bounds = Bounds(); Size(SizeFromInts(bounds.w, bounds.h)); }
        //</f>

        //<f> Scale
        Scale LocalScale() const { return m_scale; }
        void LocalScale(const Scale& scale) { m_scale = scale; }
        void LocalScaleX(float scale_x){ m_scale.x = scale_x; }
        void LocalScaleY(float scale_y){ m_scale.y = scale_y; }

        Scale GlobalScale();
        //</f>

        //<f> Anchoring

        AnchorType Anchor() const { return m_anchor; }
        void Anchor(AnchorType anchor) { m_anchor = anchor; }

        /**
         * \brief Align this element specific point to the same point in the parent.
         * \n Ex: top-left child to top-left of parent, ot middle-centre in child to middle-centre in parent
         * @param point_type Point to align with
         * \param offset Offset (x,y) for the element in relation of the point. Default 0 so points match
         */
        void AlignWithParentPoint(AnchorType point_type, Position offset = {0,0});
        //</f>
        //</f>

        //<f> Interaction & Colliders
        /* Add Box collider */
        void AddGuiCollider(const Position& local_position, const Dimensions& size, GuiTransform* owner_transform);
        /* Add Circle collider */
        void AddGuiCollider(const Position& local_position, float circle_radius, GuiTransform* owner_transform);

        Collider* GetColliderPtr() const { return m_collider.get(); }

        /**
         * \brief Update collider size. Checks if collider exists or not
         * \n Returns true if collider exists and value was changed, false otherwise
         */
        bool UpdateColliderSize(ColliderSizeUnion size)
        {
            if(m_collider == nullptr) return false;
            else m_collider->UpdateColliderSize(size); return true;
        }

        /**
         * \brief Update collider size to match element size. Internally calls \see bool UpdateColliderSize(ColliderSizeUnion size)
         */
        bool FitColliderSize() { return  UpdateColliderSize({Size()}); };

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
        bool m_enabled;

        //<f> ID, Position and Size
        UID m_uid;
        GuiTransform m_transform;

        AnchorType m_anchor;

        /**
         * Render dimensions of the element
         */
        Dimensions m_size;

        Scale m_scale;
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

        /**
         * \brief Index for rendering sorting. The lower the value the further back the element (ex: 1 is over -1)
         */
        int m_render_index;
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

        //<f>Anchor position
        Position CalculateAnchoredPosition();
        //</f>
};

#endif //GUI_ELEMENT_HPP

}//namespace
