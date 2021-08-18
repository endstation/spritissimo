// Spritissimo64
// Copyright 2016-2021 Matthew Clarke <pclar7@yahoo.co.uk>

// This file is part of Spritissimo64.

// Spritissimo64 is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.

// Spritissimo64 is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.

// You should have received a copy of the GNU General Public License
// along with Spritissimo64.  If not, see <http://www.gnu.org/licenses/>.




#ifndef Palette_h
#define Palette_h

#include <SFML/Graphics.hpp>




namespace sprmo
{

class Palette
{

    public:

        enum C64_color {
            Black,
            White,
            Red,
            Cyan,
            Violet,
            Green,
            Blue,
            Yellow,
            Orange,
            Brown,
            Light_red,
            Grey1,
            Grey2,
            Light_green,
            Light_blue, 
            Grey3
        };

        enum Color_type { Background, Sprite, SPMC0, SPMC1 };

        static const C64_color Default_bg_color     = Blue;
        static const C64_color Default_spmc0        = Violet;
        static const C64_color Default_spmc1        = Black;
        static const C64_color Default_spr_color    = White;
        static const sf::Color RGBs[];

        Palette(C64_color bg = Default_bg_color,
                C64_color spr = Default_spr_color,
                C64_color spmc0 = Default_spmc0,
                C64_color spmc1 = Default_spmc1);
        ~Palette();

        const sf::Color& cycle(int delta, int i);
        const sf::Color& get_color(int i) const;
        const sf::Color& set_color(int i, C64_color color);
        C64_color get_color_index(int i) const { return m_colors[i]; }
        const sf::Color& get_color_at(int i) const { return RGBs[i]; }
        const sf::Color& get_sfcolor(C64_color c) const { return RGBs[c]; }
        C64_color cycle2(C64_color color, int delta) const;

    protected:

    private:

        C64_color m_colors[4];

}; // class Palette

} // namespace sprmo




#endif

