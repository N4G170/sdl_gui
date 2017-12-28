#include "sdl_gui_element.hpp"
#include "sdl_gui_image.hpp"
#include <vector>
#include <cmath>
#include <memory>
#include <algorithm>

namespace sdl_gui
{

#ifndef SDL_GUI_BASIC_FRAME_ANIMATION_HPP
#define SDL_GUI_BASIC_FRAME_ANIMATION_HPP

class BasicFrameAnimation : public GuiElement
{
    public:
        //<f> Constructors & operator=
        /** brief Default constructor */
        BasicFrameAnimation(GuiMainPointers main_pointers, const Position& position, const Dimensions& size);
        /** brief Default destructor */
        virtual ~BasicFrameAnimation() noexcept;

        /** brief Copy constructor */
        BasicFrameAnimation(const BasicFrameAnimation& other);
        /** brief Move constructor */
        BasicFrameAnimation(BasicFrameAnimation&& other) noexcept;

        /** brief Copy operator */
        BasicFrameAnimation& operator= (const BasicFrameAnimation& other);
        /** brief Move operator */
        BasicFrameAnimation& operator= (BasicFrameAnimation&& other) noexcept;
        //</f>

        //<f> Overrides GUIElement
        // void Input(const SDL_Event& event) override;

        // void FixedLogic(float fixed_delta_time) override;
        void Logic(float delta_time) override;

        void Render(float delta_time) override;
        void Render(float delta_time, Camera* camera) override;
        //</f>

        //<f> Virtual Methods

        //</f>

        //<f> Getters/Setters
        /**
         * \brief Set animation FPS. Value will never be < 1 (std::max(fps, 1))
         */
        void FPS(int fps) { m_fps = std::max(fps, 1); m_time_per_frame = 1.f/m_fps; }
        int FPS() const { return m_fps; }

        void IncreaseFPS(int value){ FPS(FPS() + value); }
        void DecreaseFPS(int value){ FPS(FPS() - value); }

        Image* AddFrame(std::string file_path, SDL_Rect* src_rect = nullptr);

        Image* GetFrame(int frame_index){ return frame_index < m_frames.size()? m_frames[frame_index].get() : nullptr;  }

        int FrameIndex() const { return m_current_frame; }
        int TotalFrames() const { return m_frames.size(); }

        bool IsPlaying() const { return m_playing; }
        bool Loop() const { return m_loop; }
        void Loop(bool loop){ m_loop = loop; }
        bool Backwards() const { return m_backwards; }
        void Backwards(bool backwards){ m_backwards = backwards; }
        //</f>

        //<f> Methods
        void Play();
        void Pause();
        void Stop();
        //</f>

    protected:
        // vars and stuff
        Image* m_frame;
        //One image per frame
        std::vector<std::unique_ptr<Image>> m_frames;

        int m_fps;
        float m_time_per_frame;
        float m_current_animation_time;
        float m_current_frame_time;
        int m_current_frame;

        bool m_playing;
        bool m_loop;
        bool m_backwards;

    private:
};

#endif //SDL_GUI_BASIC_FRAME_ANIMATION_HPP

}//namespace
