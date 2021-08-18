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




#ifndef Editing_status_panel_h
#define Editing_status_panel_h

#include <SFML/Graphics.hpp>
#include <memory>
#include <boost/format.hpp>
#include "Globals.h"
#include <iostream>
#include "Project.h"
#include "Editing_panel.h"
#include "Palette.h"
#include "Panel_dims.h"




namespace sprmo
{

class Editing_status_panel : public sf::Drawable
{

    public:

        Editing_status_panel(const sf::Font& font, std::shared_ptr<Project> p,
                const Panel_dims& dims, unsigned int text_size); 
        ~Editing_status_panel();

        void handle_cell_changed(const sf::Vector2i& cell);
        void handle_new_current_sprite(bool is_multicolor, Palette::C64_color i);
        void handle_mask_enabled_toggled(bool enabled);
        void handle_mode_toggled(Editing_panel::Editing_mode mode);
        void handle_must_preshift_toggled(bool b);
        void handle_sprite_moved(size_t from);
        void set_current_sprite_i(size_t i, size_t n);

    protected:

    private:

        sf::Text                    m_text;
        sf::Text                    m_current_sprite_text;
        //sf::Text                    m_mask_enabled_text;
        //sf::Text                    m_must_preshift_text;
        sf::Color                   m_mask_color;
        sf::View                    m_view;
        std::shared_ptr<Project>    m_project_ptr;

        virtual void draw(sf::RenderTarget& target, 
                sf::RenderStates states) const;
        void init_view(const Panel_dims& dims);

}; // class Editing_status_panel

} // namespace sprmo




#endif
