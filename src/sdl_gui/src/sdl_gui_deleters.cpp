#include "sdl_gui_deleters.hpp"
#include <sdl_gui_log.hpp>

namespace sdl_gui
{

void SDLDeleters::operator()(SDL_Texture *texture)
{
    // Log("Calling destroy for SDL_DestroyTexture object pointer... \n");
    SDL_DestroyTexture( texture );
}

void SDLDeleters::operator()(TTF_Font* font)
{
    // Log("Calling close for TTF_Font object pointer... \n");
    TTF_CloseFont(font);
}

// void DeleteSDLTexture(SDL_Texture* texture)
// {
//     Log("Calling destroy for SDL_DestroyTexture object pointer... \n");
//     SDL_DestroyTexture( texture );
// }

}//namespace
