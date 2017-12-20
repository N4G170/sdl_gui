#include "sdl_gui_scroll_box.hpp"
#include <utility>
#include "sdl_gui_log.hpp"
#include "sdl_gui_label.hpp"

namespace sdl_gui
{

//<f> Constructors & operator=
ScrollBox::ScrollBox(GuiMainPointers main_pointers, const Position& position, const Dimensions& size) : GuiElement{main_pointers, position, size},
    m_content_layout{main_pointers, position, size}, m_scrollbar{main_pointers, position, size}
{
    m_content_layout.Parent(this);
    m_content_layout.ParentViewport(this->TransformPtr());
    m_content_layout.LocalPosition({0,0});
    m_content_layout.Config({5,5,5,3,5,3});

    m_content_layout.Size({size.w-10, size.h});//remove L/R margins

    m_scrollbar.Parent(this);
    m_scrollbar.LocalPosition( {size.w, 0} );//
    m_scrollbar.Resize( {25, size.h} );
    m_scrollbar.HeadRatio(Size().h / m_content_layout.Size().h);
    m_scrollbar.MinValue( 0 );
    m_scrollbar.MaxValue( std::abs(Size().h - m_content_layout.Size().h) );
    m_scrollbar.ValueChangedCallback(std::bind(&ScrollBox::ScrollMoved, this, std::placeholders::_1));
}

ScrollBox::~ScrollBox() noexcept {}

ScrollBox::ScrollBox(const ScrollBox& other) : GuiElement(other), m_content_layout{other.m_content_layout}, m_scrollbar{other.m_scrollbar} {}

ScrollBox::ScrollBox(ScrollBox&& other) noexcept : GuiElement(std::move(other)), m_content_layout{std::move(other.m_content_layout)},
    m_scrollbar{std::move(other.m_scrollbar)} {}

ScrollBox& ScrollBox::operator=(const ScrollBox& other)
{
    if(this != &other)//not same ref
    {
        auto tmp(other);
        *this = std::move(tmp);
    }

    return *this;
}

ScrollBox& ScrollBox::operator=(ScrollBox&& other) noexcept
{
    if(this != &other)//not same ref
    {
        GuiElement::operator=(std::move(other));
        m_content_layout = std::move(other.m_content_layout);
        m_scrollbar = std::move(other.m_scrollbar);
    }
    return *this;
}
//</f>

//<f> Overrides GuiElement
void ScrollBox::Input(const SDL_Event &event)
{
    m_scrollbar.Input(event);
    m_content_layout.Input(event);
}

void ScrollBox::Logic(float delta_time)
{
    m_scrollbar.Logic(delta_time);
    m_content_layout.Logic(delta_time);
}

void ScrollBox::Render(float delta_time)
{
    Render(delta_time, m_main_pointers.main_camera_ptr);
}

void ScrollBox::Render(float delta_time, Camera* camera)
{
    if(!m_render)
        return;

    SDL_Rect dst{RenderRect()};

    //apply camera position
    dst.x += camera->CameraPosition().x;
    dst.y += camera->CameraPosition().y;

    if(camera->RectInsideCamera(dst))
    {
        SDL_SetRenderDrawColor(m_main_pointers.main_renderer_ptr, 255,0,255,255);
        SDL_RenderDrawRect(m_main_pointers.main_renderer_ptr, &dst);

        //draw scrollbar
        m_scrollbar.Render(delta_time, camera);

        SDL_RenderSetViewport(m_main_pointers.main_renderer_ptr, &dst);
        m_content_layout.Render(delta_time, camera);
        SDL_RenderSetViewport(m_main_pointers.main_renderer_ptr, nullptr);
    }
}

//</f>

//<f> Item Management
Label* ScrollBox::CreateLabelItem(const std::string& item_text)
{
    // auto label
    return nullptr;
}

SelectableButton* ScrollBox::CreateSelectableItem(const std::string& item_text)
{
    return nullptr;
}

void ScrollBox::CreateItem(const std::string& item_text)
{
    m_content_layout.CreateItem(item_text);

    m_scrollbar.HeadRatio(Size().h / m_content_layout.Size().h);
    m_scrollbar.MinValue( 0 );
    m_scrollbar.MaxValue( std::abs(Size().h - m_content_layout.Size().h) );
}
//</f>

void ScrollBox::ScrollMoved(float value)
{
    auto content_position{m_content_layout.LocalPosition()};
    content_position.y = -value;//value is negative because scroll down moves content up
    m_content_layout.LocalPosition(content_position);
}

}
