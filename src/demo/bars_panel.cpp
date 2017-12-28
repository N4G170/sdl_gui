#include "bars_panel.hpp"
#include "sdl_gui_constants.hpp"
#include <utility>
#include <algorithm>

//<f> Constructors & operator=
BarsPanel::BarsPanel(sdl_gui::GuiManager* gui_manager): m_gui_manager{gui_manager}, m_playing{false}, m_loop{false}, m_step{0.05f}, m_timer{0} , m_value_direction_h{1},
    m_value_direction_v{1}, m_run_sim_h{false}, m_run_sim_v{false}
{
    auto y{5.f};

    m_panel = m_gui_manager->CreateElement<sdl_gui::GuiElement>({0,0}, {0,0});

    m_background = m_gui_manager->CreateElement<sdl_gui::Image>({0,0}, {700,650});
    m_background->Parent(m_panel);
    m_background->RenderIndex(10);
    m_background->ColourModulation({12,34,56,200});


    //<f> Progress Bars
    // auto pb_x_spacing{10.f};
    auto pb_y_spacing{10.f};
    auto regular_y_jump{25 + pb_y_spacing};

    m_bar_container = m_gui_manager->CreateElement<sdl_gui::GuiElement>({0,0}, {0,0});
    m_bar_container->Parent(m_panel);

    //<f> Horizontal
    //create label
    m_ratio_label = m_gui_manager->CreateElement<sdl_gui::Label>({0,0}, {0,0});
    m_ratio_label->Parent(m_bar_container);
    y = 5;
    m_ratio_label->LocalPosition({5,y});
    m_ratio_label->Text("Ratio:", sdl_gui::Colour::White);

    //create & config bar
    m_progress_bar_h = m_gui_manager->CreateElement<sdl_gui::ProgressBar>({0,0}, {200,25});
    m_progress_bar_h->Parent(m_bar_container);
    y += m_ratio_label->Size().h + pb_y_spacing;
    m_progress_bar_h->LocalPosition({5,y});
    //background
    auto img = m_gui_manager->CreateElement<sdl_gui::Image>({0,0}, {200,25});
    m_progress_bar_h->BackgroundElement(img);
    //bar
    auto img2 = m_gui_manager->CreateElement<sdl_gui::Image>({0,0}, {200,25});
    img2->ColourModulation({0,0,0,255});
    m_progress_bar_h->BarElement(img2);
    img2->LocalPosition({img->ScaledSize().w, img->ScaledSize().h / 2});

    m_progress_bar_h->BarValueChangedCallback(std::bind(&BarsPanel::UpdateRatioLabel, this, std::ref(m_ratio_label), std::placeholders::_1));
    //</f> /Horizontal

    //<f> Vertical
    //create & config bar
    m_progress_bar_v = m_gui_manager->CreateElement<sdl_gui::ProgressBar>({0,0}, {25,200});
    m_progress_bar_v->Parent(m_bar_container);
    y += regular_y_jump;
    m_progress_bar_v->LocalPosition({5,y});
    m_progress_bar_v->BarDirection(sdl_gui::ProgressBarDirection::DOWN);
    //background
    auto img_v = m_gui_manager->CreateElement<sdl_gui::Image>({0,0}, {25,200});
    m_progress_bar_v->BackgroundElement(img_v);
    //bar
    auto img2_v = m_gui_manager->CreateElement<sdl_gui::Image>({0,0}, {25,200});
    img2_v->ColourModulation({0,0,0,255});
    m_progress_bar_v->BarElement(img2_v);
    img2_v->LocalPosition({img_v->ScaledSize().w, img_v->ScaledSize().h / 2});
    //</f> /Vertical

    //<f> Controls

    //<f> Play Pause
    m_play_btn = m_gui_manager->CreateElement<sdl_gui::BaseButton>({0,0}, {25,25});
    m_play_btn->Parent(m_bar_container);
    m_play_btn->LocalPosition({65,y});
    m_play_btn->TransitionType(sdl_gui::ButtonTransitionType::SINGLE_IMAGE);
    m_play_btn->TransitionSingleImagePtr()->ChangeTexture("data/demo/img/play_btn.png");
    m_play_btn->TransitionSingleImagePtr()->SetStateTextureSrcRect({150,0,50,50}, {0,0,50,50}, {50,0,50,50}, {100,0,50,50});
    m_play_btn->MouseInteractionPtr()->MouseButtonCallback(SDL_BUTTON_LEFT, sdl_gui::InputKeyCallbackType::CLICK,
        std::bind(&BarsPanel::PlayPause, this, true));

    m_pause_btn = m_gui_manager->CreateElement<sdl_gui::BaseButton>({0,0}, {25,25});
    m_pause_btn->Parent(m_bar_container);
    m_pause_btn->LocalPosition({35,y});
    m_pause_btn->TransitionType(sdl_gui::ButtonTransitionType::SINGLE_IMAGE);
    m_pause_btn->TransitionSingleImagePtr()->ChangeTexture("data/demo/img/pause_btn.png");
    m_pause_btn->TransitionSingleImagePtr()->SetStateTextureSrcRect({150,0,50,50}, {0,0,50,50}, {50,0,50,50}, {100,0,50,50});
    m_pause_btn->MouseInteractionPtr()->MouseButtonCallback(SDL_BUTTON_LEFT, sdl_gui::InputKeyCallbackType::CLICK,
        std::bind(&BarsPanel::PlayPause, this, false));
    PlayPause(false);
    //</f> /Play Pause

    //<f> Steps
    y += regular_y_jump;
    //Step
    m_step_info = m_gui_manager->CreateElement<sdl_gui::Label>({0,0}, {0,0});
    m_step_info->Parent(m_bar_container);
    m_step_info->LocalPosition({35,y});
    m_step_info->Text("Step values range from 0 to 0.1(10%)\nand can be changed by +-0.01", sdl_gui::Colour::White);

    y += m_step_info->Size().h + pb_y_spacing;
    //+- step
    m_minus_btn = m_gui_manager->CreateElement<sdl_gui::BaseButton>({0,0}, {25,25});
    m_minus_btn->Parent(m_bar_container);
    m_minus_btn->LocalPosition({35,y});
    m_minus_btn->TransitionType(sdl_gui::ButtonTransitionType::SINGLE_IMAGE);
    m_minus_btn->TransitionSingleImagePtr()->ChangeTexture("data/demo/img/minus_btn.png");
    m_minus_btn->TransitionSingleImagePtr()->SetStateTextureSrcRect({150,0,50,50}, {0,0,50,50}, {50,0,50,50}, {100,0,50,50});
    m_minus_btn->MouseInteractionPtr()->MouseButtonCallback(SDL_BUTTON_LEFT, sdl_gui::InputKeyCallbackType::CLICK,
        [this](){ m_step = (m_step - 0.01) < 0 ? 0 : m_step - 0.01; m_step_label->Text("Step: "+std::to_string(m_step), sdl_gui::Colour::White); } );

    m_step_label = m_gui_manager->CreateElement<sdl_gui::Label>({0,0}, {0,0});
    m_step_label->Parent(m_bar_container);
    m_step_label->LocalPosition({95,y});
    m_step_label->Text("Step: "+std::to_string(m_step), sdl_gui::Colour::White);

    m_plus_btn = m_gui_manager->CreateElement<sdl_gui::BaseButton>({0,0}, {25,25});
    m_plus_btn->Parent(m_bar_container);
    m_plus_btn->LocalPosition({65,y});
    m_plus_btn->TransitionType(sdl_gui::ButtonTransitionType::SINGLE_IMAGE);
    m_plus_btn->TransitionSingleImagePtr()->ChangeTexture("data/demo/img/plus_btn.png");
    m_plus_btn->TransitionSingleImagePtr()->SetStateTextureSrcRect({150,0,50,50}, {0,0,50,50}, {50,0,50,50}, {100,0,50,50});
    m_plus_btn->MouseInteractionPtr()->MouseButtonCallback(SDL_BUTTON_LEFT, sdl_gui::InputKeyCallbackType::CLICK,
        [this](){ m_step = ((m_step + 0.01) > 0.1 ? 0.1 : m_step + 0.01); m_step_label->Text("Step: "+std::to_string(m_step), sdl_gui::Colour::White); } );
    //</f> /Steps

    //<f> Loop
    y += regular_y_jump;
    //Loop
    m_loop_checkbox = m_gui_manager->CreateElement<sdl_gui::CheckBox>({0,0}, {25,25});
    m_loop_checkbox->Parent(m_bar_container);
    m_loop_checkbox->LocalPosition({35,y});
    //checkmark
    auto mark = m_gui_manager->CreateElement<sdl_gui::Image>({0,0}, {15,15});
    mark->ColourModulation({0,0,0,255});
    m_loop_checkbox->CheckMarkElement(mark);
    mark->AlignWithParentPoint(sdl_gui::AnchorType::MIDDLE_CENTRE);
    //callback
    m_loop_checkbox->StateChanged = std::bind(&BarsPanel::ChangeLoop, this, std::placeholders::_1);

    m_loop_label = m_gui_manager->CreateElement<sdl_gui::Label>({0,0}, {0,0});
    m_loop_label->Parent(m_bar_container);
    m_loop_label->LocalPosition({65,y});
    ChangeLoop(m_loop_checkbox);
    //</f> /Loop

    //<f> Reset
    y += regular_y_jump;
    //reset
    m_reset_btn = m_gui_manager->CreateElement<sdl_gui::BaseButton>({0,0}, {25,25});
    m_reset_btn->Parent(m_bar_container);
    m_reset_btn->LocalPosition({35,y});
    m_reset_btn->TransitionType(sdl_gui::ButtonTransitionType::SINGLE_IMAGE);
    m_reset_btn->TransitionSingleImagePtr()->ChangeTexture("data/demo/img/reset_btn.png");
    m_reset_btn->TransitionSingleImagePtr()->SetStateTextureSrcRect({150,0,50,50}, {0,0,50,50}, {50,0,50,50}, {100,0,50,50});
    m_reset_btn->MouseInteractionPtr()->MouseButtonCallback(SDL_BUTTON_LEFT, sdl_gui::InputKeyCallbackType::CLICK,
        [this](){ m_progress_bar_h->Ratio(0); m_progress_bar_v->Ratio(0); } );
    //reset Label
    m_reset_label = m_gui_manager->CreateElement<sdl_gui::Label>({0,0}, {0,0});
    m_reset_label->Parent(m_bar_container);
    m_reset_label->LocalPosition({65,y});
    m_reset_label->Text("Reset bar to minimum", sdl_gui::Colour::White);
    //</f> /Reset

    //<f> Directions
    y += regular_y_jump;

    m_directions_label = m_gui_manager->CreateElement<sdl_gui::Label>({0,0}, {0,0});
    m_directions_label->Parent(m_bar_container);
    m_directions_label->LocalPosition({35,y});
    m_directions_label->Text("Directions\nTop for horizontal\nBottom for vertical", sdl_gui::Colour::White);

    //<f> Horizontal
    y += m_directions_label->Size().h + pb_y_spacing;
    //direction btns
    m_direction_btn_left = m_gui_manager->CreateElement<sdl_gui::BaseButton>({0,0}, {25,25});
    m_direction_btn_left->Parent(m_bar_container);
    m_direction_btn_left->LocalPosition({35,y});
    m_direction_btn_left->TransitionType(sdl_gui::ButtonTransitionType::SINGLE_IMAGE);
    m_direction_btn_left->TransitionSingleImagePtr()->ChangeTexture("data/demo/img/direction_btn.png");
    m_direction_btn_left->TransitionSingleImagePtr()->SetStateTextureSrcRect({150,50,50,50}, {0,50,50,50}, {50,50,50,50}, {100,50,50,50});
    m_direction_btn_left->MouseInteractionPtr()->MouseButtonCallback(SDL_BUTTON_LEFT, sdl_gui::InputKeyCallbackType::CLICK,
        std::bind(&BarsPanel::ChangeDirection, this, "left"));

    m_direction_btn_right = m_gui_manager->CreateElement<sdl_gui::BaseButton>({0,0}, {25,25});
    m_direction_btn_right->Parent(m_bar_container);
    m_direction_btn_right->LocalPosition({65,y});
    m_direction_btn_right->TransitionType(sdl_gui::ButtonTransitionType::SINGLE_IMAGE);
    m_direction_btn_right->TransitionSingleImagePtr()->ChangeTexture("data/demo/img/direction_btn.png");
    m_direction_btn_right->TransitionSingleImagePtr()->SetStateTextureSrcRect({150,150,50,50}, {0,150,50,50}, {50,150,50,50}, {100,150,50,50});
    m_direction_btn_right->MouseInteractionPtr()->MouseButtonCallback(SDL_BUTTON_LEFT, sdl_gui::InputKeyCallbackType::CLICK,
        std::bind(&BarsPanel::ChangeDirection, this, "right"));

    ChangeDirection("left");
    //</f> /Horizontal
    //<f> Vertical
    y += regular_y_jump;
    //direction btns
    m_direction_btn_up = m_gui_manager->CreateElement<sdl_gui::BaseButton>({0,0}, {25,25});
    m_direction_btn_up->Parent(m_bar_container);
    m_direction_btn_up->LocalPosition({35,y});
    m_direction_btn_up->TransitionType(sdl_gui::ButtonTransitionType::SINGLE_IMAGE);
    m_direction_btn_up->TransitionSingleImagePtr()->ChangeTexture("data/demo/img/direction_btn.png");
    m_direction_btn_up->TransitionSingleImagePtr()->SetStateTextureSrcRect({150,0,50,50}, {0,0,50,50}, {50,0,50,50}, {100,0,50,50});
    m_direction_btn_up->MouseInteractionPtr()->MouseButtonCallback(SDL_BUTTON_LEFT, sdl_gui::InputKeyCallbackType::CLICK,
        std::bind(&BarsPanel::ChangeDirection, this, "up"));

    m_direction_btn_down = m_gui_manager->CreateElement<sdl_gui::BaseButton>({0,0}, {25,25});
    m_direction_btn_down->Parent(m_bar_container);
    m_direction_btn_down->LocalPosition({65,y});
    m_direction_btn_down->TransitionType(sdl_gui::ButtonTransitionType::SINGLE_IMAGE);
    m_direction_btn_down->TransitionSingleImagePtr()->ChangeTexture("data/demo/img/direction_btn.png");
    m_direction_btn_down->TransitionSingleImagePtr()->SetStateTextureSrcRect({150,100,50,50}, {0,100,50,50}, {50,100,50,50}, {100,100,50,50});
    m_direction_btn_down->MouseInteractionPtr()->MouseButtonCallback(SDL_BUTTON_LEFT, sdl_gui::InputKeyCallbackType::CLICK,
        std::bind(&BarsPanel::ChangeDirection, this, "down"));

    ChangeDirection("up");
    //</f> /Vertical
    //</f> /Directions

    //<f> Select Bar
    y += regular_y_jump;
    m_select_bar_label = m_gui_manager->CreateElement<sdl_gui::Label>({0,0},{0,0});
    m_select_bar_label->Parent(m_bar_container);
    m_select_bar_label->LocalPosition({35,y});
    m_select_bar_label->Text("Turn ON/OFF simulation", sdl_gui::Colour::White);

    y += regular_y_jump;
    m_select_bar_group = m_gui_manager->CreateElement<sdl_gui::SelectableGroup>({0,0}, {0,0});
    m_select_bar_group->Parent(m_bar_container);
    m_select_bar_group->LocalPosition({35,y});

    //<f> horizontal select
    //horizontal
    m_select_bar_checkbox_h = m_gui_manager->CreateElement<sdl_gui::CheckBox>({0,0}, {25,25});
    m_select_bar_group->AddSelectable(m_select_bar_checkbox_h);
    m_select_bar_checkbox_h->LocalPosition({0,0});//kid of group
    //checkmark
    auto select_mark_h = m_gui_manager->CreateElement<sdl_gui::Image>({0,0}, {15,15});
    select_mark_h->ColourModulation({0,0,0,255});
    m_select_bar_checkbox_h->CheckMarkElement(select_mark_h);
    select_mark_h->AlignWithParentPoint(sdl_gui::AnchorType::MIDDLE_CENTRE);

    m_select_bar_checkbox_h->StateChanged = [this](sdl_gui::SelectableButton* selectable)
    {
        if(selectable->IsSelected())
        {
            m_run_sim_h = true;
            m_select_bar_label_h->Text("Horizontal bar <b>ON</b>", sdl_gui::Colour::White);
        }
        else
        {
            m_run_sim_h = false;
            m_select_bar_label_h->Text("Horizontal bar <b>OFF</b>", sdl_gui::Colour::White);
        }
    };

    m_select_bar_label_h = m_gui_manager->CreateElement<sdl_gui::Label>({0,0},{0,0});
    m_select_bar_label_h->Parent(m_select_bar_group);
    m_select_bar_label_h->LocalPosition({35,0});
    m_select_bar_label_h->Text("Horizontal bar <b>OFF</b>", sdl_gui::Colour::White);
    //</f> /horizontal select

    //<f> vertical select
    //vertical
    m_select_bar_checkbox_v = m_gui_manager->CreateElement<sdl_gui::CheckBox>({0,0}, {25,25});
    m_select_bar_group->AddSelectable(m_select_bar_checkbox_v);
    m_select_bar_checkbox_v->LocalPosition({0, 25 + pb_y_spacing});//kid of group
    //checkmark
    auto select_mark_v = m_gui_manager->CreateElement<sdl_gui::Image>({0,0}, {15,15});
    select_mark_v->ColourModulation({0,0,0,255});
    m_select_bar_checkbox_v->CheckMarkElement(select_mark_v);
    select_mark_v->AlignWithParentPoint(sdl_gui::AnchorType::MIDDLE_CENTRE);

    m_select_bar_checkbox_v->StateChanged = [this](sdl_gui::SelectableButton* selectable)
    {
        if(selectable->IsSelected())
        {
            m_run_sim_v = true;
            m_select_bar_label_v->Text("Vertical bar <b>ON</b>", sdl_gui::Colour::White);
        }
        else
        {
            m_run_sim_v = false;
            m_select_bar_label_v->Text("Vertical bar <b>OFF</b>", sdl_gui::Colour::White);
        }
    };

    m_select_bar_label_v = m_gui_manager->CreateElement<sdl_gui::Label>({0,0},{0,0});
    m_select_bar_label_v->Parent(m_select_bar_group);
    m_select_bar_label_v->LocalPosition({35, 25 + pb_y_spacing});
    m_select_bar_label_v->Text("Vertical bar <b>OFF</b>", sdl_gui::Colour::White);
    //</f> /vertical select
    //</f> /Select Bar

    //</f> /Controls

    // auto bounds = m_panel->Bounds(); auto margin{10};
    // m_background->Size( sdl_gui::SizeFromInts(bounds.w + margin, bounds.h + 5) );
    //</f> /Progress Bars

    //<f> Sliders
    y = 10;

    m_slider_container = m_gui_manager->CreateElement<sdl_gui::GuiElement>({250,0}, {0,0});
    m_slider_container->Parent(m_panel);

    m_h_slider = m_gui_manager->CreateElement<sdl_gui::HSlider>({350,350}, {200,25});
    m_h_slider_label = m_gui_manager->CreateElement<sdl_gui::Label>({1375,50},{0,0});
    m_h_slider_label->Text("H slider value: ", sdl_gui::Colour::White);
    m_h_slider->ValueChangedCallback([this](float value){ m_h_slider_label->Text("H slider value: "+std::to_string(value), sdl_gui::Colour::White); });
    m_h_slider->Parent(m_slider_container);
    m_h_slider->LocalPosition({105,y});
    y += 30;
    m_h_slider_label->Parent(m_slider_container);
    m_h_slider_label->LocalPosition({105,y});

    m_v_slider = m_gui_manager->CreateElement<sdl_gui::VSlider>({350,400}, {25,200});
    m_v_slider_label = m_gui_manager->CreateElement<sdl_gui::Label>({1375,50},{0,0});
    m_v_slider_label->Text("V slider value: ", sdl_gui::Colour::White);
    m_v_slider->ValueChangedCallback([this](float value){ m_v_slider_label->Text("V slider value: "+std::to_string(value), sdl_gui::Colour::White); });
    m_v_slider->Parent(m_slider_container);
    m_v_slider->LocalPosition({65,y});
    y += 30;
    m_v_slider_label->Parent(m_slider_container);
    m_v_slider_label->LocalPosition({105,y});

    m_scrollbar = m_gui_manager->CreateElement<sdl_gui::VScrollbar>({1300,50}, {25,200});
    m_scroll_label = m_gui_manager->CreateElement<sdl_gui::Label>({1375,50},{0,0});
    m_scroll_label->Text("Scroll value:", sdl_gui::Colour::White);
    m_scrollbar->ValueChangedCallback([this](float value){ m_scroll_label->Text("Scroll value: "+std::to_string(value), sdl_gui::Colour::White); });
    y += 175;
    m_scrollbar->Parent(m_slider_container);
    m_scrollbar->LocalPosition({65,y});

    m_scroll_label->Parent(m_slider_container);
    m_scroll_label->LocalPosition({105,y});
    //</f> /Sliders
}

BarsPanel::~BarsPanel() noexcept
{
    //all gui pointers are managed by the manager
}
//</f>

void BarsPanel::Logic(float delta_time)
{
    if(m_playing)
    {
        m_timer += delta_time;

        if(m_timer >= 0.05)
        {
            //<f> Vertical
            if(m_run_sim_v)
            {
                float ratio{m_progress_bar_v->Ratio()};
                m_progress_bar_v->Ratio(ratio + m_step * m_value_direction_v);
                if(m_progress_bar_v->Ratio() >= 1 && m_value_direction_v > 0)//full
                {
                    if(m_loop)
                        m_value_direction_v = -1;
                    else
                        PlayPause(false);
                }
                else if(m_progress_bar_v->Ratio() <= 0 && m_value_direction_v < 0)
                {
                    if(m_loop)
                        m_value_direction_v = 1;
                    else
                        PlayPause(false);
                }
            }
            //</f> /Vertical

            //<f> Horizontal
            if(m_run_sim_h)
            {
                float ratio{m_progress_bar_h->Ratio()};
                m_progress_bar_h->Ratio(ratio + m_step * m_value_direction_h);
                if(m_progress_bar_h->Ratio() >= 1 && m_value_direction_h > 0)//full
                {
                    if(m_loop)
                        m_value_direction_h = -1;
                    else
                        PlayPause(false);
                }
                else if(m_progress_bar_h->Ratio() <= 0 && m_value_direction_h < 0)
                {
                    if(m_loop)
                        m_value_direction_h = 1;
                    else
                        PlayPause(false);
                }
            }
            //</f> /Horizontal
        }
    }

}

void BarsPanel::UpdateRatioLabel(sdl_gui::Label* label, sdl_gui::ProgressBar* bar)
{
    label->Text("Ratio: "+std::to_string(bar->Ratio()), sdl_gui::Colour::White);
}

void BarsPanel::ChangeDirection(const std::string& direction)
{
    if(direction == "left")
    {
        m_direction_btn_right->Activate();
        m_direction_btn_left->Deactivate();

        m_progress_bar_h->BarDirection(sdl_gui::ProgressBarDirection::LEFT);
        auto bar{m_progress_bar_h->BarElement()};
        bar->LocalPosition({bar->Size().w, bar->Size().h / 2});
    }
    else if(direction == "right")
    {
        m_direction_btn_left->Activate();
        m_direction_btn_right->Deactivate();

        m_progress_bar_h->BarDirection(sdl_gui::ProgressBarDirection::RIGHT);
        auto bar{m_progress_bar_h->BarElement()};
        bar->LocalPosition({0, bar->Size().h / 2});
    }
    else if(direction == "up")
    {
        m_direction_btn_down->Activate();
        m_direction_btn_up->Deactivate();

        m_progress_bar_v->BarDirection(sdl_gui::ProgressBarDirection::UP);
        auto bar{m_progress_bar_v->BarElement()};
        bar->LocalPosition({bar->Size().w / 2, bar->Size().h});
    }
    else if(direction == "down")
    {
        m_direction_btn_up->Activate();
        m_direction_btn_down->Deactivate();

        m_progress_bar_v->BarDirection(sdl_gui::ProgressBarDirection::DOWN);
        auto bar{m_progress_bar_v->BarElement()};
        bar->LocalPosition({bar->Size().w / 2, 0});
    }
}

void BarsPanel::PlayPause(bool play)
{
    if(play)
    {
        m_play_btn->Deactivate();
        m_pause_btn->Activate();

        m_playing = true;
    }
    else
    {
        m_pause_btn->Deactivate();
        m_play_btn->Activate();

        m_playing = false;
    }
}
void BarsPanel::ChangeLoop(sdl_gui::SelectableButton* checkbox)
{
    if(checkbox->IsSelected())
    {
        m_loop_label->Text("Loop <b>ON</b>", sdl_gui::Colour::White);
        m_loop = true;
    }
    else
    {
        m_loop_label->Text("Loop <b>OFF</b>", sdl_gui::Colour::White);
        m_loop = false;
    }
}
