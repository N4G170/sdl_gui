#include "sdl_gui_element.hpp"
#include "sdl_gui_label.hpp"
#include "sdl_gui_image.hpp"
#include "sdl_gui_mouse_interaction.hpp"
#include "sdl_gui_enums.hpp"
#include <string>

namespace sdl_gui
{

#ifndef SDL_GUI_TEXTBOX_HPP
#define SDL_GUI_TEXTBOX_HPP

class Textbox : public GuiElement
{
    public:
        //<f> Constructors & operator=
        /* Default constructor */
        Textbox(GuiMainPointers main_pointers, const Position& position, const Dimensions& size);
        /* Default destructor */
        virtual ~Textbox() noexcept;

        /* Copy constructor */
        Textbox(const Textbox& other);
        /* Move constructor */
        Textbox(Textbox&& other) noexcept;

        /* Copy operator */
        Textbox& operator= (const Textbox& other);
        /* Move operator */
        Textbox& operator= (Textbox&& other) noexcept;
        //</f>

        //<f> Overrides GuiElement
        virtual void Input(const SDL_Event& event);

        // virtual void FixedLogic(float fixed_delta_time);
        virtual void Logic(float delta_time);

        virtual void Render(float delta_time);
        virtual void Render(float delta_time, Camera* camera);
        //</f>

        //<f> Virtual Methods
        virtual void Focus();
        virtual void UnFocus();
        //</f>

        //<f> Getters/Setters
        std::string Text() const { return m_text_label.Text(); }
        void Text(const std::string& text) { m_text_label.Text(text); }

        std::string DefaultText() const { return m_default_text; }
        void DefaultText(const std::string& text) { m_default_text = text; }

        TextboxInputType InputType() const { return m_input_type; }
        void InputType(TextboxInputType input_type) { m_input_type = input_type; }
        //</f>



    protected:
        // vars and stuff
        Image m_bg_image;
        SDL_Rect m_viewport_rect;

        Label m_text_label;
        Label m_caret_label;
        Label* m_label_to_render;
        std::string m_text;
        std::string m_text_representation;
        std::string m_default_text;

        float m_blink_time_limit;
        float m_blink_time;

        TextboxInputType m_input_type;

        MouseInteraction m_mouse_interaction;
};

#endif //SDL_GUI_TEXTBOX_HPP

}//namespace
