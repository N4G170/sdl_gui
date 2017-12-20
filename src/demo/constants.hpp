#ifndef CONSTANTS_HPP
#define CONSTANTS_HPP

#include <string>
#include "SDL.h"
#include <cmath>

constexpr float SquareRoot(float number)
{
    return std::sqrt(number);
}

namespace Math
{
    const float PI{3.14159265358979};
}

namespace Colour
{
    const SDL_Colour Black {0,0,0,255};
    const SDL_Colour White {255,255,255,255};
    const SDL_Colour Red {255,0,0,255};
    const SDL_Colour Green {0,255,0,255};
    const SDL_Colour Blue {0,0,255,255};
    const SDL_Colour Light_Blue {173,216,230,255};
    const SDL_Colour Cyan {0,255,255,255};
    const SDL_Colour Yellow {255,255,0,255};
    const SDL_Colour Magenta {255,0,255,255};
    const SDL_Colour Grey {190,190,190,255};

    const SDL_Colour Rain_Drop_Blue {3,74,236,255};
    const SDL_Colour Storm_Petrel {0,24,72,255};

    const SDL_Colour Venus_Yellow {255, 230, 181, 255};//pale yellowish
}

const std::string window_name {"Random SDL stuff"};
const int window_width {1024};
const int window_height {768};
const int window_centre_x {(window_width/2)};
const int window_centre_y {(window_height/2)};
const std::string font_path {"data/font/DejaVuSansMono.ttf"};
const int font_size {14};

namespace StateName
{
    const std::string Menu {"menu"};
    const std::string Rain {"rain"};
    const std::string Snake {"snake"};
    const std::string Solar_System {"solar_system"};
    const std::string Map_Demo {"map_demo"};
    const std::string Influence_Wars {"influence_wars"};
    const std::string Particles {"particles"};
}

//Rain
const int drop_count = 1234;
const float rain_gravity = 9.80665;// m/s^2

//Snake
const int snake_grid_size = 40;
const int cell_size = 15;

//Solar - values as int as they represent pixels
// const int sun_radius = 80;// 695,508 km
const int sun_radius = 30;// 695,508 km
const int mercury_radius = 4;// 2,439.7 km
const int venus_radius = 8;// 6,051.8 km
const int terra_radius = 8;// 6,371 km
const int xeno_radius = 12;
const float xeno_orbit_rotation_angle{0.001f};

// const int mercury_radius = std::round(2439.7/695508*sun_radius);// 2,439.7 km
// const int venus_radius = std::round(6051.8/695508*sun_radius);// 6,051.8 km
// const int terra_radius = std::round(6371/695508*sun_radius);// 6,371 km
// const int mars_radius = std::round(3389.5/695508*sun_radius);// 3,389.5 km
// const int jupiter_radius = std::round(69911/695508*sun_radius);// 69,911 km
// const int saturn_radius = std::round(58232/695508*sun_radius);// 58,232 km
// const int uranus_radius = std::round(25362/695508*sun_radius);// 25,362 km
// const int neptune_radius = std::round(24622/695508*sun_radius);// 24,622 km
// const int pluto_radius = std::round(1151/695508*sun_radius);// 1,151 km

#endif //CONSTANTS_HPP
