#include "sdl_gui_label.hpp"
#include "sdl_gui_element.hpp"
#include "sdl_gui_mouse_interaction.hpp"
#include "sdl_gui_image.hpp"
#include <unordered_map>

namespace sdl_gui
{

#ifndef SDL_GUI_MENU_ITEM_HPP
#define SDL_GUI_MENU_ITEM_HPP

class MenuItem : public GuiElement
{
    public:
        //<f> Constructors & operator=
        /** brief Default constructor */
        MenuItem(GuiMainPointers main_pointers, const Position& position, const Dimensions& size);
        /** brief Default destructor */
        virtual ~MenuItem() noexcept;

        /** brief Copy constructor */
        MenuItem(const MenuItem& other);
        /** brief Move constructor */
        MenuItem(MenuItem&& other) noexcept;

        /** brief Copy operator */
        MenuItem& operator= (const MenuItem& other);
        /** brief Move operator */
        MenuItem& operator= (MenuItem&& other) noexcept;
        //</f>

        //<f> Overrides GUIElement
        void Input(const SDL_Event& event) override;
        void ClearInput() override;

        // virtual void FixedLogic(float fixed_delta_time);
        void Logic(float delta_time) override;

        void Render(float delta_time) override;
        void Render(float delta_time, Camera* camera) override;
        //</f>

        //<f> Virtual Methods

        //</f>

        //<f> Getters/Setters
        void Text(const std::string& text);
        //</f>

    protected:
        // vars and stuff
        Image m_bg_image;
        Label m_label;
        MouseInteraction m_mouse_interaction;
        bool m_is_selected;

        std::unordered_map<ButtonState, SDL_Colour> m_state_colours;
        void ChangeStateColour(ButtonState state);
        void ChangeSelected(){ m_is_selected = !m_is_selected; }

    private:
};

#endif //SDL_GUI_MENU_ITEM_HPP

}//namespace
