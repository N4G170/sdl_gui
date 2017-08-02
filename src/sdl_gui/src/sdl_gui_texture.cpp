#include "sdl_gui_texture.hpp"
#include "sdl_gui_texture_utils.hpp"
#include "sdl_gui_log.hpp"
#include <utility>
#include <iostream>

namespace sdl_gui
{

Texture::Texture():m_renderer_ptr{nullptr}, m_texture_ptr{nullptr}, m_colour_modulation{255,255,255,255}, m_image_path{} {}

Texture::Texture(SDL_Renderer* renderer_ptr, SDL_Texture* texture_ptr, const std::string& image_path): m_renderer_ptr{renderer_ptr}, m_texture_ptr{texture_ptr},
    m_colour_modulation{255,255,255,255}, m_image_path{image_path} {}

Texture::~Texture() noexcept
{
    /* This class does not own the renderer ptr, so DO NOT DELETE IT */
}

Texture::Texture(const Texture& other): m_renderer_ptr{other.m_renderer_ptr}, m_texture_ptr{other.m_texture_ptr}, m_colour_modulation{other.m_colour_modulation},
    m_image_path{other.m_image_path} {}

Texture::Texture(Texture&& other) noexcept : m_renderer_ptr{std::move(other.m_renderer_ptr)}, m_texture_ptr{std::move(other.m_texture_ptr)},
    m_colour_modulation{std::move(other.m_colour_modulation)}, m_image_path{std::move(other.m_image_path)} {}

Texture& Texture::operator=(const Texture& other)
{
    if(this != &other)//not same ref
    {
        Texture tmp(other);
        *this = std::move(tmp);
    }

    return *this;
}

Texture& Texture::operator=(Texture&& other) noexcept
{
    if(this != &other)
    {
        this->m_renderer_ptr = std::move(other.m_renderer_ptr);
        this->m_texture_ptr = std::move(other.m_texture_ptr);
        this->m_colour_modulation = std::move(other.m_colour_modulation);
        this->m_image_path = std::move(other.m_image_path);
    }
    return *this;
}

void Texture::Render(SDL_Rect* src_rect, SDL_Rect* dst_rect)
{
    if(m_texture_ptr == nullptr)
        return;

    SDL_SetTextureAlphaMod(m_texture_ptr, m_colour_modulation.a);
    SDL_SetTextureColorMod(m_texture_ptr, m_colour_modulation.r, m_colour_modulation.g, m_colour_modulation.b);

    SDL_RenderCopy(m_renderer_ptr, m_texture_ptr, src_rect, dst_rect);

    //we reset the alpha and color modulation after rendering as the texture may be shared
    SDL_SetTextureColorMod(m_texture_ptr, 255, 255, 255);
    SDL_SetTextureAlphaMod(m_texture_ptr, 255);
}

}//namespace
