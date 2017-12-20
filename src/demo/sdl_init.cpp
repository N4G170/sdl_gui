#include "sdl_init.hpp"

bool InitSDL(SDL_Window*& window, SDL_Renderer*& renderer, const SDLInitConfig& config)
{


    //Initialize SDL
    int result = SDL_Init( SDL_INIT_VIDEO | SDL_INIT_AUDIO );
    if( result < 0 )
    {
        std::cout<<"SDL could not initialize! SDL_Error: "<<SDL_GetError()<<std::endl;

        return false;
    }
    else
    {
        //Create window
        window = SDL_CreateWindow( config.window_name.c_str(), config.window_x, config.window_y, config.window_w, config.window_h, config.window_flags);
        if( window == nullptr )
        {
            std::cout<<"Window could not be created! SDL_Error: "<<SDL_GetError()<<std::endl;

            return false;
        }
        else
        {
            //Create renderer for window
            renderer = SDL_CreateRenderer( window, config.renderer_index, config.renderer_flags );

            if( renderer == nullptr )
            {
                std::cout<<"Renderer could not be created! SDL Error: "<<SDL_GetError()<<std::endl;

                return false;
            }
            else
            {
                //Initialize renderer color
                SDL_SetRenderDrawColor( renderer, 0x35, 0x35, 0x35, 0xFF );

                //Initialize PNG loading
                int imgFlags = IMG_INIT_PNG;
                if( !( IMG_Init( imgFlags ) & imgFlags ) )
                {
                    std::cout<<"SDL_image could not initialize! SDL_image Error: "<<IMG_GetError()<<std::endl;

                    return false;
                }
                else//init fonts
                {
                    //Initialize SDL_ttf
                    if( TTF_Init() == -1 )
                    {
                       std::cout<<"SDL_ttf could not initialize! SDL_ttf Error: "<< TTF_GetError()<<std::endl;
                       return false;
                    }
                    // else
                    // {
                    //     //Initialize SDL_mixer - 44100 standard frequency(CD), 2 channels (stereo)
                    //     if( Mix_OpenAudio( 44100, MIX_DEFAULT_FORMAT, 2, 2048 ) < 0 )
                    //     {
                    //         std::cout<<"SDL_mixer could not initialize! SDL_mixer Error: "<< Mix_GetError()<<std::endl;
                    //         return false;
                    //     }
                    // }
                }
            }
        }
    }

    return true;
}

void TerminateSDL()
{
    //Quit SDL subsystems
    // Mix_Quit();
    TTF_Quit();
    IMG_Quit();
    SDL_Quit();
}

void DeleteSDLPointers(SDL_Window* window, SDL_Renderer* renderer)
{
    if(window)
        SDL_DestroyWindow(window);
    if(renderer)
        SDL_DestroyRenderer(renderer);
}
