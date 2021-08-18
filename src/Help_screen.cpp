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




#include "Help_screen.h"




// *******************
// *** CONSTRUCTOR ***
// *******************

sprmo::Help_screen::Help_screen(const sf::Font& f, unsigned int text_size,
        int win_w) :
    Gap(text_size),
    Section_gap(text_size * (8/5.0)),
    Margin(text_size * (5/4.0)),
    m_font(f),
    m_current_state(Stopped),
    m_sprite_pos_y(0)
{
    init();
    //display();
    create_sprite(text_size, win_w);

} // sprmo::Help_screen::Help_screen

// ******************
// *** DESTRUCTOR ***
// ******************

sprmo::Help_screen::~Help_screen()
{
    // NYI...

} // sprmo::Help_screen::~Help_screen




// ************************
// *** MEMBER FUNCTIONS ***
// ************************

// **************************************************

void sprmo::Help_screen::init()
{
    tinyxml2::XMLDocument doc;
    tinyxml2::XMLError err = doc.LoadFile(
            PKG_DATA_DIR "/help/shortcuts.xml");
    if (err)
    {
        std::cerr << "Cannot load shortcuts.xml!  Error code=" << err << "\n";
        exit(EXIT_FAILURE);
    } // if

    tinyxml2::XMLElement* el = doc.FirstChildElement();//->
    el = el->FirstChildElement();
    do
    {
        process_section(el);
    } while ( (el = el->NextSiblingElement()) );

} // sprmo::Help_screen::init

// **************************************************

void sprmo::Help_screen::process_section(tinyxml2::XMLElement* el)
{
    std::string s( el->Attribute("name") );
    //std::cout << s << "\n";
    m_sections[s] = std::vector<std::pair<std::string,std::string>>();

    tinyxml2::XMLElement* entry_el = el->FirstChildElement();
    do
    {
        process_entry(entry_el, s);
    } while ( (entry_el = entry_el->NextSiblingElement()) );

} // sprmo::Help_screen::process_section

// **************************************************

void sprmo::Help_screen::process_entry(tinyxml2::XMLElement* el,
        const std::string& section)
{
    std::string keys( el->FirstChildElement("keys")->GetText() );
    std::string desc( el->FirstChildElement("description")->GetText() );
    //std::cout << "keys=" << keys << "\n";
    //std::cout << "description=" << desc << "\n";

    m_sections[section].push_back( std::make_pair(keys, desc) );

} // sprmo::Help_screen::process_entry

// **************************************************

void sprmo::Help_screen::display() const
{
    for (auto v : m_sections)
    {
        std::cout << v.first << std::endl;
        auto vec = v.second;
        for (auto entry : vec)
        {
            std::cout << entry.first << ":" << entry.second << std::endl;
        } // for
    } // for

} // sprmo::Help_screen::display

// **************************************************

void sprmo::Help_screen::create_sprite(unsigned int text_size, int win_w)
{
    // Calculate total number of entries.
    size_t num_entries = 0;
    for (auto x : m_sections)
    {
        num_entries += x.second.size();
    } // for
    m_sprite_height = (Margin * 2) + (7 * Section_gap) + (num_entries * Gap);
    m_min_y = (-1) * (m_sprite_height - Globals::Window_h);

    m_tex.create(win_w, m_sprite_height);
    m_tex.clear(sf::Color(20,20,20));

    size_t y = Margin;
    std::string section_names[] = { "EDITING", "FILE_IO", "SPRITE_SET", "ANIMATION" };
    for (auto sec : section_names)
    {
        sf::Text txt(sec, m_font, static_cast<int>(text_size*(7/5.0)));
        txt.setFillColor(sf::Color(155,155,200));
        txt.setStyle(sf::Text::Bold);
        txt.setPosition(Margin, y);
        y += Section_gap;
        m_tex.draw(txt);

        txt.setCharacterSize(text_size);
        txt.setStyle(sf::Text::Regular);
        for (auto entry : m_sections[sec])
        {
            txt.setFillColor(sf::Color(200,200,120));
            txt.setString(entry.first);
            txt.setPosition(Margin, y);
            m_tex.draw(txt);

            txt.setFillColor(sf::Color(200,200,200));
            txt.setString(entry.second);
            txt.setPosition(Margin + win_w*(3/8.0), y);
            m_tex.draw(txt);
            y += Gap;
        } // for
        y += Section_gap;
    } // for

    m_tex.display();
    m_sprite.setTexture( m_tex.getTexture() );

} // sprmo::Help_screen::create_sprite

// **************************************************

void sprmo::Help_screen::update()
{
    if (m_current_state == Scrolling_up)
    {
        m_sprite_pos_y += Vy * (1.0 / 60);
        if (m_sprite_pos_y > 0)
        {
            m_sprite_pos_y = 0;
        } // if
    }
    else if (m_current_state == Scrolling_down)
    {
        m_sprite_pos_y -= Vy * (1.0 / 60);
        if (m_sprite_pos_y < m_min_y)
        {
            m_sprite_pos_y = m_min_y;
        } // if
    } // if ... else

    m_sprite.setPosition(0, m_sprite_pos_y);

} // sprmo::Help_screen::update

// **************************************************

void sprmo::Help_screen::draw(sf::RenderTarget& target,
        sf::RenderStates states) const
{
    target.draw(m_sprite);

} // sprmo::Help_screen::draw

// **************************************************

void sprmo::Help_screen::scroll_up()
{
    //std::cout << "up\n";
    m_current_state = Scrolling_up;

} // sprmo::Help_screen::scroll_up

// **************************************************

void sprmo::Help_screen::scroll_down()
{
    //std::cout << "down\n";
    m_current_state = Scrolling_down;

} // sprmo::Help_screen::scroll_down

// **************************************************

void sprmo::Help_screen::stop_up()
{
    if (m_current_state == Scrolling_up)
    {
        m_current_state = Stopped;
    } // if

} // sprmo::Help_screen::stop_up

// **************************************************

void sprmo::Help_screen::stop_down()
{
    if (m_current_state == Scrolling_down)
    {
        m_current_state = Stopped;
    } // if

} // sprmo::Help_screen::stop_up

// **************************************************

// sprmo::Help_screen::
// {
// } // sprmo::Help_screen::

// **************************************************

// sprmo::Help_screen::
// {
// } // sprmo::Help_screen::

// **************************************************

// sprmo::Help_screen::
// {
// } // sprmo::Help_screen::

// **************************************************

// sprmo::Help_screen::
// {
// } // sprmo::Help_screen::

// **************************************************

// sprmo::Help_screen::
// {
// } // sprmo::Help_screen::

// **************************************************

// sprmo::Help_screen::
// {
// } // sprmo::Help_screen::

// **************************************************

// sprmo::Help_screen::
// {
// } // sprmo::Help_screen::

// **************************************************

// sprmo::Help_screen::
// {
// } // sprmo::Help_screen::
