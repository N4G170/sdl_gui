#include "animation_panel.hpp"
#include <utility>
#include "sdl_gui_constants.hpp"

//<f> Constructors & operator=
AnimationPanel::AnimationPanel(sdl_gui::GuiManager* manager): m_gui_manager{manager}
{
    m_panel = m_gui_manager->CreateElement<sdl_gui::GuiElement>({0,0}, {0,0});

    m_background = m_gui_manager->CreateElement<sdl_gui::Image>({0,0}, {700,650});
    m_background->Parent(m_panel);
    m_background->RenderIndex(10);
    m_background->ColourModulation({12,34,56,200});

    m_animation = m_gui_manager->CreateElement<sdl_gui::BasicFrameAnimation>({0,0}, {0,0});
    m_animation->Parent(m_panel);

    m_play_btn = m_gui_manager->CreateElement<sdl_gui::BaseButton>({0,0}, {0,0});
    m_play_btn->Parent(m_panel);

    m_pause_btn = m_gui_manager->CreateElement<sdl_gui::BaseButton>({0,0}, {0,0});
    m_pause_btn->Parent(m_panel);

    m_stop_btn = m_gui_manager->CreateElement<sdl_gui::BaseButton>({0,0}, {0,0});
    m_stop_btn->Parent(m_panel);

    m_increase_fps = m_gui_manager->CreateElement<sdl_gui::BaseButton>({0,0}, {0,0});
    m_increase_fps->Parent(m_panel);

    m_decrease_fps = m_gui_manager->CreateElement<sdl_gui::BaseButton>({0,0}, {0,0});
    m_decrease_fps->Parent(m_panel);

    m_fps_label = m_gui_manager->CreateElement<sdl_gui::Label>({0,0}, {0,0});
    m_fps_label->Parent(m_panel);

    m_current_frame_label = m_gui_manager->CreateElement<sdl_gui::Label>({0,0}, {0,0});
    m_current_frame_label->Parent(m_panel);

    m_loop_checkbox = m_gui_manager->CreateElement<sdl_gui::CheckBox>({0,0}, {0,0});
    m_loop_checkbox->Parent(m_panel);

    m_checkbox_label = m_gui_manager->CreateElement<sdl_gui::Label>({0,0}, {0,0});
    m_checkbox_label->Parent(m_panel);

    m_backwards_checkbox = m_gui_manager->CreateElement<sdl_gui::CheckBox>({0,0}, {0,0});
    m_backwards_checkbox->Parent(m_panel);

    m_backwards_checkbox_label = m_gui_manager->CreateElement<sdl_gui::Label>({0,0}, {0,0});
    m_backwards_checkbox_label->Parent(m_panel);

    //set callbacks
    SetCallbacks();

    ConfigElements();

    // auto bounds = m_panel->Bounds(); auto margin{10};
    // m_background->Size( sdl_gui::SizeFromInts(bounds.w + margin, bounds.h) );
}

AnimationPanel::~AnimationPanel() noexcept
{

}

AnimationPanel::AnimationPanel(const AnimationPanel& other): m_gui_manager{other.m_gui_manager}, m_panel{other.m_panel}, m_animation{other.m_animation},
    m_play_btn{other.m_play_btn}, m_pause_btn{other.m_pause_btn}, m_stop_btn{other.m_stop_btn}, m_increase_fps{other.m_increase_fps}, m_decrease_fps{other.m_decrease_fps},
    m_fps_label{other.m_fps_label}, m_current_frame_label{other.m_current_frame_label}, m_loop_checkbox{other.m_loop_checkbox}, m_checkbox_label{other.m_checkbox_label},
    m_backwards_checkbox{other.m_backwards_checkbox}, m_backwards_checkbox_label{other.m_backwards_checkbox_label}
{
    //reset Callbacks
    SetCallbacks();
}

AnimationPanel::AnimationPanel(AnimationPanel&& other) noexcept: m_gui_manager{std::move(other.m_gui_manager)}
{
    m_panel = std::move(other.m_panel);
    m_animation = std::move(other.m_animation);
    m_play_btn = std::move(other.m_play_btn);
    m_pause_btn = std::move(other.m_pause_btn);
    m_stop_btn = std::move(other.m_stop_btn);
    m_increase_fps = std::move(other.m_increase_fps);
    m_decrease_fps = std::move(other.m_decrease_fps);
    m_fps_label = std::move(other.m_fps_label);
    m_current_frame_label = std::move(other.m_current_frame_label);
    m_loop_checkbox = std::move(other.m_loop_checkbox);
    m_checkbox_label = std::move(other.m_checkbox_label);
    m_backwards_checkbox = std::move(other.m_backwards_checkbox);
    m_backwards_checkbox_label = std::move(other.m_backwards_checkbox_label);

    //reset Callbacks
    SetCallbacks();
}

AnimationPanel& AnimationPanel::operator=(const AnimationPanel& other)
{
    if(this != &other)//not same ref
    {
        auto tmp(other);
        *this = std::move(tmp);
    }

    return *this;
}

AnimationPanel& AnimationPanel::operator=(AnimationPanel&& other) noexcept
{
    if(this != &other)//not same ref
    {
        m_gui_manager = std::move(other.m_gui_manager);
        m_panel = std::move(other.m_panel);
        m_animation = std::move(other.m_animation);
        m_play_btn = std::move(other.m_play_btn);
        m_pause_btn = std::move(other.m_pause_btn);
        m_stop_btn = std::move(other.m_stop_btn);
        m_increase_fps = std::move(other.m_increase_fps);
        m_decrease_fps = std::move(other.m_decrease_fps);
        m_fps_label = std::move(other.m_fps_label);
        m_current_frame_label = std::move(other.m_current_frame_label);
        m_loop_checkbox = std::move(other.m_loop_checkbox);
        m_checkbox_label = std::move(other.m_checkbox_label);
        m_backwards_checkbox = std::move(other.m_backwards_checkbox);
        m_backwards_checkbox_label = std::move(other.m_backwards_checkbox_label);

        //reset Callbacks
        SetCallbacks();
    }
    return *this;
}
//</f>

//<f> State
void AnimationPanel::Input(const SDL_Event& event)
{

}

void AnimationPanel::FixedLogic(float fixed_delta_time)
{

}

void AnimationPanel::Logic(float delta_time)
{
    //update labels
    std::string current_frame{"Frame\n"+std::to_string(m_animation->FrameIndex() + 1)+"/"+std::to_string(m_animation->TotalFrames())};

    if(m_current_frame_label->Text() != current_frame)
        m_current_frame_label->Text(current_frame, sdl_gui::Colour::White);

    std::string fps{"<b>"+std::to_string(m_animation->FPS())+"</b> FPS"};

    if(m_fps_label->Text() != fps)
        m_fps_label->Text(fps, sdl_gui::Colour::White);

    std::string loop{};
    if(m_animation->Loop())
        loop.append("<-Loop <b>ON</b>");
    else
        loop.append("<-Loop <b>OFF</b>");

    if(m_checkbox_label->Text() != loop)
        m_checkbox_label->Text(loop, sdl_gui::Colour::White);

    std::string backwards{};
    if(m_animation->Backwards())
        backwards.append("<-Backwards <b>ON</b>");
    else
        backwards.append("<-Backwards <b>OFF</b>");

    if(m_backwards_checkbox_label->Text() != backwards)
        m_backwards_checkbox_label->Text(backwards, sdl_gui::Colour::White);

    //control buttons visibility
    if(m_animation->IsPlaying())
    {
        m_pause_btn->Enable();
        m_play_btn->Disable();

        m_increase_fps->IsActive(false);
        m_decrease_fps->IsActive(false);
    }
    else
    {
        m_play_btn->Enable();
        m_pause_btn->Disable();

        m_increase_fps->IsActive(true);
        m_decrease_fps->IsActive(true);
    }
}

void AnimationPanel::Render(SDL_Renderer* renderer, float delta_time)
{

}
//</f>

void AnimationPanel::ConfigElements()
{
    //<f> Animation Demo
    m_panel->LocalPosition({50,600});
    //Set animation
    auto demo_anim = Animation();
    demo_anim->Size({150,150});
    for(auto i{0}; i< 8; i++)
    {
        demo_anim->AddFrame("data/demo/img/fx-"+std::to_string(i)+".png");
    }
    demo_anim->LocalPosition({5,5});
    //control buttons
    auto stop_btn = StopBtn();
    stop_btn->TransitionType(sdl_gui::ButtonTransitionType::SINGLE_IMAGE);
    stop_btn->TransitionSingleImagePtr()->ChangeTexture("data/demo/img/stop_btn.png");
    stop_btn->TransitionSingleImagePtr()->SetStateTextureSrcRect({150,0,50,50}, {0,0,50,50}, {50,0,50,50}, {100,0,50,50});
    stop_btn->Size({40,40});
    stop_btn->FitColliderSize();
    stop_btn->LocalPosition({5,160});

    auto play_btn = PlayBtn();
    play_btn->TransitionType(sdl_gui::ButtonTransitionType::SINGLE_IMAGE);
    play_btn->TransitionSingleImagePtr()->ChangeTexture("data/demo/img/play_btn.png");
    play_btn->TransitionSingleImagePtr()->SetStateTextureSrcRect({150,0,50,50}, {0,0,50,50}, {50,0,50,50}, {100,0,50,50});
    play_btn->Size({40,40});
    play_btn->FitColliderSize();
    play_btn->LocalPosition({50,160});

    auto pause_btn = PauseBtn();
    pause_btn->TransitionType(sdl_gui::ButtonTransitionType::SINGLE_IMAGE);
    pause_btn->TransitionSingleImagePtr()->ChangeTexture("data/demo/img/pause_btn.png");
    pause_btn->TransitionSingleImagePtr()->SetStateTextureSrcRect({150,0,50,50}, {0,0,50,50}, {50,0,50,50}, {100,0,50,50});
    pause_btn->Size({40,40});
    pause_btn->FitColliderSize();
    pause_btn->LocalPosition({50,160});

    auto current_frame_label = CurrentFrameLabel();
    current_frame_label->FontSize(16);
    current_frame_label->Text("Frame\n10/10");
    current_frame_label->LocalPosition({100,160});

    //change fps
    auto minus_btn = DecreaseFpsBtn();
    minus_btn->TransitionType(sdl_gui::ButtonTransitionType::SINGLE_IMAGE);
    minus_btn->TransitionSingleImagePtr()->ChangeTexture("data/demo/img/minus_btn.png");
    minus_btn->TransitionSingleImagePtr()->SetStateTextureSrcRect({150,0,50,50}, {0,0,50,50}, {50,0,50,50}, {100,0,50,50});
    minus_btn->Size({40,40});
    minus_btn->FitColliderSize();
    minus_btn->LocalPosition({5,210});

    auto plus_btn = IncreaseFpsBtn();
    plus_btn->TransitionType(sdl_gui::ButtonTransitionType::SINGLE_IMAGE);
    plus_btn->TransitionSingleImagePtr()->ChangeTexture("data/demo/img/plus_btn.png");
    plus_btn->TransitionSingleImagePtr()->SetStateTextureSrcRect({150,0,50,50}, {0,0,50,50}, {50,0,50,50}, {100,0,50,50});
    plus_btn->Size({40,40});
    plus_btn->FitColliderSize();
    plus_btn->LocalPosition({50,210});

    auto current_fps = FpsLabel();
    current_fps->FontSize(16);
    current_fps->Text("<b>24</b> FPS", sdl_gui::Colour::White);
    current_fps->LocalPosition({100,220});

    auto loop_checkbox = LoopCheckBox();
    loop_checkbox->Size({25,25});
    loop_checkbox->FitColliderSize();
    loop_checkbox->LocalPosition({5, 270});
    auto loop_check_mark{m_gui_manager->CreateElement<sdl_gui::Image>({0,0}, {15,15})};
    loop_checkbox->CheckMarkElement(loop_check_mark);
    loop_check_mark->AlignWithParentPoint(sdl_gui::AnchorType::MIDDLE_CENTRE);
    loop_check_mark->ColourModulation({0,0,0,255});

    auto loop_checkbox_label = CheckBoxLabel();
    loop_checkbox_label->FontSize(16);
    loop_checkbox_label->Text("<-Loop <b>OFF</b>", sdl_gui::Colour::White);
    loop_checkbox_label->LocalPosition({40,275});

    auto backwards_checkbox = BackwardsCheckBox();
    backwards_checkbox->Size({25,25});
    backwards_checkbox->FitColliderSize();
    backwards_checkbox->LocalPosition({5, 300});
    auto backwards_check_mark{m_gui_manager->CreateElement<sdl_gui::Image>({0,0}, {15,15})};
    backwards_checkbox->CheckMarkElement(backwards_check_mark);
    backwards_check_mark->AlignWithParentPoint(sdl_gui::AnchorType::MIDDLE_CENTRE);
    backwards_check_mark->ColourModulation({0,0,0,255});

    auto backwards_checkbox_label = BackwardsCheckBoxLabel();
    backwards_checkbox_label->FontSize(16);
    backwards_checkbox_label->Text("<-Backwards <b>OFF</b>", sdl_gui::Colour::White);
    backwards_checkbox_label->LocalPosition({40,305});
    //</f>
}

void AnimationPanel::SetCallbacks()
{
    m_increase_fps->MouseInteractionPtr()->MouseButtonCallback(SDL_BUTTON_LEFT, sdl_gui::InputKeyCallbackType::CLICK,
                    std::bind(&sdl_gui::BasicFrameAnimation::IncreaseFPS, m_animation, 1));
    m_decrease_fps->MouseInteractionPtr()->MouseButtonCallback(SDL_BUTTON_LEFT, sdl_gui::InputKeyCallbackType::CLICK,
                    std::bind(&sdl_gui::BasicFrameAnimation::DecreaseFPS, m_animation, 1));

    m_play_btn->MouseInteractionPtr()->MouseButtonCallback(SDL_BUTTON_LEFT, sdl_gui::InputKeyCallbackType::CLICK,
                    std::bind(&sdl_gui::BasicFrameAnimation::Play, m_animation));
    m_pause_btn->MouseInteractionPtr()->MouseButtonCallback(SDL_BUTTON_LEFT, sdl_gui::InputKeyCallbackType::CLICK,
                    std::bind(&sdl_gui::BasicFrameAnimation::Pause, m_animation));
    m_stop_btn->MouseInteractionPtr()->MouseButtonCallback(SDL_BUTTON_LEFT, sdl_gui::InputKeyCallbackType::CLICK,
                    std::bind(&sdl_gui::BasicFrameAnimation::Stop, m_animation));

    m_loop_checkbox->StateChanged = std::bind(&AnimationPanel::ChangeLoop, this, std::placeholders::_1);
    m_backwards_checkbox->StateChanged = std::bind(&AnimationPanel::ChangeBackwards, this, std::placeholders::_1);
}

void AnimationPanel::ChangeLoop(sdl_gui::SelectableButton* checkbox)
{
    m_animation->Loop(checkbox->IsSelected());
}

void AnimationPanel::ChangeBackwards(sdl_gui::SelectableButton* checkbox)
{
    m_animation->Backwards(checkbox->IsSelected());
}
