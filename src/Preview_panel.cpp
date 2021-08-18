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




#include "Preview_panel.h"




// *******************
// *** CONSTRUCTOR ***
// *******************

sprmo::Preview_panel::Preview_panel(std::shared_ptr<Project> p,
        const Panel_dims& dims, int win_w, const Sprite_dims& sprdims) :
    m_project_ptr(p),
    m_first_visible_sprite(0),
    m_state(Stopped),
    //m_full_width(Sprite::Img_width + 2 * Gap),
    m_full_width(sprdims.Img_width + 2 * Gap),
    m_camera_x(0),
    m_right_edge(0),
    m_sequence_set(false),
    m_dims_ref(dims),
    m_window_w(win_w),
    m_sprdims_ref(sprdims)
{
    init_view(dims);
    init_seq_markers();

} // sprmo::Preview_panel::Preview_panel

// ******************
// *** DESTRUCTOR ***
// ******************

sprmo::Preview_panel::~Preview_panel()
{
    // NYI...

} // sprmo::Preview_panel::~Preview_panel




// ************************
// *** MEMBER FUNCTIONS ***
// ************************

// **************************************************

void sprmo::Preview_panel::draw(sf::RenderTarget& target, 
        sf::RenderStates states) const
{
    //size_t begin_i = static_cast<int>(m_camera_x / (Gap + Sprite::Img_width));
    size_t begin_i = static_cast<int>(m_camera_x / (Gap + m_sprdims_ref.Img_width));
    size_t end_i = std::min(m_project_ptr->get_num_sprites(), 
            begin_i + Num_visible_sprites);
    float offset = (-m_camera_x);

    target.setView(m_view);
    for (size_t i = begin_i; i < end_i; ++i)
    {
        std::shared_ptr<Sprite> s = m_project_ptr->get_sprite(i);
        //float xpos = Gap + offset + (i * (Sprite::Img_width + Gap));
        float xpos = Gap + offset + (i * (m_sprdims_ref.Img_width + Gap));
        sf::Transform tfm;
        tfm.translate(xpos, 0);
        target.draw(s->get_quads_pixels(), tfm);
        if (i == m_project_ptr->get_current_sprite_i())
        {
            // FIXME: design hack!  Need to create a new RectangleShape each 
            // frame because not allowed to modify position inside this 
            // function.  Should probably be making better use of 'update' 
            // function...
            // Though takes about 10 microseconds to execute...
            sf::RectangleShape rs(sf::Vector2f(
                        m_sprdims_ref.Img_width, m_sprdims_ref.Img_height));
            rs.setFillColor(sf::Color(220, 220, 220, 100));
            rs.setPosition(xpos, 0);
            target.draw(rs);
        } // if
    } // for

    if (m_sequence_set)
    {
        target.draw(m_seq_start_marker);
        target.draw(m_seq_start_marker2);
        target.draw(m_seq_start_marker3);
        target.draw(m_seq_start_marker4);
        target.draw(m_seq_end_marker);
        target.draw(m_seq_end_marker2);
        target.draw(m_seq_end_marker3);
        target.draw(m_seq_end_marker4);
    } // if

    target.setView(target.getDefaultView());

} // sprmo::Preview_panel::draw


// **************************************************

void sprmo::Preview_panel::init_view(const Panel_dims& dims)
{
    m_view.setCenter(dims.center);
    m_view.setSize(dims.size);
    m_view.setViewport(dims.viewport);
    //m_view.zoom(0.75);

} // sprmo::Preview_panel::init_view


// **************************************************

void sprmo::Preview_panel::update()
{
    switch (m_state)
    {
        case Stopped:
            break;
        case Left:
            m_camera_x -= (Vx * (1.0/60));
            if (m_camera_x < 0)
            {
                m_camera_x = 0;
                m_state = Stopped;
            } // if
            update_seq_markers();
            break;
        case Right:
            m_camera_x += (Vx * (1.0/60));
            if (m_camera_x > m_right_edge)
            {
                m_camera_x = m_right_edge;
                m_state = Stopped;
            } // if
            update_seq_markers();
            break;
    } // switch

} // sprmo::Preview_panel::update

// **************************************************

void sprmo::Preview_panel::left()
{
    if (m_camera_x > 0)
    {
        m_state = Left;
    } // if

} // sprmo::Preview_panel::left

// **************************************************

void sprmo::Preview_panel::right()
{
    if (m_camera_x < m_right_edge)
    {
        m_state = Right;
    } // if

} // sprmo::Preview_panel::right

// **************************************************

// NOTE: the new sprite may already be visible in preview because it could
// have been inserted or added to the end but there aren't so many sprites
// yet.  So just update width and, if necessary, camera and right edge here.
void sprmo::Preview_panel::handle_new_sprite_added(int i)
{
    //m_full_width += (Sprite::Img_width + Gap);
    m_full_width += (m_sprdims_ref.Img_width + Gap);
    if (m_full_width > m_window_w/*Globals::Window_w*/)
    {
        m_right_edge = m_full_width - m_window_w/*Globals::Window_w*/;
    } // if

    // If the sprite was inserted we need to check that any animation
    // sequence is still valid.  If the sprite was added to the end, 'i' will
    // be negative.
    if (m_sequence_set && i >= 0)
    {
        verify_sequence_after_insert(static_cast<size_t>(i));
    } // if

} // sprmo::Preview_panel::handle_new_sprite_added

// **************************************************

void sprmo::Preview_panel::left_stop()
{
    if (m_state == Left)
    {
        m_state = Stopped;
    } // if

} // sprmo::Preview_panel::left_stop

// **************************************************

void sprmo::Preview_panel::right_stop()
{
    if (m_state == Right)
    {
        m_state = Stopped;
    } // if

} // sprmo::Preview_panel::right_stop

// **************************************************

bool sprmo::Preview_panel::handle_mouse_button_pressed(
        sf::Event::MouseButtonEvent& e, sf::RenderTarget& target)
{
    // FIXME: shouldn't be hard-coded!
    //bool in_bounds = e.y >= (450+11) && e.y < ((450+11) + Sprite::Img_height);
    bool in_bounds = e.y >= m_dims_ref.position.y
        && e.y < (m_dims_ref.position.y+m_dims_ref.size.y);

    if (in_bounds)
    {
        bool shift = sf::Keyboard::isKeyPressed(sf::Keyboard::LShift)
                || sf::Keyboard::isKeyPressed(sf::Keyboard::RShift);

        sf::Vector2f coords = target.mapPixelToCoords(
                sf::Vector2i(e.x, e.y), m_view);
        coords.x += m_camera_x;
        //size_t i = static_cast<int>(coords.x / (Gap + Sprite::Img_width));
        size_t i = static_cast<int>(coords.x / (Gap + m_sprdims_ref.Img_width));
        if (i < m_project_ptr->get_num_sprites())
        {
            //float diff = coords.x - (i * (Gap + Sprite::Img_width));
            float diff = coords.x - (i * (Gap + m_sprdims_ref.Img_width));
            if (diff >= Gap)
            {
                if (shift)
                {
                    size_t current = m_project_ptr->get_current_sprite_i();
                    set_sequence(std::min(current, i), std::max(current, i));
                }
                else
                {
                    // New current sprite selection.
                    m_project_ptr->set_current_sprite(i);
                } // if ... else
            } // if
        } // if 
    } // if

    return in_bounds;

} // sprmo::Preview_panel::handle_mouse_button_pressed

// **************************************************

// NOTE: called in response to a new project being loaded in.
void sprmo::Preview_panel::refresh_size()
{
    size_t n = m_project_ptr->get_num_sprites();
    //m_full_width = (Sprite::Img_width * n) + (Gap * (n + 1));
    m_full_width = (m_sprdims_ref.Img_width * n) + (Gap * (n + 1));
    m_right_edge = std::max(0, (m_full_width - m_window_w/*Globals::Window_w*/));
    m_camera_x = 0;
    m_sequence_set = false;
    
} // sprmo::Preview_panel::refresh_size

// **************************************************

void sprmo::Preview_panel::set_sequence(size_t start, size_t end)
{
    bool ok = validate_sequence(start, end);
    if (!ok)
    {
        std::cerr << "Invalid sequence!\n";
        return;
    } // if

    // Record 'world' coordinates (on x-axis) of start/end sequence markers.
    m_seq_start_x = static_cast<float>((Gap + m_sprdims_ref.Img_width) * start);
    m_seq_end_x   = static_cast<float>(((Gap + m_sprdims_ref.Img_width) * end) +
            m_sprdims_ref.Img_width + (Gap * 2));
    m_sequence_set = true;
    update_seq_markers();

    m_seq_start = start;
    m_seq_end = end;
    m_signal_new_sequence.emit(start, end);

} // sprmo::Preview_panel::set_sequence

// **************************************************

sigc::signal<void, size_t, size_t>& sprmo::Preview_panel::signal_new_sequence()
{
    return m_signal_new_sequence;

} // sprmo::Preview_panel::signal_new_sequence

// **************************************************

void sprmo::Preview_panel::handle_new_current_sprite(bool is_multicolor,
        Palette::C64_color c)
{
    // Get the current sprite's position in 'world' coordinates.
    size_t i = m_project_ptr->get_current_sprite_i();
    float x1 = static_cast<float>((Gap + m_sprdims_ref.Img_width) * i);
    float x2 = x1 + Gap * 2 + m_sprdims_ref.Img_width;

    // Is the sprite fully visible in the panel?
    if (x1 < m_camera_x)
    {
        m_camera_x = x1;
        update_seq_markers();
    }
    else if (x2 > (m_camera_x + m_window_w/*Globals::Window_w*/))
    {
        m_camera_x = x2 - m_window_w/*Globals::Window_w*/;
        update_seq_markers();
    } // if ... else

} // sprmo::Preview_panel::handle_new_current_sprite

// **************************************************

void sprmo::Preview_panel::init_seq_markers()
{
    m_seq_start_marker.setSize(sf::Vector2f(Marker_len, Marker_height));
    m_seq_start_marker.setFillColor(sf::Color(255, 0, 255));

    m_seq_start_marker2.setSize(sf::Vector2f(Marker_height, Marker_len));
    m_seq_start_marker2.setFillColor(sf::Color(255, 0, 255));

    m_seq_start_marker3.setSize(sf::Vector2f(Marker_len, Marker_height));
    m_seq_start_marker3.setFillColor(sf::Color(255, 0, 255));

    m_seq_start_marker4.setSize(sf::Vector2f(Marker_height, Marker_len));
    m_seq_start_marker4.setFillColor(sf::Color(255, 0, 255));

    m_seq_end_marker.setSize(sf::Vector2f(Marker_len, Marker_height));
    m_seq_end_marker.setFillColor(sf::Color(255, 0, 255));

    m_seq_end_marker2.setSize(sf::Vector2f(Marker_height, Marker_len));
    m_seq_end_marker2.setFillColor(sf::Color(255, 0, 255));

    m_seq_end_marker3.setSize(sf::Vector2f(Marker_len, Marker_height));
    m_seq_end_marker3.setFillColor(sf::Color(255, 0, 255));

    m_seq_end_marker4.setSize(sf::Vector2f(Marker_height, Marker_len));
    m_seq_end_marker4.setFillColor(sf::Color(255, 0, 255));

} // sprmo::Preview_panel::init_seq_markers

// **************************************************

void sprmo::Preview_panel::update_seq_markers()
{
    if (!m_sequence_set) { return; }

    m_seq_start_marker.setPosition(sf::Vector2f(
                m_seq_start_x - m_camera_x, 0));
    m_seq_start_marker2.setPosition(sf::Vector2f(
                m_seq_start_x - m_camera_x, 0));
    m_seq_start_marker3.setPosition(sf::Vector2f(
                m_seq_start_x - m_camera_x, m_sprdims_ref.Img_height - Gap));
    m_seq_start_marker4.setPosition(sf::Vector2f(
                m_seq_start_x - m_camera_x,
                m_sprdims_ref.Img_height - Marker_len));
    m_seq_end_marker.setPosition(sf::Vector2f(
                m_seq_end_x - m_camera_x - Marker_len, 0));
    m_seq_end_marker2.setPosition(sf::Vector2f(
                m_seq_end_x - m_camera_x - Gap, 0));
    m_seq_end_marker3.setPosition(sf::Vector2f(
                m_seq_end_x - m_camera_x - Marker_len,
                m_sprdims_ref.Img_height - Gap));
    m_seq_end_marker4.setPosition(sf::Vector2f(
                m_seq_end_x - m_camera_x - Gap,
                m_sprdims_ref.Img_height - Marker_len));

} // sprmo::Preview_panel::update_seq_markers

// **************************************************

// Change of current sprite will be handled later (in another signal handler).
// Here, just need to make sure m_full_width and m_right_edge variables are
// updated correctly.
void sprmo::Preview_panel::handle_sprite_removed(size_t i)
{
    m_full_width -= (m_sprdims_ref.Img_width + Gap);
    if (m_full_width > m_window_w)
    {
        m_right_edge = m_full_width - m_window_w;
    }
    else
    {
        m_right_edge = 0;
    } // if

    // Slight adjustment so we don't end up with a gap after the final sprite
    // which shouldn't be there.
    if (m_right_edge < m_camera_x)
    {
        m_camera_x = m_right_edge;
    } // if

    if (m_sequence_set)
    {
        verify_sequence_after_remove(i);
    } // if

} // sprmo::Preview_panel::handle_sprite_removed

// **************************************************

void sprmo::Preview_panel::verify_sequence_after_remove(size_t i)
{
    if (i >= m_seq_start && i <= m_seq_end)
    {
        invalidate_sequence();
    }
    else if (i < m_seq_start)
    {
        set_sequence(m_seq_start - 1, m_seq_end - 1);
    } // if ... else

} // sprmo::Preview_panel::verify_sequence_after_remove

// **************************************************

void sprmo::Preview_panel::verify_sequence_after_insert(size_t i)
{
    if (i >= m_seq_start && i <= m_seq_end)
    {
        invalidate_sequence();
    }
    else if (i < m_seq_start)
    {
        set_sequence(m_seq_start + 1, m_seq_end + 1);
    } // if ... else

} // sprmo::Preview_panel::verify_sequence_after_insert

// **************************************************

// Return true if proposed animation sequence is valid; otherwise false.
// Must be more than one sprite and they should all be the same size.
bool sprmo::Preview_panel::validate_sequence(size_t start, size_t end)
{
    if (start == end) { return false; }

    bool ok = true;
    size_t w = m_project_ptr->get_sprite(start)->get_width();
    size_t h = m_project_ptr->get_sprite(start)->get_height();
    for (size_t i = start+1; i <= end; ++i)
    {
        if (m_project_ptr->get_sprite(i)->get_width() != w
                || m_project_ptr->get_sprite(i)->get_height() != h)
        {
            ok = false;
            break;
        } // if
    } // for

    return ok;

} // sprmo::Preview_panel::validate_sequence

// **************************************************

void sprmo::Preview_panel::invalidate_sequence()
{
    m_sequence_set = false;
    m_signal_sequence_invalidated.emit();

} // sprmo::Preview_panel::invalidate_sequence

// **************************************************

sigc::signal<void>& sprmo::Preview_panel::signal_sequence_invalidated()
{
    return m_signal_sequence_invalidated;

} // sprmo::Preview_panel::signal_sequence_invalidated

// **************************************************

void sprmo::Preview_panel::handle_sprite_moved(size_t from)
{
    if (!m_sequence_set) { return; }

    size_t to = m_project_ptr->get_current_sprite_i();
    if (from >= m_seq_start && from <= m_seq_end)
    {
        if (to < m_seq_start || to > m_seq_end)
        {
            // Sprite has moved out of animation sequence.
            invalidate_sequence();
        }
    }
    else if (from < m_seq_start || from > m_seq_end)
    {
        if (to >= m_seq_start && to <= m_seq_end)
        {
            // Sprite has moved into the animation sequence.
            invalidate_sequence();
        }
    } // if ... else

} // sprmo::Preview_panel::handle_sprite_moved

// **************************************************

void sprmo::Preview_panel::home()
{
    m_camera_x = 0;
    update_seq_markers();

} // sprmo::Preview_panel::home

// **************************************************

void sprmo::Preview_panel::end()
{
    m_camera_x = m_right_edge;
    update_seq_markers();

} // sprmo::Preview_panel::end

// **************************************************

// sprmo::Preview_panel::
// {
// } // sprmo::Preview_panel::

// **************************************************

// sprmo::Preview_panel::
// {
// } // sprmo::Preview_panel::

// **************************************************

// sprmo::Preview_panel::
// {
// } // sprmo::Preview_panel::

// **************************************************

// sprmo::Preview_panel::
// {
// } // sprmo::Preview_panel::

// **************************************************

// sprmo::Preview_panel::
// {
// } // sprmo::Preview_panel::

// **************************************************

// sprmo::Preview_panel::
// {
// } // sprmo::Preview_panel::

// **************************************************

// sprmo::Preview_panel::
// {
// } // sprmo::Preview_panel::

// **************************************************

// sprmo::Preview_panel::
// {
// } // sprmo::Preview_panel::

// **************************************************

// sprmo::Preview_panel::
// {
// } // sprmo::Preview_panel::

// **************************************************

// sprmo::Preview_panel::
// {
// } // sprmo::Preview_panel::

// **************************************************

// sprmo::Preview_panel::
// {
// } // sprmo::Preview_panel::

// **************************************************

// sprmo::Preview_panel::
// {
// } // sprmo::Preview_panel::

// **************************************************

// sprmo::Preview_panel::
// {
// } // sprmo::Preview_panel::

// **************************************************

// sprmo::Preview_panel::
// {
// } // sprmo::Preview_panel::

// **************************************************

// sprmo::Preview_panel::
// {
// } // sprmo::Preview_panel::

// **************************************************

// sprmo::Preview_panel::
// {
// } // sprmo::Preview_panel::

