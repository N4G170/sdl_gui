#include "sdl_gui_layout.hpp"
#include "sdl_gui_log.hpp"
#include "sdl_gui_utils.hpp"
#include <utility>
#include <algorithm>

namespace sdl_gui
{

Layout::Layout(GuiMainPointers main_pointers, const Position& position, const Dimensions& size): GuiElement{main_pointers, position, size},
    // m_layout_config{LayoutWrapMode::LAYOUT_OVERFLOW, {0,0},0,0,0,0,0,0,0,0}
    m_layout_config{LayoutWrapMode::LAYOUT_OVERFLOW, {200,25},3,2,5,5,5,5,5,5}
{

}

Layout::~Layout() noexcept
{

}

Layout::Layout(const Layout& other): GuiElement{other}, m_elements{other.m_elements}, m_layout_config{other.m_layout_config}
{

}

Layout::Layout(Layout&& other) noexcept:  GuiElement{std::move(other)}, m_elements{std::move(other.m_elements)}, m_layout_config{std::move(other.m_layout_config)}
{

}

Layout& Layout::operator=(const Layout& other)
{
    if(this != &other)//not same ref
    {
        Layout tmp(other);
        *this = std::move(tmp);
    }

    return *this;
}

Layout& Layout::operator=(Layout&& other) noexcept
{
    if(this != &other)
    {
        GuiElement::operator=(other);
        m_elements= std::move(other.m_elements);
        m_layout_config = std::move(other.m_layout_config);
    }
    return *this;
}


//<f> Overrides GuiElement

void Layout::Render(float delta_time)
{
    Render(delta_time, m_main_pointers.main_camera_ptr);
}

void Layout::Render(float delta_time, Camera* camera)
{
    if(!m_render)
        return;

    SDL_Rect dst{RenderRect()};

    //apply camera position
    dst.x += camera->CameraPosition().x;
    dst.y += camera->CameraPosition().y;

    if(camera->RectInsideCamera(dst))
    {
        SDL_SetRenderDrawColor(m_main_pointers.main_renderer_ptr, 255,255,255,255);
        SDL_RenderDrawRect(m_main_pointers.main_renderer_ptr, &dst);
    }
}

//</f>

//<f> Contents management
void Layout::AddElement(GuiElement* element)
{
    element->Parent(this);

    if(m_layout_config.wrap_mode == LayoutWrapMode::LAYOUT_HIDDEN)
        element->TransformPtr()->ParentViewport(this->TransformPtr());

    m_elements.push_back(element);

    //update internal positioning
    UpdateElementsPositions();
}

void Layout::RemoveElement(GuiElement* element)
{
    auto search_result{std::find_if(std::begin(m_elements), std::end(m_elements), [element](GuiElement* vec_element)->bool{ return *element == *vec_element; } )};
    if(search_result != std::end(m_elements))
        m_elements.erase(search_result);

    //update internal positioning
    UpdateElementsPositions();
}
//</f>

void Layout::UpdateElementsPositions()
{
    auto total_lines = m_layout_config.num_of_lines <= 0 ? m_elements.size() : m_layout_config.num_of_lines;
    auto total_columns = m_layout_config.num_of_columns <= 0 ? m_elements.size() : m_layout_config.num_of_columns;

    auto line{0};
    auto column{0};
    for(auto& element : m_elements)
    {
        Position tmp_position{element->LocalPosition()};

        tmp_position.x = m_layout_config.left_margin + m_layout_config.horizontal_element_spacing * column + (column==0 ? 0 : m_layout_config.element_size.w) * column;
        //sum margin with spacing and the height of previous element
        tmp_position.y = m_layout_config.top_margin + m_layout_config.vertical_element_spacing * line + (line==0 ? 0 : m_layout_config.element_size.h) * line;

        element->LocalPosition(tmp_position);

        //Manage line/column index
        ++column;
        if(column >= total_columns)//reset
        {
            column = 0;
            ++line;

            if(line >= total_lines)
                break;//finish loop
        }
    }

    ResizeToFit();
}

void Layout::ResizeToFit()
{
    auto bounds{Bounds()};

    //add right and bottom margin to size
    Size(SizeFromInts(bounds.w + m_layout_config.right_margin, bounds.h + m_layout_config.bottom_margin));
}

}//namespace
