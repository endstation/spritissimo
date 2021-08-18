// Spritissimo
// Copyright 2016-2021 Matthew Clarke <pclar7@yahoo.co.uk>

// This file is part of Spritissimo.

// Spritissimo is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.

// Spritissimo is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.

// You should have received a copy of the GNU General Public License
// along with Spritissimo.  If not, see <http://www.gnu.org/licenses/>.




#ifndef Application_h
#define Application_h

#include <SFML/Graphics.hpp>
#include "Palette_panel.h"
#include "Editing_panel.h"
#include <memory>
#include <iostream>
#include "Project.h"
#include "Preview_panel.h"
#include "Editing_status_panel.h"
#include "Heartbeat.h"
#include "File_IO.h"
#include "Status_bar.h"
#include "Config.h"
#include <boost/format.hpp>
#include <string>
#include "Animation_panel.h"
#include "Animation_status_panel.h"
#include "Help_screen.h"
#include <locale>
#include <algorithm>
#include "Palette.h"
#include "Panel_dims.h"
#include <vector>
#include "Sprite_dims.h"




namespace sprmo
{

class Application
{

    public:
        // FIXME: rubbish - indices into the vector of 'Panel_dims' objects.
        enum Widgets {
            Widget_edit_panel,
            Widget_anim_panel,
            Widget_edit_status_panel,
            Widget_palette_panel,
            Widget_preview_panel,
            Widget_status_bar,
            Widget_anim_status_panel
        }; // enum Widgets

        enum Mode { 
                Editing, 
                Animating, 
                New_sprite, 
                New_project,
                Selecting, 
                Closing,
                Loading,
                Saving,
                Saving_prg,
                Saving_png_file,
                Start_up,
                Confirm_close,
                Display_help,
                Color_palette
        }; // enum Mode

        /*enum Dim_typs { Editing, Animation, Editing_status, Palette, Preview,
            Status_bar, Animation_status };*/

        Application(sf::RenderWindow& rwin, const std::vector<Panel_dims>& pd,
                unsigned int text_size, const sprmo::Sprite_dims& sprdims, sprmo::Config& cfg);
        ~Application();

        void start();
        void init_panel_backgrounds(const std::vector<Panel_dims>& pd);
        void handle_project_changed();
        
    protected:

    private:

        Mode                                    m_current_mode;
        sf::RenderWindow&                       m_rwin_ref;
        //Resolution                              m_resolution;
        std::shared_ptr<Palette_panel>          m_palette_panel;
        std::shared_ptr<Editing_panel>          m_editing_panel; 
        std::shared_ptr<Project>                m_project;
        std::shared_ptr<Preview_panel>          m_preview_panel;
        std::shared_ptr<Editing_status_panel>   m_editing_status_panel;
        std::shared_ptr<Status_bar>             m_status_bar;
        std::shared_ptr<Animation_panel>        m_animation_panel;
        std::shared_ptr<Animation_status_panel> m_animation_status_panel;
        sf::Font                                m_bold_font;
        Heartbeat                               m_heartbeat;
        sf::VertexArray                         m_panel_backgrounds;
        Config&                                 m_config_ref;
        std::string                             m_current_filename;
        std::shared_ptr<Help_screen>            m_help_screen;
        Palette                                 m_palette;

        void handle_closed();
        void handle_key_pressed(sf::Event::KeyEvent& e);
        void handle_key_released(sf::Event::KeyEvent& e);
        void handle_mouse_button_pressed(sf::Event::MouseButtonEvent& e);
        void handle_mouse_button_released(sf::Event::MouseButtonEvent& e);
        void handle_mouse_wheel_scrolled(sf::Event::MouseWheelScrollEvent& e);
        void handle_mouse_moved(sf::Event::MouseMoveEvent& e);
        void handle_text_entered(sf::Event::TextEvent& e);
        void update();
        void try_load_save(const std::string& filename);
        void refresh_title();
        void init_new_project();
        void handle_confirm_close(const std::string& response);
        void really_close();
        void write_prg_or_png(bool alt);

}; // class Application

} // namespace sprmo




#endif

