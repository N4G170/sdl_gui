#include "SDL.h"
#include "sdl_gui_structs.hpp"

namespace sdl_gui
{

#ifndef CAMERA_HPP
#define CAMERA_HPP

class Camera
{
    public:
        //<f> Constructors & operator=
        /* Default constructor */
        Camera();
        Camera(const Position& m_position, const Dimensions& m_size);
        /* Default destructor */
        virtual ~Camera() noexcept;

        /* Copy constructor */
        Camera(const Camera& other);
        /* Move constructor */
        Camera(Camera&& other) noexcept;

        /* Copy operator */
        Camera& operator= (const Camera& other);
        /* Move operator */
        Camera& operator= (Camera&& other) noexcept;
        //</f>

        //<f> Methods
        SDL_Rect CameraRect() const { return {static_cast<int>(m_position.x), static_cast<int>(m_position.y), static_cast<int>(m_size.w), static_cast<int>(m_size.h)}; }

        bool RectInsideCamera(const SDL_Rect& other);
        //</f>

        //<f> Getters/Setters
        Position CameraPosition() const { return m_position; }
        void CameraPosition(const Position& new_position) { m_position = new_position; }
        Dimensions CameraSize() const { return m_size; }
        void CameraSize(const Dimensions& new_size) { m_size = new_size; }
        //</f>

    private:
        // vars and stuff
        Position m_position;
        Dimensions m_size;

        //z index interval for rendering elements ex: 0-10, render elements from z{0} to z{10} inclusive
        float m_min_depth;
        float m_max_depth;
};

#endif //CAMERA_HPP
}
