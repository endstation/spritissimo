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




#include "Palette.h"




const sf::Color sprmo::Palette::RGBs[] = {
    sf::Color(0,0,0),
    sf::Color(255,255,255),
    sf::Color(104,55,43),
    sf::Color(112,164,178),
    sf::Color(111,61,134),
    sf::Color(88,141,67),
    sf::Color(53,40,121),
    sf::Color(184,199,111),
    sf::Color(111,79,37),
    sf::Color(67,57,0),
    sf::Color(154,103,89),
    sf::Color(68,68,68),
    sf::Color(108,108,108),
    sf::Color(154,210,132),
    sf::Color(108,94,181),
    sf::Color(149,149,149)
};


// *******************
// *** CONSTRUCTOR ***
// *******************

sprmo::Palette::Palette(
        C64_color bg,
        C64_color spr,
        C64_color spmc0,
        C64_color spmc1)
{
    m_colors[0] = bg;
    m_colors[1] = spr;
    m_colors[2] = spmc0;
    m_colors[3] = spmc1;

} // sprmo::Palette::Palette

// ******************
// *** DESTRUCTOR ***
// ******************

sprmo::Palette::~Palette()
{
    // NYI...

} // sprmo::Palette::~Palette




// ************************
// *** MEMBER FUNCTIONS ***
// ************************

// **************************************************

const sf::Color& sprmo::Palette::cycle(int delta, int i)
{
    int mycolor = m_colors[i];
    mycolor += delta;
    if (mycolor < Black)
    {
        mycolor = Grey3;
    }
    else if (mycolor > Grey3)
    {
        mycolor = Black;
    } // if ... else

    m_colors[i] = static_cast<C64_color>(mycolor);
    return RGBs[mycolor];

} // sprmo::Palette::cycle

// **************************************************

const sf::Color& sprmo::Palette::get_color(int i) const
{
    Color_type t = static_cast<Color_type>(i);
    return RGBs[ m_colors[t] ];

} // sprmo::Palette::get_color

// **************************************************

const sf::Color& sprmo::Palette::set_color(int i, C64_color color)
{
    m_colors[i] = color;
    return RGBs[color];

} // sprmo::Palette::set_color

// **************************************************

sprmo::Palette::C64_color sprmo::Palette::cycle2(
        C64_color color, int delta) const
{
    int c = color;
    c += delta;
    if (c < Black)
    {
        c = Grey3;
    }
    else if (c > Grey3)
    {
        c = Black;
    } // if ... else

    return static_cast<C64_color>(c);

} // sprmo::Palette::cycle2

// **************************************************

// sprmo::Palette::
// {
// } // sprmo::Palette::

// **************************************************

// sprmo::Palette::
// {
// } // sprmo::Palette::

// **************************************************

// sprmo::Palette::
// {
// } // sprmo::Palette::

// **************************************************

// sprmo::Palette::
// {
// } // sprmo::Palette::

// **************************************************

// sprmo::Palette::
// {
// } // sprmo::Palette::

// **************************************************

// sprmo::Palette::
// {
// } // sprmo::Palette::

// **************************************************

// sprmo::Palette::
// {
// } // sprmo::Palette::

// **************************************************

// sprmo::Palette::
// {
// } // sprmo::Palette::

// **************************************************

// sprmo::Palette::
// {
// } // sprmo::Palette::

// **************************************************

// sprmo::Palette::
// {
// } // sprmo::Palette::

// **************************************************

// sprmo::Palette::
// {
// } // sprmo::Palette::

// **************************************************

// sprmo::Palette::
// {
// } // sprmo::Palette::

// **************************************************

// sprmo::Palette::
// {
// } // sprmo::Palette::

// **************************************************

// sprmo::Palette::
// {
// } // sprmo::Palette::

// **************************************************

// sprmo::Palette::
// {
// } // sprmo::Palette::

// **************************************************

// sprmo::Palette::
// {
// } // sprmo::Palette::

// **************************************************

// sprmo::Palette::
// {
// } // sprmo::Palette::

// **************************************************

// sprmo::Palette::
// {
// } // sprmo::Palette::

// **************************************************

// sprmo::Palette::
// {
// } // sprmo::Palette::

// **************************************************

// sprmo::Palette::
// {
// } // sprmo::Palette::

// **************************************************

// sprmo::Palette::
// {
// } // sprmo::Palette::

// **************************************************

// sprmo::Palette::
// {
// } // sprmo::Palette::

// **************************************************

// sprmo::Palette::
// {
// } // sprmo::Palette::

// **************************************************

// sprmo::Palette::
// {
// } // sprmo::Palette::

// **************************************************

// sprmo::Palette::
// {
// } // sprmo::Palette::

// **************************************************

// sprmo::Palette::
// {
// } // sprmo::Palette::

