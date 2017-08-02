#include "sdl_gui_tooltip.hpp"
#include "sdl_gui_constants.hpp"
#include "sdl_gui_log.hpp"
#include "sdl_gui_utils.hpp"
#include <utility>

namespace sdl_gui
{

//<f> Constructors & operator=
Tooltip::Tooltip(GuiMainPointers main_pointers, const Position& position, const Dimensions& size): GuiElement{main_pointers, position, size},
    m_background_texture{m_main_pointers.resource_manager_ptr->GetTexture(c_img_white_dot)}, m_label_ptr{new Label(main_pointers, position, size)}, m_text{},
    m_wait_time{0}, m_max_wait_time{1.5}, m_can_show{false}
{
    m_render = false;//hide label
    m_label_ptr->ConfigLabel(c_default_font_path, c_default_font_size, "tooltip text\ntooltip text", Colour::Black);

    AddGuiCollider({0,0}, Size(), this->TransformPtr());

    // m_mouse_interaction.MouseCallback(sdl_gui::MouseCallbackType::ENTER, std::bind(&Tooltip::ShowTooltip, this));
    // m_mouse_interaction.MouseCallback(sdl_gui::MouseCallbackType::EXIT, std::bind(&Tooltip::HideTooltip, this));
    m_mouse_interaction.MouseCallback(sdl_gui::MouseCallbackType::OVER, std::bind(&Tooltip::ShowTooltip, this));
    m_mouse_interaction.MouseCallback(sdl_gui::MouseCallbackType::MOVE, std::bind(&Tooltip::HideTooltip, this));
}

Tooltip::~Tooltip() noexcept
{

}

Tooltip::Tooltip(const Tooltip& other): GuiElement{other}, m_background_texture{other.m_background_texture}, m_label_ptr{new Label{*other.m_label_ptr.get()}},
    m_text{other.m_text}, m_mouse_interaction{other.m_mouse_interaction}, m_wait_time{other.m_wait_time}, m_max_wait_time{other.m_max_wait_time}, m_can_show{other.m_can_show}
{

}

Tooltip::Tooltip(Tooltip&& other) noexcept : GuiElement{std::move(other)}, m_background_texture{std::move(other.m_background_texture)},
    m_label_ptr{std::move(other.m_label_ptr)}, m_text{std::move(other.m_text)}, m_mouse_interaction{std::move(other.m_mouse_interaction)},
    m_wait_time{std::move(other.m_wait_time)}, m_max_wait_time{std::move(other.m_max_wait_time)}, m_can_show{std::move(other.m_can_show)}
{

}

Tooltip& Tooltip::operator=(const Tooltip& other)
{
    if(this != &other)//not same ref
    {
        auto tmp(other);
        *this = std::move(tmp);
    }

    return *this;
}

Tooltip& Tooltip::operator=(Tooltip&& other) noexcept
{
    if(this != &other)//not same ref
    {
        //move here
        GuiElement::operator=(std::move(other));
        m_background_texture = std::move(other.m_background_texture);
        m_label_ptr = std::move(other.m_label_ptr);
        m_text = std::move(other.m_text);
        m_mouse_interaction = std::move(other.m_mouse_interaction);
        m_wait_time = std::move(other.m_wait_time);
        m_max_wait_time = std::move(other.m_max_wait_time);
        m_can_show = std::move(other.m_can_show);
    }
    return *this;
}
//</f>

//<f> Overrides GUIElement
void Tooltip::Input(const SDL_Event& event)
{
    if( !m_active )
        return;

    m_mouse_interaction.Input(event, m_collider.get());
}

void Tooltip::ClearInput()
{
    m_mouse_interaction.ClearInput();
}

void Tooltip::Logic(float delta_time)
{
    if(m_active)
    {
        m_mouse_interaction.Logic(delta_time);

        //process wait timer
        m_wait_time += delta_time;

        if(!m_render && m_wait_time >= m_max_wait_time && m_can_show)
            m_render = true;
    }
}

void Tooltip::Render(float delta_time)
{
    Render(delta_time, m_main_pointers.main_camera_ptr);
}

void Tooltip::Render(float delta_time, Camera* camera)
{
    if(m_collider)
        m_collider->DebugRender(m_main_pointers.main_renderer_ptr);

    if(!m_render)
        return;

    //move label to mouse position
    int mouse_x{}, mouse_y{};
    SDL_GetMouseState(&mouse_x, &mouse_y);
    m_label_ptr->GlobalPosition(PositionFromInts(mouse_x, mouse_y - m_label_ptr->Size().h));

    SDL_Rect dst{m_label_ptr->RenderRect()};

    //apply camera position
    if(!m_transform.ParentViewport())//if inside viewport we cant add camera position
    {
        dst.x += camera->CameraPosition().x;
        dst.y += camera->CameraPosition().y;
    }

    if(camera->RectInsideCamera(dst))
        m_background_texture.Render( nullptr, &dst);

    m_label_ptr->Render(delta_time, camera);

}
//</f>

//<f> Getters/Setters
void Tooltip::TooltipText(const std::string& text)
{
    m_text = text;
    m_label_ptr->Text(text);
}
//</f>

void Tooltip::ShowTooltip()
{
    if(!m_can_show)//to protect the timer
    {
        m_wait_time = 0;//reset timer
        m_can_show = true;
    }
}

void Tooltip::HideTooltip()
{
    m_can_show = false;
    m_render = false;
}

}//namespace
