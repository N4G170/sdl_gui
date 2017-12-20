#include "sdl_gui_enums.hpp"
#include "SDL.h"

namespace sdl_gui
{

#ifndef SDL_GUI_BUTTON_TRANSITION_HPP
#define SDL_GUI_BUTTON_TRANSITION_HPP

/**
 * \brief Interface class for button transition
 */
class ButtonTransition
{
    public:
        virtual ~ButtonTransition() noexcept{};
        //<f> Virtual Methods
        virtual void Render(const SDL_Rect& dst) = 0;
        virtual void ChangeState(ButtonState state) = 0;
        //</f>
};

#endif //SDL_GUI_BUTTON_TRANSITION_HPP

}//namespace
