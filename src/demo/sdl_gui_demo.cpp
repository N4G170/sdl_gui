#include "sdl_gui_demo.hpp"
#include "sdl_gui_log.hpp"
#include "sdl_gui_constants.hpp"
#include "sdl_gui_enums.hpp"
#include "sdl_gui_camera.hpp"
#include "sdl_gui_utils.hpp"
#include "constants.hpp"
#include <iostream>
#include <vector>


GuiDemo::GuiDemo(SDL_Renderer* renderer_ptr, sdl_gui::ResourceManager* resource_manager_ptr, const SDLInitConfig& sdl_config):
    m_gui_manager{renderer_ptr, resource_manager_ptr}, m_animation_panel{&m_gui_manager}, m_buttons_panel{&m_gui_manager}, m_bars_panel{&m_gui_manager},
    m_others_panel{&m_gui_manager}
{
    // most of these variables will be reused through the constructor
    auto label_ptr{ m_gui_manager.CreateElement<sdl_gui::Label>({0,0}, {0,0}) };
    label_ptr->FontSize(30);
    // label_ptr->Text("sdl_gui Elements Demo");
    label_ptr->Text("<b>sdl_gui Elements Demo</b>");//title
    //move label to screen top_middle y=5
    float x{0}, y{0};
    y = 5;
    x = sdl_config.window_w/2 - label_ptr->Size().w/2;
    label_ptr->LocalPosition({x,y});

    //<f> Panels
    auto buttons_layout{ m_gui_manager.CreateElement<sdl_gui::Layout>({150,20},{0,0}) };

    //<f> Buttons button
    auto buttons_panel_button{m_gui_manager.CreateElement<sdl_gui::SelectableButton>({0,0}, {100,30})};
    buttons_panel_button->TransitionColourPtr()->SetStateColour(sdl_gui::ButtonState::SELECTED, {200,200,200,255});
    auto panel_label{m_gui_manager.CreateElement<sdl_gui::Label>({0,0}, {0,0})};
    panel_label->FontSize(18);
    panel_label->Text("<b>Buttons</b>");
    panel_label->Parent(buttons_panel_button);
    buttons_panel_button->ResizeToBounds();//resize to fit Label
    buttons_panel_button->FitColliderSize();
    panel_label->AlignWithParentPoint(sdl_gui::AnchorType::MIDDLE_CENTRE);
    //</f>
    //<f> Animation button
    auto animation_panel_button{m_gui_manager.CreateElement<sdl_gui::SelectableButton>({0,0}, {50,30})};
    animation_panel_button->TransitionColourPtr()->SetStateColour(sdl_gui::ButtonState::SELECTED, {200,200,200,255});
    panel_label = m_gui_manager.CreateElement<sdl_gui::Label>({0,0}, {0,0});
    panel_label->FontSize(18);
    panel_label->Text("<b>Animation</b>");
    panel_label->Parent(animation_panel_button);
    animation_panel_button->ResizeToBounds();//resize to fit Label
    animation_panel_button->FitColliderSize();
    panel_label->AlignWithParentPoint(sdl_gui::AnchorType::MIDDLE_CENTRE);
    //</f>
    //<f> Bars & scrolls button
    auto bars_panel_button{m_gui_manager.CreateElement<sdl_gui::SelectableButton>({0,0}, {50,30})};
    bars_panel_button->TransitionColourPtr()->SetStateColour(sdl_gui::ButtonState::SELECTED, {200,200,200,255});
    panel_label = m_gui_manager.CreateElement<sdl_gui::Label>({0,0}, {0,0});
    panel_label->FontSize(18);
    panel_label->Text("<b>Bars & Scrolls</b>");
    panel_label->Parent(bars_panel_button);
    bars_panel_button->ResizeToBounds();//resize to fit Label
    bars_panel_button->FitColliderSize();
    panel_label->AlignWithParentPoint(sdl_gui::AnchorType::MIDDLE_CENTRE);
    //</f>
    //<f> Others button
    auto others_panel_button{m_gui_manager.CreateElement<sdl_gui::SelectableButton>({0,0}, {50,30})};
    others_panel_button->TransitionColourPtr()->SetStateColour(sdl_gui::ButtonState::SELECTED, {200,200,200,255});
    panel_label = m_gui_manager.CreateElement<sdl_gui::Label>({0,0}, {0,0});
    panel_label->FontSize(18);
    panel_label->Text("<b>Other elements</b>");
    panel_label->Parent(others_panel_button);
    others_panel_button->ResizeToBounds();//resize to fit Label
    others_panel_button->FitColliderSize();
    panel_label->AlignWithParentPoint(sdl_gui::AnchorType::MIDDLE_CENTRE);
    //</f>


    buttons_layout->NumOfLines(1);
    buttons_layout->AddElement(buttons_panel_button);
    buttons_layout->AddElement(animation_panel_button);
    buttons_layout->AddElement(bars_panel_button);
    buttons_layout->AddElement(others_panel_button);
    buttons_layout->AllowVariableSizes(true);
    //reposition Layout
    buttons_layout->LocalPosition({ sdl_config.window_w/2 - buttons_layout->Size().w/2 , 50});

    buttons_panel_button->StateChanged = std::bind(&GuiDemo::ChangePanel, this, "btn", std::placeholders::_1);
    animation_panel_button->StateChanged = std::bind(&GuiDemo::ChangePanel, this, "anim", std::placeholders::_1);
    bars_panel_button->StateChanged = std::bind(&GuiDemo::ChangePanel, this, "bars", std::placeholders::_1);
    others_panel_button->StateChanged = std::bind(&GuiDemo::ChangePanel, this, "others", std::placeholders::_1);

    m_current_panel_name = "";
    m_panels["btn"] = m_buttons_panel.Panel();
    m_panels["btn"]->Disable();
    m_panels["btn"]->LocalPosition({sdl_config.window_w/2 - 350.f,105});

    m_panels["anim"] = m_animation_panel.Panel();
    m_panels["anim"]->Disable();
    m_panels["anim"]->LocalPosition({sdl_config.window_w/2 - 350.f,105});

    m_panels["bars"] = m_bars_panel.Panel();
    m_panels["bars"]->Disable();
    m_panels["bars"]->LocalPosition({sdl_config.window_w/2 - 350.f,105});

    m_panels["others"] = m_others_panel.Panel();
    m_panels["others"]->Disable();
    m_panels["others"]->LocalPosition({sdl_config.window_w/2 - 350.f,105});

    buttons_panel_button->IsSelected(true);//select btn
    ChangePanel("btn", buttons_panel_button);

    //</f>//Panels
}

GuiDemo::~GuiDemo()
{

}

void GuiDemo::Input(const SDL_Event& event)
{
    m_gui_manager.Input(event);

    if(event.type == SDL_KEYDOWN)
    {
        switch(event.key.keysym.sym)
        {

        }
    }
}

void GuiDemo::FixedLogic(float fixed_delta_time)
{

}

void GuiDemo::Logic(float delta_time)
{
    m_gui_manager.Logic(delta_time);

    m_animation_panel.Logic(delta_time);

    m_bars_panel.Logic(delta_time);
}

void GuiDemo::Render(SDL_Renderer* renderer, float delta_time)
{
    // SDL_Colour bg{255,255,255,255};
    SDL_Colour bg{128,128,128,255};
    // SDL_Colour bg{34,12,56,255};
    // SDL_Colour bg{12,56,34,255};
    // SDL_Colour bg{12,34,56,255};
    SDL_SetRenderDrawColor( renderer, bg.r, bg.g, bg.b, bg.a );
    SDL_RenderClear( renderer );

    m_gui_manager.Render(delta_time);
}

void GuiDemo::ChangePanel(const std::string& panel_name, sdl_gui::SelectableButton* btn)
{
    if(m_current_panel_name == panel_name)
    {
        if(!btn->IsSelected())//we deselected current panel button
            btn->IsSelected(true);//we force it to stay selected
        return;
    }

    if(m_panels[m_current_panel_name] != nullptr && m_current_panel_button != nullptr)
    {
        m_panels[m_current_panel_name]->Disable();//hide previous
        m_current_panel_button->IsSelected(false);//deselect previous
    }
    if(m_panels[panel_name] != nullptr)
        m_panels[panel_name]->Enable();//show new

    m_current_panel_name = panel_name;
    m_current_panel_button = btn;

    if(m_current_panel_name == "others")
        m_others_panel.EnableTextBoxFix(true);
    else
        m_others_panel.EnableTextBoxFix(false);
}
