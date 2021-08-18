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




#include "Editing_panel.h"




// *******************
// *** CONSTRUCTOR ***
// *******************

sprmo::Editing_panel::Editing_panel(std::shared_ptr<Project> p,
        const Panel_dims& dims, const Sprite_dims& sprdims) :
    m_show_grid(true),
    m_current_cell(-1, -1),
    m_state(Stopped),
    m_project_ptr(p),
    m_is_drawing(false),
    m_sprite_changed(false),
    m_edit_mode(Pixels),
    m_paint_mode(Draw),
    m_direction(Left),
    m_dims_ref(dims),
    m_sprdims_ref(sprdims)
{
    init_view(dims);
    m_zoomable = std::unique_ptr<Zoomable>(new Zoomable(m_view, Initial_zoom));

} // sprmo::Editing_panel::Editing_panel

// ******************
// *** DESTRUCTOR ***
// ******************

sprmo::Editing_panel::~Editing_panel()
{
    // NYI...

} // sprmo::Editing_panel::~Editing_panel




// ************************
// *** MEMBER FUNCTIONS ***
// ************************

// **************************************************

void sprmo::Editing_panel::draw(sf::RenderTarget& target, 
        sf::RenderStates states) const
{
    target.setView(m_view);

    // We draw the 'transparency' bg texture, then the pixels, then 
    // (optionally) the grid, in that order.
    /*
    target.draw(m_current_sprite_ptr->get_bg_quad(),
            m_project_ptr->get_mask_texture());
    */
    target.draw(m_current_sprite_ptr->get_quads_pixels());
    if (m_show_grid)
    {
        target.draw(m_grid_lines);
    } // if

    target.setView(target.getDefaultView());

} // sprmo::Editing_panel::draw

// **************************************************

void sprmo::Editing_panel::init_lines()
{
    //int csz = Sprite::Cell_size;
    float cw = m_current_sprite_ptr->get_cell_width();
    //float cw = m_sprdims_ref.Cell_size;
    // TODO: select line color based on current palette.
    sf::Color color(80,80,80);

    m_grid_lines.clear();
    m_grid_lines_mc.clear();
    m_grid_lines.setPrimitiveType(sf::Lines);
    m_grid_lines_mc.setPrimitiveType(sf::Lines);

    size_t spr_w = m_current_sprite_ptr->get_width();
    size_t spr_h = m_current_sprite_ptr->get_height();

    // Vertical lines.
    for (size_t i = 0; i <= spr_w; ++i)
    {
        m_grid_lines.append(sf::Vertex(sf::Vector2f(i*cw, 0), color));
        m_grid_lines.append(sf::Vertex(sf::Vector2f(i*cw,
                        //spr_h*Sprite::Cell_height), color));
                        spr_h*m_sprdims_ref.Cell_size), color));
        // Skip every other 'i' when drawing vertical lines for multicolor
        // sprites.
        /*
        if ( !(i & 1) )
        {
            m_grid_lines_mc.append(sf::Vertex(sf::Vector2f(i*csz, 0), color));
            m_grid_lines_mc.append(sf::Vertex(sf::Vector2f(i*csz, spr_h*csz), color));
        } // if
        */
    } // for
    // Horizontal lines.
    for (size_t i = 0; i <= spr_h; ++i)
    {
        m_grid_lines.append(sf::Vertex(sf::Vector2f(
                        0, i*m_sprdims_ref.Cell_size), color));
        m_grid_lines.append(sf::Vertex(sf::Vector2f(
                        spr_w*cw, i*m_sprdims_ref.Cell_size), color));
        //m_grid_lines_mc.append(sf::Vertex(sf::Vector2f(0, i*csz), color));
        //m_grid_lines_mc.append(sf::Vertex(sf::Vector2f(spr_w*csz, i*csz), color));
    } // for

} // sprmo::Editing_panel::init_lines

// **************************************************

void sprmo::Editing_panel::set_current_sprite(std::shared_ptr<Sprite> s)
{
    m_current_sprite_ptr = s;

} // sprmo::Editing_panel::set_current_sprite

// **************************************************

void sprmo::Editing_panel::init_view(const Panel_dims& dims)
{
    m_view.setCenter(dims.center);
    m_view.setSize(dims.size);
    m_view.setViewport(dims.viewport);
    
} // sprmo::Editing_panel::init_view

// **************************************************

bool sprmo::Editing_panel::handle_mouse_button_pressed(
        sf::Event::MouseButtonEvent& e)
{
    bool in_bounds = m_current_cell.x >= 0;
    if (in_bounds)
    {
        // Set the 'paint' mode depending on left or right mouse button.
        // If another button, operation will be cancelled.
        if (e.button == sf::Mouse::Left)
        {
            m_paint_mode = Draw;
        }
        else if (e.button == sf::Mouse::Right)
        {
            m_paint_mode = Erase;
        }
        else
        {
            return false;
        } // if ... else

        if (sf::Keyboard::isKeyPressed(sf::Keyboard::B))
        {
            // Only available in 'pixels' editing mode.
            if (m_edit_mode == Pixels)
            {
                m_project_ptr->bucket_fill(m_current_cell.x, m_current_cell.y,
                        (m_paint_mode == Erase));
            } // if
        }
        else
        {
            m_is_drawing = true;
            paint_pixel();
        } // if ... else
    } // if
    return in_bounds;
        
} // sprmo::Editing_panel::handle_mouse_button_pressed

// **************************************************

void sprmo::Editing_panel::toggle_grid()
{
    m_show_grid = !m_show_grid;

} // sprmo::Editing_panel::toggle_grid

// **************************************************

void sprmo::Editing_panel::handle_mouse_moved(
        sf::Event::MouseMoveEvent& e, sf::RenderTarget& target)
{
    bool ok = false;
    sf::Vector2i old_cell(m_current_cell);

    if (e.x >= m_dims_ref.position.x
            && e.x < m_dims_ref.position.x+m_dims_ref.size.x
            && e.y >= m_dims_ref.position.y
            && e.y < m_dims_ref.position.y+m_dims_ref.size.y)
    {
        sf::Vector2f coords = target.mapPixelToCoords(
                sf::Vector2i(e.x, e.y), m_view);
        if (coords.x >= 0 && coords.y >= 0)
        {
            m_current_cell.x = static_cast<int>(
                    coords.x / m_current_sprite_ptr->get_cell_width());
            m_current_cell.y = static_cast<int>(
                    coords.y / m_sprdims_ref.Cell_size);
            int sw = static_cast<int>(m_current_sprite_ptr->get_width());
            int sh = static_cast<int>(m_current_sprite_ptr->get_height());
            if (m_current_cell.x < sw && m_current_cell.y < sh)
            {
                ok = true;
            } // if
        } // if
    } // if

    if (!ok)
    {
        m_current_cell.x = -1;
        m_current_cell.y = -1;
        m_is_drawing = false;
    } // if ... else

    // Emit signal if cell has changed.  This will be picked up by 
    // Editing_status_panel that will update its display accordingly.
    if (m_current_cell != old_cell)
    {
        m_signal_cell_changed.emit(m_current_cell);
        if (m_is_drawing)
        {
            paint_pixel();
        } // if
    } // if

} // sprmo::Editing_panel::handle_mouse_moved

// **************************************************

sigc::signal<void, const sf::Vector2i&>& 
sprmo::Editing_panel::signal_cell_changed()
{
    return m_signal_cell_changed;

} // sprmo::Editing_panel::signal_cell_changed

// **************************************************

void sprmo::Editing_panel::handle_color_changed(
        size_t c)
{
    m_current_color2 = c;

} // sprmo::Editing_panel::handle_color_changed

// **************************************************

void sprmo::Editing_panel::update()
{
    m_zoomable->update();

    switch (m_state)
    {
        case Stopped:
            break;
        case Moving:
            update_move();
            break;
    } // switch

} // sprmo::Editing_panel::update

// **************************************************

void sprmo::Editing_panel::handle_new_current_sprite(bool is_multicolor,
        Palette::C64_color i)
{
    set_current_sprite(m_project_ptr->get_current_sprite());
    init_lines();

    m_view.setSize(m_dims_ref.size);
    m_view.setCenter(m_dims_ref.center);
    m_view.move(
            //-((m_dims_ref.size.x / 2) - (Sprite::Img_width / 2)),
            //-((m_dims_ref.size.y / 2) - (Sprite::Img_height / 2)));
            -((m_dims_ref.size.x / 2) - (m_sprdims_ref.Img_width / 2.0)),
            -((m_dims_ref.size.y / 2) - (m_sprdims_ref.Img_height / 2.0)));

    // Drop user into pixel-editing mode (default).
    m_edit_mode = Pixels;

    // NOTE: if previous sprite was hi-res, and this one's multicolor, the
    // cell may be invalid (and vice versa).  Easiest way to avoid problems is
    // simply to invalidate the current cell.
    m_current_cell.x = (-1);
    m_current_cell.y = (-1);
    m_signal_cell_changed.emit(m_current_cell);

} // sprmo::Editing_panel::handle_new_current_sprite

// **************************************************

void sprmo::Editing_panel::handle_mouse_button_released(
        sf::Event::MouseButtonEvent& e)
{
    m_is_drawing = false;
    if (m_sprite_changed)
    {
        m_signal_sprite_modified.emit();
    } // if
    m_sprite_changed = false;

} // sprmo::Editing_panel::handle_mouse_button_released

// **************************************************

void sprmo::Editing_panel::paint_pixel()
{
    m_sprite_changed = m_project_ptr->set_sprite_pixel(m_current_cell.x,
            m_current_cell.y, (m_paint_mode == Erase));

} // sprmo::Editing_panel::paint_pixel

// **************************************************

sigc::signal<void>& sprmo::Editing_panel::signal_sprite_modified()
{
    return m_signal_sprite_modified;

} // sprmo::Editing_panel::signal_sprite_modified

// **************************************************

// Toggle only if masks enabled for current sprite - if not, mode will always
// be 'Pixels'.
/*
void sprmo::Editing_panel::toggle_mode()
{
    if (m_current_sprite_ptr->is_mask_enabled())
    {
        m_edit_mode = (m_edit_mode == Pixels) ? Mask : Pixels;
    }
    else
    {
        m_edit_mode = Pixels;
    } // if ... else
    std::cout << "mode=" << m_edit_mode << "\n";
    m_signal_mode_toggled.emit(m_edit_mode);

} // sprmo::Editing_panel::toggle_mode
*/

// **************************************************

void sprmo::Editing_panel::move_left()
{
    m_state = Moving;
    m_direction = Left;

} // sprmo::Editing_panel::move_left

// **************************************************

void sprmo::Editing_panel::move_right()
{
    m_state = Moving;
    m_direction = Right;

} // sprmo::Editing_panel::move_right

// **************************************************

void sprmo::Editing_panel::move_up()
{
    m_state = Moving;
    m_direction = Up;

} // sprmo::Editing_panel::move_up

// **************************************************

void sprmo::Editing_panel::move_down()
{
    m_state = Moving;
    m_direction = Down;

} // sprmo::Editing_panel::move_down

// **************************************************

bool sprmo::Editing_panel::is_sprite_wider() const
{
    //return Sprite::Img_width > m_view.getSize().x;
    return m_sprdims_ref.Img_width > m_view.getSize().x;

} // sprmo::Editing_panel::is_sprite_wider

// **************************************************

bool sprmo::Editing_panel::is_sprite_taller() const
{
    //return Sprite::Img_height > m_view.getSize().y;
    return m_sprdims_ref.Img_height > m_view.getSize().y;

} // sprmo::Editing_panel::is_sprite_taller

// **************************************************

void sprmo::Editing_panel::update_move()
{
    switch (m_direction)
    {
        case Left:
            m_view.move( (-Move_v), 0.0 );
            break;
        case Right:
            m_view.move( Move_v, 0.0 );
            break;
        case Up:
            m_view.move( 0.0, (-Move_v) );
            break;
        case Down:
            m_view.move( 0.0, Move_v );
            break;
    } // switch

} // sprmo::Editing_panel:update_move

// **************************************************

void sprmo::Editing_panel::move_left_stop()
{
    if (m_state == Moving && m_direction == Left)
    {
        m_state = Stopped;
    } // if

} // sprmo::Editing_panel::move_left_stop

// **************************************************

void sprmo::Editing_panel::move_right_stop()
{
    if (m_state == Moving && m_direction == Right)
    {
        m_state = Stopped;
    } // if

} // sprmo::Editing_panel::move_right_stop

// **************************************************

void sprmo::Editing_panel::move_up_stop()
{
    if (m_state == Moving && m_direction == Up)
    {
        m_state = Stopped;
    } // if

} // sprmo::Editing_panel::move_up_stop

// **************************************************

void sprmo::Editing_panel::move_down_stop()
{
    if (m_state == Moving && m_direction == Down)
    {
        m_state = Stopped;
    } // if

} // sprmo::Editing_panel::move_down_stop

// **************************************************

void sprmo::Editing_panel::center_sprite()
{
    //m_view.setCenter(Sprite::Img_width / 2.0, Sprite::Img_height / 2.0);
    m_view.setCenter(m_sprdims_ref.Img_width / 2.0, m_sprdims_ref.Img_height / 2.0);

} // sprmo::Editing_panel::center_sprite

// **************************************************

size_t sprmo::Editing_panel::get_paint_color2() const
{
    if (m_paint_mode == Draw)
    {
        return m_current_color2;
    }
    else
    {
        return 0;
    } // if ... else

} // sprmo::Editing_panel::get_paint_color2

// **************************************************

// sprmo::Editing_panel::
// {
// } // sprmo::Editing_panel::

// **************************************************

// sprmo::Editing_panel::
// {
// } // sprmo::Editing_panel::

// **************************************************

// sprmo::Editing_panel::
// {
// } // sprmo::Editing_panel::

// **************************************************

// sprmo::Editing_panel::
// {
// } // sprmo::Editing_panel::

