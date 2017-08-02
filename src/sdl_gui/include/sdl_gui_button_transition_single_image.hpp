#include "sdl_gui_button_transition.hpp"
#include "sdl_gui_enums.hpp"
#include "sdl_gui_texture.hpp"
#include "sdl_gui_resource_manager.hpp"
#include <unordered_map>
#include <string>
#include <memory>

namespace sdl_gui
{

#ifndef SDL_GUI_BUTTON_TRANSITION_IMAGE_HPP
#define SDL_GUI_BUTTON_TRANSITION_IMAGE_HPP

class ButtonTransitionSingleImage : public ButtonTransition
{
    public:
        //<f> Constructors & operator=
        /** brief Default constructor */
        explicit ButtonTransitionSingleImage(ResourceManager* resource_manager_ptr);
        /** brief Default destructor */
        virtual ~ButtonTransitionSingleImage() noexcept;

        /** brief Copy constructor */
        ButtonTransitionSingleImage(const ButtonTransitionSingleImage& other);
        /** brief Move constructor */
        ButtonTransitionSingleImage(ButtonTransitionSingleImage&& other) noexcept;

        /** brief Copy operator */
        ButtonTransitionSingleImage& operator= (const ButtonTransitionSingleImage& other);
        /** brief Move operator */
        ButtonTransitionSingleImage& operator= (ButtonTransitionSingleImage&& other) noexcept;
        //</f>

        //<f> Virtual Methods
        virtual void Render(const SDL_Rect& dst);
        virtual void ChangeState(ButtonState state);
        //</f>

        //<f> Getters/Setters
        void ChangeTexture(const std::string& image_path);
        /**
         * \brief Set the source area of the image to be rendered on a specific button state
         */
        void SetStateTextureSrcRect(ButtonState state, const SDL_Rect& rect);
        /**
         * \brief Set the source area of the image to be rendered on each of the button state
         */
        void SetStateTextureSrcRect(const SDL_Rect& inactive, const SDL_Rect& active, const SDL_Rect& over, const SDL_Rect& pressed);
        /**
         * \brief Clears Source rect from a button state (the rect is stored as a pointer)
         */
        void ClearStateTextureSrcRect(ButtonState state);
        /**
         * \brief Clears Source rect from ALL button state (rects are stored as pointers)
         */
        void ClearStateTextureSrcRect();
        //</f>

    protected:
        // vars and stuff
        ResourceManager* m_resource_manager_ptr;
        Texture m_texture;
        std::unordered_map<ButtonState, std::unique_ptr<SDL_Rect>> m_state_rects;
        SDL_Rect* m_current_src_rect_ptr;

    private:
};

#endif //SDL_GUI_BUTTON_TRANSITION_IMAGE_HPP

}//namespace
