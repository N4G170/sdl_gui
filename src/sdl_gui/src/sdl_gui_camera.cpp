#include "sdl_gui_camera.hpp"
#include <utility>
#include "sdl_gui_utils.hpp"
#include "sdl_gui_log.hpp"

namespace sdl_gui
{

//<f> Constructors & operator=
Camera::Camera() : m_position{0,0}, m_size{0,0}, m_min_depth{0}, m_max_depth{10}
{

}

Camera::Camera(const Position& position, const Dimensions& size) : m_position{position}, m_size{size}, m_min_depth{0}, m_max_depth{10}
{

}

Camera::~Camera() noexcept
{

}

Camera::Camera(const Camera& other) : m_position{other.m_position}, m_size{other.m_size}, m_min_depth{other.m_min_depth}, m_max_depth{other.m_max_depth}
{

}

Camera::Camera(Camera&& other) noexcept : m_position{std::move(other.m_position)}, m_size{std::move(other.m_size)},
    m_min_depth{std::move(other.m_min_depth)}, m_max_depth{std::move(other.m_max_depth)}
{

}

Camera& Camera::operator=(const Camera& other)
{
    if(this != &other)//not same ref
    {
        Camera tmp(other);
        *this = std::move(tmp);
    }

    return *this;
}

Camera& Camera::operator=(Camera&& other) noexcept
{
    m_position = std::move(other.m_position);
    m_size = std::move(other.m_size);
    m_min_depth = std::move(other.m_min_depth);
    m_max_depth = std::move(other.m_max_depth);
    return *this;
}
//</f>

//<f> Methods
bool Camera::RectInsideCamera(const SDL_Rect& other)
{
    SDL_Rect camera_rect{CameraRect()};
    return RectIntersect(camera_rect, other);
}
//</f>

}
