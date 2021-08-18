// spritissimo64
// Copyright 2016-2021 Matthew Clarke <pclar7@yahoo.co.uk>

// This file is part of spritissimo64.

// spritissimo64 is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.

// spritissimo64 is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.

// You should have received a copy of the GNU General Public License
// along with spritissimo64.  If not, see <http://www.gnu.org/licenses/>.




#ifndef Color_selection_panel_h
#define Color_selection_panel_h

#include <SFML/Graphics.hpp>




namespace sprmo
{

class Color_selection_panel : public sf::Drawable
{

    public:

        Color_selection_panel();
        ~Color_selection_panel();

    protected:

    private:

        virtual void draw(sf::RenderTarget& target, 
                sf::RenderStates states) const;

}; // class Color_selection_panel

} // namespace sprmo




#endif

