#include "sdl_gui_checkbox.hpp"
#include <functional>
#include "sdl_gui_enums.hpp"

namespace sdl_gui
{

#ifndef SDL_GUI_SELECTABLE_GROUP_HPP
#define SDL_GUI_SELECTABLE_GROUP_HPP

class SelectableGroup : public GuiElement
{
    public:
        //<f> Constructors & operator=
        /* Default constructor */
        SelectableGroup(GuiMainPointers main_pointers, const Position& position, const Dimensions& size);
        /* Default destructor */
        virtual ~SelectableGroup() noexcept;

        /* Copy constructor */
        SelectableGroup(const SelectableGroup& other);
        /* Move constructor */
        SelectableGroup(SelectableGroup&& other) noexcept;

        /* Copy operator */
        SelectableGroup& operator= (const SelectableGroup& other);
        /* Move operator */
        SelectableGroup& operator= (SelectableGroup&& other) noexcept;
        //</f>

        //<f> Virtual Methods
        /**
         * \brief The group will not own the selectable
         */
        virtual void AddSelectable(SelectableButton* selectable);
        //</f>

        //<f> Getters/Setters
        SelectableGroupType GroupType() const { return m_group_type; }
        void GroupType(SelectableGroupType type);
        //</f>

        //<f> Callbacks
        std::function<void(std::vector<SelectableButton*>& selected_values)> ValuesChanged;
        //</f>

    protected:
        // vars and stuff
        SelectableGroupType m_group_type;

        std::unordered_map<UID, SelectableButton*> m_selectables;

        std::vector<SelectableButton*> m_selected_values;

        void SelectableChanged(SelectableButton* selectable);
};

#endif //SDL_GUI_SELECTABLE_GROUP_HPP

}//namespace
