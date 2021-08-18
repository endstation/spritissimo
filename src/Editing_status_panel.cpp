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




#include "Editing_status_panel.h"




// *******************
// *** CONSTRUCTOR ***
// *******************

sprmo::Editing_status_panel::Editing_status_panel(const sf::Font& font,
        std::shared_ptr<Project> p, const Panel_dims& dims, unsigned int text_size) :
    m_mask_color(150, 150, 255, 100),
    m_project_ptr(p)
{
    m_text.setFont(font);
    m_text.setCharacterSize(text_size);
    m_text.setFillColor(sf::Color(180, 180, 180));
    m_text.setString("()");
    m_text.setPosition(sf::Vector2f(10, 10));

    m_current_sprite_text.setFont(font);
    m_current_sprite_text.setCharacterSize(text_size);
    m_current_sprite_text.setFillColor(sf::Color(180, 180, 180));
    m_current_sprite_text.setString("goodbye");
    m_current_sprite_text.setPosition(sf::Vector2f(dims.size.x*(2/3.0), 10));

    init_view(dims);
    set_current_sprite_i(0, 1);

} // sprmo::Editing_status_panel::Editing_status_panel

// ******************
// *** DESTRUCTOR ***
// ******************

sprmo::Editing_status_panel::~Editing_status_panel()
{
    // NYI...

} // sprmo::Editing_status_panel::~Editing_status_panel




// ************************
// *** MEMBER FUNCTIONS ***
// ************************

// **************************************************

void sprmo::Editing_status_panel::init_view(const Panel_dims& dims)
{
    m_view.setCenter(dims.center);
    m_view.setSize(dims.size);
    m_view.setViewport(dims.viewport);

} // sprmo::Editing_status_panel::init_view

// **************************************************

void sprmo::Editing_status_panel::draw(sf::RenderTarget& target, 
        sf::RenderStates states) const
{
    target.setView(m_view);
    target.draw(m_text);
    target.draw(m_current_sprite_text);
    target.setView(target.getDefaultView());

} // sprmo::Editing_status_panel::draw

// **************************************************

void sprmo::Editing_status_panel::handle_cell_changed(const sf::Vector2i& cell)
{
    std::string s(boost::str(boost::format("(%2d,%2d)") % cell.x % cell.y));
    m_text.setString(s);

} // sprmo::Editing_status_panel::handle_cell_changed

// **************************************************

void sprmo::Editing_status_panel::set_current_sprite_i(size_t i, size_t n)
{
    std::string s(boost::str(boost::format("#%d/%d") % (i+1) % n));
    m_current_sprite_text.setString(s);

} // sprmo::Editing_status_panel::set_current_sprite_i

// **************************************************

void sprmo::Editing_status_panel::handle_new_current_sprite(bool is_multicolor,
        Palette::C64_color i)
{
    set_current_sprite_i(m_project_ptr->get_current_sprite_i(), 
            m_project_ptr->get_num_sprites());

    // User should be dropped into pixel-editing mode, whether or not mask
    // is enabled.
    //bool is_mask = m_project_ptr->get_current_sprite()->is_mask_enabled();
    //m_mask_color.a = is_mask ? 255 : 100;
    //m_mask_enabled_text.setColor(m_mask_color);
    //m_mask_enabled_text.setStyle(sf::Text::Regular);
    //bool preshift = m_project_ptr->get_current_sprite()->must_preshift();
    //m_mask_color.a = preshift ? 255 : 100;
    //m_must_preshift_text.setColor(m_mask_color);

} // sprmo::Editing_status_panel::handle_new_current_sprite

// **************************************************

void sprmo::Editing_status_panel::handle_mask_enabled_toggled(bool enabled)
{
    m_mask_color.a = enabled ? 255 : 100;
    //m_mask_enabled_text.setColor(m_mask_color);

} // sprmo::Editing_status_panel::handle_mask_enabled_toggled

// **************************************************

void sprmo::Editing_status_panel::handle_mode_toggled(
        Editing_panel::Editing_mode mode)
{
    /*
    sf::Text::Style style = (mode == Editing_panel::Pixels) ? 
            sf::Text::Regular : sf::Text::Underlined;
    */
    //m_mask_enabled_text.setStyle(style);

} // sprmo::Editing_status_panel::handle_mode_toggled

// **************************************************

void sprmo::Editing_status_panel::handle_must_preshift_toggled(bool b)
{
    m_mask_color.a = b ? 255 : 100;
    //m_must_preshift_text.setColor(m_mask_color);

} // sprmo::Editing_status_panel::handle_must_preshift_toggled

// **************************************************

void sprmo::Editing_status_panel::handle_sprite_moved(size_t from)
{
    set_current_sprite_i(m_project_ptr->get_current_sprite_i(), 
            m_project_ptr->get_num_sprites());

} // sprmo::Editing_status_panel::handle_sprite_moved

// **************************************************

// sprmo::Editing_status_panel::
// {
// } // sprmo::Editing_status_panel::

// **************************************************

// sprmo::Editing_status_panel::
// {
// } // sprmo::Editing_status_panel::

// **************************************************

// sprmo::Editing_status_panel::
// {
// } // sprmo::Editing_status_panel::

// **************************************************

// sprmo::Editing_status_panel::
// {
// } // sprmo::Editing_status_panel::

// **************************************************

// sprmo::Editing_status_panel::
// {
// } // sprmo::Editing_status_panel::

// **************************************************

// sprmo::Editing_status_panel::
// {
// } // sprmo::Editing_status_panel::

// **************************************************

// sprmo::Editing_status_panel::
// {
// } // sprmo::Editing_status_panel::

// **************************************************

// sprmo::Editing_status_panel::
// {
// } // sprmo::Editing_status_panel::

// **************************************************

// sprmo::Editing_status_panel::
// {
// } // sprmo::Editing_status_panel::

// **************************************************

// sprmo::Editing_status_panel::
// {
// } // sprmo::Editing_status_panel::

// **************************************************

// sprmo::Editing_status_panel::
// {
// } // sprmo::Editing_status_panel::

// **************************************************

// sprmo::Editing_status_panel::
// {
// } // sprmo::Editing_status_panel::

// **************************************************

// sprmo::Editing_status_panel::
// {
// } // sprmo::Editing_status_panel::

// **************************************************

// sprmo::Editing_status_panel::
// {
// } // sprmo::Editing_status_panel::

// **************************************************

// sprmo::Editing_status_panel::
// {
// } // sprmo::Editing_status_panel::

// **************************************************

// sprmo::Editing_status_panel::
// {
// } // sprmo::Editing_status_panel::

// **************************************************

// sprmo::Editing_status_panel::
// {
// } // sprmo::Editing_status_panel::
