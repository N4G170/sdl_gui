#include "sdl_gui_element.hpp"
#include "sdl_gui_layout.hpp"
#include "sdl_gui_menu_layout.hpp"
#include "sdl_gui_vscrollbar.hpp"

namespace sdl_gui
{

#ifndef SDL_GUI_SCROLL_BOX_HPP
#define SDL_GUI_SCROLL_BOX_HPP

class Label;
class SelectableButton;

class ScrollBox : public GuiElement
{
    public:
        //<f> Constructors & operator=
        /** brief Default constructor */
        ScrollBox(GuiMainPointers main_pointers, const Position& position, const Dimensions& size);
        /** brief Default destructor */
        virtual ~ScrollBox() noexcept;

        /** brief Copy constructor */
        ScrollBox(const ScrollBox& other);
        /** brief Move constructor */
        ScrollBox(ScrollBox&& other) noexcept;

        /** brief Copy operator */
        ScrollBox& operator= (const ScrollBox& other);
        /** brief Move operator */
        ScrollBox& operator= (ScrollBox&& other) noexcept;
        //</f>

        //<f> Overrides GuiElement
        void Input(const SDL_Event& event) override;

        // void FixedLogic(float fixed_delta_time) override;
        void Logic(float delta_time) override;

        void Render(float delta_time) override;
        void Render(float delta_time, Camera* camera) override;
        //</f>

        //<f> Virtual Methods

        //</f>

        //<f> Getters/Setters

        //</f>

        //<f> Item Management
        Label* CreateLabelItem(const std::string& item_text);
        SelectableButton* CreateSelectableItem(const std::string& item_text);
        void CreateItem(const std::string& item_text);
        //</f>

    protected:
        // vars and stuff
        MenuLayout m_content_layout;
        VScrollbar m_scrollbar;
    private:
        void ScrollMoved(float value);
};

#endif //SDL_GUI_SCROLL_BOX_HPP

}//namespace
