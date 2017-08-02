#include "sdl_gui_base_button.hpp"
#include "sdl_gui_label.hpp"
#include "sdl_gui_texture.hpp"
#include "sdl_gui_enums.hpp"
#include <map>
#include <memory>

namespace sdl_gui
{

#ifndef SDL_GUI_BUTTON_HPP
#define SDL_GUI_BUTTON_HPP

class Button : public BaseButton
{
    public:
        //<f> Constructor & operator=
        /* Default constructor */
        Button(GuiMainPointers main_pointers, const Position& position, const Dimensions& size);
        /* Default destructor */
        virtual ~Button() noexcept;

        /* Copy constructor */
        Button(const Button& other);
        /* Move constructor */
        Button(Button&& other) noexcept;

        /* Copy operator */
        Button& operator= (const Button& other);
        /* Move operator */
        Button& operator= (Button&& other) noexcept;
        //</f>

        //<f> Overrides GuiElement
        // virtual void Input(const SDL_Event& event);

        // virtual void FixedLogic(float fixed_delta_time);
        // virtual void Logic(float delta_time);

        virtual void Render(float delta_time);
        virtual void Render(float delta_time, Camera* camera);
        //</f>

        //<f>  Methods
        void CreateLabel(const std::string& text, const std::string& font_path, int font_size, const SDL_Colour& text_colour, Position local_position);
        void CentreLabel();
        //</f>

    protected:
        // vars and stuff
        std::unique_ptr<Label> m_label_ptr;
};

#endif //SDL_GUI_BUTTON_HPP

}//namespace
