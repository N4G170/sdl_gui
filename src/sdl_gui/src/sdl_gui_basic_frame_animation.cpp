#include "sdl_gui_basic_frame_animation.hpp"
#include "sdl_gui_constants.hpp"
#include "sdl_gui_log.hpp"
#include <utility>
#include <algorithm>

namespace sdl_gui
{

//<f> Constructors & operator=
BasicFrameAnimation::BasicFrameAnimation(GuiMainPointers main_pointers, const Position& position, const Dimensions& size) :
    GuiElement{main_pointers, position, size}, m_frame{nullptr}, m_frames{},
    m_fps{c_default_anim_fps}, m_time_per_frame{1.f/c_default_anim_fps}, m_current_animation_time{0},
    m_current_frame_time{0}, m_current_frame{0}, m_playing{false}, m_loop{false}, m_backwards{false} {}

BasicFrameAnimation::~BasicFrameAnimation() noexcept
{

}

BasicFrameAnimation::BasicFrameAnimation(const BasicFrameAnimation& other) : GuiElement{other},
    m_frame{other.m_frame}, m_fps{other.m_fps}, m_time_per_frame{other.m_time_per_frame}, m_current_animation_time{other.m_current_animation_time},
    m_current_frame_time{other.m_current_frame_time}, m_current_frame{other.m_current_frame}, m_playing{other.m_playing}, m_loop{other.m_loop},
    m_backwards{other.m_backwards}
{
    for(auto& frame : other.m_frames)
    {
        m_frames.push_back(std::unique_ptr<Image>{new Image{*frame.get()}});
    }
}

BasicFrameAnimation::BasicFrameAnimation(BasicFrameAnimation&& other) noexcept : GuiElement{std::move(other)},
    m_frame{std::move(other.m_frame)}, m_frames{std::move(other.m_frames)},
    m_fps{std::move(other.m_fps)}, m_time_per_frame{std::move(other.m_time_per_frame)},
    m_current_animation_time{std::move(other.m_current_animation_time)},
    m_current_frame_time{std::move(other.m_current_frame_time)}, m_current_frame{std::move(other.m_current_frame)}, m_playing{std::move(other.m_playing)},
    m_loop{std::move(other.m_loop)}, m_backwards{std::move(other.m_backwards)} {}

BasicFrameAnimation& BasicFrameAnimation::operator=(const BasicFrameAnimation& other)
{
    if(this != &other)//not same ref
    {
        auto tmp(other);
        *this = std::move(tmp);
    }

    return *this;
}

BasicFrameAnimation& BasicFrameAnimation::operator=(BasicFrameAnimation&& other) noexcept
{
    if(this != &other)//not same ref
    {
        GuiElement::operator=(std::move(other));
        m_frame = std::move(other.m_frame);
        m_frames = std::move(other.m_frames);
        m_fps = std::move(other.m_fps);
        m_time_per_frame = std::move(other.m_time_per_frame);
        m_current_animation_time = std::move(other.m_current_animation_time);
        m_current_frame_time = std::move(other.m_current_frame_time);
        m_current_frame = std::move(other.m_current_frame);
        m_playing = std::move(other.m_playing);
        m_loop = std::move(other.m_loop);
        m_backwards = std::move(other.m_backwards);
    }
    return *this;
}
//</f>

//<f> Overrides GUIElement
// void BasicFrameAnimation::Input(const SDL_Event& event)
// {
//
// }

// void BasicFrameAnimation::FixedLogic(float fixed_delta_time)
// {
//
// }

void BasicFrameAnimation::Logic(float delta_time)
{
    if(m_playing)
    {
        m_current_frame_time += delta_time;
        m_current_animation_time += delta_time;//total time

        if(m_current_frame_time >= m_time_per_frame)//reached end of frame
        {
            m_current_frame_time = 0;//reset counter

            if(m_backwards)
            {
                --m_current_frame;//move to next frame
                if(m_current_frame <= 0)//reached end of vector, loop to first (if m_loop)
                {
                    //reset
                    m_current_frame = m_frames.size() - 1;
                    m_current_animation_time = 0;

                    if(!m_loop)//stop
                    {
                        Stop();
                        //stay at last frame
                        m_current_frame = 0;
                    }
                }
            }
            else
            {
                ++m_current_frame;//move to next frame
                if(m_current_frame >= m_frames.size())//reached end of vector, loop to first (if m_loop)
                {
                    //reset
                    m_current_frame = 0;
                    m_current_animation_time = 0;

                    if(!m_loop)//stop
                    {
                        Stop();
                        //stay at last frame
                        m_current_frame = m_frames.size() - 1;
                    }
                }
            }

            //update frame pointer
            m_frame = m_frames[m_current_frame].get();
        }
    }
}

void BasicFrameAnimation::Render(float delta_time)
{
    Render(delta_time, m_main_pointers.main_camera_ptr);
}

void BasicFrameAnimation::Render(float delta_time, Camera* camera)
{
    if(!m_render)
        return;

    SDL_Rect dst{RenderRect()};

    //apply camera position
    if(!m_transform.ParentViewport())//if inside viewport we cant add camera position
    {
        dst.x += camera->CameraPosition().x;
        dst.y += camera->CameraPosition().y;
    }

    if(camera->RectInsideCamera(dst))
    {
        // std::cout<<m_current_frame<<std::endl;
        if(m_current_frame < m_frames.size())
        {
            if(m_frame)
                m_frame->Render(delta_time, camera);
        }
    }
}
//</f>

//<f> Getters/Setters

Image* BasicFrameAnimation::AddFrame(std::string file_path, SDL_Rect* src_rect)
{
    Image* img = new Image{m_main_pointers, {0,0}, Size()};
    img->Parent(this);
    img->ChangeTexture(file_path);
    if(src_rect != nullptr)
        img->SetSourceRect(*src_rect);

    m_frames.push_back( std::unique_ptr<Image>{std::move(img)} );
    img = nullptr;

    if(m_frame == nullptr)
        m_frame = m_frames[0].get();

    return m_frames.back().get();
}
//</f>

//<f> Methods
void BasicFrameAnimation::Play()
{
    m_playing = true;
}

void BasicFrameAnimation::Pause()
{
    m_playing = false;
}

void BasicFrameAnimation::Stop()
{
    m_playing = false;
    m_current_frame = 0;
}
//</f>

}//namespace
