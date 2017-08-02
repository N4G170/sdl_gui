#include "sdl_gui_texture_utils.hpp"

namespace sdl_gui
{
SDL_Texture* CreateSDLTexture(SDL_Renderer* renderer_ptr, int w, int h, Uint32 pixel_format, int access_type)
{
    return SDL_CreateTexture(renderer_ptr, pixel_format, access_type, w, h);
}

void ClearSDLTexture(SDL_Renderer* renderer_ptr, SDL_Texture* texture, const SDL_Colour& colour)
{
    SDL_SetRenderTarget(renderer_ptr, texture);
    SDL_SetTextureBlendMode(texture, SDL_BLENDMODE_BLEND);
    SDL_SetRenderDrawColor(renderer_ptr, colour.r, colour.g, colour.b, colour.a);//transparência
    SDL_RenderClear(renderer_ptr);
    SDL_SetRenderTarget(renderer_ptr, nullptr);
}

SDL_Texture* ResizeSDLTexture(SDL_Renderer* renderer_ptr, SDL_Texture* original, int new_w, int new_h, bool scale_content)
{
    Uint32 format{0};
    int access{0};
    SDL_Rect dst{0,0,0,0};
    //get current format and access
    SDL_QueryTexture(original, &format, &access, &dst.w, &dst.h);

    SDL_Texture* new_texture = SDL_CreateTexture(renderer_ptr, format, access, new_w, new_h);

    //render current content on the new texture
    SDL_SetRenderTarget(renderer_ptr, new_texture);

    if(scale_content)
        SDL_RenderCopy(renderer_ptr, original, nullptr, nullptr);//scale to cover entire new texture
    else
        SDL_RenderCopy(renderer_ptr, original, nullptr, &dst);

    SDL_SetRenderTarget(renderer_ptr, nullptr);

    return new_texture;
}

void ResizeSDLTexture(SDL_Renderer* renderer_ptr, std::unique_ptr<SDL_Texture, SDLDeleters>& original, int new_w, int new_h, bool scale_content)
{
    original.reset(ResizeSDLTexture(renderer_ptr, original.get(), new_w, new_h, scale_content));
}

SDL_Texture* DuplicateSDLTexture(SDL_Renderer* renderer_ptr, SDL_Texture* original)
{
    Uint32 format{0};
    SDL_Rect dst{0,0,0,0};
    //get current format and access
    SDL_QueryTexture(original, &format, nullptr, &dst.w, &dst.h);

    SDL_Texture* copy = SDL_CreateTexture(renderer_ptr, format, SDL_TEXTUREACCESS_TARGET, dst.w, dst.h);

    SDL_SetRenderTarget(renderer_ptr, copy);
    SDL_RenderCopy(renderer_ptr, original, nullptr, nullptr);
    SDL_SetRenderTarget(renderer_ptr, nullptr);

    return copy;
}

SDL_Texture* DuplicateSDLTexture(SDL_Renderer* renderer_ptr, const std::unique_ptr<SDL_Texture, SDLDeleters>& original)
{
    return DuplicateSDLTexture(renderer_ptr, original.get());
}

//<f> Render Funtions
void RenderTexture(SDL_Renderer* renderer_ptr, SDL_Texture* texture, SDL_Rect* source_rect, SDL_Rect* destination_rect)
{
    SDL_RenderCopy(renderer_ptr, texture, source_rect, destination_rect);
}

bool RenderOnTexture(SDL_Renderer* renderer_ptr, SDL_Texture* source_texture, SDL_Texture* destination_texture, SDL_Rect* source_rect, SDL_Rect* destination_rect)
{
    int access{0};
    SDL_QueryTexture(destination_texture, nullptr, &access, nullptr, nullptr);

    if(access == SDL_TEXTUREACCESS_TARGET)//ok, can render to texture
    {
        SDL_SetRenderTarget(renderer_ptr, destination_texture);//set texture as new target

        SDL_RenderCopy(renderer_ptr, source_texture, source_rect, destination_rect);

        SDL_SetRenderTarget(renderer_ptr, nullptr);//reset target to window

        return true;
    }

    return false;
}
//</f>

}//namespace
