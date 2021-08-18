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




#ifndef Heartbeat_h
#define Heartbeat_h

#include <SFML/System.hpp>
#include <functional>
#include <boost/noncopyable.hpp>
#include <iostream>




namespace sprmo 
{

class Heartbeat: private boost::noncopyable
{

    public:

        Heartbeat(float rate_s, std::function<void()> cb);
        ~Heartbeat();

        void check_beat();
        void pause();
        void resume();
        bool is_paused() const { return m_paused; }
        float get_rate() const { return m_rate_s; }

    protected:

    private:
        float                   m_rate_s;
        sf::Clock               m_clock;
        std::function<void()>   m_cb;
        bool                    m_paused;

}; // class Heartbeat

} // namespace sprmo




#endif

