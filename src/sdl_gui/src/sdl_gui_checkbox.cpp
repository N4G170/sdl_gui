#include "sdl_gui_checkbox.hpp"
#include <utility>
#include "sdl_gui_constants.hpp"
#include "sdl_gui_log.hpp"

namespace sdl_gui
{

CheckBox::CheckBox(GuiMainPointers main_pointers, const Position& position, const Dimensions& size) :
    BaseButton{main_pointers, position, size},
    m_check_mark_image{main_pointers, position, size}, m_is_checked{false}, m_value{}
{
    AddGuiCollider({0,0}, Size(), &m_transform);

    m_check_mark_image.Parent(this);
    ConfigAsMultiple();

    //the other needed callbacks are set by the parent BaseButton
    m_mouse_interaction.MouseButtonCallback(SDL_BUTTON_LEFT, InputKeyCallbackType::CLICK, std::bind(&CheckBox::MouseClick, this));
}

CheckBox::~CheckBox() noexcept
{

}

CheckBox::CheckBox(const CheckBox& other) : BaseButton{other}, m_check_mark_image{other.m_check_mark_image},
    m_is_checked{other.m_is_checked}, m_value{other.m_value}
{

}

CheckBox::CheckBox(CheckBox&& other) noexcept : BaseButton{std::move(other)}, m_check_mark_image{std::move(other.m_check_mark_image)},
    m_is_checked{std::move(other.m_is_checked)}, m_value{std::move(other.m_value)}
{

}

CheckBox& CheckBox::operator=(const CheckBox& other)
{
    if(this != &other)//not same ref
    {
        CheckBox tmp(other);
        *this = std::move(tmp);
    }

    return *this;
}

CheckBox& CheckBox::operator=(CheckBox&& other) noexcept
{
    if(this != &other)
    {
        this->m_check_mark_image = std::move(other.m_check_mark_image);
        this->m_is_checked = std::move(other.m_is_checked);
        this->m_value = std::move(other.m_value);
    }

    return *this;
}

//<f> Overrides GUIElement
// void CheckBox::Input(const SDL_Event& event)
// {
//
// }

// void CheckBox::Logic(float fixed_delta_time)
// {

// }

void CheckBox::Render(float delta_time)
{
    Render(delta_time, m_main_pointers.main_camera_ptr);
}

void CheckBox::Render(float delta_time, Camera* camera)
{
    if(!m_render)
        return;

    SDL_Rect dst{RenderRect()};

    //apply camera position
    if(!m_transform.ParentViewport())//if inside viewport we cant add camera position
    {
        dst.x += camera->CameraPosition().x;
        dst.y += camera->CameraPosition().y;
    }

    if(camera->RectInsideCamera(dst))
    {
        m_current_transition->Render(dst);

        //calculate checkmark coordinates and dimensions//TMP
        int check_w{dst.w/3};
        int check_h{dst.w/3};

        dst.x = dst.x + (dst.w - check_w) / 2;
        dst.y = dst.y + (dst.h - check_h) / 2;

        dst.w = check_w;
        dst.h = check_h;

        if(m_is_checked)
            m_check_mark_image.Render(delta_time, camera);
    }
}
//</f>

//<f> Checkbox type
void CheckBox::ConfigAsMultiple()
{
    float size_ratio{75/100.f};
    TransitionType(sdl_gui::ButtonTransitionType::COLOUR);
    TransitionColourPtr()->ChangeBaseTexture(c_img_white_dot);

    m_check_mark_image.ChangeTexture(c_img_white_dot);
    m_check_mark_image.ColourModulation(Colour::Black);
    m_check_mark_image.Size( {Size().w*size_ratio, Size().h*size_ratio} );
    m_check_mark_image.AlignWithParentPoint(sdl_gui::AnchorType::MIDDLE_CENTRE);
}

void CheckBox::ConfigAsRadio()
{
    float size_ratio{(75/100.f)};
    TransitionType(sdl_gui::ButtonTransitionType::COLOUR);
    TransitionColourPtr()->ChangeBaseTexture(c_img_white_circle);

    m_check_mark_image.ChangeTexture(c_img_white_circle);
    m_check_mark_image.ColourModulation(Colour::Black);
    m_check_mark_image.Size( {Size().w*size_ratio, Size().h*size_ratio} );
    m_check_mark_image.AlignWithParentPoint(sdl_gui::AnchorType::MIDDLE_CENTRE);
}
//</f>

void CheckBox::MouseClick()
{
    m_is_checked = !m_is_checked;

    if(ValueChanged)
        ValueChanged(this);
}

}//namespace
