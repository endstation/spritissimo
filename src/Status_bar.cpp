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




#include "Status_bar.h"




// *******************
// *** CONSTRUCTOR ***
// *******************

sprmo::Status_bar::Status_bar(
        const sf::Font& font,
        std::shared_ptr<Project>& p,
        const Panel_dims& dims,
        unsigned int text_size) :
    Cursor_pulse_rate(0.07),
    m_project_ptr(p),
    m_cursor(sf::Vector2f(text_size/2.0, text_size)),
    m_cursor_color(sf::Color(100, 100, 220)),
    m_entry_active(false),
    m_radians(0.0)
{
    init_view(dims);

    m_label_text.setFont(font);
    m_label_text.setCharacterSize(text_size);
    m_label_text.setFillColor(sf::Color(200, 200, 100));
    m_label_text.setString("EDITING");
    m_label_text.setPosition(sf::Vector2f(6, 0));

    m_entry_text.setFont(font);
    m_entry_text.setCharacterSize(text_size);
    m_entry_text.setFillColor(m_cursor_color);
    m_entry_text.setPosition(sf::Vector2f(400, 0));

    m_cursor.setFillColor(sf::Color(100, 100, 220));
    m_cursor.setPosition(sf::Vector2f(400, 2));

} // sprmo::Status_bar::Status_bar

// ******************
// *** DESTRUCTOR ***
// ******************

sprmo::Status_bar::~Status_bar()
{
    // NYI...

} // sprmo::Status_bar::~Status_bar




// ************************
// *** MEMBER FUNCTIONS ***
// ************************

// **************************************************

void sprmo::Status_bar::draw(sf::RenderTarget& target, 
        sf::RenderStates states) const
{
    target.setView(m_view);
    target.draw(m_label_text);
    if (m_entry_active)
    { 
        target.draw(m_entry_text);
        target.draw(m_cursor); 
    } // if
    target.setView(target.getDefaultView());

} // sprmo::Status_bar::draw

// **************************************************

void sprmo::Status_bar::init_view(const Panel_dims& dims)
{
    m_view.setCenter(dims.center);
    m_view.setSize(dims.size);
    m_view.setViewport(dims.viewport);

} // sprmo::Status_bar::init_view

// **************************************************

void sprmo::Status_bar::activate_entry()
{
    m_entry_str = "";
    m_entry_text.setString(m_entry_str);
    m_entry_active = true;
    m_radians = 0.0;

} // sprmo::Status_bar::activate_entry

// **************************************************

void sprmo::Status_bar::update()
{
    if (m_entry_active)
    {
        m_radians += Cursor_pulse_rate;
        double y = sin(m_radians);
        // Make sure y is positive.
        if (y < 0) { y *= (-1.0); }
        m_cursor_color.a = static_cast<sf::Uint8>(255 * y);
        m_cursor.setFillColor(m_cursor_color);
    } // if

} // sprmo::Status_bar::update

// **************************************************

void sprmo::Status_bar::handle_text_entered(sf::Event::TextEvent& e)
{
    // Check for alphanumeric or underscore...
    if (    (e.unicode >= 48 && e.unicode <= 57)
            || (e.unicode >= 65 && e.unicode <= 90)
            || (e.unicode >= 97 && e.unicode <= 122)
            || e.unicode == 95 || e.unicode == 46)
    {
        m_entry_str.push_back( static_cast<char>(e.unicode) );
        refresh_entry();
    }
    // Check for 'backspace'.
    else if (e.unicode == 8)
    {
        if (!m_entry_str.empty())
        {
            m_entry_str.pop_back();
            refresh_entry();
        } // if
    } // if ... else

} // sprmo::Status_bar::handle_text_entered

// **************************************************

void sprmo::Status_bar::init_load(const std::string& filename)
{
    init_file_operation("Load project: ", filename);

} // sprmo::Status_bar::init_load

// **************************************************

void sprmo::Status_bar::init_save(const std::string& filename)
{
    init_file_operation("Save project: ", filename);

} // sprmo::Status_bar::init_save

// **************************************************

void sprmo::Status_bar::init_file_operation(const std::string& msg,
        const std::string& filename)
{
    activate_entry();
    m_entry_str.assign(filename);
    m_entry_text.setString(m_entry_str);
    m_label_text.setString(msg);
    
    sf::FloatRect pos = m_label_text.getGlobalBounds();
    m_entry_text.setPosition(sf::Vector2f(pos.left + pos.width, 0));
    pos = m_entry_text.getGlobalBounds();
    m_cursor.setPosition(sf::Vector2f(pos.left + pos.width + 2, 2));

} // sprmo::Status_bar::init_file_operation

// **************************************************

void sprmo::Status_bar::refresh_entry()
{
    m_entry_text.setString(m_entry_str);
    sf::FloatRect r = m_entry_text.getGlobalBounds();
    m_cursor.setPosition(sf::Vector2f(r.left + r.width + 2, 2));

} // sprmo::Status_bar::refresh_entry

// **************************************************

void sprmo::Status_bar::reset()
{
    m_entry_active = false;
    m_label_text.setString("EDITING");

} // sprmo::Status_bar::reset

// **************************************************

void sprmo::Status_bar::init_write_prg(const std::string& filename)
{
    init_file_operation("Write PRG: ", filename);

} // sprmo::Status_bar::init_write_source_code

// **************************************************

void sprmo::Status_bar::init_write_png(const std::string& filename)
{
    init_file_operation("Write PNG file: ", filename);

} // sprmo::Status_bar::init_write_png

// **************************************************

void sprmo::Status_bar::init_confirm_quit()
{
    activate_entry();
    m_label_text.setString("Lose unsaved changes? [No] ");

    sf::FloatRect pos = m_label_text.getGlobalBounds();
    m_entry_text.setPosition(sf::Vector2f(pos.left + pos.width, 0));
    pos = m_entry_text.getGlobalBounds();
    m_cursor.setPosition(sf::Vector2f(pos.left + pos.width + 2, 2));

} // sprmo::Status_bar::init_confirm_quit

// **************************************************

// sprmo::Status_bar::
// {
// } // sprmo::Status_bar::

// **************************************************

// sprmo::Status_bar::
// {
// } // sprmo::Status_bar::

// **************************************************

// sprmo::Status_bar::
// {
// } // sprmo::Status_bar::

// **************************************************

// sprmo::Status_bar::
// {
// } // sprmo::Status_bar::

// **************************************************

// sprmo::Status_bar::
// {
// } // sprmo::Status_bar::

// **************************************************

// sprmo::Status_bar::
// {
// } // sprmo::Status_bar::

// **************************************************

// sprmo::Status_bar::
// {
// } // sprmo::Status_bar::

// **************************************************

// sprmo::Status_bar::
// {
// } // sprmo::Status_bar::

// **************************************************

// sprmo::Status_bar::
// {
// } // sprmo::Status_bar::

// **************************************************

// sprmo::Status_bar::
// {
// } // sprmo::Status_bar::

// **************************************************

// sprmo::Status_bar::
// {
// } // sprmo::Status_bar::

// **************************************************

// sprmo::Status_bar::
// {
// } // sprmo::Status_bar::

// **************************************************

// sprmo::Status_bar::
// {
// } // sprmo::Status_bar::

// **************************************************

// sprmo::Status_bar::
// {
// } // sprmo::Status_bar::

// **************************************************

// sprmo::Status_bar::
// {
// } // sprmo::Status_bar::

// **************************************************

// sprmo::Status_bar::
// {
// } // sprmo::Status_bar::

// **************************************************

// sprmo::Status_bar::
// {
// } // sprmo::Status_bar::

// **************************************************

// sprmo::Status_bar::
// {
// } // sprmo::Status_bar::
