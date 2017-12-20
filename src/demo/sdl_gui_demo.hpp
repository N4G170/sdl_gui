#ifndef GUI_DEMO_HPP
#define GUI_DEMO_HPP

#include <vector>
#include <memory>
#include "sdl_gui_manager.hpp"
#include "sdl_init.hpp"
#include "animation_panel.hpp"
#include "buttons_panel.hpp"
#include "bars_panel.hpp"
#include "others_panel.hpp"

class GuiDemo
{
    public:
        GuiDemo(SDL_Renderer* renderer_ptr, sdl_gui::ResourceManager* resource_manager_ptr, const SDLInitConfig& sdl_config = {});

        virtual ~GuiDemo();
        /**
         * \brief Precess SDL user input
         */
        void Input(const SDL_Event&);

        /**
         * \brief Process any logic, runs after input
         */
        void FixedLogic(float fixed_delta_time = 1);

        /**
         * \brief Process any logic, runs after input
         */
        void Logic(float delta_time = 1);

        /**
         * \brief Render the state visual elements
         */
        void Render(SDL_Renderer*, float delta_time);

    private:
        sdl_gui::GuiManager m_gui_manager;

        AnimationPanel m_animation_panel;
        ButtonsPanel m_buttons_panel;
        BarsPanel m_bars_panel;
        OthersPanel m_others_panel;

        std::unordered_map<std::string, sdl_gui::GuiElement*> m_panels;
        std::string m_current_panel_name;
        sdl_gui::SelectableButton* m_current_panel_button;
        void ChangePanel(const std::string& panel_name, sdl_gui::SelectableButton* btn);
};
#endif //GUI_DEMO_HPP
