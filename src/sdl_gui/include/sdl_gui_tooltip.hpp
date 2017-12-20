#include "sdl_gui_element.hpp"
#include "sdl_gui_label.hpp"
#include "sdl_gui_mouse_interaction.hpp"
#include <string>
#include <memory>

namespace sdl_gui
{

#ifndef SDL_GUI_TOOLTIP_HPP
#define SDL_GUI_TOOLTIP_HPP

class Tooltip: public GuiElement
{
    public:
        //<f> Constructors & operator=
        /** brief Default constructor */
        Tooltip(GuiMainPointers main_pointers, const Position& position, const Dimensions& size);
        /** brief Default destructor */
        virtual ~Tooltip() noexcept;

        /** brief Copy constructor */
        Tooltip(const Tooltip& other);
        /** brief Move constructor */
        Tooltip(Tooltip&& other) noexcept;

        /** brief Copy operator */
        Tooltip& operator= (const Tooltip& other);
        /** brief Move operator */
        Tooltip& operator= (Tooltip&& other) noexcept;
        //</f>

        //<f> Overrides GUIElement
        virtual void Input(const SDL_Event& event);
        virtual void ClearInput();

        // virtual void FixedLogic(float fixed_delta_time);
        virtual void Logic(float delta_time);

        virtual void Render(float delta_time);
        virtual void Render(float delta_time, Camera* camera);
        //</f>

        //<f> Virtual Methods

        //</f>

        //<f> Getters/Setters
        void Content(GuiElement* content);
        GuiElement* Content() const { return m_content; }

        // void ChangeCollider(const Collider& collider){ m_colliders[0] = collider; }
        //</f>

    protected:
        // vars and stuff
        GuiElement* m_content;

        MouseInteraction m_mouse_interaction;
        float m_wait_time;
        float m_max_wait_time;
        bool m_can_show;

        void ShowTooltip();
        void HideTooltip();
    private:
};

#endif //SDL_GUI_TOOLTIP_HPP

}//namespace
