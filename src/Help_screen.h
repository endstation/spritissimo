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




#ifndef Help_screen_h
#define Help_screen_h

#include <tinyxml2.h>
#include <utility>
#include <vector>
#include <string>
#include <iostream>
#include <cstdlib>
#include <unordered_map>
#include <SFML/Graphics.hpp>
#include "Globals.h"




namespace sprmo
{

class Help_screen : public sf::Drawable
{

    public:

        //enum Section { Editing, Sprite_set, Animation, File_IO };
        enum State { Stopped, Scrolling_up, Scrolling_down };

        Help_screen(const sf::Font& f, unsigned int text_size, int win_w);
        ~Help_screen();

        void update();
        void scroll_up();
        void scroll_down();
        void stop_up();
        void stop_down();

    protected:

    private:

        void init();
        void process_section(tinyxml2::XMLElement* el);
        void process_entry(tinyxml2::XMLElement* el,
                const std::string& section);
        void create_sprite(unsigned int text_size, int win_w);
        void display() const;
        virtual void draw(sf::RenderTarget& target, 
                sf::RenderStates states) const;

        //const size_t        Gap = 20;   // On y-axis.
        //const size_t        Section_gap = 32;
        //const size_t        Margin = 25;
        const float         Vy = 400;
        float               Gap = 20;
        float               Section_gap = 32;
        float               Margin = 25;

        int                 m_sprite_height;
        float               m_min_y;
        sf::Font            m_font;
        sf::RenderTexture   m_tex;
        sf::Sprite          m_sprite;
        State               m_current_state;
        float               m_sprite_pos_y;
        std::unordered_map<std::string,
            std::vector<std::pair<std::string,std::string>>> m_sections;

}; // class Help_screen

} // namespace sprmo




#endif
