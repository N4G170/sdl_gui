#include "SDL.h"
#include <memory>
#include "sdl_gui_texture.hpp"
#include "sdl_gui_deleters.hpp"

namespace sdl_gui
{

#ifndef SDL_GUI_LOCAL_TEXTURE_HPP
#define SDL_GUI_LOCAL_TEXTURE_HPP

class LocalTexture
{
    public:
        /* Default constructor */
        LocalTexture();

        /**
         * The texture will be created with TARGET texture access type and pixel format RGBA8888
         */
        LocalTexture(SDL_Renderer* renderer_ptr, int w, int h);

        /**
         * NOTE: This class will create a copy of the texture and manage it
         * \return [description]
         */
        LocalTexture(SDL_Renderer* renderer_ptr, SDL_Texture* texture_ptr);

        /**
         * Will create a copy of the pointer
         */
        LocalTexture(SDL_Renderer* renderer_ptr, const Texture& texture);

        /* Default destructor */
        virtual ~LocalTexture() noexcept;

        /* Copy constructor */
        LocalTexture(const LocalTexture& other);
        /* Move constructor */
        LocalTexture(LocalTexture&& other) noexcept;

        /* Copy operator */
        LocalTexture& operator= (const LocalTexture& other);
        /* Move operator */
        LocalTexture& operator= (LocalTexture&& other) noexcept;

        //<f> Getters/Setters
        SDL_Rect* SourceRect() const { return m_src_rect; };
        void SourceRect(SDL_Rect src_rect);
        SDL_Rect* DestinationRect() const { return m_dst_rect; };
        void DestinationRect(SDL_Rect dst_rect);
        void ClearSourceRect();
        void ClearDestinationRect();
        SDL_Colour ColourModulation() const { return m_colour_modulation; };
        void ColourModulation(const SDL_Colour& colour);
        int AlphaModulation() const { return m_colour_modulation.a; };
        void AlphaModulation(int alpha){ m_colour_modulation.a = alpha;};

        void TexturePtr(SDL_Texture* texture);
        SDL_Texture* TexturePtr() const {return m_texture.get(); }
        //</f>

        void Render();
        void Render(SDL_Rect* src_rect, SDL_Rect* dst_rect);

        void Resize(int new_w, int new_h, bool scale_content = false);
        /**
         * \brief Clears the content of the texture and sets it to the given colour (defaul {255,255,255,0})
         */
        void Clear(const SDL_Colour& colour = {255,255,255,0});

    private:
        // vars and stuff
        //LocalTexture DOES NOT OWN THIS POINTER
        SDL_Renderer* m_renderer_ptr;

        std::unique_ptr<SDL_Texture, SDLDeleters> m_texture;

        SDL_Rect* m_src_rect;
        SDL_Rect* m_dst_rect;
        SDL_Colour m_colour_modulation;

        void ColourModulation(Uint8 r, Uint8 g, Uint8 b);
};

#endif //SDL_GUI_LOCAL_TEXTURE_HPP

}
