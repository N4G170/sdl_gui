#include "SDL.h"
#include <memory>
#include "sdl_gui_deleters.hpp"

namespace sdl_gui
{

/**
 * \brief Creates a new SDL_Texture with the given params.
 * \n The user is responsible for managing it.
 */
SDL_Texture* CreateSDLTexture(SDL_Renderer* renderer_ptr, int w, int h, Uint32 pixel_format = SDL_PIXELFORMAT_RGBA8888, int access_type = SDL_TEXTUREACCESS_TARGET);

/**
 * \brief Clears the content of a texture to the given colour.
 * \n SDL_Texture access type must be "SDL_TEXTUREACCESS_TARGET"
 * \n NOTE: function is ALPHA AWARE. Default colour {255,255,255,0}
 */
void ClearSDLTexture(SDL_Renderer* renderer_ptr, SDL_Texture* texture, const SDL_Colour& colour = {255,255,255,0});

/**
 * \brief Resizes a texture for it to have the new size. Note: The original pointer will NOT be deleted
 * @param original The original texture to be resized (NOT DELETED BY THIS FUNCTION)
 * @param scale_content If true the content of the original texture will be scaled to fit the entirety of the new texture
 * \return  Pointer to the new texture
 */
SDL_Texture* ResizeSDLTexture(SDL_Renderer* renderer_ptr, SDL_Texture* original, int new_w, int new_h, bool scale_content = false);

/**
 * \brief Resizes and REPLACES the texture provide in the unique_ptr.
 * The content of the original texture will be
 * @param original the texture to be resized
 * @param scale_content If true the content of the original texture will be scaled to fit the entirety of the new texture
 */
void ResizeSDLTexture(SDL_Renderer* renderer_ptr, std::unique_ptr<SDL_Texture, SDLDeleters>& original, int new_w, int new_h, bool scale_content = false);

/**
 * \brief Duplicates an SDL_Texture. New Texture Access will be TARGET
 */
SDL_Texture* DuplicateSDLTexture(SDL_Renderer* renderer_ptr, SDL_Texture* original);

/**
 * \brief Duplicates an SDL_Texture. New Texture Access will be TARGET
 */
SDL_Texture* DuplicateSDLTexture(SDL_Renderer* renderer_ptr, const std::unique_ptr<SDL_Texture, SDLDeleters>& original);

//<f> Render Funtions
void RenderTexture(SDL_Renderer* renderer_ptr, SDL_Texture* texture, SDL_Rect* source_rect = nullptr, SDL_Rect* destination_rect = nullptr);
bool RenderOnTexture(SDL_Renderer* renderer_ptr, SDL_Texture* source_texture, SDL_Texture* destination_texture, SDL_Rect* source_rect = nullptr, SDL_Rect* destination_rect = nullptr);
//</f>

}//namespace
