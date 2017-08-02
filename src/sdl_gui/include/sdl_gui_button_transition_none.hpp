#include "sdl_gui_button_transition.hpp"
#include "sdl_gui_texture.hpp"
#include "sdl_gui_resource_manager.hpp"

namespace sdl_gui
{

#ifndef SDL_GUI_BUTTON_TRANSITION_NONE_HPP
#define SDL_GUI_BUTTON_TRANSITION_NONE_HPP

class ButtonTransitionNone : public ButtonTransition
{
    public:
        //<f> Constructors & operator=
        /** brief Default constructor */
        explicit ButtonTransitionNone(ResourceManager* resource_manager_ptr);
        /** brief Default destructor */
        virtual ~ButtonTransitionNone() noexcept;

        /** brief Copy constructor */
        ButtonTransitionNone(const ButtonTransitionNone& other);
        /** brief Move constructor */
        ButtonTransitionNone(ButtonTransitionNone&& other) noexcept;

        /** brief Copy operator */
        ButtonTransitionNone& operator= (const ButtonTransitionNone& other);
        /** brief Move operator */
        ButtonTransitionNone& operator= (ButtonTransitionNone&& other) noexcept;
        //</f>

        //<f> Virtual Methods
        virtual void Render(const SDL_Rect& dst);
        virtual void ChangeState(ButtonState state);
        //</f>

        //<f> Getters/Setters

        //</f>

    protected:
        // vars and stuff
        Texture m_texture;

    private:
};

#endif //SDL_GUI_BUTTON_TRANSITION_NONE_HPP

}//namespace
