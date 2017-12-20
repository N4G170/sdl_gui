#include "sdl_gui_element.hpp"
#include "sdl_gui_manager.hpp"

#ifndef OTHERS_PANEL_HPP
#define OTHERS_PANEL_HPP

class OthersPanel
{
    public:
        //<f> Constructors & operator=
        /** brief Default constructor */
        explicit OthersPanel(sdl_gui::GuiManager* gui_manager);
        /** brief Default destructor */
        virtual ~OthersPanel() noexcept;

        /** brief Copy constructor */
        OthersPanel(const OthersPanel& other);
        /** brief Move constructor */
        OthersPanel(OthersPanel&& other) noexcept;

        /** brief Copy operator */
        OthersPanel& operator= (const OthersPanel& other);
        /** brief Move operator */
        OthersPanel& operator= (OthersPanel&& other) noexcept;
        //</f>

        //<f> Methods
        void EnableTextBoxFix(bool enable);
        //</f> Methods

        //<f> Getters/Setters
        sdl_gui::GuiElement* Panel() const { return m_panel; }
        sdl_gui::Image* Background() const { return m_background; }
        //</f>

    protected:
        // vars and stuff
        sdl_gui::GuiManager* m_gui_manager;
        sdl_gui::GuiElement* m_panel;

        sdl_gui::Image* m_background;

        sdl_gui::Textbox* m_textbox;
        sdl_gui::Label* m_textbox_label;
    private:
};

#endif //OTHERS_PANEL_HPP
