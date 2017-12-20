#include "sdl_gui_element.hpp"
#include "sdl_gui_enums.hpp"
#include "sdl_gui_utils.hpp"
#include <functional>

namespace sdl_gui
{

#ifndef SDL_GUI_PROGRESS_BAR_HPP
#define SDL_GUI_PROGRESS_BAR_HPP

class ProgressBar : public GuiElement
{
    public:
        //<f> Constructors & operator=
        /* Default constructor */
        ProgressBar(GuiMainPointers main_pointers, const Position& position, const Dimensions& size);
        /* Default destructor */
        virtual ~ProgressBar() noexcept;

        /* Copy constructor */
        ProgressBar(const ProgressBar& other);
        /* Move constructor */
        ProgressBar(ProgressBar&& other) noexcept;

        /* Copy operator */
        ProgressBar& operator= (const ProgressBar& other);
        /* Move operator */
        ProgressBar& operator= (ProgressBar&& other) noexcept;
        //</f>

        //<f> Overrides GuiElement
        // virtual void Input(const SDL_Event& event);

        // virtual void FixedLogic(float fixed_delta_time);
        // virtual void Logic(float delta_time);

        // virtual void Render(float delta_time);
        // virtual void Render(float delta_time, Camera* camera);
        //</f>

        //<f> Virtual Methods

        //</f>

        //<f> Getters/Setters
        ProgressBarDirection BarDirection() const { return m_bar_direction; }
        void BarDirection(ProgressBarDirection bar_direction);

        float Value() const { return m_value; }
        /**
         * Set the current value for the ProgressBar. If the value > max or value < min, value will be set to the right limit
         */
        void Value(float new_value);

        float Ratio() const { return m_value / m_max_value; }
        void Ratio(float ratio);

        float MaxValue() const { return m_max_value; }
        //update value if we change the range
        void MaxValue(float max_value) { m_max_value = max_value; m_value = KeepInInterval(m_value, m_min_value, m_max_value); }
        float MinValue() const { return m_min_value; }
        //update value if we change the range
        void MinValue(float min_value) { m_min_value = min_value; m_value = KeepInInterval(m_value, m_min_value, m_max_value); }

        void ValueChangedCallback(const std::function<void(float)>& callback){ m_value_changed_callback = callback; }
        std::function<void(float)>* ValueChangedCallback() { return &m_value_changed_callback; }

        void BarValueChangedCallback(const std::function<void(ProgressBar*)>& callback){ m_bar_value_changed_callback = callback; }
        std::function<void(ProgressBar*)>* BarValueChangedCallback() { return &m_bar_value_changed_callback; }

        GuiElement* BackgroundElement() const { return m_background; }
        void BackgroundElement(GuiElement* background) { m_background = background; m_background->Parent(this); }

        GuiElement* BarElement() const { return m_bar; }
        void BarElement(GuiElement* bar) { m_bar = bar; m_bar->Parent(this); UpdateBar(); m_bar->RenderIndex(-10); ConfigBar(); }
        //</f>

    protected:
        // vars and stuff
        GuiElement* m_background;
        GuiElement* m_bar;

        ProgressBarDirection m_bar_direction;

        float m_min_value;
        float m_max_value;
        float m_value;

        std::function<void(float)> m_value_changed_callback;
        std::function<void(ProgressBar*)> m_bar_value_changed_callback;

        /**
         * \brief Set the correct anchor for the bar element
         */
        void ConfigBar();
        /**
         * \brief Update the scale of the bar (x or y depending on the direction)
         */
        void UpdateBar();
};

#endif //SDL_GUI_PROGRESS_BAR_HPP

}//namespace
