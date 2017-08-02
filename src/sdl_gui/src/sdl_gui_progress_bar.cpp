#include "sdl_gui_progress_bar.hpp"
#include <utility>
#include "sdl_gui_constants.hpp"
#include "sdl_gui_log.hpp"

namespace sdl_gui
{

//<f> Constructors & operator=
ProgressBar::ProgressBar(GuiMainPointers main_pointers, const Position& position, const Dimensions& size) :
    GuiElement{main_pointers, position, size},
    m_bg_texture{m_main_pointers.resource_manager_ptr->GetTexture(c_img_white_dot)}, m_bar_texture{m_main_pointers.resource_manager_ptr->GetTexture(c_img_white_dot)},
    m_bar_direction{ProgressBarDirection::RIGHT}, m_min_value{0}, m_max_value{100}, m_value{0}
{
    m_bg_texture.ColourModulation({0,0,0,255});
}

ProgressBar::~ProgressBar() noexcept {}

ProgressBar::ProgressBar(const ProgressBar& other) : GuiElement{other},
    m_bg_texture{other.m_bg_texture}, m_bar_texture{other.m_bar_texture},
    m_bar_direction{other.m_bar_direction}, m_min_value{other.m_min_value}, m_max_value{other.m_max_value}, m_value{other.m_value},
    m_value_changed_callback{other.m_value_changed_callback} {}

ProgressBar::ProgressBar(ProgressBar&& other) noexcept : GuiElement{other},
    m_bg_texture{std::move(other.m_bg_texture)}, m_bar_texture{std::move(other.m_bar_texture)},
    m_bar_direction{std::move(other.m_bar_direction)}, m_min_value{std::move(other.m_min_value)}, m_max_value{std::move(other.m_max_value)}, m_value{std::move(other.m_value)},
    m_value_changed_callback{std::move(other.m_value_changed_callback)} {}

ProgressBar& ProgressBar::operator=(const ProgressBar& other)
{
    if(this != &other)//not same ref
    {
        ProgressBar tmp(other);
        *this = std::move(tmp);
    }

    return *this;
}

ProgressBar& ProgressBar::operator=(ProgressBar&& other) noexcept
{
    if(this != &other)
    {
        GuiElement::operator=(other);
        m_bg_texture = std::move(other.m_bg_texture);
        m_bar_texture = std::move(other.m_bar_texture);
        m_bar_direction = std::move(other.m_bar_direction);
        m_min_value = std::move(other.m_min_value);
        m_max_value = std::move(other.m_max_value);
        m_value = std::move(other.m_value);
        m_value_changed_callback = std::move(other.m_value_changed_callback);
    }
    return *this;
}
//</f>

//<f> Overrides GuiElement
void ProgressBar::Render(float delta_time)
{
    Render(delta_time, m_main_pointers.main_camera_ptr);
}

void ProgressBar::Render(float delta_time, Camera* camera)
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
        m_bg_texture.Render(nullptr, &dst);

        switch(m_bar_direction)
        {
            case ProgressBarDirection::LEFT: RenderLeft(dst); break;
            case ProgressBarDirection::RIGHT: RenderRight(dst); break;
            case ProgressBarDirection::UP: RenderUp(dst); break;
            case ProgressBarDirection::DOWN: RenderDown(dst); break;
        }
    }
}
//</f>

void ProgressBar::RenderLeft(SDL_Rect& dst)
{
    int ten_percent{dst.w/10 > dst.h/10 ? dst.h/10 : dst.w/10};

    int prev_w = dst.w;
    dst.w =  (dst.w - (ten_percent * 2)) * (float)m_value/m_max_value;
    dst.x += prev_w - ten_percent - dst.w;

    dst.y += ten_percent;
    dst.h -= (ten_percent * 2);

    m_bar_texture.Render(nullptr, &dst);
}

void ProgressBar::RenderRight(SDL_Rect& dst)
{
    int ten_percent{dst.w/10 > dst.h/10 ? dst.h/10 : dst.w/10};

    dst.x += ten_percent;
    dst.w =  (dst.w - (ten_percent * 2)) * (float)m_value/m_max_value;

    dst.y += ten_percent;
    dst.h -= (ten_percent * 2);

    m_bar_texture.Render(nullptr, &dst);
}

void ProgressBar::RenderUp(SDL_Rect& dst)
{
    int ten_percent{dst.w/10 > dst.h/10 ? dst.h/10 : dst.w/10};

    dst.x += ten_percent;
    dst.w -= (ten_percent * 2);

    int prev_h = dst.h;
    dst.h = (dst.h - (ten_percent * 2)) * (float)m_value/m_max_value;
    dst.y += prev_h - ten_percent - dst.h;

    m_bar_texture.Render(nullptr, &dst);
}

void ProgressBar::RenderDown(SDL_Rect& dst)
{
    int ten_percent{dst.w/10 > dst.h/10 ? dst.h/10 : dst.w/10};

    dst.x += ten_percent;
    dst.w -= (ten_percent * 2);

    dst.y += ten_percent;
    dst.h = (dst.h - (ten_percent * 2)) * (float)m_value/m_max_value;

    m_bar_texture.Render(nullptr, &dst);
}

}//namespace
