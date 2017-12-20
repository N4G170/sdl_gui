#include <unordered_map>
#include "SDL.h"
#include "sdl_gui_uid.hpp"
#include "sdl_gui_structs.hpp"
//#include "sdl_gui_element.hpp"//include nested too deep

namespace sdl_gui
{

#ifndef SDL_GUI_TRANSFORM_HPP
#define SDL_GUI_TRANSFORM_HPP

class GuiElement;

class GuiTransform
{
    public:
        //<f> Constructors and operator=
        /* Default constructor */
        explicit GuiTransform(GuiElement* owner);
        /* Default destructor */
        virtual ~GuiTransform() noexcept;

        /* Copy constructor */
        GuiTransform(const GuiTransform& other);
        /* Move constructor */
        GuiTransform(GuiTransform&& other) noexcept;

        /* Copy operator */
        GuiTransform& operator= (const GuiTransform& other);
        /* Move operator */
        GuiTransform& operator= (GuiTransform&& other) noexcept;
        //</f>

        //<f> Getters/Setters

        Position GlobalPositionIgnoreViewport() const;
        /**
        * The position is ajusted to account for the existence or not of a parent viewport (as it has a new origin).
        */
        Position GlobalPosition() const;
        void GlobalPosition(const Position& new_global_position);
        Position LocalPosition() const { return m_local_position; };
        void LocalPosition(const Position& new_local_position);

        void ParentViewport(GuiTransform* parent_viewport);
        GuiTransform* ParentViewport() const { return m_parent_viewport; };

        // void Parent(GuiTransform* parent);
        // GuiTransform* Parent() const { return m_parent; }

        GuiElement* Owner() const { return m_owner; }
        /**
         * \Use only after move operator and  copy&move constructor (in owner)
         */
        void Owner(GuiElement* owner) { m_owner = owner; }
        //</f>

    protected:

        // vars and stuff
        /**
         * \brief Element that owns this transform
         */
        GuiElement* m_owner;

        /**
         * Position of the element relative to its parent. parent == nullptr ? global_position : local_position
         */
        Position m_local_position;//reltive to parent. Same as global if no parent

        /**
         * Pointer to the viewport in which this element is. nullptr if element is in the world
         */
        GuiTransform* m_parent_viewport;

        // /**
        //  * Pointer to parent element. nullptr if element is directly in the world
        //  */
        // GuiTransform* m_parent;

        // /**
        //  * List of all the children elements of this one
        //  */
        // std::unordered_map<UID, GuiTransform*> m_children;

        // void AddChild(UID child_uid, GuiTransform* child);
        // void RemoveChild(UID child_uid);
};

#endif //GUI_TRANSFORM_HPP

}//namespace
