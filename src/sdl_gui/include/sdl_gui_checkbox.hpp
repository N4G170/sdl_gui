#include "sdl_gui_base_button.hpp"
#include "sdl_gui_image.hpp"
#include <functional>

namespace sdl_gui
{
#ifndef SDL_GUI_CHECKBOX_HPP
#define SDL_GUI_CHECKBOX_HPP

class CheckBox: public BaseButton
{
    public:
        /* Default constructor */
        CheckBox(GuiMainPointers main_pointers, const Position& position, const Dimensions& size);
        /* Default destructor */
        virtual ~CheckBox() noexcept;

        /* Copy constructor */
        CheckBox(const CheckBox& other);
        /* Move constructor */
        CheckBox(CheckBox&& other) noexcept;

        /* Copy operator */
        CheckBox& operator= (const CheckBox& other);
        /* Move operator */
        CheckBox& operator= (CheckBox&& other) noexcept;

        //<f> Overrides GuiElement
        // virtual void Input(const SDL_Event& event);

        // virtual void FixedLogic(float fixed_delta_time);
        // virtual void Logic(float delta_time);

        virtual void Render(float delta_time);
        virtual void Render(float delta_time, Camera* camera);
        //</f>

        //<f> Getters/Setters
        bool IsChecked() const { return m_is_checked; }
        void IsChecked(bool is_checked) { m_is_checked = is_checked; }

        int Value() const { return m_value; }
        void Value(int new_value) { m_value = new_value; }

        Image* CheckMarkImage() { return &m_check_mark_image; }
        //</f>

        //<f> Checkbox type
        void ConfigAsMultiple();
        void ConfigAsRadio();
        //</f>

        //<f> Callbacks
        /**
         * \brief Called when the value of this box is changed by user input
         */
        std::function<void(CheckBox*)> ValueChanged;

        void MouseClick();
        //</f>
    private:
        // vars and stuff
        Image m_check_mark_image;
        bool m_is_checked;
        int m_value;
};

#endif //SDL_GUI_CHECKBOX_HPP
}//namespace
