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




#ifndef Palette_panel_h
#define Palette_panel_h

//#include "Resolution.h"
#include <SFML/Graphics.hpp>
#include <sigc++/sigc++.h>
#include "Globals.h"
#include "Palette.h"
#include <vector>
#include <iostream>
#include "Panel_dims.h"




namespace sprmo
{

class Palette_panel : public sf::Drawable
{

    public:

        static const int Panel_x = 0;
        static const int Panel_y = 0;
        static const int Panel_w = 250;
        static const int Panel_h = 50;
        //static const int Spacing = 10;
        //static const int Pool_size = 30;
        
        float Spacing;
        float Pool_size;

        float Selected_quad_base_x_coords[4];
        static const size_t Num_pools = 4;
        enum Pool_type { Bg, Spr, SPMC0, SPMC1 };

        struct Summary
        {
            Summary() : colors(Num_pools), type(Spr) {}
            std::vector<std::pair<sf::Color,Palette::C64_color>> colors;
            Pool_type   type;
        }; // struct Summary


        Palette_panel(Palette& pal, const Panel_dims& dims);
        ~Palette_panel();

        void next_pool(bool is_multicolor);
        void previous_pool(bool is_multicolor);
        sigc::signal<void, size_t>& signal_pool_changed();
        sigc::signal<void, size_t>& signal_color_cycled();
        void handle_new_current_sprite(bool is_multicolor, Palette::C64_color i);
        void handle_loading_sprite(Palette::C64_color color);
        size_t get_current_selection_i() const { return m_current_pool; }
        void cycle_current_pool(int delta);
        //void recolorize();
        const std::vector<Palette::C64_color>& get_colors() const { return m_colors; }
        void reset_global(int bg, int spmc0, int spmc1);
        const Summary& get_summary() { return m_summary; }

    protected:

    private:

        virtual void draw(sf::RenderTarget& target, 
                sf::RenderStates states) const;
        void init_quads();
        void init_view(const Panel_dims& dims);
        void handle_new_current_pool();
        void init_colors();
        const sf::Color& get_current_sfcolor() const;
        void update_summary(bool colors_changed = true);
        void move_cursor();

        sf::VertexArray     m_pool_quads;
        size_t              m_current_pool;
        sigc::signal<void, size_t> m_signal_pool_changed;
        sigc::signal<void, size_t> m_signal_color_cycled;
        sf::View            m_view;
        Palette&            m_palette_ref;

        std::vector<Palette::C64_color> m_colors;
        Summary             m_summary;

}; // class Palette_panel


} // namespace sprmo




#endif

