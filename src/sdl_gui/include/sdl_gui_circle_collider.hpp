#include "sdl_gui_structs.hpp"
#include "sdl_gui_transform.hpp"
#include "sdl_gui_collider.hpp"

namespace sdl_gui
{

#ifndef SDL_GUI_CIRCLE_COLLIDER
#define SDL_GUI_CIRCLE_COLLIDER

class CircleCollider : public Collider
{
    public:
        //<f> Constructors & operator=
        /* Box constructor */
        CircleCollider(const Position& position, float radius, GuiTransform* owner_transform);

        /* Default destructor */
        virtual ~CircleCollider() noexcept;

        /* Copy constructor */
        CircleCollider(const CircleCollider& other);
        /* Move constructor */
        CircleCollider(CircleCollider&& other) noexcept;

        /**
         * \brief Creates a copy of this element as a pointer to its base class. Usefull for copying unique_ptrs
         */
        Collider* Duplicate() override;

        /* Copy operator */
        CircleCollider& operator= (const CircleCollider& other);
        /* Move operator */
        CircleCollider& operator= (CircleCollider&& other) noexcept;
        //</f>

        //<f> Intersection
        bool IsPointColliding(int mouse_x, int mouse_y) override;
        //</f>

        //<f> Collider Update
        void UpdateColliderSize(const ColliderSizeUnion& size) override;
        //</f>

        //<f> Debug
        void DebugRender(SDL_Renderer* renderer_ptr, const SDL_Colour& colour = {0,0,0,255}) override;
        //</f>
    private:
        float m_radius;
        float m_squared_radius;
};

#endif//SDL_GUI_CIRCLE_COLLIDER

}//namespace
