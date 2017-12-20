#include "sdl_gui_menu_layout.hpp"
#include "sdl_gui_log.hpp"
#include "sdl_gui_utils.hpp"
#include <utility>
#include <algorithm>

namespace sdl_gui
{

MenuLayout::MenuLayout(GuiMainPointers main_pointers, const Position& position, const Dimensions& size): GuiElement{main_pointers, position, size},
     m_elements{}, m_layout_config{5,5,5,5,5,5}
{

}

MenuLayout::~MenuLayout() noexcept
{

}

MenuLayout::MenuLayout(const MenuLayout& other): GuiElement{other}, m_elements{}, m_layout_config{other.m_layout_config}
{
    for(auto& element : m_elements)
        m_elements.push_back(std::unique_ptr<MenuItem>{new MenuItem{*element.get()}});
}

MenuLayout::MenuLayout(MenuLayout&& other) noexcept:  GuiElement{std::move(other)}, m_elements{std::move(other.m_elements)}, m_layout_config{std::move(other.m_layout_config)}
{

}

MenuLayout& MenuLayout::operator=(const MenuLayout& other)
{
    if(this != &other)//not same ref
    {
        MenuLayout tmp(other);
        *this = std::move(tmp);
    }

    return *this;
}

MenuLayout& MenuLayout::operator=(MenuLayout&& other) noexcept
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
void MenuLayout::Input(const SDL_Event& event)
{
    for(auto& element : m_elements)
        element->Input(event);
}

void MenuLayout::Logic(float delta_time)
{
    for(auto& element : m_elements)
        element->Logic(delta_time);
}

void MenuLayout::Render(float delta_time)
{
    Render(delta_time, m_main_pointers.main_camera_ptr);
}

void MenuLayout::Render(float delta_time, Camera* camera)
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

        for(auto& element : m_elements)
            element->Render(delta_time, camera);
    }
}

//</f>

//<f> Contents management
void MenuLayout::CreateItem(const std::string& text)
{
    // ResizeToFit();
    auto item{ new MenuItem{m_main_pointers, {0,0}, {(Size().w - m_layout_config.left_margin - m_layout_config.right_margin), 0} } };

    item->Text(text);
    AddElement(item);
}
void MenuLayout::AddElement(MenuItem* item)
{
    item->Parent(this);

    m_elements.emplace_back(std::unique_ptr<MenuItem>{item});

    //update internal positioning
    UpdateElementsPositions();
}
//</f>

void MenuLayout::UpdateElementsPositions()
{
    auto line{0};
    for(auto& element : m_elements)
    {
        Position tmp_position{element->LocalPosition()};

        tmp_position.x = m_layout_config.left_margin + m_layout_config.horizontal_element_spacing;
        //sum margin with spacing and the height of previous element
        tmp_position.y = m_layout_config.top_margin + m_layout_config.vertical_element_spacing * line + (line==0 ? 0 : element->Size().h) * line;

        element->LocalPosition(tmp_position);

        ++line;
    }

    ResizeToFit();
}

void MenuLayout::ResizeToFit()
{
    // LogLine("---------------------------------------");
    //remove Margins
    // LogLine("->"+ToString(Size()));
    Size({Size().w - m_layout_config.left_margin - m_layout_config.right_margin, Size().h - m_layout_config.top_margin - m_layout_config.bottom_margin});
    // LogLine(ToString(Size()));
    auto bounds{Bounds()};
    //Keep original Width
    Size(SizeFromInts(Size().w + m_layout_config.left_margin + m_layout_config.right_margin, bounds.h + m_layout_config.top_margin + m_layout_config.bottom_margin));
}

}//namespace
