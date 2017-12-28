#include "sdl_gui_element.hpp"
#include "sdl_gui_manager.hpp"

#ifndef BARS_PANEL_HPP
#define BARS_PANEL_HPP

class BarsPanel
{
    public:
        //<f> Constructors & operator=
        /** brief Default constructor */
        explicit BarsPanel(sdl_gui::GuiManager* gui_manager);
        /** brief Default destructor */
        virtual ~BarsPanel() noexcept;
        //</f>

        void Logic(float delta_time);

        //<f> Virtual Methods

        //</f>

        //<f> Getters/Setters
        sdl_gui::GuiElement* Panel() const { return m_panel; }
        sdl_gui::Image* Background() const { return m_background; }
        //</f>

    protected:
        // vars and stuff
        sdl_gui::GuiManager* m_gui_manager;
        sdl_gui::GuiElement* m_panel;
        sdl_gui::Image* m_background;

        sdl_gui::GuiElement* m_bar_container;
        sdl_gui::GuiElement* m_slider_container;

        //<f> Progress Bars
        sdl_gui::ProgressBar* m_progress_bar_h;
        sdl_gui::Label* m_ratio_label;
        sdl_gui::ProgressBar* m_progress_bar_v;

        //</f> /Progress Bars

        //<f> Progress Bar control
        sdl_gui::BaseButton* m_play_btn;
        sdl_gui::BaseButton* m_pause_btn;

        sdl_gui::BaseButton* m_plus_btn;
        sdl_gui::BaseButton* m_minus_btn;
        sdl_gui::Label* m_step_info;
        sdl_gui::Label* m_step_label;

        sdl_gui::CheckBox* m_loop_checkbox;
        sdl_gui::Label* m_loop_label;
        sdl_gui::BaseButton* m_reset_btn;
        sdl_gui::Label* m_reset_label;

        sdl_gui::Label* m_directions_label;
        sdl_gui::BaseButton* m_direction_btn_left;
        sdl_gui::BaseButton* m_direction_btn_right;
        sdl_gui::BaseButton* m_direction_btn_up;
        sdl_gui::BaseButton* m_direction_btn_down;

        sdl_gui::Label* m_select_bar_label;
        sdl_gui::SelectableGroup* m_select_bar_group;
        sdl_gui::CheckBox* m_select_bar_checkbox_h;
        sdl_gui::Label* m_select_bar_label_h;
        sdl_gui::CheckBox* m_select_bar_checkbox_v;
        sdl_gui::Label* m_select_bar_label_v;

        sdl_gui::Label* m_progress_bar_note;

        bool m_playing;
        bool m_loop;
        float m_step;
        float m_timer;
        float m_value_direction_h;
        float m_value_direction_v;

        bool m_run_sim_h;
        bool m_run_sim_v;
        //</f> /Progress Bar control

        //<f> Sliders
        sdl_gui::HSlider* m_h_slider;
        sdl_gui::VSlider* m_v_slider;

        sdl_gui::Label* m_h_slider_label;
        sdl_gui::Label* m_v_slider_label;

        sdl_gui::VScrollbar* m_scrollbar;
        sdl_gui::Label* m_scroll_label;
        //</f> /Sliders

    private:

        //<f> Progress Bar CallBacks
        void UpdateRatioLabel(sdl_gui::Label* label, sdl_gui::ProgressBar* Bar);
        void ChangeDirection(const std::string& direction);
        void PlayPause(bool play);
        void ChangeLoop(sdl_gui::SelectableButton* checkbox);
        //</f> /Progress Bar CallBacks
};

#endif //BARS_PANEL_HPP
