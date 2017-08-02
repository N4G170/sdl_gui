#include "SDL.h"
#include <vector>
#include <unordered_map>
#include <functional>
#include "sdl_gui_collider.hpp"
#include "sdl_gui_enums.hpp"
#include "sdl_gui_structs.hpp"

namespace sdl_gui
{

#ifndef SDL_GUI_INTERACTION_HPP
#define SDL_GUI_INTERACTION_HPP

class MouseInteraction
{
    public:
        //<f> Constructors & operator=
        /* Default constructor */
        MouseInteraction();
        /* Default destructor */
        virtual ~MouseInteraction() noexcept;

        /* Copy constructor */
        MouseInteraction(const MouseInteraction& other);
        /* Move constructor */
        MouseInteraction(MouseInteraction&& other) noexcept;

        /* Copy operator */
        MouseInteraction& operator= (const MouseInteraction& other);
        /* Move operator */
        MouseInteraction& operator= (MouseInteraction&& other) noexcept;
        //</f>

        //<f> Virtual Methods
        /**
         * \brief Evaluate mouse position in relation to given list of colliders
         */
        virtual void Input(const SDL_Event& event, Collider* collider);

        /**
         * \brief Process the result from the input function, in a fixed time step
         * @param fixed_delta_time Fixed time step
         */
        virtual void FixedLogic(float fixed_delta_time);

        /**
         * \brief Process the result from the input function, in a fixed time step
         * @param delta_time Variable time step
         */
        virtual void Logic(float delta_time);
        //</f>

        //<f> Callbacks
        /**
         * \brief Get a pointer to the callback for the given mouse action type
         * Possible actions \see MouseCallbackType
         */
        std::function<void()>* MouseCallback(MouseCallbackType type) { return &m_mouse_callbacks[type]; }
        /**
         * \brief Sets the callback fot the given mouse action type
         * Possible actions \see MouseCallbackType
         */
        void MouseCallback(MouseCallbackType type, const std::function<void()>& callback) { m_mouse_callbacks[type] = callback; }

        /**
         * \brief Get a pointer to the callback for the given mouse button action type
         * Possible actions \see InputKeyCallbackType
         */
        std::function<void()>* MouseButtonCallback(Uint8 mouse_button, InputKeyCallbackType type);
        /**
         * \brief Sets the callback fot the given mouse button action type
         * Possible actions \see InputKeyCallbackType
         */
        void MouseButtonCallback(Uint8 mouse_button, InputKeyCallbackType type, const std::function<void()>& callback);
        //</f>

        //<f> Getters/Setters
        int MouseFlag() const { return m_mouse_flags; }
        /**
         * \brief Get current valid click time limit
         */
        float ClickTimeLimit() const { return m_click_time_limit; }
        /**
         * \brief Set current valid click time limit
         */
        void ClickTimeLimit(float click_time_limit){ m_click_time_limit = click_time_limit; }
        //</f>

        //<f> Other Methods
        void ClearInput();
        //</f>

    protected:
        // vars and stuff
        /** \brief mouse flags */
        int m_mouse_flags;
        /**
         * \brief Counts the time passed to detect if we have a valid click. Fixed time step
         */
        float m_fixed_valid_click_timer;
        /**
         * \brief Counts the time passed to detect if we have a valid click. Variable time step
         */
        float m_valid_click_timer;

        /**
         * \brief Time limit to accept a click as valid
         */
        float m_click_time_limit;

        /* < CallBacks > */
        std::unordered_map<MouseCallbackType, std::function<void()>> m_mouse_callbacks;
        /**
         * Mouse buttons to be processed
         */
        std::unordered_map<Uint8, InputKey> m_mouse_buttons;

        /* </ CallBacks > */
        bool MouseInsideCollider(int mouse_x, int mouse_y, Collider* collider);

        /**
         * Function called by both Logic and FixedLogic as both are the same but with different time input.
         * \nAlso the ref of the corresponding timer needs to be provided as \param valid_click_timer
         */
        void MouseLogic(float delta_time, float& valid_click_timer);
};

#endif //GUI_INTERACTION_HPP
}//namespace
