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




#include "Project.h"




// *******************
// *** CONSTRUCTOR ***
// *******************

sprmo::Project::Project(Palette& pal, const Palette_panel::Summary& summary,
        const Sprite_dims& sprdims) :
    m_unsaved_changes(false),
    m_clipboard_ptr(std::shared_ptr<Clipboard>(new Clipboard(1,1))),
    m_palette_ref(pal),
    m_summary_ref(summary),
    m_sprdims_ref(sprdims)
{
    m_mask_texture.loadFromFile(PKG_DATA_DIR "/images/bg.png");
    //std::cout << "Creating new empty project...\n";
    /*
    add_new_sprite(m_resolution_ref.get_default_width_pixels(),
            m_resolution_ref.get_default_height_pixels());
    */
    add_new_sprite2(false);

} // sprmo::Project::Project

// ******************
// *** DESTRUCTOR ***
// ******************

sprmo::Project::~Project()
{
    // NYI...

} // sprmo::Project::~Project




// ************************
// *** MEMBER FUNCTIONS ***
// ************************

// **************************************************

std::shared_ptr<sprmo::Sprite> sprmo::Project::get_current_sprite()
{
    return m_sprites[m_current_sprite];

} // sprmo::Project::get_current_sprite

// **************************************************

/*
void sprmo::Project::add_new_sprite(int w, int h, bool insert)
{
    sf::Color c = m_resolution_ref.get_clear_color(0);
    std::shared_ptr<Sprite> s(new Sprite(w, h, c));

    if (insert)
    {
        m_sprites.insert(m_sprites.begin() + m_current_sprite, s);
    }
    else
    {
        m_sprites.push_back(s);
        m_current_sprite = m_sprites.size() - 1;
    } // if ... else

    // If the sprite has been inserted, supply its index (as an int); if added
    // to the end, -1.
    m_signal_new_sprite_added.emit(
            insert ? static_cast<int>(m_current_sprite) : (-1));
    m_unsaved_changes = true;
    handle_new_current_sprite();

} // sprmo::Project::add_new_sprite
*/

// **************************************************

void sprmo::Project::next_sprite()
{
    if (m_current_sprite < (m_sprites.size() - 1))
    {
        ++m_current_sprite;
        handle_new_current_sprite();
    } // if

} // sprmo::Project::next_sprite

// **************************************************

void sprmo::Project::previous_sprite()
{
    if (m_current_sprite)
    {
        --m_current_sprite;
        handle_new_current_sprite();
    } // if

} // sprmo::Project::previous_sprite

// **************************************************

std::shared_ptr<sprmo::Sprite> sprmo::Project::get_sprite(size_t i)
{
    assert(i < m_sprites.size());
    return m_sprites[i];

} // sprmo::Project::get_sprite

// **************************************************

void sprmo::Project::set_current_sprite(size_t i)
{
    m_current_sprite = i;
    handle_new_current_sprite();

} // sprmo::Project::set_current_sprite

// **************************************************

sigc::signal<void,bool,sprmo::Palette::C64_color>&
sprmo::Project::signal_new_current_sprite()
{
    return m_signal_new_current_sprite;

} // sprmo::Project::signal_new_current_sprite

// **************************************************

sigc::signal<void,int>& sprmo::Project::signal_new_sprite_added()
{
    return m_signal_new_sprite_added;

} // sprmo::Project::signal_new_sprite_added

// **************************************************

// NOTE: any modifications to the sprite, causing us to push the old pixels
// onto the undo stack, invalidates the redo stack (and which is therefore
// cleared).
void sprmo::Project::handle_sprite_modified()
{
    m_undo_stack.push_undo(*m_current_sprite_pixels_copy);
    m_undo_stack.clear_redo();
    copy_current_sprite_pixels();
    m_unsaved_changes = true;

} // sprmo::Project::handle_sprite_modified

// **************************************************

void sprmo::Project::copy_current_sprite_pixels()
{
    m_current_sprite_pixels_copy = 
            m_sprites[m_current_sprite]->get_pixel_data_copy();
    
} // sprmo::Project::copy_current_sprite_pixels

// **************************************************

void sprmo::Project::undo()
{
    // Check if operation valid first - the undo stack must have a size of at
    // least one.
    if (!m_undo_stack.empty_undo())
    {
        m_undo_stack.push_redo(*m_current_sprite_pixels_copy);
        const std::vector<int>& v = m_undo_stack.back_undo(); 
        m_sprites[m_current_sprite]->set_all_pixels(v);
        m_undo_stack.pop_undo();
        copy_current_sprite_pixels();
        m_sprites[m_current_sprite]->refresh_quads(m_summary_ref);
        // NOTE: don't call handle_sprite_modified because that messes with
        // the stacks.
        m_unsaved_changes = true;
    } // if

} // sprmo::Project::undo

// **************************************************

void sprmo::Project::redo()
{
    if (!m_undo_stack.empty_redo())
    {
        m_undo_stack.push_undo(*m_current_sprite_pixels_copy);
        const std::vector<int>& v = m_undo_stack.back_redo();
        m_sprites[m_current_sprite]->set_all_pixels(v);
        m_undo_stack.pop_redo();
        copy_current_sprite_pixels();
        m_sprites[m_current_sprite]->refresh_quads(m_summary_ref);
    } // if

} // sprmo::Project::redo

// **************************************************

void sprmo::Project::to_console(const std::vector<int>& v) const
{
    for (size_t r = 0; r < 16; ++r)
    {
        for (size_t c = 0; c < 16; ++c)
        {
            std::cout << v[r*16+c] << "  ";
        } // for
        std::cout << "\n";
    } // for

} // sprmo::Project::to_console

// **************************************************

void sprmo::Project::copy()
{
    int w = m_sprites[m_current_sprite]->get_width();
    int h = m_sprites[m_current_sprite]->get_height();
    m_clipboard_ptr = std::shared_ptr<Clipboard>(new Clipboard(w, h));
    std::copy(
            m_current_sprite_pixels_copy->begin(),
            m_current_sprite_pixels_copy->end(), 
            m_clipboard_ptr->data.begin());


} // sprmo::Project::copy

// **************************************************

// FIXME: make sure destination is same size as source!!!
void sprmo::Project::paste()
{
    if (m_sprites[m_current_sprite]->get_width() == m_clipboard_ptr->width
            && m_sprites[m_current_sprite]->get_height() == m_clipboard_ptr->height)
    {
        m_sprites[m_current_sprite]->set_all_pixels(m_clipboard_ptr->data);
        handle_sprite_modified();
    }
    else
    {
        std::cerr << "paste not allowed - different dimensions\n";
    } // if ... else

} // sprmo::Project::paste

// **************************************************

void sprmo::Project::handle_add_new_sprite()
{
    /*
    add_new_sprite(m_resolution_ref.get_default_width_pixels(),
            m_resolution_ref.get_default_height_pixels());
    */
    add_new_sprite2(false);

} // sprmo::Project::handle_add_new_sprite

// **************************************************

void sprmo::Project::remove()
{
    // NOTE: cannot remove current sprite if it's the only one.
    if (m_sprites.size() > 1)
    {
        size_t removed = m_current_sprite;
        auto i = m_sprites.begin() + m_current_sprite;
        m_sprites.erase(i);
        if (m_current_sprite == m_sprites.size())
        {
            --m_current_sprite;
        } // if
        m_signal_sprite_removed.emit(removed);
        handle_new_current_sprite();
    } // if

} // sprmo::Project::remove

// **************************************************

void sprmo::Project::handle_new_current_sprite()
{
    m_signal_new_current_sprite.emit( is_current_sprite_multicolor(),
            m_sprites[m_current_sprite]->get_color2());
    m_undo_stack.clear_stacks();
    copy_current_sprite_pixels();


} // sprmo::Project::handle_new_current_sprite

// **************************************************

/*
void sprmo::Project::toggle_mask_enabled()
{
    m_sprites[m_current_sprite]->toggle_mask_enabled(
            m_resolution_ref.get_palette(0));
    m_signal_mask_enabled_toggled.emit(
            m_sprites[m_current_sprite]->is_mask_enabled());
    // FIXME: temporary code.  For the time being, toggling of masks isn't
    // handled by the undo/redo stacks.  We just clear them when it happens.
    std::cerr << "This needs fixing!  Undo/redo stacks cleared!!!\n";
    m_undo_stack.clear_stacks();
    copy_current_sprite_pixels();

} // sprmo::Project::toggle_mask_enabled
*/

// **************************************************

/*
void sprmo::Project::toggle_must_preshift()
{
    m_sprites[m_current_sprite]->toggle_must_preshift();
    m_signal_must_preshift_toggled.emit(
            m_sprites[m_current_sprite]->must_preshift());

} // sprmo::Project::toggle_must_preshift
*/

// **************************************************

/*
void sprmo::Project::auto_mask()
{
    m_sprites[m_current_sprite]->apply_auto_mask();
    handle_sprite_modified();

} // sprmo::Project::auto_mask
*/

// **************************************************

void sprmo::Project::flip_vertical()
{
    m_sprites[m_current_sprite]->flip_vertical(m_summary_ref);
    handle_sprite_modified();

} // sprmo::Project::flip_vertical

// **************************************************

void sprmo::Project::flip_horizontal()
{
    m_sprites[m_current_sprite]->flip_horizontal(m_summary_ref);
    handle_sprite_modified();

} // sprmo::Project::flip_horizontal

// **************************************************

void sprmo::Project::rotate_pixels_left()
{
    m_sprites[m_current_sprite]->rotate_pixels_left(m_summary_ref);
    handle_sprite_modified();

} // sprmo::Project::rotate_pixels_left

// **************************************************

void sprmo::Project::rotate_pixels_right()
{
    m_sprites[m_current_sprite]->rotate_pixels_right(m_summary_ref);
    handle_sprite_modified();

} // sprmo::Project::rotate_pixels_right

// **************************************************

void sprmo::Project::rotate_pixels_up()
{
    m_sprites[m_current_sprite]->rotate_pixels_up(m_summary_ref);
    handle_sprite_modified();

} // sprmo::Project::rotate_pixels_up

// **************************************************

void sprmo::Project::rotate_pixels_down()
{
    m_sprites[m_current_sprite]->rotate_pixels_down(m_summary_ref);
    handle_sprite_modified();

} // sprmo::Project::rotate_pixels_down

// **************************************************

void sprmo::Project::delete_pixels()
{
    m_sprites[m_current_sprite]->delete_pixels(m_summary_ref);
    handle_sprite_modified();

} // sprmo::Project::delete_pixels()

// **************************************************

/*
void sprmo::Project::add_new_sprite(int w, int h, bool is_mask, bool preshift,
        const std::vector<int>& data)
{
    //std::cout << "add new sprite (loaded!)\n";
    sf::Color c = m_resolution_ref.get_clear_color(0);
    std::shared_ptr<Sprite> spr(new Sprite(w, h, c));
    //spr->set_mask_enabled(is_mask);
    //spr->set_must_preshift(preshift);
    spr->set_all_pixels(data, m_resolution_ref.get_palette(0));
    m_sprites.push_back(spr);

} // sprmo::Project::add_new_sprite
*/

// **************************************************

void sprmo::Project::clear_all_sprites()
{
    m_sprites.clear();

} // sprmo::Project::clear_all_sprites

// **************************************************

void sprmo::Project::handle_new_file_loaded()
{
    m_unsaved_changes = false;
    m_current_sprite = 0;
    handle_new_current_sprite();

} // sprmo::Project::handle_new_file_loaded

// **************************************************

void sprmo::Project::calc_source_code_bytes()
{
    for (auto s : m_sprites)
    {
        //s->calc_source_code_bytes(m_resolution_ref.get_pixels_per_byte());
    } // for

} // sprmo::Project::calc_source_code_bytes

// **************************************************

/*
void sprmo::Project::handle_palette_changed(const Resolution::Palette& p)
{
    for (auto s : m_sprites)
    {
        s->refresh_quads2(m_resolution_ref);
    } // for

} // sprmo::Project::handle_palette_changed
*/

// **************************************************

void sprmo::Project::move_sprite_left()
{
    if (m_current_sprite > 0)
    {
        std::shared_ptr<Sprite> s = m_sprites[m_current_sprite];
        m_sprites[m_current_sprite] = m_sprites[m_current_sprite - 1];
        m_sprites[m_current_sprite - 1] = s;
        --m_current_sprite;
        m_signal_sprite_moved.emit(m_current_sprite + 1);
        m_unsaved_changes = true;
    } // if

} // sprmo::Project::move_sprite_left

// **************************************************

void sprmo::Project::move_sprite_right()
{
    if (m_current_sprite < (m_sprites.size() - 1))
    {
        std::shared_ptr<Sprite> s = m_sprites[m_current_sprite];
        m_sprites[m_current_sprite] = m_sprites[m_current_sprite + 1];
        m_sprites[m_current_sprite + 1] = s;
        ++m_current_sprite;
        m_signal_sprite_moved.emit(m_current_sprite - 1);
        m_unsaved_changes = true;
    } // if

} // sprmo::Project::move_sprite_right

// **************************************************

sigc::signal<void,size_t>& sprmo::Project::signal_sprite_moved()
{
    return m_signal_sprite_moved;

} // sprmo::Project::signal_sprite_moved

// **************************************************

sigc::signal<void,size_t>& sprmo::Project::signal_sprite_removed()
{
    return m_signal_sprite_removed;

} // sprmo::Project::signal_sprite_removed

// **************************************************

void sprmo::Project::first_sprite()
{
    if (m_current_sprite)
    {
        m_current_sprite = 0;
        handle_new_current_sprite();
    } // if

} // sprmo::Project::first_sprite

// **************************************************

void sprmo::Project::last_sprite()
{
    if (m_current_sprite < (m_sprites.size() - 1))
    {
        m_current_sprite = m_sprites.size() - 1;
        handle_new_current_sprite();
    } // if

} // sprmo::Project::last_sprite

// **************************************************

void sprmo::Project::bucket_fill(size_t col, size_t row, bool erase)
{
    m_sprites[m_current_sprite]->bucket_fill(col, row, erase, m_summary_ref);
    handle_sprite_modified();

} // sprmo::Project::bucket_fill

// **************************************************

/*
const sprmo::Resolution::Palette_color&
sprmo::Project::get_clear_color() const
{
    // FIXME: making assumption about palette.
    return m_resolution_ref.get_palette_color(0);

} // sprmo::Project::get_clear_color
*/

// **************************************************

void sprmo::Project::add_new_sprite2(bool multicolor, bool insert)
{
    std::shared_ptr<Sprite> s(
            new Sprite(multicolor, m_summary_ref, m_sprdims_ref));

    if (insert)
    {
        m_sprites.insert(m_sprites.begin() + m_current_sprite, s);
    }
    else
    {
        m_sprites.push_back(s);
        m_current_sprite = m_sprites.size() - 1;
    } // if ... else

    // If the sprite has been inserted, supply its index (as an int); if added
    // to the end, -1.
    m_signal_new_sprite_added.emit(
            insert ? static_cast<int>(m_current_sprite) : (-1));
    m_unsaved_changes = true;
    handle_new_current_sprite();
    
} // sprmo::Project::add_new_sprite2

// **************************************************

bool sprmo::Project::is_current_sprite_multicolor() const
{
    return m_sprites[m_current_sprite]->is_multicolor();

} // sprmo::Project::is_current_sprite_multicolor

// **************************************************

void sprmo::Project::handle_color_cycled(size_t i)
{
    // NOTE: if i=1, that's the sprite's individual color, so only current
    // sprite needs to be refreshed.
    if (i == 1)
    {
        m_sprites[m_current_sprite]->refresh_quads(m_summary_ref);
        Palette::C64_color color = m_summary_ref.colors[1].second;
        m_sprites[m_current_sprite]->set_color(color);
    }
    else
    {
        for (auto s : m_sprites)
        {
            s->refresh_quads(m_summary_ref, false);
        } // for
    } // if ... else

} // sprmo::Project::handle_color_cycled

// **************************************************

void sprmo::Project::add_new_sprite2(bool multicolor, int color,
        const std::vector<int>& data)
{
    Palette::C64_color c = static_cast<Palette::C64_color>(color);
    // Will this have an effect in time?
    m_signal_loading_sprite.emit(c);

    std::shared_ptr<Sprite> s(
            new Sprite(multicolor, m_summary_ref, c, data, m_sprdims_ref) );
    m_sprites.push_back(s);
    m_unsaved_changes = true;
 
} // sprmo::Project::add_new_sprite2

// **************************************************

std::vector<int> sprmo::Project::get_palette() const
{
    std::vector<int> v(3);
    v[0] = m_palette_ref.get_color_index(Palette::Background);
    v[1] = m_palette_ref.get_color_index(Palette::SPMC0);
    v[2] = m_palette_ref.get_color_index(Palette::SPMC1);
    return v;

} // sprmo::Project::get_palette

// **************************************************

bool sprmo::Project::set_sprite_pixel(size_t col, size_t row, bool erase)
{
    return m_sprites[m_current_sprite]->set_pixel(col, row, erase, m_summary_ref);

} // sprmo::Project::set_sprite_pixel

// **************************************************

sigc::signal<void,sprmo::Palette::C64_color>&
sprmo::Project::signal_loading_sprite()
{
    return m_signal_loading_sprite;

} // sprmo::Project::signal_loading_sprite

// **************************************************

std::shared_ptr<std::vector<int>>
sprmo::Project::write_prg_data(int dest_addr) const
{
    // Calculate how big the vector should be.
    // Address (2 bytes) + num-sprites * 64.
    auto mydata_ptr = std::shared_ptr<std::vector<int>>(
            new std::vector<int>(2 + m_sprites.size() * Sprite::Bytes_per_sprite)); 

    (*mydata_ptr)[0] = dest_addr % 256;
    (*mydata_ptr)[1] = dest_addr / 256;
    
    for (size_t i = 0; i < m_sprites.size(); ++i)
    {
        auto spr_data_ptr = m_sprites[i]->get_C64_data();
        // Copy data at the correct offset.
        int offset = 2 + (i * Sprite::Bytes_per_sprite);
        std::copy(spr_data_ptr->begin(), spr_data_ptr->end(),
                mydata_ptr->begin() + offset);
    } // for

    /*
    for (auto x : *mydata_ptr)
    {
        std::cout << x << ",";
    } // for
    std::cout << "\n";
    */
    return mydata_ptr;

} // sprmo::Project::write_prg_data

// **************************************************

// sprmo::Project::
// {
// } // sprmo::Project::

// **************************************************

// sprmo::Project::
// {
// } // sprmo::Project::

// **************************************************

// sprmo::Project::
// {
// } // sprmo::Project::

// **************************************************

// sprmo::Project::
// {
// } // sprmo::Project::

// **************************************************

// sprmo::Project::
// {
// } // sprmo::Project::

// **************************************************

// sprmo::Project::
// {
// } // sprmo::Project::

// **************************************************

// sprmo::Project::
// {
// } // sprmo::Project::

// **************************************************

// sprmo::Project::
// {
// } // sprmo::Project::

// **************************************************

// sprmo::Project::
// {
// } // sprmo::Project::

// **************************************************

// sprmo::Project::
// {
// } // sprmo::Project::

// **************************************************

// sprmo::Project::
// {
// } // sprmo::Project::

// **************************************************

// sprmo::Project::
// {
// } // sprmo::Project::

// **************************************************

// sprmo::Project::
// {
// } // sprmo::Project::

// **************************************************

// sprmo::Project::
// {
// } // sprmo::Project::

// **************************************************

// sprmo::Project::
// {
// } // sprmo::Project::

// **************************************************

// sprmo::Project::
// {
// } // sprmo::Project::

// **************************************************

// sprmo::Project::
// {
// } // sprmo::Project::

// **************************************************

// sprmo::Project::
// {
// } // sprmo::Project::

// **************************************************

// sprmo::Project::
// {
// } // sprmo::Project::

