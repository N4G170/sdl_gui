#include "sdl_gui_element.hpp"
#include "sdl_gui_texture.hpp"
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

        virtual void Render(float delta_time);
        virtual void Render(float delta_time, Camera* camera);
        //</f>

        //<f> Virtual Methods

        //</f>

        //<f> Getters/Setters
        ProgressBarDirection BarDirection() const { return m_bar_direction; }
        void BarDirection(ProgressBarDirection bar_direction) { m_bar_direction = bar_direction; }

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

        void ValueChangedCallback(const std::function<void(float)>& callback){ m_value_changed_callback = callback; }
        std::function<void(float)>* ValueChangedCallback() { return &m_value_changed_callback; }
        //</f>

    protected:
        // vars and stuff
        Texture m_bg_texture;
        Texture m_bar_texture;

        ProgressBarDirection m_bar_direction;

        float m_min_value;
        float m_max_value;
        float m_value;

        std::function<void(float)> m_value_changed_callback;

        void RenderLeft(SDL_Rect& dst);
        void RenderRight(SDL_Rect& dst);
        void RenderUp(SDL_Rect& dst);
        void RenderDown(SDL_Rect& dst);
};

#endif //SDL_GUI_PROGRESS_BAR_HPP

}//namespace
