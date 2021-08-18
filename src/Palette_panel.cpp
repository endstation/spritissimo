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




#include "Palette_panel.h"



/*
const float sprmo::Palette_panel::Selected_quad_base_x_coords[] = {
        2, 
        2*sprmo::Palette_panel::Spacing + sprmo::Palette_panel::Pool_size - 2,
        2*sprmo::Palette_panel::Spacing + sprmo::Palette_panel::Pool_size - 2,
        2 
};
*/


// *******************
// *** CONSTRUCTOR ***
// *******************

sprmo::Palette_panel::Palette_panel(Palette& pal, const Panel_dims& dims) :
        m_current_pool(Spr),
        m_palette_ref(pal),
        m_colors(Num_pools)
{
    init_colors();
    init_view(dims);
    init_quads();
    update_summary();

} // sprmo::Palette_panel::Palette_panel

// ******************
// *** DESTRUCTOR ***
// ******************

sprmo::Palette_panel::~Palette_panel()
{
    // NYI...

} // sprmo::Palette_panel::~Palette_panel




// ************************
// *** MEMBER FUNCTIONS ***
// ************************

// **************************************************

void sprmo::Palette_panel::draw(sf::RenderTarget& target, 
        sf::RenderStates states) const
{
    target.setView(m_view);
    target.draw(m_pool_quads);
    target.setView(target.getDefaultView());

} // sprmo::Palette_panel::draw

// **************************************************

void sprmo::Palette_panel::init_quads()
{
    m_pool_quads.clear();
    m_pool_quads.setPrimitiveType(sf::Quads);

    for (size_t i = 0; i < Num_pools; ++i)
    {
        sf::Color c = m_palette_ref.get_sfcolor( m_colors[i] );
        m_pool_quads.append(sf::Vertex(sf::Vector2f(
                Spacing + (i * (Pool_size + Spacing)), Spacing), c));
        m_pool_quads.append(sf::Vertex(sf::Vector2f(
                Spacing + (i * (Pool_size + Spacing)) + Pool_size, Spacing), 
                c));
        m_pool_quads.append(sf::Vertex(sf::Vector2f(
                Spacing + (i * (Pool_size + Spacing)) + Pool_size, 
                Spacing + Pool_size), c));
        m_pool_quads.append(sf::Vertex(sf::Vector2f(
                Spacing + (i * (Pool_size + Spacing)), Spacing + Pool_size), 
                c));
    } // for

    // 5th quad for the selected color - visual feedback to user.
    //sf::Color c = get_current_sfcolor();
    m_pool_quads.append(sf::Vertex(sf::Vector2f(
            Selected_quad_base_x_coords[0]+(Pool_size+Spacing), 2)));
    m_pool_quads.append(sf::Vertex(sf::Vector2f(
            Selected_quad_base_x_coords[1]+(Pool_size+Spacing), 2)));
    m_pool_quads.append(sf::Vertex(sf::Vector2f(
            Selected_quad_base_x_coords[2]+(Pool_size+Spacing),
            2*Spacing+Pool_size-2)));
    m_pool_quads.append(sf::Vertex(sf::Vector2f( 
            Selected_quad_base_x_coords[3]+(Pool_size+Spacing),
            2*Spacing+Pool_size-2)));

} // sprmo::Palette_panel::init_quads

// **************************************************

void sprmo::Palette_panel::next_pool(bool is_multicolor)
{
    size_t n = is_multicolor ? 4 : 2;
    m_current_pool = (m_current_pool + 1) % n;
    handle_new_current_pool();
    update_summary(false);
    //m_signal_pool_changed.emit(m_current_pool);

} // sprmo::Palette_panel::next_pool

// **************************************************

void sprmo::Palette_panel::previous_pool(bool is_multicolor)
{
    size_t n = is_multicolor ? 4 : 2;
    if (m_current_pool)
    {
        --m_current_pool;
    }
    else
    {
        m_current_pool = n - 1;
    } // if ... else

    handle_new_current_pool();
    update_summary(false);
    //m_signal_pool_changed.emit(m_current_pool);

} // sprmo::Palette_panel::previous_pool

// **************************************************

//sigc::signal<void, const sprmo::Resolution::Palette_color&>& 
sigc::signal<void, size_t>& 
sprmo::Palette_panel::signal_pool_changed()
{
    return m_signal_pool_changed;

} // sprmo::Palette_panel::signal_pool_changed

// **************************************************

void sprmo::Palette_panel::init_view(const Panel_dims& dims)
{
    m_view.setCenter(dims.center);
    m_view.setSize(dims.size);
    m_view.setViewport(dims.viewport);

    Pool_size = dims.size.y * (4/5.0);
    Spacing = dims.size.y * (1/5.0);
    float extra_pool = Pool_size * (1/15.0);
    Selected_quad_base_x_coords[0] = extra_pool;
    Selected_quad_base_x_coords[1] = 2*Spacing + Pool_size - extra_pool;
    Selected_quad_base_x_coords[2] = 2*Spacing + Pool_size - extra_pool;
    Selected_quad_base_x_coords[3] = extra_pool;

} // sprmo::Palette_panel::init_view

// **************************************************

// The user has selected a different drawing color.  This method will 
// recalculate the '5th quad' so that current color is drawn bigger than
// the others.  Provides visual feedback to user about which is the current
// drawing color.
void sprmo::Palette_panel::handle_new_current_pool()
{
    // FIXME: hard coded value!!!
    // There are 4 vertices per color/quad and 4 colors, hence 4*4 = 16.
    size_t base_i = 16;
    sf::Color c = get_current_sfcolor();
    float offset = m_current_pool * (Spacing + Pool_size);
    for (size_t i = 0; i < 4; ++i)
    {
        m_pool_quads[base_i+i].position.x = 
                Selected_quad_base_x_coords[i] + offset;
        m_pool_quads[base_i+i].color = c;
    } // for

} // sprmo::Palette_panel::handle_new_current_pool

// **************************************************

void sprmo::Palette_panel::handle_new_current_sprite(bool is_multicolor,
        Palette::C64_color color)
{
    // Make sure a valid pool is selected (if moving from multicolor to
    // single color sprite).
    if ( !is_multicolor && (m_current_pool >= SPMC0) )
    {
        m_current_pool = Spr;
        move_cursor();
    } // if

    // Change color for 'Spr' pool and the cursor if applicable.
    m_colors[Spr] = color;
    sf::Color c = m_palette_ref.get_sfcolor(color);
    bool must_repaint_cursor = (m_current_pool == Spr);
    for (int i = 0; i < 4; ++i)
    {
        // NOTE: 4 refers to vertex offset.  We're coloring the second quad,
        // so must skip over four vertices.
        m_pool_quads[4+i].color = c;
        if (must_repaint_cursor)
        {
            m_pool_quads[16+i].color = c;
        } // if
    } // for

    update_summary();

} // sprmo::Palette_panel::handle_new_current_sprite

// **************************************************

void sprmo::Palette_panel::cycle_current_pool(int delta)
{
    m_colors[m_current_pool] = m_palette_ref.cycle2(m_colors[m_current_pool],
            delta);
    sf::Color c = get_current_sfcolor();
    size_t base = m_current_pool * 4;
    for (int i = 0; i < 4; ++i)
    {
        m_pool_quads[base+i].color = c;
    } // for
    handle_new_current_pool();
    update_summary();

    // TODO: if current color = 1 (i.e. sprite color), repaint all current
    // sprite's quads.  Otherwise repaint quads for ALL sprites...
    m_signal_color_cycled.emit(m_current_pool);

} // sprmo::Palette_panel::cycle_current_pool

// **************************************************

sigc::signal<void, size_t>& sprmo::Palette_panel::signal_color_cycled()
{
    return m_signal_color_cycled;

} // sprmo::Palette_panel::signal_color_cycled

// **************************************************

/*
void sprmo::Palette_panel::recolorize()
{
    for (int i = 0; i < 4; ++i)
    {
        if (i == 1) continue;
        sf::Color c = m_palette_ref.get_color(i);
        for (int j = 0; j < 4; ++j)
        {
            m_pool_quads[ i*4+j ].color = c;
        } // for
    } // for

} // sprmo::Palette_panel::recolorize
*/

// **************************************************

void sprmo::Palette_panel::init_colors()
{
    m_colors[Bg]    = Palette::Default_bg_color;
    m_colors[Spr]   = Palette::Default_spr_color;
    m_colors[SPMC0] = Palette::Default_spmc0;
    m_colors[SPMC1] = Palette::Default_spmc1;

} // sprmo::Palette_panel::init_colors

// **************************************************

const sf::Color& sprmo::Palette_panel::get_current_sfcolor() const
{
    return m_palette_ref.get_sfcolor( m_colors[m_current_pool] );

} // sprmo::Palette_panel::get_current_sfcolor

// **************************************************

void sprmo::Palette_panel::update_summary(bool colors_changed)
{
    if (colors_changed)
    {
        for (size_t i = 0; i < Num_pools; ++i)
        {
            m_summary.colors[i].first = m_palette_ref.get_sfcolor( m_colors[i] );
            m_summary.colors[i].second = m_colors[i];
        } // for
    } // if
    m_summary.type = static_cast<Pool_type>(m_current_pool);

} // sprmo::Palette_panel::update_summary

// **************************************************

// This is called when a new project (together with a new global palette)
// has been loaded in.
void sprmo::Palette_panel::reset_global(int bg, int spmc0, int spmc1)
{
    m_colors[Bg]    = static_cast<Palette::C64_color>(bg);
    m_colors[SPMC0] = static_cast<Palette::C64_color>(spmc0);
    m_colors[SPMC1] = static_cast<Palette::C64_color>(spmc1);

    // FIXME: we're ignoring the fifth 'cursor' quad.  Problem?!
    for (int i = 0; i < 4; ++i)
    {
        // Skip over 'Spr' pool.
        if (i == Spr) continue;
        sf::Color c = m_palette_ref.get_sfcolor( m_colors[i] );
        for (int j = 0; j < 4; ++j)
        {
            m_pool_quads[i*4+j].color = c;
        } // for 
    } // for

    update_summary();

} // sprmo::Palette_panel::reset_global

// **************************************************

// A new pool has been selected so move the fifth quad (= the 'cursor).
// Don't bother recoloring it yet though.
void sprmo::Palette_panel::move_cursor()
{
    size_t base_i = 16;
    float offset = m_current_pool * (Spacing + Pool_size);
    for (size_t i = 0; i < 4; ++i)
    {
        m_pool_quads[base_i+i].position.x =
            Selected_quad_base_x_coords[i] + offset;
    } // for

} // sprmo::Palette_panel::move_cursor

// **************************************************

void sprmo::Palette_panel::handle_loading_sprite(Palette::C64_color color)
{
    // Temporarily set Summary object to this sprite color.
    // No need to re-color quads.
    m_summary.colors[Spr].first = m_palette_ref.get_sfcolor(color);
    m_summary.colors[Spr].second = color;

} // sprmo::Palette_panel::handle_loading_sprite

// **************************************************

// sprmo::Palette_panel::
// {
// } // sprmo::Palette_panel::

// **************************************************

// sprmo::Palette_panel::
// {
// } // sprmo::Palette_panel::
