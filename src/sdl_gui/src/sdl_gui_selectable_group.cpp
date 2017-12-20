#include "sdl_gui_selectable_group.hpp"
#include <utility>
#include "sdl_gui_log.hpp"
#include "sdl_gui_constants.hpp"
#include <algorithm>

namespace sdl_gui
{

//<f> Constructors & operator=
SelectableGroup::SelectableGroup(GuiMainPointers main_pointers, const Position& position, const Dimensions& size):
    GuiElement{main_pointers, position, size}, ValuesChanged{}, m_group_type{SelectableGroupType::MULTIPLE}, m_selectables{}, m_selected_values{} {}

SelectableGroup::~SelectableGroup() noexcept {}

SelectableGroup::SelectableGroup(const SelectableGroup& other) : GuiElement{other}, ValuesChanged{other.ValuesChanged},
    m_group_type{other.m_group_type}, m_selectables{other.m_selectables}, m_selected_values{other.m_selected_values}
{

}

SelectableGroup::SelectableGroup(SelectableGroup&& other) noexcept : GuiElement{std::move(other)}, ValuesChanged{std::move(other.ValuesChanged)},
    m_group_type{std::move(other.m_group_type)}, m_selectables{std::move(other.m_selectables)}, m_selected_values{std::move(other.m_selected_values)}
{
    for(auto& selectable : m_selectables)
        selectable.second->StateChanged = std::bind(&SelectableGroup::SelectableChanged, this, std::placeholders::_1);
}

SelectableGroup& SelectableGroup::operator=(const SelectableGroup& other)
{
    if(this != &other)//not same ref
    {
        SelectableGroup tmp(other);
        *this = std::move(tmp);
    }

    return *this;
}

SelectableGroup& SelectableGroup::operator=(SelectableGroup&& other) noexcept
{
    if(this != &other)
    {
        GuiElement::operator=(other);

        this->m_group_type = std::move(other.m_group_type);
        this->m_selectables = std::move(other.m_selectables);
        this->m_selected_values = std::move(other.m_selected_values);

        for(auto& selectable : m_selectables)
            selectable.second->StateChanged = std::bind(&SelectableGroup::SelectableChanged, this, std::placeholders::_1);
    }
    return *this;
}
//</f>

//<f> Virtual Methods
void SelectableGroup::AddSelectable(SelectableButton* selectable)
{
    selectable->Parent(this);
    selectable->RenderIndex(-1);
    selectable->StateChanged = std::bind(&SelectableGroup::SelectableChanged, this, std::placeholders::_1);
    m_selectables.emplace( selectable->ElementUID(), selectable );
}
//</f>

//<f> Getters/Setters
void SelectableGroup::GroupType(SelectableGroupType type)
{
    m_group_type = type;
}
//</f>

void SelectableGroup::SelectableChanged(SelectableButton* selectable)
{
    if(m_group_type == SelectableGroupType::MULTIPLE)
    {
        if(selectable->IsSelected())
        {
            m_selected_values.push_back(selectable);
        }
        else
        {
            m_selected_values.erase( std::find_if(std::begin( m_selected_values), std::end(m_selected_values),
                [selectable](SelectableButton* element)->bool{ return selectable->ElementUID() == element->ElementUID(); } ) );
        }

        if(ValuesChanged)//callback
            ValuesChanged(m_selected_values);
    }
    else
    {
        //check if same checkbox
        if(!m_selected_values.empty() && selectable->ElementUID() == m_selected_values[0]->ElementUID())//same element
        {
            m_selected_values[0]->IsSelected(true);//keep selected
            return;
        }
        //deselect all other (actually only one)
        for(auto& selected : m_selected_values)
            selected->IsSelected(false);

        m_selected_values.clear();
        m_selected_values.push_back(selectable);

        if(ValuesChanged)//callback
            ValuesChanged(m_selected_values);
    }
}

}//namespace
