#include "sdl_gui_textbox.hpp"
#include "sdl_gui_constants.hpp"
#include "sdl_gui_log.hpp"
#include "sdl_gui_utils.hpp"
#include <utility>

namespace sdl_gui
{

//<f> Constructors & operator=
Textbox::Textbox(GuiMainPointers main_pointers, const Position& position, const Dimensions& size) :
    GuiElement{main_pointers, position, size}, m_bg_image{main_pointers, position, size},
    m_text_label{main_pointers, position, size}, m_caret_label{main_pointers, position, size}, m_label_to_render{&m_text_label}, m_text{""},
    m_text_representation{""}, m_default_text{"<i>Click to edit</i>"},
    m_blink_time_limit{0.5}, m_blink_time{0}, m_input_type{TextboxInputType::ALPHANUMERIC}, m_mouse_interaction{}
{
    AddGuiCollider({0,0}, Size(), &m_transform);

    m_viewport_rect = RenderRect();
    m_bg_image.Parent(this);
    m_bg_image.TransformPtr()->ParentViewport(&m_transform);
    m_bg_image.LocalPosition({0,0});
    
    // m_viewport_rect = RectFromStructs(position, size);
    // m_text_label.RenderBorder(true);
    m_text_label.Parent(this);
    m_text_label.TransformPtr()->ParentViewport(&m_transform);
    m_text_label.AlignWithParentPoint(sdl_gui::AnchorType::MIDDLE_LEFT);
    m_text_label.Text(m_default_text);

    m_caret_label.Text("|");

    m_mouse_interaction.MouseButtonCallback(SDL_BUTTON_LEFT, sdl_gui::InputKeyCallbackType::CLICK, std::bind(&Textbox::Focus,this));
}

Textbox::~Textbox() noexcept
{

}

Textbox::Textbox(const Textbox& other) : GuiElement{other}, m_bg_image{other.m_bg_image}, m_viewport_rect{other.m_viewport_rect}, m_text_label{other.m_text_label},
    m_caret_label{other.m_caret_label}, m_label_to_render{other.m_label_to_render}, m_text{other.m_text},
    m_text_representation{other.m_text_representation}, m_default_text{other.m_default_text},
    m_blink_time_limit{other.m_blink_time_limit}, m_blink_time{other.m_blink_time}, m_input_type{other.m_input_type}, m_mouse_interaction{other.m_mouse_interaction}
{

}

Textbox::Textbox(Textbox&& other) noexcept : GuiElement{std::move(other)}, m_bg_image{std::move(other.m_bg_image)}, m_viewport_rect{std::move(other.m_viewport_rect)},
    m_text_label{std::move(other.m_text_label)}, m_caret_label{std::move(other.m_caret_label)},
    m_label_to_render{std::move(other.m_label_to_render)}, m_text{std::move(other.m_text)},
    m_text_representation{std::move(other.m_text_representation)}, m_default_text{std::move(other.m_default_text)},
    m_blink_time_limit{std::move(other.m_blink_time_limit)}, m_blink_time{std::move(other.m_blink_time)},
    m_input_type{std::move(other.m_input_type)}, m_mouse_interaction{std::move(other.m_mouse_interaction)}
{

}

Textbox& Textbox::operator=(const Textbox& other)
{
    if(this != &other)//not same ref
    {
        Textbox tmp(other);
        *this = std::move(tmp);
    }
    return *this;
}

Textbox& Textbox::operator=(Textbox&& other) noexcept
{
    if(this != &other)
    {
        GuiElement::operator=(std::move(other));
        m_bg_image = std::move(other.m_bg_image);
        m_viewport_rect = std::move(other.m_viewport_rect);
        m_text_label = std::move(other.m_text_label);
        m_caret_label = std::move(other.m_caret_label);
        m_label_to_render = std::move(other.m_label_to_render);
        m_text = std::move(other.m_text);
        m_text_representation = std::move(other.m_text_representation);
        m_default_text = std::move(other.m_default_text);
        m_blink_time_limit = std::move(other.m_blink_time_limit);
        m_blink_time = std::move(other.m_blink_time);
        m_input_type = std::move(other.m_input_type);
        m_mouse_interaction = std::move(other.m_mouse_interaction);
    }
    return *this;
}
//</f>

//<f> Overrides GUIElement
void Textbox::Input(const SDL_Event& event)
{
    if( !m_active )
        return;

    m_mouse_interaction.Input(event, m_collider.get());

    //QUICK FIX
    auto mouse_inside_collider = [](int mouse_x, int mouse_y, Collider* collider)->bool
    {
        if(collider != nullptr && collider->IsPointColliding(mouse_x, mouse_y))
            return true;
        return false;
    };
    int mouse_x{},mouse_y{};
    SDL_GetMouseState(&mouse_x, &mouse_y);
    if(event.button.type == SDL_MOUSEBUTTONUP &&  event.button.button == SDL_BUTTON_LEFT && !mouse_inside_collider(mouse_x, mouse_y, m_collider.get()))//outisde
        UnFocus();
    //END QUICK FIX

    //text input
    if(m_focused)
    {
        if(event.type == SDL_KEYDOWN)
        {
            if(event.key.keysym.sym == SDLK_BACKSPACE && !m_text.empty())
            {
                m_text_representation.pop_back();
                m_text.pop_back();
                m_text_label.Text(m_text_representation);
            }
        }
        if(event.type == SDL_TEXTINPUT)
        {
            if(m_input_type == TextboxInputType::PASSWORD)
            {
                m_text_representation += '*';
                m_text += event.text.text;
            }
            else if(m_input_type == TextboxInputType::INTEGER)
            {
                try//check if valid int
                {
                    std::stoi(event.text.text);
                    m_text += event.text.text;
                    m_text_representation += event.text.text;
                }
                catch(std::exception) {}
            }
            else
            {
                m_text_representation += event.text.text;
                m_text += event.text.text;
            }
            m_text_label.Text(m_text_representation);
        }
    }
}

void Textbox::Logic(float delta_time)
{
    m_mouse_interaction.Logic(delta_time);

    if(m_focused)
    {
        m_blink_time += delta_time;

        if(m_blink_time >= m_blink_time_limit)
        {
            m_blink_time = 0;

            m_caret_label.CanRender( !m_caret_label.CanRender() );
        }

        //reposition label if it is too big for textbox
        if(m_text_label.Size().w + m_caret_label.Size().w > Size().w)//mode label left
        {
            Position position{Size().w - (m_text_label.Size().w + m_caret_label.Size().w), m_text_label.LocalPosition().y};
            m_text_label.LocalPosition(position);
        }
        else if(m_text_label.LocalPosition().x < 0)//label moved left during edit
        {
            Position position{0, m_text_label.LocalPosition().y};
            m_text_label.LocalPosition(position);
        }

        //calculate caret position
        auto text_width{m_text_label.Size().w};
        auto text_position{m_text_label.LocalPosition()};

        m_caret_label.LocalPosition({text_position.x + text_width, text_position.y});
    }
    else
    {
        if(m_text.empty())
            m_text_label.Text(m_default_text);
    }
}

void Textbox::Render(float delta_time)
{
    Render(delta_time, m_main_pointers.main_camera_ptr);
}

void Textbox::Render(float delta_time, Camera* camera)
{
    if(!m_render)
        return;

    SDL_Rect dst{RenderRect()};

    //apply camera position
    dst.x += camera->CameraPosition().x;
    dst.y += camera->CameraPosition().y;

    if(camera->RectInsideCamera(dst))
    {
        //render thin border
        if(m_focused)
            SDL_SetRenderDrawColor(m_main_pointers.main_renderer_ptr, 0, 255, 0, 255);
        else
            SDL_SetRenderDrawColor(m_main_pointers.main_renderer_ptr, 255,0,0, 255);

        SDL_RenderDrawRect(m_main_pointers.main_renderer_ptr, &m_viewport_rect);
        //set viewport
        SDL_RenderSetViewport(m_main_pointers.main_renderer_ptr, &m_viewport_rect);
        m_bg_image.Render(delta_time, camera);
        m_text_label.Render(delta_time, camera);
        m_caret_label.Render(delta_time, camera);
        SDL_RenderSetViewport(m_main_pointers.main_renderer_ptr, nullptr);

        // SDL_RenderDrawRect(m_main_pointers.main_renderer_ptr, &m_viewport_rect);
        SDL_RenderDrawRect(m_main_pointers.main_renderer_ptr, &dst);
    }

}
//</f>

//<f> Virtual Methods
void Textbox::Focus()
{
    m_focused = true;
    SDL_StartTextInput();
    m_blink_time = 0;
    m_text_label.Text(m_text_representation);

    if(m_text_label.Size().w + m_caret_label.Size().w > Size().w)//mode label left
    {
        Position position{Size().w - (m_text_label.Size().w + m_caret_label.Size().w), m_text_label.LocalPosition().y};
        m_text_label.LocalPosition(position);
    }
}

void Textbox::UnFocus()
{
    m_focused = false;
    SDL_StopTextInput();
    //hide caret
    m_caret_label.CanRender(false);

    if(m_text_label.LocalPosition().x < 0)//label moved left during edit
    {
        Position position{0, m_text_label.LocalPosition().y};
        m_text_label.LocalPosition(position);
    }
}
//</f>

//<f> Private Methods

//</f>

}//namespace
