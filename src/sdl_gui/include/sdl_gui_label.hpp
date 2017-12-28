#include <string>
#include "SDL.h"
#include "sdl_gui_element.hpp"
#include "sdl_gui_local_texture.hpp"
#include "sdl_gui_font.hpp"
#include <mutex>

namespace sdl_gui
{

#ifndef SDL_GUI_LABEL_HPP
#define SDL_GUI_LABEL_HPP

class Label : public GuiElement
{
    public:
        //<f> Constructors & operator=
        /* Default constructor */
        // Label();
        Label(GuiMainPointers main_pointers, const Position& position, const Dimensions& size);
        // Label(GuiMainPointers main_pointers, Font* font_ptr, const std::string& text, const SDL_Colour& text_colour, Position position);

        /* Default destructor */
        virtual ~Label() noexcept;

        /* Copy constructor */
        Label(const Label& other);
        /* Move constructor */
        Label(Label&& other) noexcept;

        /* Copy operator */
        Label& operator= (const Label& other);
        /* Move operator */
        Label& operator= (Label&& other) noexcept;
        //</f>

        //<f> Config
        void ConfigLabel(const std::string& font_path, int font_size, const std::string& text, const SDL_Colour& text_colour);
        void ConfigLabel(Font* font, const std::string& text, const SDL_Colour& text_colour);
        //</f>

        //<f> Overrides GuiElement
        // virtual void Input(const SDL_Event& event);

        // virtual void FixedLogic(float fixed_delta_time);
        // virtual void Logic(float delta_time);

        virtual void Render(float delta_time);
        virtual void Render(float delta_time, Camera* camera);
        //</f>

        //<f> Getters/Setters
        std::string Text() const { return m_text; }
        void Text(const std::string& text, const SDL_Colour& text_colour = {0,0,0,255});

        int FontSize() const{ return m_font_ptr->FontSize(); }
        void FontSize(int size);

        /**
         * \brief Line Length in px
         */
        int LineLength() const { return m_line_length; }
        /**
         * \brief Line Length in px. If <=0 no limit
         */
        void LineLength(int line_length);
        //</f>

    private:
        /**
         * \brief Texture with the text to be rendered
         */
        LocalTexture m_text_texture;
        /**
         * \brief The std::string being rendered
         */
        std::string m_text;

        /**
         * \brief Color of the text
         */
        SDL_Colour m_text_colour;

        /**
         * \brief Pointer to the TTF font being used
         */
        Font* m_font_ptr;

        /**
         * Max line length allowed
         */
        int m_line_length;

        std::mutex m_text_mutex;
};

#endif //LABEL_HPP

}//namespace
