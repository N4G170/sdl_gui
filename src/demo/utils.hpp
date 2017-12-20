#include "SDL.h"
#include <string>

#ifndef UTILS_HPP
#define UTILS_HPP

inline void SaveBMP(SDL_Renderer* renderer_ptr, const std::string& file_name, SDL_Texture* texture_ptr, int width, int height)
{
    // Create an empty RGB surface that will be used to create the bmp file
    SDL_Surface* pScreenShot = SDL_CreateRGBSurface(0, width, height, 32, 0x00ff0000, 0x0000ff00, 0x000000ff, 0xff000000);

    if(pScreenShot)
    {
        SDL_SetRenderTarget(renderer_ptr, texture_ptr);
        // Read the pixels from the current render target and save them onto the surface
        SDL_RenderReadPixels(renderer_ptr, NULL, SDL_PIXELFORMAT_RGBA8888, pScreenShot->pixels, pScreenShot->pitch);

        // Create the bmp file
        SDL_SaveBMP(pScreenShot, file_name.c_str());

        // Destroy the surface
        SDL_FreeSurface(pScreenShot);
        SDL_SetRenderTarget(renderer_ptr, nullptr);
    }
}

inline void SaveBMP(const std::string& file_name, SDL_Surface* surface_ptr)
{
    if(surface_ptr)
    {
        // Create the bmp file
        SDL_SaveBMP(surface_ptr, file_name.c_str());
    }
}

#endif//UTILS_HPP
