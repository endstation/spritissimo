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




#ifndef Editing_panel_h
#define Editing_panel_h

#include <SFML/Graphics.hpp>
#include "Sprite.h"
#include <memory>
#include <iostream>
#include <sigc++/sigc++.h>
#include "Globals.h"
#include "Project.h"
#include <SFML/System.hpp>
#include "Panel_dims.h"
#include <ctime>
#include "Zoomable.h"
#include "Sprite_dims.h"




namespace sprmo
{

class Editing_panel : public sf::Drawable
{

    public:

        enum State { Stopped, Moving };
        enum Direction { Left, Right, Up, Down };
        enum Editing_mode { Pixels, Mask };
        enum Paint_mode { Draw, Erase };

        Editing_panel(std::shared_ptr<Project> p, const Panel_dims& dims,
                const Sprite_dims& sprdims);
        ~Editing_panel();

        void zoom_in()          { m_zoomable->zoom_in(); }
        void zoom_out()         { m_zoomable->zoom_out(); }
        void zoom_in_stop()     { m_zoomable->zoom_in_stop(); }
        void zoom_out_stop()    { m_zoomable->zoom_out_stop(); }
        void stop_all_zoom()    { m_zoomable->stop_all(); }

        void set_current_sprite(std::shared_ptr<Sprite> s);
        bool handle_mouse_button_pressed(sf::Event::MouseButtonEvent& e);
        void handle_mouse_button_released(sf::Event::MouseButtonEvent& e);
        void handle_mouse_moved(sf::Event::MouseMoveEvent& e, 
                sf::RenderTarget& target);
        void handle_color_changed(size_t c);
        void handle_new_current_sprite(bool is_multicolor,
                Palette::C64_color);
        void toggle_grid();
        sf::Vector2i get_current_cell() const { return m_current_cell; }
        sigc::signal<void, const sf::Vector2i&>& signal_cell_changed(); 
        sigc::signal<void>& signal_sprite_modified();
        void update();
        void move_left();
        void move_right();
        void move_up();
        void move_down();
        void move_left_stop();
        void move_right_stop();
        void move_up_stop();
        void move_down_stop();
        void center_sprite();
        size_t get_paint_color2() const;


    protected:

    private:

        const float Initial_zoom = 0.4;
        const float Move_v = 0.5;

        virtual void draw(sf::RenderTarget& target, 
                sf::RenderStates states) const;
        void init_lines();
        void init_view(const Panel_dims& dims);
        void paint_pixel();
        // Helper function.
        bool is_sprite_wider() const;
        bool is_sprite_taller() const;
        void update_move();

        sf::VertexArray                         m_grid_lines;
        // Grid lines for multicolor sprites (horizontal resolution halved).
        sf::VertexArray                         m_grid_lines_mc;
        std::shared_ptr<Sprite>                 m_current_sprite_ptr;
        sf::View                                m_view;
        bool                                    m_show_grid;
        sf::Vector2i                            m_current_cell;
        sigc::signal<void, const sf::Vector2i&> m_signal_cell_changed;
        sigc::signal<void>                      m_signal_sprite_modified;
        sigc::signal<void,Editing_mode>         m_signal_mode_toggled;
        State                                   m_state;
        std::shared_ptr<Project>                m_project_ptr;
        bool                                    m_is_drawing;
        bool                                    m_sprite_changed;
        Editing_mode                            m_edit_mode;
        Paint_mode                              m_paint_mode;
        Direction                               m_direction;
        size_t                                  m_current_color2;
        const Panel_dims&                       m_dims_ref;
        std::unique_ptr<Zoomable>               m_zoomable;
        const Sprite_dims&                      m_sprdims_ref;

}; // class Editing_panel

} // namespace sprmo




#endif

