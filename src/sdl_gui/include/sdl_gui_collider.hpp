#include "SDL.h"
#include <functional>
#include <vector>
#include "sdl_gui_transform.hpp"

namespace sdl_gui
{

#ifndef SDL_GUI_COLLIDER_HPP
#define SDL_GUI_COLLIDER_HPP

union ColliderSizeUnion
{
    Dimensions size;
    float radius;
};

class Collider
{
    public:
        //<f> Constructors, Duplicate & operator=
        /* constructor */
        Collider(const Position& position, GuiTransform* owner_transform);

        /* Default destructor */
        virtual ~Collider() noexcept;

        /* Copy constructor */
        Collider(const Collider& other);
        /* Move constructor */
        Collider(Collider&& other) noexcept;

        /**
         * \brief Creates a copy of a derivedelement as a pointer to its base class()this class. Usefull for copying unique_ptrs
         */
        virtual Collider* Duplicate() = 0;

        /* Copy operator */
        Collider& operator= (const Collider& other);
        /* Move operator */
        Collider& operator= (Collider&& other) noexcept;
        //</f>

        //<f> Intersection
        virtual bool IsPointColliding(int mouse_x, int mouse_y) = 0;
        //</f>

        //<f> Collider Update
        virtual void UpdateColliderSize(const ColliderSizeUnion& size) = 0;
        void UpdateColliderPosition(const Position& position) { m_local_position = position; }
        //</f>

        virtual void DebugRender(SDL_Renderer* renderer_ptr, const SDL_Colour& colour = {0,0,0,255}) = 0;

    protected:
        // vars and stuff
        Position m_local_position;
        GuiTransform* m_owner_transform;
};

#endif //GUI_COLLIDER_HPP
}//namespace
