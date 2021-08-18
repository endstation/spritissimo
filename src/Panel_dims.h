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




#ifndef Panel_dims_h
#define Panel_dims_h

#include <SFML/System/Vector2.hpp>
#include <iostream>




namespace sprmo
{

struct Panel_dims
{
    Panel_dims(
            const sf::Vector2f& size_,
            const sf::Vector2f& center_,
            const sf::FloatRect viewport_,
            const sf::Vector2f& position_ = sf::Vector2f()) : 
        size(size_), center(center_), viewport(viewport_), position(position_) {}

    sf::Vector2f    size;
    sf::Vector2f    center;
    sf::FloatRect   viewport;
    sf::Vector2f    position;

    void display()
    {
        std::cout << "size:(" << size.x << "," << size.y << "), ";
        std::cout << "center:(" << center.x << "," << center.y << "), ";
        std::cout << "viewport:(" << viewport.left << "," << viewport.top <<
            "," << viewport.width << "," << viewport.height << ")\n";
        std::cout << "position:(" << position.x << "," << position.y << ")\n";
    } // display_dims

}; // struct Panel_dims


} // namespace sprmo




#endif

