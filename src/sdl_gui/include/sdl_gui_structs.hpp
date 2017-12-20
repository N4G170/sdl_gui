#include <map>
#include "SDL.h"
#include "sdl_gui_enums.hpp"
#include <functional>
#include "sdl_gui_resource_manager.hpp"

namespace sdl_gui
{

#ifndef SDL_GUI_STRUCTS
#define SDL_GUI_STRUCTS

class Camera;

struct Position
{
    float x, y;
    bool operator == (const Position& other){ return other.x == x && other.y == y; }
    bool operator != (const Position& other){ return other.x != x || other.y != y; }
    Position operator + (const Position& other){ return {this->x + other.x, this->y + other.y}; }
    Position& operator += (const Position& other){ this->x += other.x; this->y += other.y; return *this; }
    Position operator - (const Position& other){ return {this->x - other.x, this->y - other.y}; }
    Position& operator -= (const Position& other){ this->x -= other.x; this->y -= other.y; return *this; }
};

struct Dimensions
{
    float w, h;
    float Area(){ return w * h; }//just to stop linter from nagging about unused vars 'w' & 'h' (they are used but not here)
};

struct Scale
{
    float x, y;

    Scale operator * (const Scale& other) { return {x * other.x, y * other.y}; }
    Scale& operator *= (const Scale& other) { x *= other.x;  y *= other.y; return *this; } 
};

/**
 * \brief Store pointer for elements used by gui elements.
 * \nNOTE: this struct does not own any of them.
 */
struct GuiMainPointers
{
    SDL_Renderer* main_renderer_ptr;
    ResourceManager* resource_manager_ptr;
    Camera* main_camera_ptr;
};

// struct Rectangle
// {
//     float x;
//     float y;
//     float w;
//     float h;
//
//
// };

struct InputKey
{
    /**
     * SDL Key/Button ID
     */
    Uint8 id;
    /**
     * Key state flag (up/down/click/...)
     */
    int state_flags;
    /**
     * if != null called on key down
     */
    std::function<void()> OnDown;
    /**
     * if != null called on key up
     */
    std::function<void()> OnUp;
    /**
     * if != null called when holding key
     */
    std::function<void()> OnHold;
    /**
     * if != null called on click
     */
    std::function<void()> OnClick;
};

#endif//SDL_GUI_STRUCTS
}//namespace
