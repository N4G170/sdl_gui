#include "sdl_gui_basic_frame_animation.hpp"
#include "sdl_gui_base_button.hpp"
#include "sdl_gui_label.hpp"
#include "sdl_gui_manager.hpp"
#include <memory>

#ifndef ANIMATION_DEMO_HPP
#define ANIMATION_DEMO_HPP

class AnimationPanel
{
    public:
        //<f> Constructors & operator=
        /** brief Default constructor */
        explicit AnimationPanel(sdl_gui::GuiManager* manager);
        /** brief Default destructor */
        virtual ~AnimationPanel() noexcept;

        /** brief Copy constructor */
        AnimationPanel(const AnimationPanel& other);
        /** brief Move constructor */
        AnimationPanel(AnimationPanel&& other) noexcept;

        /** brief Copy operator */
        AnimationPanel& operator= (const AnimationPanel& other);
        /** brief Move operator */
        AnimationPanel& operator= (AnimationPanel&& other) noexcept;
        //</f>

        //<f> State
        void Input(const SDL_Event& event);

        void FixedLogic(float fixed_delta_time);

        void Logic(float delta_time);

        void Render(SDL_Renderer* renderer, float delta_time);
        //</f>

        //<f> Virtual Methods

        //</f>

        //<f> Getters/Setters
        sdl_gui::GuiElement* Panel() const { return m_panel;}
        sdl_gui::Image* Background() const { return m_background; }
        sdl_gui::BasicFrameAnimation* Animation() const { return m_animation;}
        sdl_gui::BaseButton* PlayBtn() const { return m_play_btn;}
        sdl_gui::BaseButton* PauseBtn() const { return m_pause_btn;}
        sdl_gui::BaseButton* StopBtn() const { return m_stop_btn;}
        sdl_gui::BaseButton* IncreaseFpsBtn() const { return m_increase_fps;}
        sdl_gui::BaseButton* DecreaseFpsBtn() const { return m_decrease_fps;}
        sdl_gui::Label* FpsLabel() const { return m_fps_label;}
        sdl_gui::Label* CurrentFrameLabel() const { return m_current_frame_label;}

        sdl_gui::CheckBox* LoopCheckBox() const { return m_loop_checkbox; }
        sdl_gui::Label* CheckBoxLabel() const { return m_checkbox_label; }

        sdl_gui::CheckBox* BackwardsCheckBox() const { return m_backwards_checkbox; }
        sdl_gui::Label* BackwardsCheckBoxLabel() const { return m_backwards_checkbox_label; }

        void Position(sdl_gui::Position local_positon){ m_panel->LocalPosition(local_positon); }
        //</f>

    protected:
        // vars and stuff

    private:
        sdl_gui::GuiManager* m_gui_manager;
        sdl_gui::GuiElement* m_panel;
        sdl_gui::Image* m_background;

        sdl_gui::BasicFrameAnimation* m_animation;
        sdl_gui::BaseButton* m_play_btn;
        sdl_gui::BaseButton* m_pause_btn;
        sdl_gui::BaseButton* m_stop_btn;
        sdl_gui::BaseButton* m_increase_fps;
        sdl_gui::BaseButton* m_decrease_fps;
        sdl_gui::Label* m_fps_label;
        sdl_gui::Label* m_current_frame_label;

        sdl_gui::CheckBox* m_loop_checkbox;
        sdl_gui::Label* m_checkbox_label;
        sdl_gui::CheckBox* m_backwards_checkbox;
        sdl_gui::Label* m_backwards_checkbox_label;

        void ConfigElements();
        void SetCallbacks();
        void ChangeLoop(sdl_gui::SelectableButton* checkbox);
        void ChangeBackwards(sdl_gui::SelectableButton* checkbox);
};

#endif //ANIMATION_DEMO_HPP
