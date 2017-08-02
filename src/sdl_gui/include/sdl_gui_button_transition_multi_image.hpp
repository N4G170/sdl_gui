#include "sdl_gui_button_transition.hpp"
#include "sdl_gui_texture.hpp"
#include <unordered_map>
#include "sdl_gui_enums.hpp"
#include "sdl_gui_resource_manager.hpp"
#include <string>

namespace sdl_gui
{

#ifndef SDL_GUI_BUTTON_TRANSITION_MULTI_IMAGE_HPP
#define SDL_GUI_BUTTON_TRANSITION_MULTI_IMAGE_HPP

class ButtonTransitionMultiImage : public ButtonTransition
{
    public:
        //<f> Constructors & operator=
        /** brief Default constructor */
        explicit ButtonTransitionMultiImage(ResourceManager* resource_manager_ptr);
        /** brief Default destructor */
        virtual ~ButtonTransitionMultiImage() noexcept;

        /** brief Copy constructor */
        ButtonTransitionMultiImage(const ButtonTransitionMultiImage& other);
        /** brief Move constructor */
        ButtonTransitionMultiImage(ButtonTransitionMultiImage&& other) noexcept;

        /** brief Copy operator */
        ButtonTransitionMultiImage& operator= (const ButtonTransitionMultiImage& other);
        /** brief Move operator */
        ButtonTransitionMultiImage& operator= (ButtonTransitionMultiImage&& other) noexcept;
        //</f>

        //<f> Virtual Methods
        virtual void Render(const SDL_Rect& dst);
        virtual void ChangeState(ButtonState state);
        //</f>

        //<f> Getters/Setters
        /**
         * \brief Set the texture to be used to on a specific button state
         */
        void SetStateTexture(ButtonState state, const std::string& image_path);
        /**
         * \brief Set the texture to be used to on a each of the button state
         */
        void SetStateTexture(const std::string& inactive, const std::string& active, const std::string& over, const std::string& pressed);
        //</f>

    protected:
        // vars and stuff
        ResourceManager* m_resource_manager_ptr;
        std::unordered_map<ButtonState, Texture> m_state_textures;
        Texture* m_current_texture_ptr;

    private:
};

#endif //SDL_GUI_BUTTON_TRANSITION_MULTI_IMAGE_HPP

}//namespace
