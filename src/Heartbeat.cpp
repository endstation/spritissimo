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




#include "Heartbeat.h"




// *******************
// *** CONSTRUCTOR ***
// *******************

sprmo::Heartbeat::Heartbeat(float rate_s, std::function<void()> cb) : 
        m_rate_s(rate_s),
        m_cb(cb),
        m_paused(false)
{
    // Nothing to do.

} // sprmo::Heartbeat::Heartbeat

// ******************
// *** DESTRUCTOR ***
// ******************

sprmo::Heartbeat::~Heartbeat()
{
    // Nothing to do.

} // sprmo::Heartbeat::~Heartbeat




// ************************
// *** MEMBER FUNCTIONS ***
// ************************

// **************************************************

void sprmo::Heartbeat::check_beat()
{
    float dt = m_clock.getElapsedTime().asSeconds();
    if (dt >= m_rate_s)
    {
        m_clock.restart();
        if (!m_paused)
        {
            m_cb();
        } // if
    } // if

} // sprmo::Heartbeat::check_beat

// **************************************************

void sprmo::Heartbeat::pause()
{
    m_paused = true;

} // sprmo::Heartbeat::pause

// **************************************************

void sprmo::Heartbeat::resume()
{
    m_paused = false;

} // sprmo::Heartbeat::resume

// **************************************************

// sprmo::Heartbeat::
// {
// } // sprmo::Heartbeat::

// **************************************************

// sprmo::Heartbeat::
// {
// } // sprmo::Heartbeat::
