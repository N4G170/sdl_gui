#include "sdl_gui_basic_frame_animation.hpp"
#include "sdl_gui_base_button.hpp"
#include "sdl_gui_label.hpp"
#include "sdl_gui_element.hpp"
#include "sdl_gui_manager.hpp"
#include <memory>

#ifndef BUTTONS_PANEL_HPP
#define BUTTONS_PANEL_HPP

class ButtonsPanel
{
    public:
        //<f> Constructors & operator=
        /** brief Default constructor */
        explicit ButtonsPanel(sdl_gui::GuiManager* manager);
        /** brief Default destructor */
        virtual ~ButtonsPanel() noexcept;

        /** brief Copy constructor */
        ButtonsPanel(const ButtonsPanel& other);
        /** brief Move constructor */
        ButtonsPanel(ButtonsPanel&& other) noexcept;

        /** brief Copy operator */
        ButtonsPanel& operator= (const ButtonsPanel& other);
        /** brief Move operator */
        ButtonsPanel& operator= (ButtonsPanel&& other) noexcept;
        //</f>

        //<f> Virtual Methods

        //</f>

        //<f> Getters/Setters
        sdl_gui::GuiElement* Panel() const { return m_panel;}
        //</f>

    protected:
        // vars and stuff
        sdl_gui::GuiManager* m_gui_manager;
        sdl_gui::GuiElement* m_panel;
    private:
};

#endif //BUTTONS_PANEL_HPP
