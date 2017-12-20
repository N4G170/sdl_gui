#include "sdl_gui_resource_manager.hpp"
#include <utility>
#include "SDL_image.h"
#include "sdl_gui_log.hpp"

namespace sdl_gui
{

ResourceManager::ResourceManager(SDL_Renderer* renderer_ptr): m_renderer_ptr{renderer_ptr}
{

}

ResourceManager::~ResourceManager() noexcept
{
    m_renderer_ptr = nullptr;
}

ResourceManager::ResourceManager(const ResourceManager& other): m_renderer_ptr{other.m_renderer_ptr}
{

}

ResourceManager::ResourceManager(ResourceManager&& other) noexcept
{
    m_renderer_ptr = std::move(other.m_renderer_ptr);
}

ResourceManager& ResourceManager::operator=(const ResourceManager& other)
{
    if(this != &other)//not same ref
    {
        ResourceManager tmp(other);
        *this = std::move(tmp);
    }

    return *this;
}

ResourceManager& ResourceManager::operator=(ResourceManager&& other) noexcept
{
    this->m_renderer_ptr = std::move(other.m_renderer_ptr);

    return *this;
}

Texture ResourceManager::GetTexture(const std::string& texture_path)
{
    if(m_sdl_textures.find(texture_path) == std::end(m_sdl_textures))//did not find it
    {
        SDL_Surface* image = IMG_Load(texture_path.data());//load new
        if(image)//image loaded
        {
            SDL_Texture* texture = SDL_CreateTextureFromSurface(m_renderer_ptr, image);
            if(texture)
            {
                m_sdl_textures[texture_path] = std::move(texture);
            }
            else
            {
                LogLine("Failed to create texture from surface. File path:"+texture_path);
                return {};
            }
        }
        else
        {
            LogLine("Failed to create surface from file. File path:"+texture_path);
            return {};
        }
    }
    return Texture{m_renderer_ptr, m_sdl_textures[texture_path], texture_path};//create new Texture object
}

Font* ResourceManager::GetFont(const std::string& font_path, int font_size)
{
    if(m_fonts.find(font_path) == std::end(m_fonts))//font not loaded (any size)
    {
        m_fonts[font_path][font_size].reset(new Font(m_renderer_ptr, font_path, font_size));
    }
    else if(m_fonts[font_path].find(font_size) == std::end(m_fonts[font_path]))//font size not loaded
    {
        m_fonts[font_path][font_size].reset(new Font(m_renderer_ptr, font_path, font_size));
    }

    return m_fonts[font_path][font_size].get();
}

}//namesapce
