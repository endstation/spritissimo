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




#ifndef Resolution_h
#define Resolution_h

#include <tinyxml2.h>
#include <string>
#include <iostream>
#include <cstdlib>
#include <SFML/Graphics.hpp>
#include <cassert>
#include <algorithm>




namespace sprmo
{

class Resolution
{

    public:

        enum C64_color { Black, White, Red, Cyan, Violet, Green, Blue,
                Yellow, Orange, Brown, Light_red, Grey1, Grey2, Light_green,
                Light_blue, Grey3 };

        static const C64_color Default_bg_color     = Blue;
        static const C64_color Default_spmc0        = Violet;
        static const C64_color Default_spmc1        = Black;
        static const C64_color Default_spr_color    = White;

        struct Palette_color
        {
            std::string name;
            int code;
            sf::Color color;
        }; // Palette_color

        struct Palette
        {
            std::string name;
            std::vector<Palette_color> palette_colors;
        }; // struct Palette

        Resolution();
        ~Resolution();

        // FIXME: for the C64 there's only one palette!
        //const Palette& get_palette(int i) const { return m_palettes[i]; }
        const Palette& get_palette() const { return m_palette; }
        //size_t num_palettes() const { return m_palettes.size(); }
        size_t num_colors_per_palette() const { return m_colors_per_palette; }
        /*const Palette_color& get_palette_color(
                size_t palette_i, size_t color_i) const;*/
        const Palette_color& get_palette_color(size_t color_i) const;
        size_t get_default_width_pixels() const { return m_default_width_pixels; }
        size_t get_default_height_pixels() const { return m_default_height_pixels; }
        size_t get_pixels_per_byte() const { return m_pixels_per_byte; }
        //sf::Color get_clear_color(size_t palette_i) const;

        sf::Color get_bg_color() const { 
            return m_palette.palette_colors[m_bg_color].color; }
        sf::Color get_spmc0() const {
            return m_palette.palette_colors[m_spmc0].color; }
        sf::Color get_spmc1() const {
            return m_palette.palette_colors[m_spmc1].color; }
        sf::Color get_default_spr_color() const {
            return m_palette.palette_colors[Default_spr_color].color; }

        sf::Color get_selection(size_t i) const;
        sf::Color next_selection(size_t i, int delta);
        // Returns the INDEX of the given selection.
        size_t get_selection_i(size_t i) const;
        sf::Color set_selection(size_t i, size_t color_i);

        //sf::Color get_palette_color(size_t i) const;

    protected:

    private:

        void init();
        //void print_to_console() const;

        size_t m_pixels_per_byte;
        // NOTE: absolute resolution on Dragon is 256*192.  But a 'pixel' in 
        // the lower resolution modes will be made up of several 'hardware'
        // pixels.  These two variables store how many.
        size_t m_pixel_width;
        size_t m_pixel_height;
        size_t m_colors_per_palette;
        //std::vector<Palette> m_palettes;
        Palette m_palette;
        size_t m_default_width_pixels;
        size_t m_default_height_pixels;

        // Global color settings.
        C64_color m_bg_color;
        C64_color m_spmc0;
        C64_color m_spmc1;

        std::vector<sf::Color> m_c64_palette;
        std::vector<size_t> m_selection;


}; // class Resolution

} // namespace sprmo




#endif

