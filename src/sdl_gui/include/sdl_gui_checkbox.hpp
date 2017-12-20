#include "sdl_gui_selectable_button.hpp"
#include "sdl_gui_image.hpp"
#include <functional>

namespace sdl_gui
{
#ifndef SDL_GUI_CHECKBOX_HPP
#define SDL_GUI_CHECKBOX_HPP

class CheckBox: public SelectableButton
{
    public:
        /* Default constructor */
        CheckBox(GuiMainPointers main_pointers, const Position& position, const Dimensions& size);
        /* Default destructor */
        virtual ~CheckBox() noexcept;

        /* Copy constructor */
        CheckBox(const CheckBox& other);
        /* Move constructor */
        CheckBox(CheckBox&& other) noexcept;

        /* Copy operator */
        CheckBox& operator= (const CheckBox& other);
        /* Move operator */
        CheckBox& operator= (CheckBox&& other) noexcept;

        //<f> Overrides GuiElement
        void Render(float delta_time) override;
        void Render(float delta_time, Camera* camera) override;
        //</f>

        //<f> Getters/Setters
        void IsSelected(bool is_selected) override
        {
            m_is_selected = is_selected;
            if(m_is_selected && m_mark_element != nullptr)
            {
                m_mark_element->CanRender(true);
                m_mark_element->IsActive(true);
            }
            else if(!m_is_selected && m_mark_element != nullptr)
            {
                m_mark_element->CanRender(false);
                m_mark_element->IsActive(false);
            }
        }

        GuiElement* CheckMarkElement() { return m_mark_element; }
        void CheckMarkElement(GuiElement* mark)
        {
            if(mark == nullptr)
                return;

            m_mark_element = mark;
            m_mark_element->Parent(this);
            if(m_is_selected)
            {
                m_mark_element->CanRender(true);
                m_mark_element->IsActive(true);
            }
            else if(!m_is_selected)
            {
                m_mark_element->CanRender(false);
                m_mark_element->IsActive(false);
            }

            m_mark_element->RenderIndex(-2);
        }
        //</f>

    protected:
        // vars and stuff
        GuiElement* m_mark_element;
        /**
         *  Function called when we click the button to change internal state. Will call \see StateChanged, if set
         */
        void MouseClick() override;
};

#endif //SDL_GUI_CHECKBOX_HPP
}//namespace
