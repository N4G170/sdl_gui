#include "sdl_gui_checkbox.hpp"
#include <functional>
#include "sdl_gui_enums.hpp"

namespace sdl_gui
{

#ifndef SDL_GUI_CHECKBOX_GROUP_HPP
#define SDL_GUI_CHECKBOX_GROUP_HPP

class CheckBoxGroup : public GuiElement
{
    public:
        //<f> Constructors & operator=
        /* Default constructor */
        CheckBoxGroup(GuiMainPointers main_pointers, const Position& position, const Dimensions& size);
        /* Default destructor */
        virtual ~CheckBoxGroup() noexcept;

        /* Copy constructor */
        CheckBoxGroup(const CheckBoxGroup& other);
        /* Move constructor */
        CheckBoxGroup(CheckBoxGroup&& other) noexcept;

        /* Copy operator */
        CheckBoxGroup& operator= (const CheckBoxGroup& other);
        /* Move operator */
        CheckBoxGroup& operator= (CheckBoxGroup&& other) noexcept;
        //</f>

        //<f> Overrides GuiElement
        virtual void Input(const SDL_Event& event);

        // virtual void FixedLogic(float fixed_delta_time);
        virtual void Logic(float delta_time);

        virtual void Render(float delta_time);
        virtual void Render(float delta_time, Camera* camera);
        //</f>

        //<f> Virtual Methods
        /**
         * \brief The group will not own the checkbox
         */
        virtual void AddCheckBox(CheckBox* checkbox);
        //</f>

        //<f> Getters/Setters
        CheckBoxGroupType GroupType() const { return m_group_type; }
        void GroupType(CheckBoxGroupType type);
        //</f>

        //<f> Callbacks
        std::function<void(std::vector<CheckBox*>& selected_values)> ValuesChanged;
        //</f>

    protected:
        // vars and stuff
        CheckBoxGroupType m_group_type;

        std::unordered_map<UID, CheckBox*> m_checkboxes;

        std::vector<CheckBox*> m_selected_values;

        void CheckBoxChanged(CheckBox* changed_checkbox);
};

#endif //SDL_GUI_CHECKBOX_GROUP_HPP

}//namespace
