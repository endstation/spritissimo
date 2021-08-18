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




#include "Animation_status_panel.h"




// *******************
// *** CONSTRUCTOR ***
// *******************

sprmo::Animation_status_panel::Animation_status_panel(const sf::Font& font,
        std::shared_ptr<Animation_panel>& ap, const Panel_dims& dims,
        unsigned int text_size) :
    m_animation_panel_ptr(ap),
    m_anim_type_strings(Num_anim_types),
    m_active_color(200, 200, 200),
    m_inactive_color(200, 200, 200, 50)
{
    init_view(dims);
    const char* a[] = { "LOOP", "PING-PONG", "ONE-TIME" };
    std::copy(a, a+Num_anim_types, m_anim_type_strings.begin());
    init_texts(font, text_size, dims);

} // sprmo::Animation_status_panel::Animation_status_panel

// ******************
// *** DESTRUCTOR ***
// ******************

sprmo::Animation_status_panel::~Animation_status_panel()
{
    // NYI...

} // sprmo::Animation_status_panel::~Animation_status_panel




// ************************
// *** MEMBER FUNCTIONS ***
// ************************

// **************************************************

void sprmo::Animation_status_panel::init_view(const Panel_dims& dims)
{
    m_view.setCenter(dims.center);
    m_view.setSize(dims.size);
    m_view.setViewport(dims.viewport);
    
} // sprmo::Animation_status_panel::init_view

// **************************************************

void sprmo::Animation_status_panel::draw(sf::RenderTarget& target, 
        sf::RenderStates states) const
{
    target.setView(m_view);
    target.draw(m_anim_type_text);
    target.draw(m_frame_rate_text);
    target.draw(m_current_frame_text);
    target.setView(target.getDefaultView());
    
} // sprmo::Animation_status_panel::draw

// **************************************************

void sprmo::Animation_status_panel::init_texts(const sf::Font& font,
        unsigned int text_size, const Panel_dims& dims)
{
    m_anim_type_text.setFont(font);
    m_anim_type_text.setCharacterSize(text_size);
    m_anim_type_text.setFillColor(m_inactive_color);
    m_anim_type_text.setString( 
            m_anim_type_strings[m_animation_panel_ptr->get_anim_type()] );
    m_anim_type_text.setPosition(sf::Vector2f(10, 10));

    m_frame_rate_text.setFont(font);
    m_frame_rate_text.setCharacterSize(text_size);
    m_frame_rate_text.setFillColor(m_inactive_color);
    std::string s(boost::str(boost::format("%d/60") 
                % m_animation_panel_ptr->get_frame_rate()));
    m_frame_rate_text.setString(s);
    m_frame_rate_text.setPosition(sf::Vector2f(dims.size.x/2, 10));

    m_current_frame_text.setFont(font);
    m_current_frame_text.setCharacterSize(text_size);
    m_current_frame_text.setFillColor(m_inactive_color);
    m_current_frame_text.setString("-");
    m_current_frame_text.setPosition(sf::Vector2f(dims.size.x*(3/4.0), 10));

} // sprmo::Animation_status_panel::init_texts

// **************************************************

void sprmo::Animation_status_panel::handle_anim_type_changed(
        Animation_panel::Anim_type t)
{
    m_anim_type_text.setString( m_anim_type_strings[t] );

} // sprmo::Animation_status_panel::handle_anim_type_changed

// **************************************************

void sprmo::Animation_status_panel::handle_frame_rate_changed(size_t rate)
{
    m_frame_rate_text.setString(boost::str(boost::format("%d/60") % rate));

} // sprmo::Animation_status_panel::handle_frame_rate_changed

// **************************************************

void sprmo::Animation_status_panel::handle_frame_changed(size_t frame)
{
    m_current_frame_text.setString(boost::str(boost::format("%d") % frame));

} // sprmo::Animation_status_panel::handle_frame_changed

// **************************************************

void sprmo::Animation_status_panel::handle_sequence_invalidated()
{
    m_anim_type_text.setFillColor(m_inactive_color);
    m_current_frame_text.setFillColor(m_inactive_color);
    m_frame_rate_text.setFillColor(m_inactive_color);
    m_frame_rate_text.setString("-");

} // sprmo::Animation_status_panel::handle_sequence_invalidated

// **************************************************

void sprmo::Animation_status_panel::handle_new_sequence(size_t start, size_t end)
{
    m_anim_type_text.setFillColor(m_active_color);
    m_current_frame_text.setFillColor(m_active_color);
    m_frame_rate_text.setFillColor(m_active_color);
    std::string s(boost::str(boost::format("%d") % start));
    m_current_frame_text.setString(s);

} // sprmo::Animation_status_panel::handle_new_sequence

// **************************************************

// sprmo::Animation_status_panel::
// {
// } // sprmo::Animation_status_panel::

// **************************************************

// sprmo::Animation_status_panel::
// {
// } // sprmo::Animation_status_panel::

// **************************************************

// sprmo::Animation_status_panel::
// {
// } // sprmo::Animation_status_panel::

// **************************************************

// sprmo::Animation_status_panel::
// {
// } // sprmo::Animation_status_panel::

// **************************************************

// sprmo::Animation_status_panel::
// {
// } // sprmo::Animation_status_panel::

// **************************************************

// sprmo::Animation_status_panel::
// {
// } // sprmo::Animation_status_panel::

// **************************************************

// sprmo::Animation_status_panel::
// {
// } // sprmo::Animation_status_panel::

// **************************************************

// sprmo::Animation_status_panel::
// {
// } // sprmo::Animation_status_panel::

// **************************************************

// sprmo::Animation_status_panel::
// {
// } // sprmo::Animation_status_panel::

// **************************************************

// sprmo::Animation_status_panel::
// {
// } // sprmo::Animation_status_panel::

// **************************************************

// sprmo::Animation_status_panel::
// {
// } // sprmo::Animation_status_panel::

// **************************************************

// sprmo::Animation_status_panel::
// {
// } // sprmo::Animation_status_panel::

// **************************************************

// sprmo::Animation_status_panel::
// {
// } // sprmo::Animation_status_panel::

// **************************************************

// sprmo::Animation_status_panel::
// {
// } // sprmo::Animation_status_panel::

// **************************************************

// sprmo::Animation_status_panel::
// {
// } // sprmo::Animation_status_panel::

// **************************************************

// sprmo::Animation_status_panel::
// {
// } // sprmo::Animation_status_panel::

// **************************************************

// sprmo::Animation_status_panel::
// {
// } // sprmo::Animation_status_panel::

// **************************************************

// sprmo::Animation_status_panel::
// {
// } // sprmo::Animation_status_panel::

// **************************************************

// sprmo::Animation_status_panel::
// {
// } // sprmo::Animation_status_panel::

// **************************************************

// sprmo::Animation_status_panel::
// {
// } // sprmo::Animation_status_panel::

// **************************************************

// sprmo::Animation_status_panel::
// {
// } // sprmo::Animation_status_panel::

// **************************************************

// sprmo::Animation_status_panel::
// {
// } // sprmo::Animation_status_panel::

// **************************************************

// sprmo::Animation_status_panel::
// {
// } // sprmo::Animation_status_panel::
