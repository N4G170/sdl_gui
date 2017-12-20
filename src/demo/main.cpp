#include <iostream>
#include "SDL.h"
#include "sdl_init.hpp"
#include "sdl_gui_demo.hpp"
#include <chrono>

int main(int argc, char* argv[])
{
    SDL_Window* window{nullptr};
    SDL_Renderer* renderer{nullptr};

    //init SDL subsystems
    SDLInitConfig sdl_config{};//load default values
    bool init_result = InitSDL(window, renderer, sdl_config);

    if(!init_result)//failed to initialize sdl subsystems
    {
        //terminate any initialized sdl subsystems
        TerminateSDL();
        DeleteSDLPointers(window, renderer);//they are deleted in a sepecific way
        window = nullptr;
        renderer = nullptr;

        return -2;
    }
    else//sdl started ok
    {
        //sdl_gui vars
        sdl_gui::ResourceManager resource_manager{ renderer };
        GuiDemo demo{ renderer, &resource_manager, sdl_config };

        bool quit{false};

        //Create Event handler
        SDL_Event event;
        float frame_cap {1.f / 60 * 1000};
        float last_frame_time {0};

        float fixed_frame_time {0.03};
        float accumulated_time {0};

        while(!quit)
        {
            auto start_time(std::chrono::high_resolution_clock::now());
            float fps{0};

            accumulated_time += last_frame_time;

            //Handle events on queue
            while( SDL_PollEvent( &event ) != 0 )
            {

                // SDL_Texture* f1 = SDL_CreateTextureFromSurface(renderer.get(), TTF_RenderText_Blended(f, std::u32string("Ups \u00c0 tester"), {255,255,255,255}));

                //TODO:0 send exit code to states
                //User requests quit
                if( event.type == SDL_QUIT)
                    quit = true;
                else if(event.type == SDL_KEYDOWN)
                {
                    switch(event.key.keysym.sym)
                    {
                        case SDLK_ESCAPE: quit = true; break;
                    }
                }

                demo.Input(event);

            }

            //Fixed time step Logic
            while(accumulated_time >= fixed_frame_time)
            {
                demo.FixedLogic(fixed_frame_time);
                accumulated_time -= fixed_frame_time;
            }

            //Variable time step Logic
            demo.Logic(last_frame_time);

            //Clear screen
            SDL_SetRenderDrawColor( renderer, 0x00, 0x00, 0x00, 0x00 );
            SDL_RenderClear( renderer );

			demo.Render(renderer, last_frame_time);

			//Update screen
			SDL_RenderPresent( renderer );

            //Update frame timers
            auto delta_time(std::chrono::high_resolution_clock::now() - start_time);
            float frame_time = std::chrono::duration_cast< std::chrono::duration<float, std::milli> >(delta_time).count();

            //fps cap
            if(frame_time < frame_cap)
            {
                SDL_Delay(frame_cap - frame_time);
                frame_time = frame_cap;
            }
            frame_time /= 1000.f;
            fps = 1.f / frame_time;

            last_frame_time = frame_time;

            SDL_SetWindowTitle(window, ( sdl_config.window_name +" - "+ std::to_string(fps)+" FPS").c_str() );
        }// while(!quit)
    }
    //terminate SDL subsystems
    TerminateSDL();
    DeleteSDLPointers(window, renderer);//they are deleted in a sepecific way
    window = nullptr;
    renderer = nullptr;

    return 0;
}
