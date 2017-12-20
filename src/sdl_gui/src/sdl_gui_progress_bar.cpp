#include "sdl_gui_progress_bar.hpp"
#include <utility>
#include "sdl_gui_constants.hpp"
#include "sdl_gui_log.hpp"

namespace sdl_gui
{

//<f> Constructors & operator=
ProgressBar::ProgressBar(GuiMainPointers main_pointers, const Position& position, const Dimensions& size) :
    GuiElement{main_pointers, position, size},
    m_background{nullptr}, m_bar{nullptr},
    m_bar_direction{ProgressBarDirection::RIGHT}, m_min_value{0}, m_max_value{100}, m_value{0}{}

ProgressBar::~ProgressBar() noexcept {}

ProgressBar::ProgressBar(const ProgressBar& other) : GuiElement{other},
    m_background{other.m_background}, m_bar{other.m_bar},
    m_bar_direction{other.m_bar_direction}, m_min_value{other.m_min_value}, m_max_value{other.m_max_value}, m_value{other.m_value},
    m_value_changed_callback{other.m_value_changed_callback}, m_bar_value_changed_callback{other.m_bar_value_changed_callback} {}

ProgressBar::ProgressBar(ProgressBar&& other) noexcept : GuiElement{other},
    m_background{std::move(other.m_background)}, m_bar{std::move(other.m_bar)},
    m_bar_direction{std::move(other.m_bar_direction)}, m_min_value{std::move(other.m_min_value)}, m_max_value{std::move(other.m_max_value)}, m_value{std::move(other.m_value)},
    m_value_changed_callback{std::move(other.m_value_changed_callback)}, m_bar_value_changed_callback{ std::move(other.m_bar_value_changed_callback)} {}

ProgressBar& ProgressBar::operator=(const ProgressBar& other)
{
    if(this != &other)//not same ref
    {
        ProgressBar tmp(other);
        *this = std::move(tmp);
    }

    return *this;
}

ProgressBar& ProgressBar::operator=(ProgressBar&& other) noexcept
{
    if(this != &other)
    {
        GuiElement::operator=(other);
        m_background = std::move(other.m_background);
        m_bar = std::move(other.m_bar);
        m_bar_direction = std::move(other.m_bar_direction);
        m_min_value = std::move(other.m_min_value);
        m_max_value = std::move(other.m_max_value);
        m_value = std::move(other.m_value);
        m_value_changed_callback = std::move(other.m_value_changed_callback);
        m_bar_value_changed_callback = std::move(other.m_bar_value_changed_callback);
    }
    return *this;
}
//</f>

//<f> Getters/Setters
void ProgressBar::BarDirection(sdl_gui::ProgressBarDirection bar_direction)
{
    if(m_bar_direction != bar_direction)
    {
        m_bar_direction = bar_direction;

        ConfigBar();
    }
}

void ProgressBar::Value(float new_value)
{
    if(new_value != m_value)
    {
        m_value = KeepInInterval(new_value, m_min_value, m_max_value);

        UpdateBar();

        //call callback, if set
        if(m_value_changed_callback)
            m_value_changed_callback(m_value);
        if(m_bar_value_changed_callback)
            m_bar_value_changed_callback(this);
    }
}

void ProgressBar::Ratio(float ratio)
{
    if((m_value / m_max_value) != ratio)
    {
        m_value = m_max_value * KeepInInterval(ratio, 0.f, 1.f);

        UpdateBar();

        //call callback, if set
        if(m_value_changed_callback)
            m_value_changed_callback(m_value);
        if(m_bar_value_changed_callback)
            m_bar_value_changed_callback(this);
    }
}
//</f>

void ProgressBar::ConfigBar()
{
    if(m_bar != nullptr)
    {
        if(m_bar_direction == ProgressBarDirection::LEFT)
        {
            m_bar->Anchor(sdl_gui::AnchorType::MIDDLE_RIGHT);
        }
        else if(m_bar_direction == ProgressBarDirection::RIGHT)
        {
            m_bar->Anchor(sdl_gui::AnchorType::MIDDLE_LEFT);
        }
        else if(m_bar_direction == ProgressBarDirection::UP)
        {
            m_bar->Anchor(sdl_gui::AnchorType::BOTTOM_CENTRE);
        }
        else if (m_bar_direction == ProgressBarDirection::DOWN)
        {
            m_bar->Anchor(sdl_gui::AnchorType::TOP_CENTRE);
        }
    }
}

void ProgressBar::UpdateBar()
{
    if(m_bar != nullptr)
    {
        if(m_bar_direction == ProgressBarDirection::LEFT || m_bar_direction == ProgressBarDirection::RIGHT)
            m_bar->LocalScaleX( Ratio() );
        else if(m_bar_direction == ProgressBarDirection::UP || m_bar_direction == ProgressBarDirection::DOWN)
            m_bar->LocalScaleY( Ratio() );
    }
}

}//namespace
