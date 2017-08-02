#include "sdl_gui_element.hpp"
#include "sdl_gui_texture.hpp"
#include "sdl_gui_enums.hpp"
#include "sdl_gui_utils.hpp"
#include "sdl_gui_menu_item.hpp"
#include <memory>
#include <vector>

namespace sdl_gui
{
#ifndef SDL_GUI_MENU_LAYOUT_HPP
#define SDL_GUI_MENU_LAYOUT_HPP

struct MenuLayoutConfig
{
    int vertical_element_spacing;
    int horizontal_element_spacing;
    int top_margin;
    int left_margin;
    int bottom_margin;
    int right_margin;
};

class MenuLayout: public GuiElement
{
    public:
        /* Default constructor */
        MenuLayout(GuiMainPointers main_pointers, const Position& position, const Dimensions& size);
        /* Default destructor */
        virtual ~MenuLayout() noexcept;

        /* Copy constructor */
        MenuLayout(const MenuLayout& other);
        /* Move constructor */
        MenuLayout(MenuLayout&& other) noexcept;

        /* Copy operator */
        MenuLayout& operator= (const MenuLayout& other);
        /* Move operator */
        MenuLayout& operator= (MenuLayout&& other) noexcept;

        //<f> Overrides GuiElement
        void Input(const SDL_Event& event) override;

        // void FixedLogic(float fixed_delta_time) override;
        void Logic(float delta_time) override;

        void Render(float delta_time) override;
        void Render(float delta_time, Camera* camera) override;
        //</f>

        //<f> Contents management
        void CreateItem(const std::string& text);

        /**
         * \brief Add a new item to this container.(Container takes ownership of it)
         */
        void AddElement(MenuItem* item);
        //</f>

        //<f> MenuLayout Config
        void Margins(int top, int left, int bottom, int right);
        int MarginTop() const { return m_layout_config.top_margin; }
        int MarginLeft() const { return m_layout_config.left_margin; }
        int MarginBottom() const { return m_layout_config.bottom_margin; }
        int MarginRight() const { return m_layout_config.right_margin; }

        void Spacing(int vertical, int horizontal);
        int SpacingVertical() const { return m_layout_config.vertical_element_spacing; }
        void SpacingVertical(int spacing) { m_layout_config.vertical_element_spacing = spacing; }
        int SpacingHorizontal() const { return m_layout_config.horizontal_element_spacing; }
        void SpacingHorizontal(int spacing) { m_layout_config.horizontal_element_spacing = spacing; }

        MenuLayoutConfig Config() const { return m_layout_config; }
        void Config(const MenuLayoutConfig& config){ m_layout_config = config; }

        //</f>

    private:
        //<f> Elements
        std::vector<std::unique_ptr<MenuItem>> m_elements;

        MenuLayoutConfig m_layout_config;

        void UpdateElementsPositions();

        void ResizeToFit();
        //</f>

};

#endif //SDL_GUI_MENU_LAYOUT_HPP
}//namespace
