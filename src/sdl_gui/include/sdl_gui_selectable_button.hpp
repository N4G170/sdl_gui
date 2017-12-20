#include "sdl_gui_base_button.hpp"
#include <functional>

namespace sdl_gui
{
#ifndef SDL_GUI_SELECTABLE_BUTTON_HPP
#define SDL_GUI_SELECTABLE_BUTTON_HPP

class SelectableButton: public BaseButton
{
    public:
        /* Default constructor */
        SelectableButton(GuiMainPointers main_pointers, const Position& position, const Dimensions& size);
        /* Default destructor */
        virtual ~SelectableButton() noexcept;

        /* Copy constructor */
        SelectableButton(const SelectableButton& other);
        /* Move constructor */
        SelectableButton(SelectableButton&& other) noexcept;

        /* Copy operator */
        SelectableButton& operator= (const SelectableButton& other);
        /* Move operator */
        SelectableButton& operator= (SelectableButton&& other) noexcept;

        //<f> Overrides GuiElement
        // virtual void Input(const SDL_Event& event);

        // virtual void FixedLogic(float fixed_delta_time);
        // virtual void Logic(float delta_time);

        virtual void Render(float delta_time);
        virtual void Render(float delta_time, Camera* camera);
        //</f>

        //<f> Getters/Setters
        bool IsSelected() const { return m_is_selected; }
        virtual void IsSelected(bool is_selected) { m_is_selected = is_selected; }

        int Value() const { return m_value; }
        void Value(int new_value) { m_value = new_value; }
        //</f>

        //<f> Callbacks
        /**
         * \brief Called when the state of this box is changed by user input (on/off)
         */
        std::function<void(SelectableButton*)> StateChanged;


        //</f>
    protected:
        bool m_is_selected;
        int m_value;
        /**
         * \brief bool to control if we apply the selected transition for the button
         */
        bool m_set_selected_state;

        /**
         *  Function called when we click the button to change internal state. Will call \see StateChanged, if set
         */
        virtual void MouseClick();
};

#endif //SDL_GUI_SELECTABLE_BUTTON_HPP
}//namespace
