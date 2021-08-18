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




#ifndef Zoomable_h
#define Zoomable_h

#include <SFML/Graphics/View.hpp>




namespace sprmo
{

class Zoomable
{

    public:

        enum Zoom_state { Zoom_none, Zooming_in, Zooming_out };

        Zoomable(sf::View& view, float initial_zoom = 1.0,
                float zoom_rate = 0.02);
        ~Zoomable();

        void zoom_in() { m_current_state = Zooming_in; }
        void zoom_out() { m_current_state = Zooming_out; }
        void zoom_in_stop();
        void zoom_out_stop();
        void stop_all() { m_current_state = Zoom_none; }
        void update();

    protected:

    private:

        const float Zoom_rate;
        Zoom_state  m_current_state;
        sf::View&   m_view_ref;

}; // class Zoomable

} // namespace sprmo




#endif

