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




#include "Zoomable.h"




// *******************
// *** CONSTRUCTOR ***
// *******************

sprmo::Zoomable::Zoomable(sf::View& view, float initial_zoom, float zoom_rate) :
    Zoom_rate(zoom_rate),
    m_current_state(Zoom_none),
    m_view_ref(view)
{
    m_view_ref.zoom(initial_zoom);

} // sprmo::Zoomable::Zoomable

// ******************
// *** DESTRUCTOR ***
// ******************

sprmo::Zoomable::~Zoomable()
{
    // NYI...

} // sprmo::Zoomable::~Zoomable




// ************************
// *** MEMBER FUNCTIONS ***
// ************************

// **************************************************

void sprmo::Zoomable::zoom_in_stop()
{
    if (m_current_state == Zooming_in)
    {
        m_current_state = Zoom_none;
    } // if

} // sprmo::Zoomable::zoom_in_stop

// **************************************************

void sprmo::Zoomable::zoom_out_stop()
{
    if (m_current_state == Zooming_out)
    {
        m_current_state = Zoom_none;
    } // if

} // sprmo::Zoomable::zoom_out_stop

// **************************************************

void sprmo::Zoomable::update()
{
    switch (m_current_state)
    {
        case Zoom_none:
            break;
        case Zooming_in:
            m_view_ref.zoom(1 - Zoom_rate);
            break;
        case Zooming_out:
            m_view_ref.zoom(1 + Zoom_rate);
            break;
    } // switch

} // sprmo::Zoomable::update

// **************************************************

// sprmo::Zoomable::
// {
// } // sprmo::Zoomable::

// **************************************************

// sprmo::Zoomable::
// {
// } // sprmo::Zoomable::

