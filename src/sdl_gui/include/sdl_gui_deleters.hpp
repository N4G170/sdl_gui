#include "SDL.h"
#include "SDL_ttf.h"

namespace sdl_gui
{

#ifndef SDL_GUI_DELETERS_HPP
#define SDL_GUI_DELETERS_HPP

struct SDLDeleters
{
    void operator()(SDL_Texture* texture);
    void operator()(TTF_Font* font);
};

// void DeleteSDLTexture(SDL_Texture* texture);

#endif //SDL_GUI_DELETERS_HPP

}//namespace
