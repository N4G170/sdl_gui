#include "sdl_gui_structs.hpp"
#include "sdl_gui_transform.hpp"
#include "sdl_gui_collider.hpp"

namespace sdl_gui
{

#ifndef SDL_GUI_BOX_COLLIDER
#define SDL_GUI_BOX_COLLIDER

class BoxCollider : public Collider
{
    public:
        //<f> Constructors, Duplicate & operator=
        /* Box constructor */
        BoxCollider(const Position& position, const Dimensions& size, GuiTransform* owner_transform);

        /* Default destructor */
        virtual ~BoxCollider() noexcept;

        /* Copy constructor */
        BoxCollider(const BoxCollider& other);
        /* Move constructor */
        BoxCollider(BoxCollider&& other) noexcept;

        /**
         * \brief Creates a copy of this element as a pointer to its base class. Usefull for copying unique_ptrs
         */
        Collider* Duplicate() override;

        /* Copy operator */
        BoxCollider& operator= (const BoxCollider& other);
        /* Move operator */
        BoxCollider& operator= (BoxCollider&& other) noexcept;
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
        Dimensions m_size;
};

#endif//SDL_GUI_BOX_COLLIDER

}//namespace
