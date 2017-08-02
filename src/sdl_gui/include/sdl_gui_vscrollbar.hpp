#include "sdl_gui_element.hpp"
#include "sdl_gui_enums.hpp"
#include "sdl_gui_base_button.hpp"
#include "sdl_gui_utils.hpp"
#include <functional>

namespace sdl_gui
{

#ifndef SDL_GUI_VSCROLLBAR_HPP
#define SDL_GUI_VSCROLLBAR_HPP

class VScrollbar : public GuiElement
{
    public:
        //<f> Constructors & operator=
        /* Default constructor */
        VScrollbar(GuiMainPointers main_pointers, const Position& position, const Dimensions& size);
        /* Default destructor */
        virtual ~VScrollbar() noexcept;

        /* Copy constructor */
        VScrollbar(const VScrollbar& other);
        /* Move constructor */
        VScrollbar(VScrollbar&& other) noexcept;

        /* Copy operator */
        VScrollbar& operator= (const VScrollbar& other);
        /* Move operator */
        VScrollbar& operator= (VScrollbar&& other) noexcept;
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
        float Value() const;
        /**
         * Set the current value for the ProgressBar. If the value > max or value < min, value will be set to the right limit
         */
        void Value(float new_value);

        float MaxValue() const { return m_max_value; }
        //update value if we change the range
        void MaxValue(float max_value);
        float MinValue() const { return m_min_value; }
        //update value if we change the range
        void MinValue(float min_value);

        BaseButton* HeadButton() { return m_head_button.get(); }
        void HeadRatio(float ratio) { m_head_ratio = ratio; UpdateHeadSize(); }

        void ValueChangedCallback(const std::function<void(float)>& callback){ m_value_changed_callback = callback; }
        std::function<void(float)>* ValueChangedCallback() { return &m_value_changed_callback; }

        void Resize(const Dimensions& size){ Size(size); m_head_button->Size({size.w, m_head_button->Size().h}); }
        //</f>

    protected:
        // vars and stuff
        float m_min_value;
        float m_max_value;

        std::unique_ptr<BaseButton> m_head_button;
        float m_head_ratio;

        bool m_drag;
        int m_mouse_y;
        int m_mouse_offset_y;
        float m_previous_head_y;

        std::function<void(float)> m_value_changed_callback;

        void SetDragFlag(bool drag);
        void UpdateHeadSize();
};

#endif //SDL_GUI_VSCROLLBAR_HPP

}//namespace
