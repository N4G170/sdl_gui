#include "sdl_gui_element.hpp"
#include "sdl_gui_enums.hpp"
#include "sdl_gui_base_button.hpp"
#include "sdl_gui_utils.hpp"
#include <functional>

namespace sdl_gui
{

#ifndef SDL_GUI_VSLIDER_HPP
#define SDL_GUI_VSLIDER_HPP

class VSlider : public GuiElement
{
    public:
        //<f> Constructors & operator=
        /* Default constructor */
        VSlider(GuiMainPointers main_pointers, const Position& position, const Dimensions& size);
        /* Default destructor */
        virtual ~VSlider() noexcept;

        /* Copy constructor */
        VSlider(const VSlider& other);
        /* Move constructor */
        VSlider(VSlider&& other) noexcept;

        /* Copy operator */
        VSlider& operator= (const VSlider& other);
        /* Move operator */
        VSlider& operator= (VSlider&& other) noexcept;
        //</f>

        //<f> Overrides GuiElement
        virtual void Input(const SDL_Event& event);

        // virtual void FixedLogic(float fixed_delta_time);
        virtual void Logic(float delta_time);

        virtual void Render(float delta_time);
        virtual void Render(float delta_time, Camera* camera);
        //</f>

        //<f> Virtual Methods

        //</f>

        //<f> Getters/Setters
        float Value() const { return m_value; }
        /**
         * Set the current value for the ProgressBar. If the value > max or value < min, value will be set to the right limit
         */
        void Value(float new_value)
        {
            if(new_value != m_value)
            {
                m_value = KeepInInterval(new_value, m_min_value, m_max_value);
                if(m_value_changed_callback)
                    m_value_changed_callback(m_value);
            }
        }

        float MaxValue() const { return m_max_value; }
        //update value if we change the range
        void MaxValue(float max_value) { m_max_value = max_value; m_value = KeepInInterval(m_value, m_min_value, m_max_value); }
        float MinValue() const { return m_min_value; }
        //update value if we change the range
        void MinValue(float min_value) { m_min_value = min_value; m_value = KeepInInterval(m_value, m_min_value, m_max_value); }

        BaseButton* HeadButton() { return m_head_button.get(); }

        void ValueChangedCallback(const std::function<void(float)>& callback){ m_value_changed_callback = callback; }
        std::function<void(float)>* ValueChangedCallback() { return &m_value_changed_callback; }
        //</f>

    protected:
        // vars and stuff
        float m_min_value;
        float m_max_value;
        float m_value;

        std::unique_ptr<BaseButton> m_head_button;

        bool m_drag;
        int m_mouse_y;
        int m_mouse_offset_y;

        std::function<void(float)> m_value_changed_callback;

        void SetDragFlag(bool drag);
};

#endif //SDL_GUI_VSLIDER_HPP

}//namespace
