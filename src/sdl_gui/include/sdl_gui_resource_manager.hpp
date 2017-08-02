#include <map>
#include <string>
#include "SDL.h"
#include "sdl_gui_font.hpp"
#include "sdl_gui_texture.hpp"

namespace sdl_gui
{
#ifndef SDL_GUI_RESOURCE_MANAGER_HPP
#define SDL_GUI_RESOURCE_MANAGER_HPP

class ResourceManager
{
    public:
        /* Default constructor */
        explicit ResourceManager(SDL_Renderer* renderer_ptr);
        /* Default destructor */
        virtual ~ResourceManager() noexcept;

        /* Copy constructor */
        ResourceManager(const ResourceManager& other);
        /* Move constructor */
        ResourceManager(ResourceManager&& other) noexcept;

        /* Copy operator */
        ResourceManager& operator= (const ResourceManager& other);
        /* Move operator */
        ResourceManager& operator= (ResourceManager&& other) noexcept;

        Texture GetTexture(const std::string& texture_path);
        Font* GetFont(const std::string& font_path, int font_size);
    private:
        // vars and stuff
        /* This class IS NOT THE OWNER OF THS POINTER */
        SDL_Renderer* m_renderer_ptr;

        std::map<std::string, SDL_Texture*> m_sdl_textures;
        std::map<std::string, std::map<int, std::unique_ptr<Font>>> m_fonts;
};

#endif //RESOURCE_MANAGER_HPP

}//namespace
