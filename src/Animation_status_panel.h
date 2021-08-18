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




#ifndef Animation_status_panel_h
#define Animation_status_panel_h

#include <SFML/Graphics.hpp>
#include "Globals.h"
#include <vector>
#include <string>
#include <algorithm>
#include <memory>
#include <iostream>
#include "Animation_panel.h"
#include <boost/format.hpp>
#include "Panel_dims.h"




namespace sprmo
{

class Animation_status_panel : public sf::Drawable
{

    public:

        const size_t Num_anim_types = 3;

        Animation_status_panel(const sf::Font& font,
                std::shared_ptr<Animation_panel>& ap, const Panel_dims& dims,
                unsigned int text_size);
        ~Animation_status_panel();

        void handle_anim_type_changed(Animation_panel::Anim_type t);
        void handle_frame_rate_changed(size_t rate);
        void handle_frame_changed(size_t frame);
        void handle_sequence_invalidated();
        void handle_new_sequence(size_t start, size_t end);

    protected:

    private:

        void init_view(const Panel_dims& dims);
        void init_texts(const sf::Font& font, unsigned int text_size,
                const Panel_dims& dims);
        virtual void draw(sf::RenderTarget& target, 
                sf::RenderStates states) const;
            
        std::shared_ptr<Animation_panel>    m_animation_panel_ptr;
        sf::View                            m_view;
        std::vector<std::string>            m_anim_type_strings;
        sf::Text                            m_anim_type_text;
        sf::Text                            m_current_frame_text;
        sf::Text                            m_frame_rate_text;
        sf::Color                           m_active_color;
        sf::Color                           m_inactive_color;

}; // class Animation_status_panel

} // namespace sprmo




#endif
