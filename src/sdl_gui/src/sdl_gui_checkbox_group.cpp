#include "sdl_gui_checkbox_group.hpp"
#include <utility>
#include "sdl_gui_log.hpp"
#include "sdl_gui_constants.hpp"
#include <algorithm>

namespace sdl_gui
{

//<f> Constructors & operator=
CheckBoxGroup::CheckBoxGroup(GuiMainPointers main_pointers, const Position& position, const Dimensions& size):
    GuiElement{main_pointers, position, size}, m_group_type{CheckBoxGroupType::MULTIPLE} {}

CheckBoxGroup::~CheckBoxGroup() noexcept {}

CheckBoxGroup::CheckBoxGroup(const CheckBoxGroup& other) : GuiElement{other},
    m_group_type{other.m_group_type}, m_checkboxes{other.m_checkboxes}, m_selected_values{other.m_selected_values}
{

}

CheckBoxGroup::CheckBoxGroup(CheckBoxGroup&& other) noexcept : GuiElement{other},
    m_group_type{std::move(other.m_group_type)}, m_checkboxes{std::move(other.m_checkboxes)}, m_selected_values{std::move(other.m_selected_values)}
{
    for(auto& checkbox : m_checkboxes)
        checkbox.second->ValueChanged = std::bind(&CheckBoxGroup::CheckBoxChanged, this, std::placeholders::_1);
}

CheckBoxGroup& CheckBoxGroup::operator=(const CheckBoxGroup& other)
{
    if(this != &other)//not same ref
    {
        CheckBoxGroup tmp(other);
        *this = std::move(tmp);
    }

    return *this;
}

CheckBoxGroup& CheckBoxGroup::operator=(CheckBoxGroup&& other) noexcept
{
    if(this != &other)
    {
        GuiElement::operator=(other);

        this->m_group_type = std::move(other.m_group_type);
        this->m_checkboxes = std::move(other.m_checkboxes);
        this->m_selected_values = std::move(other.m_selected_values);

        for(auto& checkbox : m_checkboxes)
            checkbox.second->ValueChanged = std::bind(&CheckBoxGroup::CheckBoxChanged, this, std::placeholders::_1);
    }
    return *this;
}
//</f>

//<f> Overrides GuiElement
void CheckBoxGroup::Input(const SDL_Event& event)
{
    if(!m_active)
        return;
}

void CheckBoxGroup::Logic(float delta_time)
{

}

void CheckBoxGroup::Render(float delta_time)
{
    Render(delta_time, m_main_pointers.main_camera_ptr);
}

void CheckBoxGroup::Render(float delta_time, Camera* camera)
{
    if(!m_render)
        return;
}

//</f>

//<f> Virtual Methods
void CheckBoxGroup::AddCheckBox(CheckBox* checkbox)
{
    m_checkboxes.emplace( checkbox->ElementUID(), checkbox );

    m_checkboxes[checkbox->ElementUID()]->ValueChanged = std::bind(&CheckBoxGroup::CheckBoxChanged, this, std::placeholders::_1);
}
//</f>

//<f> Getters/Setters
void CheckBoxGroup::GroupType(CheckBoxGroupType type)
{
    m_group_type = type;

    //change images
    if(m_group_type == CheckBoxGroupType::MULTIPLE)
    {
        for(auto& checkbox : m_checkboxes)
        {
            checkbox.second->ConfigAsMultiple();
        }
    }
    else if(m_group_type == CheckBoxGroupType::RADIO)
    {
        for(auto& checkbox : m_checkboxes)
        {
            checkbox.second->ConfigAsRadio();
        }
    }
}
//</f>

void CheckBoxGroup::CheckBoxChanged(CheckBox* changed_checkbox)
{
    if(m_group_type == CheckBoxGroupType::MULTIPLE)
    {
        if(changed_checkbox->IsChecked())
        {
            m_selected_values.push_back(changed_checkbox);
        }
        else
        {
            m_selected_values.erase( std::find_if(std::begin( m_selected_values), std::end(m_selected_values),
                [changed_checkbox](CheckBox* element)->bool{ return changed_checkbox->ElementUID() == element->ElementUID(); } ) );
        }

        if(ValuesChanged)//callback
            ValuesChanged(m_selected_values);
    }
    else
    {
        //check if same checkbox
        if(!m_selected_values.empty() && changed_checkbox->ElementUID() == m_selected_values[0]->ElementUID())//same element
        {
            m_selected_values[0]->IsChecked(true);//keep selected
            return;
        }
        //deselect all other (actually only one)
        for(auto& selected : m_selected_values)
            selected->IsChecked(false);

        m_selected_values.clear();
        m_selected_values.push_back(changed_checkbox);

        if(ValuesChanged)//callback
            ValuesChanged(m_selected_values);
    }
}

}//namespace
