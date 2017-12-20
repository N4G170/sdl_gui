#include "SDL.h"
#include "SDL_ttf.h"
#include "SDL_image.h"
#include <iostream>

#ifndef SDL_INIT_HPP
#define SDL_INIT_HPP
/**
 * \brief struck holding sdl init configurations
 */
struct SDLInitConfig
{
    //<f> Window
    std::string window_name {"SDL Window"};
    int window_x {SDL_WINDOWPOS_UNDEFINED};
    int window_y {SDL_WINDOWPOS_UNDEFINED};
    int window_w {1024};
    int window_h {768};
    Uint32 window_flags {SDL_WINDOW_SHOWN};
    //</f>

    //<f> Main Renderer
    int renderer_index {-1};
    Uint32 renderer_flags {SDL_RENDERER_ACCELERATED | SDL_RENDERER_TARGETTEXTURE};
    //</f>

    //<f> Image
    /**
     * \brief Var holding flags for image init.\n
     * Possible values IMG_INIT_JPG, IMG_INIT_PNG, IMG_INIT_TIF, to init more than one use bitwise or '|' ex: IMG_INIT_JPG | IMG_INIT_PNG
     */
    int image_flags {IMG_INIT_PNG};
    //</f>

    //<f> Audio

    //</f>
};

/**
 * \brief Struct with operator() overrides of SDL pointers correct destruction (to be used by unique_ptr)
 */
struct SDLDeleters
{
    void operator() (SDL_Window* window)
    {
        //MessageWriter::Instance()->WriteLineToConsole("Calling destroy for SDL_window object pointer...");
        std::cout << "Calling destroy for SDL_window object pointer... \n";
        SDL_DestroyWindow(window);
    }

    void operator() (SDL_Renderer* screen_renderer)
    {
        //MessageWriter::Instance()->WriteLineToConsole("Calling destroy for SDL_Renderer object pointer...");
        std::cout << "Calling destroy for SDL_Renderer object pointer... \n";
        SDL_DestroyRenderer(screen_renderer);
    }

    void operator() (TTF_Font* font)
    {
        //MessageWriter::Instance()->WriteLineToConsole("Calling destroy for TTF_Font object pointer...");
        std::cout << "Calling destroy for TTF_Font object pointer... \n";
        TTF_CloseFont( font );
    }
};

bool InitSDL(SDL_Window*& window, SDL_Renderer*& renderer, const SDLInitConfig& config = {});

/**
 * \brief Terminate SDL subsystems.\n
 * You are responsible for clearing any SDL pointer used (ex: window, renderer), after Terminate is called
 */
void TerminateSDL();

void DeleteSDLPointers(SDL_Window* window, SDL_Renderer* renderer);

#endif //SDL_INIT_HPP
