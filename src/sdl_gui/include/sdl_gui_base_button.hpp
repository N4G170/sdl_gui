#include "sdl_gui_element.hpp"
#include "sdl_gui_image.hpp"
#include "sdl_gui_mouse_interaction.hpp"
#include "sdl_gui_enums.hpp"
#include <memory>
#include <unordered_map>
#include <utility>
#include "sdl_gui_button_transition.hpp"
#include "sdl_gui_button_transition_none.hpp"
#include "sdl_gui_button_transition_colour.hpp"
#include "sdl_gui_button_transition_single_image.hpp"
#include "sdl_gui_button_transition_multi_image.hpp"

namespace sdl_gui
{

#ifndef SDL_GUI_BASE_BUTTON_HPP
#define SDL_GUI_BASE_BUTTON_HPP

class BaseButton : public GuiElement
{
    public:
        //<f> Constructors & operator=
        /* Default constructor */
        BaseButton(GuiMainPointers main_pointers, const Position& position, const Dimensions& size);
        /* Default destructor */
        virtual ~BaseButton() noexcept;

        /* Copy constructor */
        BaseButton(const BaseButton& other);
        /* Move constructor */
        BaseButton(BaseButton&& other) noexcept;

        /* Copy operator */
        BaseButton& operator= (const BaseButton& other);
        /* Move operator */
        BaseButton& operator= (BaseButton&& other) noexcept;
        //</f>

        //<f> Overrides GUIElement
        void Input(const SDL_Event& event) override;
        void ClearInput() override;

        // virtual void FixedLogic(float fixed_delta_time);
        void Logic(float delta_time) override;

        void Render(float delta_time) override;
        void Render(float delta_time, Camera* camera) override;
        //</f>

        //<f> Methods
        //</f>

        //<f> Getters/Setters
        ButtonTransitionType TransitionType() const { return m_transition_type; }
        void TransitionType(ButtonTransitionType);

        MouseInteraction* MouseInteractionPtr() { return &m_mouse_interaction; }

        ButtonTransitionNone* TransitionNonePtr() { return m_transition_none_ptr.get(); }
        ButtonTransitionColour* TransitionColourPtr() { return m_transition_colour_ptr.get(); }
        ButtonTransitionSingleImage* TransitionSingleImagePtr() { return m_transition_single_image_ptr.get(); }
        ButtonTransitionMultiImage* TransitionMultiImagePtr() { return m_transition_multi_image_ptr.get(); }
        //</f>

    protected:
        // vars and stuff
        ButtonTransitionType m_transition_type;

        std::unique_ptr<ButtonTransitionNone> m_transition_none_ptr;
        std::unique_ptr<ButtonTransitionColour> m_transition_colour_ptr;
        std::unique_ptr<ButtonTransitionSingleImage> m_transition_single_image_ptr;
        std::unique_ptr<ButtonTransitionMultiImage> m_transition_multi_image_ptr;

        ButtonTransition* m_current_transition;

        //interaction
        MouseInteraction m_mouse_interaction;

        // std::function<void()> TransitionLogic;
        // void ColourTransitionLogic();
        // void TextureTransitionLogic();
        void ButtonTransitionCallback(ButtonState state);

        void DeleteTransition(ButtonTransitionType);
        void CreateTransition(ButtonTransitionType);
        void SelectTransition(ButtonTransitionType);
};

#endif //SDL_GUI_BASE_BUTTON_HPP

}
