#include <string>
#include "SDL.h"

namespace sdl_gui
{

#ifndef SDL_GUI_CONSTANTS_HPP
#define SDL_GUI_CONSTANTS_HPP

//<f> Unicode
const int c_new_line_code{int('\n')};
const int c_horizontal_tab_code{int('\t')};
//</f>

//<f> Default File Paths

const std::string c_img_white_dot{"data/sdl_gui/img/white_dot.png"};
const std::string c_img_white_circle{"data/sdl_gui/img/white_circle.png"};
const std::string c_button_base{"data/sdl_gui/img/button_base.png"};

const std::string c_img_btn_active{"data/sdl_gui/img/active.png"};
const std::string c_img_btn_inactive{"data/sdl_gui/img/inactive.png"};
const std::string c_img_btn_over{"data/sdl_gui/img/over.png"};
const std::string c_img_btn_pressed{"data/sdl_gui/img/pressed.png"};

const std::string c_default_font_path{"data/sdl_gui/font/DejaVuSansMono.ttf"};

//</f>

//<f> Colours
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
//</f>

//<f> Default Sizes
const int c_default_font_size{12};
//</f>

//<f> Buttons Defaults
const SDL_Colour c_active_colour {255,255,255,255};
const SDL_Colour c_inactive_colour {128,128,128,128};
const SDL_Colour c_over_colour {128,128,128,255};
const SDL_Colour c_pressed_colour {96,96,96,255};
const SDL_Colour c_selected_colour {64,64,64,255};
//</f>

//<f> Default Ints
const int c_default_anim_fps{24};
const int c_tooltip_render_index{-50};
//</f>
#endif //SDL_GUI_CONSTANTS_HPP

}//namespace
