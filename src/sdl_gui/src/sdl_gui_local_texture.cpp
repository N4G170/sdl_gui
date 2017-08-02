#include "sdl_gui_local_texture.hpp"
#include "sdl_gui_texture_utils.hpp"
#include <utility>
#include <iostream>

namespace sdl_gui
{

LocalTexture::LocalTexture() : m_renderer_ptr{nullptr}, m_texture{nullptr}, m_src_rect{nullptr},
    m_dst_rect{nullptr}, m_colour_modulation{255,255,255,255}
{

}

LocalTexture::LocalTexture(SDL_Renderer* renderer_ptr, int w, int h) : m_renderer_ptr{renderer_ptr}, m_texture{nullptr}, m_src_rect{nullptr},
    m_dst_rect{nullptr}, m_colour_modulation{255,255,255,255}
{
    m_texture.reset(SDL_CreateTexture(m_renderer_ptr, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_TARGET, w, h));
}

LocalTexture::LocalTexture(SDL_Renderer* renderer_ptr, SDL_Texture* texture_ptr):m_renderer_ptr{renderer_ptr}, m_texture{DuplicateSDLTexture(m_renderer_ptr, texture_ptr)},
    m_src_rect{nullptr}, m_dst_rect{nullptr}, m_colour_modulation{255,255,255,255}
{
    
}

LocalTexture::~LocalTexture() noexcept
{
    /* This class does not own the renderer ptr, so DO NOT DELETE IT */

    ClearSourceRect();
    ClearDestinationRect();
}

LocalTexture::LocalTexture(const LocalTexture& other): m_renderer_ptr{other.m_renderer_ptr}, m_texture{DuplicateSDLTexture(other.m_renderer_ptr, other.m_texture)},
    m_src_rect{other.m_src_rect}, m_dst_rect{other.m_dst_rect}, m_colour_modulation{other.m_colour_modulation}
{

}

LocalTexture::LocalTexture(LocalTexture&& other) noexcept
{
    m_renderer_ptr = std::move(other.m_renderer_ptr);
    m_texture = std::move(other.m_texture);

    m_src_rect = std::move(other.m_src_rect);
    m_dst_rect = std::move(other.m_dst_rect);

    m_colour_modulation = std::move(other.m_colour_modulation);
}

LocalTexture& LocalTexture::operator=(const LocalTexture& other)
{
    if(this != &other)//not same ref
    {
        LocalTexture tmp(other);
        *this = std::move(tmp);
    }

    return *this;
}

LocalTexture& LocalTexture::operator=(LocalTexture&& other) noexcept
{
    this->m_renderer_ptr = std::move(other.m_renderer_ptr);
    this->m_texture = std::move(other.m_texture);

    this->m_src_rect = std::move(other.m_src_rect);
    this->m_dst_rect = std::move(other.m_dst_rect);
    this->m_colour_modulation = std::move(other.m_colour_modulation);

    return *this;
}

void LocalTexture::SourceRect(SDL_Rect src_rect)
{
    if(m_src_rect == nullptr)
    {
        m_src_rect = new SDL_Rect{src_rect};
    }
    else
    {
        *m_src_rect = src_rect;
    }
}

void LocalTexture::DestinationRect(SDL_Rect dst_rect)
{
    if(m_dst_rect == nullptr)
    {
        m_dst_rect = new SDL_Rect{dst_rect};
    }
    else
    {
        *m_dst_rect = dst_rect;
    }
}

void LocalTexture::ClearSourceRect()
{
    delete(m_src_rect);
    m_src_rect = nullptr;
}

void LocalTexture::ClearDestinationRect()
{
    delete(m_dst_rect);
    m_dst_rect = nullptr;
}

void LocalTexture::TexturePtr(SDL_Texture* texture)
{
    //will delete the previous one on its own
    m_texture.reset(texture);
}

void LocalTexture::Render()
{
    Render(m_src_rect, m_dst_rect);
}

void LocalTexture::Render(SDL_Rect* src_rect, SDL_Rect* dst_rect)
{
    if(m_texture == nullptr)
        return;

    SDL_SetTextureAlphaMod(m_texture.get(), m_colour_modulation.a);
    SDL_SetTextureColorMod(m_texture.get(), m_colour_modulation.r, m_colour_modulation.g, m_colour_modulation.b);
    SDL_RenderCopy(m_renderer_ptr, m_texture.get(), src_rect, dst_rect);
    //we reset the alpha and color modulation after rendering as the texture may be shared
    SDL_SetTextureColorMod(m_texture.get(), 255, 255, 255);
    SDL_SetTextureAlphaMod(m_texture.get(), 255);
}

void LocalTexture::Resize(int new_w, int new_h, bool scale_content)
{
    //store new texture. unique_ptr will use the deleter to delete the previous one
    ResizeSDLTexture(m_renderer_ptr, m_texture, new_w, new_h, scale_content);
}

void LocalTexture::Clear(const SDL_Colour& colour)
{
    ClearSDLTexture(m_renderer_ptr, m_texture.get(), colour);
}

void LocalTexture::ColourModulation(const SDL_Colour& colour)
{
    ColourModulation(colour.r, colour.g, colour.b);
}

void LocalTexture::ColourModulation(Uint8 r, Uint8 g, Uint8 b)
{
    m_colour_modulation.r = r;
    m_colour_modulation.g = g;
    m_colour_modulation.b = b;
}

}//namespace
