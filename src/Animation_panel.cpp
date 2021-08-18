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




#include "Animation_panel.h"




// *******************
// *** CONSTRUCTOR ***
// *******************

sprmo::Animation_panel::Animation_panel(std::shared_ptr<Project> p,
        const Panel_dims& dims, const Sprite_dims& sprdims) :
    m_project_ptr(p),
    m_sequence_start(0),
    m_sequence_end(0),
    m_animating(false),
    m_fps(Default_fps),
    m_anim_type(Ping_pong),
    m_ready(false)
{
    init_view(dims, sprdims);
    m_zoomable = std::unique_ptr<Zoomable>(new Zoomable(m_view));

} // sprmo::Animation_panel::Animation_panel

// ******************
// *** DESTRUCTOR ***
// ******************

sprmo::Animation_panel::~Animation_panel()
{
    // NYI...

} // sprmo::Animation_panel::~Animation_panel




// ************************
// *** MEMBER FUNCTIONS ***
// ************************

// **************************************************

void sprmo::Animation_panel::handle_new_sequence(size_t start, size_t end)
{
    m_sequence_start = start;
    m_sequence_end = end;
    m_ready = true;

} // sprmo::Animation_panel::handle_new_sequence

// **************************************************

void sprmo::Animation_panel::update()
{
    if (!m_animating) { return; }

    m_zoomable->update();
    bool frame_changed = false;

    --m_count;
    if (!m_count)
    {
        m_current_frame += m_delta;
        frame_changed = true;
        switch (m_anim_type)
        {
            case Loop:
                m_count = m_fps;
                if (m_current_frame > static_cast<int>(m_sequence_end))
                {
                    m_current_frame = m_sequence_start;
                } // if
                break;
            case Ping_pong:
                m_count = m_fps;
                if (m_current_frame > static_cast<int>(m_sequence_end))
                {
                    m_delta *= (-1);
                    m_current_frame = m_sequence_end - 1;
                }
                else if (m_current_frame < static_cast<int>(m_sequence_start))
                {
                    m_delta *= (-1);
                    m_current_frame = m_sequence_start + 1;
                } // if ... else
                break;
            case One_time:
                if (m_current_frame > static_cast<int>(m_sequence_end))
                {
                    m_current_frame = m_sequence_start;
                    m_count = m_fps;
                }
                else if (m_current_frame == static_cast<int>(m_sequence_end))
                {
                    m_count = Pause_frames; 
                }
                else
                {
                    m_count = m_fps;
                } // if ... else
                break;
        } // switch
    } // if

    if (frame_changed)
    {
        m_signal_frame_changed.emit(m_current_frame);
    } // if

} // sprmo::Animation_panel::update

// **************************************************

void sprmo::Animation_panel::start_stop()
{
    if (m_animating)
    {
        // Stop animating.
        m_animating = false;
        m_current_frame = m_sequence_start;
        m_signal_frame_changed.emit(m_current_frame);
    }
    else if (m_ready)
    {
        // Start animating.
        m_animating = true;
        m_count = m_fps;
        m_current_frame = m_sequence_start;
        m_delta = 1;
    } // if ... else

} // sprmo::Animation_panel::start_stop

// **************************************************

void sprmo::Animation_panel::init_view(const Panel_dims& dims,
        const Sprite_dims& sprdims)
{
    //m_view.setCenter(sf::Vector2f(200, 200));
    //m_view.setSize(sf::Vector2f(400, 400));
    //m_view.setViewport(sf::FloatRect(
            //400.0 / Globals::Window_w, 0,
            //400.0 / Globals::Window_w, 400.0 / Globals::Window_h));
    //m_view.move(-136, -136);
    // size/2 - spr_w/2, size/2 - spr_h/2
    //m_view.move(-104,-116);
    m_view.setCenter(dims.center);
    m_view.setSize(dims.size);
    m_view.setViewport(dims.viewport);

    m_view.move(
            -((dims.size.x / 2) - (sprdims.Img_width / 2.0)),
            -((dims.size.y / 2) - (sprdims.Img_height / 2.0)));

} // sprmo::Animation_panel::init_view

// **************************************************

void sprmo::Animation_panel::draw(sf::RenderTarget& target, 
                sf::RenderStates states) const
{
    if (m_animating)
    {
        target.setView(m_view);
        std::shared_ptr<Sprite> spr = m_project_ptr->get_sprite(m_current_frame);
        target.draw(spr->get_quads_pixels());
    }
    else
    {
        // TODO: if there's a valid sequence, draw first frame but 'greyed out'
        // in some way - shader?!
    } // if ... else

    target.setView(target.getDefaultView());

} // sprmo::Animation_panel::draw

// **************************************************

void sprmo::Animation_panel::inc_fps()
{
    if (m_animating && m_fps > Min_fps)
    {
        --m_fps;
        m_animating = false;
        start_stop();
    } // if

    m_signal_frame_rate_changed.emit(m_fps);

} // sprmo::Animation_panel::inc_fps

// **************************************************

void sprmo::Animation_panel::dec_fps()
{
    if (m_animating && m_fps < Max_fps)
    {
        ++m_fps;
        m_animating = false;
        start_stop();
    } // if

    m_signal_frame_rate_changed.emit(m_fps);

} // sprmo::Animation_panel::dec_fps

// **************************************************

void sprmo::Animation_panel::next_anim_type()
{
    if (m_anim_type == One_time)
    {
        m_anim_type = Loop;
    }
    else
    {
        m_anim_type = static_cast<Anim_type>(static_cast<int>(m_anim_type) + 1);
    } // if ... else

    // Make sure animation variables reset if we're already animating to avoid
    // crashes.
    if (m_animating)
    {
        m_animating = false;
        start_stop();
    } // if

    m_signal_anim_type_changed.emit(m_anim_type);

} // sprmo::Animation_panel::next_anim_type

// **************************************************

sigc::signal<void, sprmo::Animation_panel::Anim_type>& 
sprmo::Animation_panel::signal_anim_type_changed()
{
    return m_signal_anim_type_changed;

} // sprmo::Animation_panel::signal_anim_type_changed

// **************************************************

sigc::signal<void, size_t>& sprmo::Animation_panel::signal_frame_rate_changed()
{
    return m_signal_frame_rate_changed;

} // sprmo::Animation_panel::signal_frame_rate_changed

// **************************************************

sigc::signal<void, size_t>& sprmo::Animation_panel::signal_frame_changed()
{
    return m_signal_frame_changed;

} // sprmo::Animation_panel::signal_frame_changed

// **************************************************

void sprmo::Animation_panel::handle_sequence_invalidated()
{
    m_animating = false;
    m_ready = false;

} // sprmo::Animation_panel::handle_sequence_invalidated

// **************************************************

// sprmo::Animation_panel::
// {
// } // sprmo::Animation_panel::

// **************************************************

// sprmo::Animation_panel::
// {
// } // sprmo::Animation_panel::

// **************************************************

// sprmo::Animation_panel::
// {
// } // sprmo::Animation_panel::

// **************************************************

// sprmo::Animation_panel::
// {
// } // sprmo::Animation_panel::

// **************************************************

// sprmo::Animation_panel::
// {
// } // sprmo::Animation_panel::

// **************************************************

// sprmo::Animation_panel::
// {
// } // sprmo::Animation_panel::

// **************************************************

// sprmo::Animation_panel::
// {
// } // sprmo::Animation_panel::

// **************************************************

// sprmo::Animation_panel::
// {
// } // sprmo::Animation_panel::

// **************************************************

// sprmo::Animation_panel::
// {
// } // sprmo::Animation_panel::

// **************************************************

// sprmo::Animation_panel::
// {
// } // sprmo::Animation_panel::

// **************************************************

// sprmo::Animation_panel::
// {
// } // sprmo::Animation_panel::

// **************************************************

// sprmo::Animation_panel::
// {
// } // sprmo::Animation_panel::

// **************************************************

// sprmo::Animation_panel::
// {
// } // sprmo::Animation_panel::

// **************************************************

// sprmo::Animation_panel::
// {
// } // sprmo::Animation_panel::

// **************************************************

// sprmo::Animation_panel::
// {
// } // sprmo::Animation_panel::
