#include "SDL.h"
#include <string>

namespace sdl_gui
{

#ifndef SDL_GUI_TEXTURE_HPP
#define SDL_GUI_TEXTURE_HPP

class Texture
{
    public:
        /* Default constructor */
        Texture();
        Texture(SDL_Renderer* renderer_ptr, SDL_Texture* texture_ptr, const std::string& image_path);
        /* Default destructor */
        virtual ~Texture() noexcept;

        /* Copy constructor */
        Texture(const Texture& other);
        /* Move constructor */
        Texture(Texture&& other) noexcept;

        /* Copy operator */
        Texture& operator= (const Texture& other);
        /* Move operator */
        Texture& operator= (Texture&& other) noexcept;

        //<f> Getters/Setters
        void ColourModulation(const SDL_Colour& colour){ m_colour_modulation = colour; }
        SDL_Colour ColourModulation() const { return m_colour_modulation; }

        Uint8 RedModulation() const { return m_colour_modulation.b; }
        void RedModulation(Uint8 red){ m_colour_modulation.b = red;}

        Uint8 GreenModulation() const { return m_colour_modulation.b; }
        void GreenModulation(Uint8 green){ m_colour_modulation.b = green;}

        Uint8 BlueModulation() const { return m_colour_modulation.b; }
        void BlueModulation(Uint8 blue){ m_colour_modulation.b = blue;}

        Uint8 AlphaModulation() const { return m_colour_modulation.a; }
        void AlphaModulation(Uint8 alpha){ m_colour_modulation.a = alpha;}

        void ChangeTexturePtr(SDL_Texture* new_texture) { m_texture_ptr = new_texture; }

        std::string ImagePath() const { return m_image_path; }
        //</f>

        void Render(SDL_Rect* src_rect, SDL_Rect* dst_rect);

        // SDL_Texture* DuplicateTexture();

    private:
        // vars and stuff
        //Texture DOES NOT OWN THIS POINTER
        SDL_Renderer* m_renderer_ptr;
        //Texture DOES NOT OWN THIS POINTER IF m_manage_memory == FALSE
        SDL_Texture* m_texture_ptr;

        SDL_Colour m_colour_modulation;
        std::string m_image_path;
};

#endif //TEXTURE_HPP

}
