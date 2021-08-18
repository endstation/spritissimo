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




#ifndef Preview_panel_h
#define Preview_panel_h

#include <SFML/Graphics.hpp>
#include "Project.h"
#include <memory>
#include "Globals.h"
#include <SFML/System.hpp>
#include <SFML/Window.hpp>
#include <algorithm>
#include <sigc++/sigc++.h>
#include "Palette.h"
#include "Panel_dims.h"
#include "Sprite_dims.h"




namespace sprmo
{

class Preview_panel : public sf::Drawable
{

    public:

        enum State { Stopped, Left, Right };
        static const int Marker_len = 30;
        static const int Marker_height = 8;

        Preview_panel(std::shared_ptr<Project> p, const Panel_dims& dims,
                int win_w, const Sprite_dims& sprdims);
        ~Preview_panel();

        void handle_new_sprite_added(int i);
        bool handle_mouse_button_pressed(sf::Event::MouseButtonEvent& e,
                sf::RenderTarget& target);
        //void handle_new_current_sprite(bool is_multicolor, Resolution::C64_color c);
        void handle_new_current_sprite(bool is_multicolor, Palette::C64_color c);
        void handle_sprite_removed(size_t i);
        void handle_sprite_moved(size_t from);
        void refresh_size();
        sigc::signal<void, size_t, size_t>& signal_new_sequence();
        sigc::signal<void>& signal_sequence_invalidated();
        void left();
        void right();
        void left_stop();
        void right_stop();
        void home();
        void end();
        void update();

    protected:

    private:

        const size_t Num_visible_sprites = 7;
        const int Gap = 8;
        const float Vx = 250;
        const float Tolerance = 0.000001;

        virtual void draw(sf::RenderTarget& target, 
                sf::RenderStates states) const;
        void init_view(const Panel_dims& dims);
        void init_seq_markers();
        void set_sequence(size_t start, size_t end);
        void update_seq_markers();
        void verify_sequence_after_remove(size_t i);
        void verify_sequence_after_insert(size_t i);
        bool validate_sequence(size_t start, size_t end);
        void invalidate_sequence();

        sf::View                    m_view;
        std::shared_ptr<Project>    m_project_ptr;
        size_t                      m_first_visible_sprite;
        State                       m_state;
        // 'Full width' is width of all the sprites together plus gaps.
        // Begin with 1 sprite.
        int                         m_full_width;
        float                       m_camera_x;
        float                       m_right_edge;
        sigc::signal<void, size_t, size_t>
                                    m_signal_new_sequence;
        sigc::signal<void>          m_signal_sequence_invalidated;

        // FIXME: VERY MESSY!!!
        sf::RectangleShape          m_seq_start_marker;
        sf::RectangleShape          m_seq_start_marker2;
        sf::RectangleShape          m_seq_start_marker3;
        sf::RectangleShape          m_seq_start_marker4;
        sf::RectangleShape          m_seq_end_marker;
        sf::RectangleShape          m_seq_end_marker2;
        sf::RectangleShape          m_seq_end_marker3;
        sf::RectangleShape          m_seq_end_marker4;
        float                       m_seq_start_x;
        float                       m_seq_end_x;
        size_t                      m_seq_start;
        size_t                      m_seq_end;

        // FIXME: hack - stop sequence markers from being drawn before a 
        // sequence has been set.
        bool                        m_sequence_set;

        const Panel_dims&           m_dims_ref;
        int                         m_window_w;
        const Sprite_dims&          m_sprdims_ref;

}; // class Preview_panel

} // namespace sprmo




#endif

