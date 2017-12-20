#include "buttons_panel.hpp"
#include "sdl_gui_constants.hpp"
#include <utility>

//<f> Constructors & operator=
ButtonsPanel::ButtonsPanel(sdl_gui::GuiManager* manager) : m_gui_manager{manager}
{
    // most of these variables will be reused through the constructor
    float x{0}, y{0}, prev_w{0}, prev_h{0};
    sdl_gui::Label* label_ptr{ nullptr };

    //<f> Buttons demo
    //parent element for all button demo elements
    m_panel = m_gui_manager->CreateElement<sdl_gui::GuiElement>({25,25}, {0,0});
    std::vector<sdl_gui::Image*> lines;

    //Area bg image
    auto img = m_gui_manager->CreateElement<sdl_gui::Image>({0,0}, {700,650});
    img->ColourModulation({12,34,56,200});
    img->Parent(m_panel);
    x = 0; y = 0;
    img->LocalPosition({x, y});
    img->RenderIndex(1);

    //base buttons
    label_ptr = m_gui_manager->CreateElement<sdl_gui::Label>({5,0}, {0,0});
    label_ptr->Text("sdl_gui::BaseButton - the base for a button, \ncan have one of three types of transition", sdl_gui::Colour::White);
    label_ptr->Parent(m_panel);
    x = 5; y += 5;
    label_ptr->LocalPosition({x, y});
    prev_h = label_ptr->Size().h;

    //<f> Colour
    label_ptr = m_gui_manager->CreateElement<sdl_gui::Label>({0,0}, {0,0});
    label_ptr->Text("Colour transition -> Button made of a single white image\nwhere you choose its colour for every state\n(left:active | right:inactive) - Default colours", sdl_gui::Colour::White);
    label_ptr->Parent(m_panel);
    y += prev_h + 5;
    label_ptr->LocalPosition({x, y});
    prev_h = label_ptr->Size().h;

    //LEFT colour example button (default transition)
    auto btn = m_gui_manager->CreateElement<sdl_gui::BaseButton>({0,0}, {100, 25});
    btn->Parent(m_panel);
    y += prev_h + 5;
    btn->LocalPosition({x, y});
    prev_w = btn->Size().w;
    //RIGHT
    btn = m_gui_manager->CreateElement<sdl_gui::Button>({0,0}, {100, 25});
    btn->IsActive(false);
    btn->Parent(m_panel);
    btn->LocalPosition({x + prev_w + 5, y});//same y and to the right
    prev_h = btn->Size().h;
    //</f>//Colour

    //---------------------------------------------------------------------------------------------------------
    lines.push_back( m_gui_manager->CreateElement<sdl_gui::Image>({0,0}, {10, 2}) );
    lines.back()->ColourModulation({0,0,0,255});
    lines.back()->Parent(m_panel);
    y += prev_h + 5;
    lines.back()->LocalPosition({img->LocalPosition().x, y});
    prev_h = lines.back()->Size().h;
    //---------------------------------------------------------------------------------------------------------

    //<f> Single
    label_ptr = m_gui_manager->CreateElement<sdl_gui::Label>({0,0}, {0,0});
    label_ptr->Text("Single image transition -> Button made of a section(SDL_Rect) of a single image\n(left:active | right:inactive) - Default image", sdl_gui::Colour::White);
    label_ptr->Parent(m_panel);
    y += prev_h + 5;
    label_ptr->LocalPosition({x, y});
    prev_h = label_ptr->Size().h;

    //single image example button
    btn = m_gui_manager->CreateElement<sdl_gui::BaseButton>({0,0}, {100, 25});
    btn->Parent(m_panel);
    y += prev_h + 5;
    btn->LocalPosition({x, y});
    prev_h = btn->Size().h;
    //set single image
    btn->TransitionType(sdl_gui::ButtonTransitionType::SINGLE_IMAGE);
    btn->TransitionSingleImagePtr()->SetStateTextureSrcRect({0,0,1,1}, {0,1,1,1}, {0,2,1,1}, {0,3,1,1});
    prev_w = btn->Size().w;
    //RIGHT
    btn = m_gui_manager->CreateElement<sdl_gui::Button>({0,0}, {100, 25});
    btn->IsActive(false);
    btn->Parent(m_panel);
    btn->LocalPosition({x + prev_w + 5, y});//same y and to the right
    prev_h = btn->Size().h;
    //set single image
    btn->TransitionType(sdl_gui::ButtonTransitionType::SINGLE_IMAGE);
    btn->TransitionSingleImagePtr()->SetStateTextureSrcRect({0,0,1,1}, {0,1,1,1}, {0,2,1,1}, {0,3,1,1});
    //</f>//Single

    //---------------------------------------------------------------------------------------------------------
    lines.push_back( m_gui_manager->CreateElement<sdl_gui::Image>({0,0}, {10, 2}) );
    lines.back()->ColourModulation({0,0,0,255});
    lines.back()->Parent(m_panel);
    y += prev_h + 5;
    lines.back()->LocalPosition({img->LocalPosition().x, y});
    prev_h = lines.back()->Size().h;
    //---------------------------------------------------------------------------------------------------------

    //<f> Multi
    label_ptr = m_gui_manager->CreateElement<sdl_gui::Label>({0,0}, {0,0});
    label_ptr->Text("Multiple image transition -> Button made of an image per transition state\n(left:active | right:inactive) - Default images", sdl_gui::Colour::White);
    label_ptr->Parent(m_panel);
    y += prev_h + 5;
    label_ptr->LocalPosition({x, y});
    prev_h = label_ptr->Size().h;

    //single image example button
    btn = m_gui_manager->CreateElement<sdl_gui::BaseButton>({0,0}, {100, 25});
    btn->Parent(m_panel);
    y += prev_h + 5;
    btn->LocalPosition({x, y});
    prev_h = btn->Size().h;
    //set multi image
    btn->TransitionType(sdl_gui::ButtonTransitionType::MULTI_IMAGE);
    prev_w = btn->Size().w;
    //RIGHT
    btn = m_gui_manager->CreateElement<sdl_gui::BaseButton>({0,0}, {100, 25});
    btn->IsActive(false);
    btn->Parent(m_panel);
    btn->LocalPosition({x + prev_w + 5, y});//same y and to the right
    prev_h = btn->Size().h;
    //set multi image
    btn->TransitionType(sdl_gui::ButtonTransitionType::MULTI_IMAGE);
    //</f>//Multi

    //---------------------------------------------------------------------------------------------------------
    lines.push_back( m_gui_manager->CreateElement<sdl_gui::Image>({0,0}, {10, 2}) );
    lines.back()->ColourModulation({0,0,0,255});
    lines.back()->Parent(m_panel);
    y += prev_h + 5;
    lines.back()->LocalPosition({img->LocalPosition().x, y});
    prev_h = lines.back()->Size().h;
    //---------------------------------------------------------------------------------------------------------

    //<f> Button with Text
    label_ptr = m_gui_manager->CreateElement<sdl_gui::Label>({0,0}, {0,0});
    label_ptr->Text("Button with label -> Basic button (has previous transitions) and with a built-in label\n(This button class exists for convenience as it can be built by attaching a label to basic button)", sdl_gui::Colour::White);
    label_ptr->Parent(m_panel);
    y += prev_h + 5;
    label_ptr->LocalPosition({x, y});
    prev_h = label_ptr->Size().h;
    //button
    auto btn_with_text = m_gui_manager->CreateElement<sdl_gui::Button>({0,0}, {150,25});
    btn_with_text->Parent(m_panel);
    y += prev_h + 5;
    btn_with_text->LocalPosition({x, y});
    btn_with_text->CreateLabel("Button with text", sdl_gui::c_default_font_path, 15, sdl_gui::Colour::Black, {0,0});
    btn_with_text->CentreLabel();
    //</f>//Button with Text

    lines.push_back( m_gui_manager->CreateElement<sdl_gui::Image>({0,0}, {10, 2}) );
    lines.back()->ColourModulation({0,0,0,255});
    lines.back()->Parent(m_panel);
    y += prev_h + 5;
    lines.back()->LocalPosition({img->LocalPosition().x, y});
    prev_h = lines.back()->Size().h;

    //<f> Selectables
    label_ptr = m_gui_manager->CreateElement<sdl_gui::Label>({0,0}, {0,0});
    label_ptr->Text("Selectable Button -> Basic button (has previous transitions) that can be selected.\nUsed as the base for checkboxes", sdl_gui::Colour::White);
    label_ptr->Parent(m_panel);
    y += prev_h + 5;
    label_ptr->LocalPosition({x, y});
    prev_h = label_ptr->Size().h;

    auto selectable = m_gui_manager->CreateElement<sdl_gui::SelectableButton>({0,0}, {100,25});
    selectable->TransitionColourPtr()->SetStateColour(sdl_gui::ButtonState::SELECTED, {128,128,128,255});
    selectable->Parent(m_panel);
    y += prev_h + 5;
    selectable->LocalPosition({x,y});
    prev_h = selectable->Size().h;

    lines.push_back( m_gui_manager->CreateElement<sdl_gui::Image>({0,0}, {10, 2}) );
    lines.back()->ColourModulation({0,0,0,255});
    lines.back()->Parent(m_panel);
    y += prev_h + 5;
    lines.back()->LocalPosition({img->LocalPosition().x, y});
    prev_h = lines.back()->Size().h;

    //<f> CheckBox
    label_ptr = m_gui_manager->CreateElement<sdl_gui::Label>({0,0}, {0,0});
    label_ptr->Text("Selectable Button -> Basic button (has previous transitions) that can be selected.\nUsed as the base for checkboxes", sdl_gui::Colour::White);
    label_ptr->LineLength(300);
    label_ptr->Parent(m_panel);
    y += prev_h + 5;
    label_ptr->LocalPosition({x, y});
    prev_h = label_ptr->Size().h;

    auto selectable_group = m_gui_manager->CreateElement<sdl_gui::SelectableGroup>({0,0}, {0,0});
    for(auto i{0}; i < 5;++i)
    {
        auto checkbox{m_gui_manager->CreateElement<sdl_gui::CheckBox>({0,0}, {15,15})};
        selectable_group->AddSelectable(checkbox);
        checkbox->LocalPosition({0, 15.f * i + 5 * i});
        checkbox->Value(i);

        auto check_mark{m_gui_manager->CreateElement<sdl_gui::Image>({0,0}, {11,11})};
        checkbox->CheckMarkElement(check_mark);//add to checkbox
        check_mark->AlignWithParentPoint(sdl_gui::AnchorType::MIDDLE_CENTRE);
        check_mark->ColourModulation({0,0,0,255});
    }
    // selectable_group->ValuesChanged = std::bind(&Check, std::placeholders::_1);

    selectable_group->Parent(m_panel);
    selectable_group->ResizeToBounds();
    y += prev_h + 5;
    selectable_group->LocalPosition({x,y});

    //Second group
    auto selectable_radio_group = m_gui_manager->CreateElement<sdl_gui::SelectableGroup>({0,0}, {0,0});
    for(auto i{0}; i < 5;++i)
    {
        auto checkbox{m_gui_manager->CreateElement<sdl_gui::CheckBox>({0,0}, {15,15})};
        selectable_radio_group->AddSelectable(checkbox);
        checkbox->TransitionColourPtr()->ChangeBaseTexture(sdl_gui::c_img_white_circle);
        checkbox->LocalPosition({0, 15.f * i + 5 * i});
        checkbox->Value(i);

        auto check_mark{m_gui_manager->CreateElement<sdl_gui::Image>({0,0}, {11,11})};
        checkbox->CheckMarkElement(check_mark);//add to checkbox
        //switch image to circle
        check_mark->ChangeTexture(sdl_gui::c_img_white_circle);
        check_mark->AlignWithParentPoint(sdl_gui::AnchorType::MIDDLE_CENTRE);
        check_mark->ColourModulation({0,0,0,255});
    }
    // selectable_radio_group->ValuesChanged = std::bind(&Check, std::placeholders::_1);
    selectable_radio_group->GroupType(sdl_gui::SelectableGroupType::RADIO);

    selectable_radio_group->Parent(m_panel);
    selectable_radio_group->ResizeToBounds();
    selectable_radio_group->LocalPosition({x + 50,y});//move this group 50px to the right
    prev_h = selectable_radio_group->Size().h;
    //</f>//checkbox

    lines.push_back( m_gui_manager->CreateElement<sdl_gui::Image>({0,0}, {10, 2}) );
    lines.back()->ColourModulation({0,0,0,255});
    lines.back()->Parent(m_panel);
    y += prev_h + 5;
    lines.back()->LocalPosition({img->LocalPosition().x, y});
    // prev_h = lines.back()->Size().h;
    //</f>//selectables

    //resize image & lines
    auto bounds = m_panel->Bounds(); //auto margin{10};
    // img->Size( sdl_gui::SizeFromInts(bounds.w + margin, bounds.h) );
    for(auto& line : lines)
        line->Size(sdl_gui::SizeFromInts(bounds.w, 2));
    // std::cout<<bounds.w<<" - "<<bounds.h<<std::endl;
    //</f>//buttons
}

ButtonsPanel::~ButtonsPanel() noexcept
{

}

ButtonsPanel::ButtonsPanel(const ButtonsPanel& other)
{

}

ButtonsPanel::ButtonsPanel(ButtonsPanel&& other) noexcept
{

}

ButtonsPanel& ButtonsPanel::operator=(const ButtonsPanel& other)
{
    if(this != &other)//not same ref
    {
        auto tmp(other);
        *this = std::move(tmp);
    }

    return *this;
}

ButtonsPanel& ButtonsPanel::operator=(ButtonsPanel&& other) noexcept
{
    if(this != &other)//not same ref
    {
        //move here
    }
    return *this;
}
//</f>
