#include "sdl_gui_element.hpp"
#include "sdl_gui_texture.hpp"
#include "sdl_gui_enums.hpp"
#include "sdl_gui_utils.hpp"
#include <unordered_map>

namespace sdl_gui
{
#ifndef SDL_GUI_LAYOUT_HPP
#define SDL_GUI_LAYOUT_HPP

struct LayoutConfig
{
    LayoutWrapMode wrap_mode;
    Dimensions element_size;
    int num_of_lines;
    int num_of_columns;
    int vertical_element_spacing;
    int horizontal_element_spacing;
    int top_margin;
    int left_margin;
    int bottom_margin;
    int right_margin;
    std::string ToString()
    {
        return sdl_gui::ToString(element_size)+" "+std::to_string(num_of_lines)+" "+std::to_string(num_of_columns);
    }
};

class Layout: public GuiElement
{
    public:
        /* Default constructor */
        Layout(GuiMainPointers main_pointers, const Position& position, const Dimensions& size);
        /* Default destructor */
        virtual ~Layout() noexcept;

        /* Copy constructor */
        Layout(const Layout& other);
        /* Move constructor */
        Layout(Layout&& other) noexcept;

        /* Copy operator */
        Layout& operator= (const Layout& other);
        /* Move operator */
        Layout& operator= (Layout&& other) noexcept;

        //<f> Overrides GuiElement
        // void Input(const SDL_Event& event) override;

        // void FixedLogic(float fixed_delta_time) override;
        // void Logic(float delta_time) override;

        void Render(float delta_time) override;
        void Render(float delta_time, Camera* camera) override;
        //</f>

        //<f> Contents management
        /**
         * \brief Add a new element to this container.
         */
        void AddElement(GuiElement* element);

        void RemoveElement(GuiElement* element);
        //</f>

        //<f> Layout Config
        Dimensions ElementSize() const { return m_layout_config.element_size; }
        void ElementSize(const Dimensions& element_size) { m_layout_config.element_size = element_size; }

        int NumOfLines() const { return m_layout_config.num_of_lines; }
        void NumOfLines(int lines) { m_layout_config.num_of_lines = lines; }
        int NumOfColumns() const { return m_layout_config.num_of_columns; }
        void NumOfColumns(int columns) { m_layout_config.num_of_columns = columns; }

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

        LayoutConfig Config() const { return m_layout_config; }
        void Config(const LayoutConfig& config){ m_layout_config = config; }

        bool AllowVariableSizes() const { return m_allow_variable_sizes; }
        void AllowVariableSizes(bool allow_variable_sizes)
        {
            if(allow_variable_sizes != m_allow_variable_sizes)
            {
                m_allow_variable_sizes = allow_variable_sizes;

                if(m_allow_variable_sizes)
                    UpdateElementsPositionsVariableSize();
                else
                    UpdateElementsPositions();
            }
        }
        //</f>

    private:
        //<f> Elements
        std::vector<GuiElement*> m_elements;

        LayoutConfig m_layout_config;


        void UpdateElementsPositions();
        void UpdateElementsPositionsVariableSize();

        void ResizeToFit();
        //</f>
        /**
         * \brief If true Layout will not enforce element_size from config and will adapt to element size
         */
        bool m_allow_variable_sizes;

};

#endif //SDL_GUI_LAYOUT_HPP
}//namespace
