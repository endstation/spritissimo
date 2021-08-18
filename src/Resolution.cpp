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




#include "Resolution.h"




// *******************
// *** CONSTRUCTOR ***
// *******************

sprmo::Resolution::Resolution() :
        m_bg_color(Default_bg_color),
        m_spmc0(Default_spmc0),
        m_spmc1(Default_spmc1),
        m_selection(4)
{
    // FIXME: user should specify resolution on application start-up.
    init();
    //print_to_console();

} // sprmo::Resolution::Resolution

// ******************
// *** DESTRUCTOR ***
// ******************

sprmo::Resolution::~Resolution()
{
    // NYI...

} // sprmo::Resolution::~Resolution




// ************************
// *** MEMBER FUNCTIONS ***
// ************************

// **************************************************

void sprmo::Resolution::init()
{
    tinyxml2::XMLDocument doc;
    tinyxml2::XMLError err = doc.LoadFile(
            PKG_DATA_DIR "/resolutions/c64.xml");
    if (err)
    {
        std::cerr << "Cannot load XML file!\n";
        exit(EXIT_FAILURE);
    } // if

    // 'Pixels per byte':
    tinyxml2::XMLElement* el = doc.FirstChildElement("resolution")->
            FirstChildElement("pixels_per_byte");
    m_pixels_per_byte = atoi(el->GetText());

    // Pixel dimensions:
    el = doc.FirstChildElement("resolution")->FirstChildElement("pixel");
    m_pixel_width   = el->IntAttribute("width");
    m_pixel_height  = el->IntAttribute("height");

    // Default width and height of sprites (in pixels):
    el = doc.FirstChildElement("resolution")->FirstChildElement(
            "default_width_pixels");
    m_default_width_pixels = atoi(el->GetText());
    el = doc.FirstChildElement("resolution")->FirstChildElement(
            "default_height_pixels");
    m_default_height_pixels = atoi(el->GetText());

    // Every resolution needs at least one palette, and every palette at least
    // two colors.
    tinyxml2::XMLElement* palette_el = 
            doc.FirstChildElement("resolution")->FirstChildElement("palette");

    while (palette_el)
    {
        //Palette my_palette;
        //my_palette.name = palette_el->Attribute("name");
        m_palette.name = palette_el->Attribute("name");

        tinyxml2::XMLElement* color_el = 
                palette_el->FirstChildElement("color");
        while (color_el)
        {
            const char* name = color_el->Attribute("name");
            int code = color_el->IntAttribute("code");
            int r = color_el->IntAttribute("r");
            int g = color_el->IntAttribute("g");
            int b = color_el->IntAttribute("b");
            Palette_color my_palette_color = {
                    name, code, sf::Color(r,g,b) };
            //my_palette.palette_colors.push_back(my_palette_color);
            m_palette.palette_colors.push_back(my_palette_color);
            color_el = color_el->NextSiblingElement();
            m_c64_palette.push_back( sf::Color(r, g, b) );
        } // while
        //m_palettes.push_back(my_palette);
        palette_el = palette_el->NextSiblingElement();
    } // while

    //m_colors_per_palette = m_palettes[0].palette_colors.size();
    m_colors_per_palette = m_palette.palette_colors.size();

    // Set default palette choice.  Order is: BG, SPR, MC0, MC1.
    size_t a[] = { Blue, White, Violet, Black }; 
    std::copy( a, a+4, m_selection.begin() );

} // sprmo::Resolution::init

// **************************************************

/*
void sprmo::Resolution::print_to_console() const
{
    std::cout << "pixels per byte=" << m_pixels_per_byte << "\n";
    std::cout << "    pixel width=" << m_pixel_width << "\n";
    std::cout << "   pixel height=" << m_pixel_height << "\n";
    std::cout << "   num palettes=" << m_palettes.size() << "\n";
    for (auto x : m_palettes)
    {
        std::cout << "-> " << x.name << "\n";
        for (auto c : x.palette_colors)
        {
            std::cout << c.name << " [" << c.code << "]: " << 
                    static_cast<int>(c.color.r) << "," << 
                    static_cast<int>(c.color.g) << "," << 
                    static_cast<int>(c.color.b) << "\n";
        } // for
    } // for

} // sprmo::Resolution::print_to_console
*/

// **************************************************

const sprmo::Resolution::Palette_color& 
sprmo::Resolution::get_palette_color(size_t color_i) const
{
    //assert(palette_i >= 0);
    //assert(palette_i < m_palettes.size());
    assert(color_i >= 0);
    assert(color_i < m_colors_per_palette);
    
    //return m_palettes[palette_i].palette_colors[color_i];
    //return m_palette.palette_colors[color_i];
    return m_palette.palette_colors[ m_selection[color_i] ];

} // sprmo::Resolution::get_palette_color

// **************************************************

/*
sf::Color sprmo::Resolution::get_clear_color(size_t palette_i) const
{
    return m_palettes[palette_i].palette_colors[0].color;

} // sprmo::Resolution::get_clear_color
*/

// **************************************************

sf::Color sprmo::Resolution::get_selection(size_t i) const
{
    size_t choice = m_selection[i];
    return m_c64_palette[choice];

} // sprmo::Resolution::get_selection

// **************************************************

/*
sf::Color sprmo::Resolution::get_palette_color(size_t i) const
{
    return m_c64_palette[i];

} // sprmo::Resolution::get_palette_color
*/

// **************************************************

// For the given color selection at index 'i', change to the next or previous
// color (according to 'delta') and return the corresponding sf::Color object.
sf::Color sprmo::Resolution::next_selection(size_t i, int delta)
{
    // Cast to int because adding 'delta' may give -1.
    int sel = static_cast<int>(m_selection[i]);
    sel = (sel + delta) % m_c64_palette.size();
    if (sel < 0) { sel = m_c64_palette.size() - 1; }
    m_selection[i] = static_cast<size_t>(sel);
    return m_c64_palette[sel];

} // sprmo::Resolution::next_selection

// **************************************************

size_t sprmo::Resolution::get_selection_i(size_t i) const
{
    return m_selection[i];

} // sprmo::Resolution::get_selection_i

// **************************************************

sf::Color sprmo::Resolution::set_selection(size_t i, size_t color_i)
{
    std::cout << "i=" << i << ", color_i=" << color_i << "\n";
    m_selection[i] = color_i;
    return m_c64_palette[color_i];

} // sprmo::Resolution::set_selection

// **************************************************

// sprmo::Resolution::
// {
// } // sprmo::Resolution::

// **************************************************

// sprmo::Resolution::
// {
// } // sprmo::Resolution::

// **************************************************

// sprmo::Resolution::
// {
// } // sprmo::Resolution::

// **************************************************

// sprmo::Resolution::
// {
// } // sprmo::Resolution::

// **************************************************

// sprmo::Resolution::
// {
// } // sprmo::Resolution::

// **************************************************

// sprmo::Resolution::
// {
// } // sprmo::Resolution::

// **************************************************

// sprmo::Resolution::
// {
// } // sprmo::Resolution::

// **************************************************

// sprmo::Resolution::
// {
// } // sprmo::Resolution::

// **************************************************

// sprmo::Resolution::
// {
// } // sprmo::Resolution::

// **************************************************

// sprmo::Resolution::
// {
// } // sprmo::Resolution::

// **************************************************

// sprmo::Resolution::
// {
// } // sprmo::Resolution::

// **************************************************

// sprmo::Resolution::
// {
// } // sprmo::Resolution::

// **************************************************

// sprmo::Resolution::
// {
// } // sprmo::Resolution::

// **************************************************

// sprmo::Resolution::
// {
// } // sprmo::Resolution::

// **************************************************

// sprmo::Resolution::
// {
// } // sprmo::Resolution::
