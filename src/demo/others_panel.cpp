#include "others_panel.hpp"
#include <utility>
#include "sdl_gui_constants.hpp"

//<f> Constructors & operator=
OthersPanel::OthersPanel(sdl_gui::GuiManager* gui_manager): m_gui_manager{gui_manager}
{
    auto y{5.f};

    m_panel = m_gui_manager->CreateElement<sdl_gui::GuiElement>({0,0}, {0,0});

    m_background = m_gui_manager->CreateElement<sdl_gui::Image>({0,0}, {700,650});
    m_background->Parent(m_panel);
    m_background->RenderIndex(10);
    m_background->ColourModulation({12,34,56,200});

    //<f> Tooltip
    // y+=35;
    auto tooltip_label{ m_gui_manager->CreateElement<sdl_gui::Label>({0,0}, {0,0}) };
    tooltip_label->Text("Put mouse over square\nand wait for tooltip to appear.", sdl_gui::Colour::White);
    tooltip_label->Parent(m_panel);
    tooltip_label->LocalPosition({5, y});

    auto tooltip{m_gui_manager->CreateElement<sdl_gui::Tooltip>({0,0}, {50,50})};
    auto tooltip_text{m_gui_manager->CreateElement<sdl_gui::Label>({0,0}, {50,50})};
    tooltip_text->Text("Tooltip content can be any type of gui element.", sdl_gui::Colour::White);
    tooltip->Content(tooltip_text);

    y += 50;
    tooltip->Parent(m_panel);
    tooltip->LocalPosition({5, y});

    //image for tooltip marking
    auto tooltip_image{m_gui_manager->CreateElement<sdl_gui::Image>({0,0}, {50,50})};
    tooltip_image->Parent(m_panel);
    tooltip_image->LocalPosition({5,y});

    tooltip_image->ColourModulation({sdl_gui::Colour::Black});
    //</f> /Tooltip

    //<f> List
    y += 60;
    auto box = m_gui_manager->CreateElement<sdl_gui::ScrollBox>({0,0}, {250,300});
    for(int i{0}; i<25; ++i)
    {
        box->CreateItem("Label "+std::to_string(i));
    }

    // y += 30;
    box->Parent(m_panel);
    box->LocalPosition({5, y});
    //</f> /List

    // auto bounds = m_panel->Bounds(); auto margin{10};
    // m_background->Size( sdl_gui::SizeFromInts(bounds.w + margin, bounds.h) );

    //<f> Input field
    m_textbox = m_gui_manager->CreateElement<sdl_gui::Textbox>({600, 110}, {200,40});

    m_textbox_label = m_gui_manager->CreateElement<sdl_gui::Label>({600,175}, {0,0});
    m_textbox_label->LineLength(200);//same as box
    m_textbox_label->Text("The text input box is not working 100%. You can't move the caret or select text.\nAlso as it is a viewport, it does not work well in a tree structure.",
                    sdl_gui::Colour::White);
    //</f> /Input field

    //<f> Label with specials
    auto special_label{m_gui_manager->CreateElement<sdl_gui::Label>({0,0}, {0,0})};
    special_label->Parent(m_panel);
    special_label->LocalPosition({400, 275});

    special_label->LineLength(300);
    special_label->FontSize(20);
    special_label->Text("Example label with max width of 300px, size 20 and some markup.\nNormal\n<b>BOLD</b>\n<i>italic</i>\n<i>italic <b>BOLD</b></i>\n It needs some refining.", sdl_gui::Colour::White);
    //</f> /Label with specials
}

OthersPanel::~OthersPanel() noexcept
{

}

OthersPanel::OthersPanel(const OthersPanel& other): m_gui_manager{other.m_gui_manager}, m_panel{other.m_panel}, m_background{other.m_background}
{

}

OthersPanel::OthersPanel(OthersPanel&& other) noexcept: m_gui_manager{std::move(other.m_gui_manager)}, m_panel{std::move(other.m_panel)},
    m_background{std::move(other.m_background)}
{

}

OthersPanel& OthersPanel::operator=(const OthersPanel& other)
{
    if(this != &other)//not same ref
    {
        auto tmp(other);
        *this = std::move(tmp);
    }

    return *this;
}

OthersPanel& OthersPanel::operator=(OthersPanel&& other) noexcept
{
    if(this != &other)//not same ref
    {
        m_gui_manager = std::move(other.m_gui_manager);
        m_panel = std::move(other.m_panel);
        m_background = std::move(other.m_background);
    }
    return *this;
}
//</f>

//<f> Methods
void OthersPanel::EnableTextBoxFix(bool enable)
{
    m_textbox->Enabled(enable);
    m_textbox_label->Enabled(enable);
}
//</f> Methods
