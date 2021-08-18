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




#ifndef Status_bar_h
#define Status_bar_h

#include <SFML/Graphics.hpp>
#include <memory>
#include "Project.h"
#include <iostream>
#include "Globals.h"
#include <cmath>
#include <string>
#include "Panel_dims.h"




namespace sprmo
{

class Status_bar : public sf::Drawable
{

    public:

        // NOTE: measured in radians per 1/60s.
        const double Cursor_pulse_rate;

        Status_bar(const sf::Font& font, std::shared_ptr<Project>& p,
                const Panel_dims& dims, unsigned int text_size);
        ~Status_bar();

        void update();
        void handle_text_entered(sf::Event::TextEvent& e);
        void init_load(const std::string& filename = "");
        void init_save(const std::string& filename = "");
        void init_write_prg(const std::string& filename);
        void init_write_png(const std::string& filename);
        void init_confirm_quit();
        void reset();
        const std::string& get_entry_text() const { return m_entry_str; }
        void display_error_msg(const std::string& msg);

    protected:

    private:

        virtual void draw(sf::RenderTarget& target, 
                sf::RenderStates states) const;
        void init_view(const Panel_dims& dims);
        void init_file_operation(const std::string& msg,
                const std::string& filename);
        void refresh_entry();
        void activate_entry();

        std::shared_ptr<Project>    m_project_ptr;
        sf::View                    m_view;
        sf::Text                    m_label_text;
        sf::Text                    m_entry_text;
        sf::RectangleShape          m_cursor;
        sf::Color                   m_cursor_color;
        bool                        m_entry_active;
        double                      m_radians;
        std::string                 m_entry_str;

}; // class Status_bar

} // namespace sprmo




#endif
