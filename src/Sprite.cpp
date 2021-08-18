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




#include "Sprite.h"




// FIXME: a bit messy.  
//const float cs = sprmo::Sprite::Cell_size;
//const float cs = 8;
// NOTE: need to skip over every other row for multicolor sprites...
const float sprmo::Sprite::Quads_base_x_coords[] = {
        0,      8,      8,      0, 
        8,      2*8,    2*8,    8,
        2*8,    3*8,    3*8,    2*8,
        3*8,    4*8,    4*8,    3*8,
        4*8,    5*8,    5*8,    4*8,
        5*8,    6*8,    6*8,    5*8,
        6*8,    7*8,    7*8,    6*8,
        7*8,    8*8,    8*8,    7*8,
        8*8,    9*8,    9*8,    8*8,
        9*8,    10*8,   10*8,   9*8,
        10*8,   11*8,   11*8,   10*8,
        11*8,   12*8,   12*8,   11*8,
        12*8,   13*8,   13*8,   12*8,
        13*8,   14*8,   14*8,   13*8,
        14*8,   15*8,   15*8,   14*8,
        15*8,   16*8,   16*8,   15*8, 
        16*8,   17*8,   17*8,   16*8, 
        17*8,   18*8,   18*8,   17*8, 
        18*8,   19*8,   19*8,   18*8, 
        19*8,   20*8,   20*8,   19*8, 
        20*8,   21*8,   21*8,   20*8, 
        21*8,   22*8,   22*8,   21*8, 
        22*8,   23*8,   23*8,   22*8, 
        23*8,   24*8,   24*8,   23*8 
};

const float sprmo::Sprite::Quads_base_x_coords_mc[] = {
        0,      16,      16,      0, 
        16,     2*16,    2*16,    16,
        2*16,   3*16,    3*16,    2*16,
        3*16,   4*16,    4*16,    3*16,
        4*16,   5*16,    5*16,    4*16,
        5*16,   6*16,    6*16,    5*16,
        6*16,   7*16,    7*16,    6*16,
        7*16,   8*16,    8*16,    7*16,
        8*16,   9*16,    9*16,    8*16,
        9*16,   10*16,   10*16,   9*16,
        10*16,  11*16,   11*16,   10*16,
        11*16,  12*16,   12*16,   11*16
};

// NOTE: initially transparent as it will be for newly-created sprite.
//const sf::Color sprmo::Sprite::Mask_color = { 255, 255, 255, 50 };


// *******************
// *** CONSTRUCTOR ***
// *******************
sprmo::Sprite::Sprite(bool multicolor, const Palette_panel::Summary& s,
        const Sprite_dims& sprdims) :
    m_color(Palette::Default_spr_color),
    m_multicolor(multicolor),
    m_sprdims_ref(sprdims)
{
    if (m_multicolor)
    {
        m_width = 12;
        //m_cell_width = s_cell_size * 2;
        m_cell_width = m_sprdims_ref.Cell_size * 2;
    }
    else
    {
        m_width = 24;
        //m_cell_width = s_cell_size;
        m_cell_width = m_sprdims_ref.Cell_size;
    } // if ... else
    m_height = 21;

    m_pixel_data = std::unique_ptr<std::vector<int>>(
            new std::vector<int>(m_width * m_height, 0));
    //init_base_x_coords();
    init_quads();
    refresh_quads(s);

} // sprmo::Sprite::Sprite

sprmo::Sprite::Sprite(
        bool multicolor,
        const Palette_panel::Summary& s,
        Palette::C64_color color,
        const std::vector<int>& v,
        const Sprite_dims& sprdims) :
    // Initializer list.
    m_color(color),
    m_multicolor(multicolor),
    m_sprdims_ref(sprdims)
{
    if (m_multicolor)
    {
        m_width = 12;
        //m_cell_width = s_cell_size * 2;
        m_cell_width = m_sprdims_ref.Cell_size * 2;
    }
    else
    {
        m_width = 24;
        //m_cell_width = s_cell_size;
        m_cell_width = m_sprdims_ref.Cell_size;
    } // if ... else
    m_height = 21;

    m_pixel_data = std::unique_ptr<std::vector<int>>(
            new std::vector<int>(v));
    //init_base_x_coords();
    init_quads();
    refresh_quads(s);

} // sprmo::Sprite::Sprite


// ******************
// *** DESTRUCTOR ***
// ******************

sprmo::Sprite::~Sprite()
{
    // NYI...

} // sprmo::Sprite::~Sprite




// ************************
// *** MEMBER FUNCTIONS ***
// ************************

// **************************************************

int sprmo::Sprite::get_pixel(size_t col, size_t row) const
{
    assert(col >= 0 && col < m_width);
    assert(row >= 0 && row < m_height);

    int p = (*m_pixel_data)[(row * m_width) + col];
    /*
    if (p == Mask_on)
    {
        p = 0;
    } // if
    */
    return p;

} // sprmo::Sprite::get_pixel

// **************************************************

bool sprmo::Sprite::set_pixel(
        size_t col,
        size_t row,
        size_t color_selection, 
        const Palette& r)
{
    assert(col >= 0 && col < m_width);
    assert(row >= 0 && row < m_height);

    bool pixel_changed = false;

    // FIXME: get rid of hard-coded value.  Should check valid colors for 
    // current sprite format.
    // See if we need to set the pixel or not - it may already be the same 
    // color as the current palette color.

    // If the current sprite is multicolor, double the 'col' value first and
    // record the fact that we need to paint 8 vertices rather than 4 (i.e.
    // two quads).
    //int nv = 4;
    //if (m_multicolor)
    //{
    //    nv *= 2;
    //    col *= 2;
    //} // if
    if ( color_selection != static_cast<size_t>(get_pixel(col, row)) )
    {
        size_t base_i = row*m_width*4+col*4;
        //sf::Color color = r.get_selection(color_selection);
        sf::Color color = r.get_color(color_selection);
        for (size_t i = base_i; i < (base_i + 4); ++i)
        {
            m_cell_quads_pixels[i].color = color;
        } // for
        (*m_pixel_data)[row*m_width+col] = color_selection;
        //if (m_multicolor) { (*m_pixel_data)[row*m_width+col+1] = color_selection; }
        pixel_changed = true;
    } // if

    //display_pixel_array();
    return pixel_changed;

} // sprmo::Sprite::set_pixel

// **************************************************

void sprmo::Sprite::init_quads()
{
    //int ch = s_cell_size;   //Sprite::Cell_height;
    int ch = m_sprdims_ref.Cell_size;
    m_cell_quads_pixels.clear();
    m_cell_quads_pixels.setPrimitiveType(sf::Quads);
    // NOTE: offsets are different for hires and multicolor sprites.
    /*const float* base_x = 
        m_multicolor ? Quads_base_x_coords_mc : Quads_base_x_coords;*/
    /*const float* base_x = 
        m_multicolor ? quads_base_x_coords_mc2 : quads_base_x_coords2;*/
    const float* base_x = m_multicolor ?
        m_sprdims_ref.Quads_base_x_coords_mc :
        m_sprdims_ref.Quads_base_x_coords;
    
    for (size_t r = 0; r < m_height; ++r)
    {
        for (size_t i = 0; i < m_width; ++i)
        {
            sf::Vertex v(sf::Vector2f(base_x[4*i], r*ch));
            m_cell_quads_pixels.append(v);
            v = sf::Vertex(sf::Vector2f(base_x[4*i+1], r*ch));
            m_cell_quads_pixels.append(v);
            v = sf::Vertex(sf::Vector2f(base_x[4*i+2], (r*ch)+ch));
            m_cell_quads_pixels.append(v);
            v = sf::Vertex(sf::Vector2f(base_x[4*i+3], (r*ch)+ch));
            m_cell_quads_pixels.append(v);
        } // for
    } // for

} // sprmo::Sprite::init_quads

// **************************************************

const sf::VertexArray& sprmo::Sprite::get_quads_pixels() const
{
    return m_cell_quads_pixels;

} // sprmo::Sprite::get_quads_pixels

// **************************************************

// FIXME: move semantics?!
std::shared_ptr<std::vector<int>> sprmo::Sprite::get_pixel_data_copy() const
{
    std::shared_ptr<std::vector<int>> pv(new std::vector<int>(
            m_pixel_data->size()));
    std::copy(m_pixel_data->cbegin(), m_pixel_data->cend(), pv->begin());
    return pv;

} // sprmo::Sprite::get_pixel_data_copy

// **************************************************

void sprmo::Sprite::set_all_pixels(const std::vector<int>& v)
{
    assert(v.size() == m_pixel_data->size());

    std::copy(v.cbegin(), v.cend(), m_pixel_data->begin());
        
} // sprmo::Sprite::set_all_pixels

// **************************************************

void sprmo::Sprite::flip_vertical(const Palette_panel::Summary& s)
{
    for (size_t i = 0; i < m_pixel_data->size(); i += m_width)
    {
        std::reverse(m_pixel_data->begin() + i, 
                m_pixel_data->begin() + i + m_width);
    } // for
    refresh_quads(s);

} // sprmo::Sprite::flip_vertical

// **************************************************

void sprmo::Sprite::flip_horizontal(const Palette_panel::Summary& s)
{
    std::vector<int>* p_copy = new std::vector<int>(m_pixel_data->size());
    for (size_t i = 0; i < m_height; ++i)
    {
        std::copy(m_pixel_data->begin() + (i * m_width),
                  m_pixel_data->begin() + (i * m_width) + m_width,
                  p_copy->end() - m_width - (i * m_width));
    } // for

    m_pixel_data.reset(p_copy);
    refresh_quads(s);

} // sprmo::Sprite::flip_horizontal

// **************************************************

void sprmo::Sprite::rotate_pixels_left(const Palette_panel::Summary& s)
{
    for (size_t i = 0; i < m_height; ++i)
    {
        std::rotate(m_pixel_data->begin() + (i * m_width),
                    m_pixel_data->begin() + (i * m_width) + 1,
                    m_pixel_data->begin() + (i * m_width) + m_width);
    } // for
    refresh_quads(s);

} // sprmo::Sprite::rotate_pixels_left

// **************************************************

void sprmo::Sprite::rotate_pixels_right(const Palette_panel::Summary& s)
{
    for (size_t i = 0; i < m_height; ++i)
    {
        std::rotate(m_pixel_data->begin() + (i * m_width),
                    m_pixel_data->begin() + (i * m_width) + m_width - 1,
                    m_pixel_data->begin() + (i * m_width) + m_width);
    } // for
    refresh_quads(s);

} // sprmo::Sprite::rotate_pixels_right

// **************************************************

void sprmo::Sprite::rotate_pixels_up(const Palette_panel::Summary& s)
{
    // Make a copy of the pixel data but skip the first row.
    std::vector<int>* p_copy = new std::vector<int>(m_pixel_data->size());
    std::copy(m_pixel_data->begin() + m_width, m_pixel_data->end(),
            p_copy->begin());
    // Then copy the first row of the old vector into the last row of the
    // new vector.
    std::copy(m_pixel_data->begin(), m_pixel_data->begin() + m_width,
            p_copy->end() - m_width);
    m_pixel_data.reset(p_copy);
    refresh_quads(s);

} // sprmo::Sprite::rotate_pixels_up

// **************************************************

void sprmo::Sprite::rotate_pixels_down(const Palette_panel::Summary& s)
{
    // Create a new vector, same size as pixel data.
    std::vector<int>* p_copy = new std::vector<int>(m_pixel_data->size());

    // Copy the last row of data from the old vector to the beginning of the
    // new vector.
    std::copy(m_pixel_data->end() - m_width, m_pixel_data->end(),
            p_copy->begin());

    // Then the rest of the old data goes after that.
    std::copy(m_pixel_data->begin(), m_pixel_data->end() - m_width,
            p_copy->begin() + m_width);

    m_pixel_data.reset(p_copy);
    refresh_quads(s);

} // sprmo::Sprite::rotate_pixels_down

// **************************************************

void sprmo::Sprite::delete_pixels(const Palette_panel::Summary& s)
{
    for (auto& x : *m_pixel_data) { x = 0; }
    refresh_quads(s);

} // sprmo::Sprite::delete_pixels

// **************************************************

void sprmo::Sprite::calc_source_code_bytes(size_t pixels_per_byte)
{
    m_bytes.clear();

    // NOTE: assume we always need an extra byte if pre-shifting.
    /*
    if (m_must_preshift)
    {
        // Create a copy of the pixel data vector, with an extra 'byte' of
        // data added to the end of each row - this will be a number of ints
        // equal to pixels_per_byte.
        // FIXME: rewrite this routine - it's rubbish.
        std::vector<int> pixels_copy;
        size_t count = 0;
        for (auto x : *m_pixel_data)
        {
            if (count == m_width)
            {
                pixels_copy.insert(pixels_copy.end(), pixels_per_byte,
                        (m_mask_enabled ? Mask_on : 0));
                count = 0;
            } // if
            pixels_copy.push_back(x);
            ++count;
        } // for
        pixels_copy.insert(pixels_copy.end(), pixels_per_byte,
                (m_mask_enabled ? Mask_on : 0));


        // We now have our copy of the pixel data.
        // Record the new width (in pixels) for the code that follows.
        size_t w = m_width + pixels_per_byte;
        for (size_t shift = 0; shift < pixels_per_byte; ++shift)
        {
            if (shift > 0)
            {
                for (size_t row = 0; row < m_height; ++row)
                {
                    std::rotate(
                            pixels_copy.begin() + row * w,
                            pixels_copy.begin() + row * w + w - 1,
                            pixels_copy.begin() + row * w + w);
                } // for
            } // if
            // Calculate the byte values for this data.
            calc_bytes(pixels_copy, w, pixels_per_byte);
        } // for
    }
    */
    //else
    //{
        calc_bytes(*m_pixel_data, m_width, pixels_per_byte);
    //} // if ... else

    std::cout << "how many bytes altogether? " << m_bytes.size() << "\n";

} // sprmo::Sprite::calc_source_code_bytes

// **************************************************

void sprmo::Sprite::calc_bytes(
        const std::vector<int>& pixel_data, 
        size_t width_pixels,
        size_t pixels_per_byte)
{
    size_t bytes_per_row = width_pixels / pixels_per_byte;

    for (size_t row = 0; row < m_height; ++row)
    {
        for (size_t byte = 0; byte < bytes_per_row; ++byte)
        {
            // Different behaviour depending on whether mask is enabled.
            // If it is, we interleave mask and pixel data, making our
            // vector twice the size.
            sf::Uint8 current_pixel_byte = 0;
            //sf::Uint8 current_mask_byte = 0;
            for (size_t pixel = 0; pixel < pixels_per_byte; ++pixel)
            {
                // Get value of current pixel.
                int p = pixel_data[row*width_pixels+byte*pixels_per_byte+pixel];

                // Masked pixels are 0 as far as pixel data is concerned.
                // Process masked pixel if masks are enabled.
                /*
                if (p == Mask_on) {
                    // FIXME: really they MUST be enabled if we've found a 
                    // 'Mask_on' pixel!!!
                    if (m_mask_enabled)
                    {
                        // FIXME: literal '3' will change for other resolutions
                        // - we're assuming two bits per pixel.
                        current_mask_byte += 
                                (3 << (((pixels_per_byte-1) - pixel)*2));
                    } // if
                    // Pixel data needs processing next so set to 0.
                    p = 0;
                } // if
                */
                // We're starting at the most significant bits, so shift 
                // value left according to its position in the byte.
                current_pixel_byte += (p << (((pixels_per_byte-1) - pixel)*2));
            } // for
            // Interleave mask bytes if mask enabled.
            /*
            if (m_mask_enabled) 
            {
                m_bytes.push_back(current_mask_byte);
            } // if
            */
            m_bytes.push_back(current_pixel_byte); 
        } // for
    } // for
    
    /*
    std::cout << "pixel bytes:\n";
    size_t count = 0;
    bytes_per_row *= (m_mask_enabled ? 2 : 1);
    for (auto x : pixel_bytes)
    {
        if (!(count % bytes_per_row))
        {
            std::cout << "\n";
        } // if
        std::cout << static_cast<int>(x) << " ";
        ++count;
    } // for
    */

} // sprmo::Sprite::calc_bytes

// **************************************************

void sprmo::Sprite::bucket_fill(
        size_t col,
        size_t row, 
        bool erase,
        const Palette_panel::Summary& s)
{
    auto source_color = (*m_pixel_data)[col+row*m_width];
    int target = erase ? 0 : s.type;
    
    if ( source_color == target )
    {
        std::cout << "Nothing to fill!\n";
        return;
    } // if

    go_fill(col, row, source_color, target);
    refresh_quads(s);

} // sprmo::Sprite::bucket_fill

// **************************************************

void sprmo::Sprite::go_fill(
        size_t col, 
        size_t row, 
        int original,
        int target)
{
    //std::cout << "original=" << original << ",target=" << target << "\n";
    // Paint this pixel first.
    (*m_pixel_data)[col+row*m_width] = target;
    //if (m_multicolor) { (*m_pixel_data)[col+1+row*m_width] = target; }

    // From our starting point (the cell where the user clicked), check in turn
    // cells to the north, east, south and west.  Each time a cell with the 
    // 'original' color is found (the one we want to paint with 'target'), make
    // a recursive call to the function with those coordinates.  This way we
    // traverse through all the cells connected to the starting point that need
    // to be painted.

    // North.
    if ( row > 0 && (*m_pixel_data)[col+(row-1)*m_width] == original )
    {
        go_fill(col, row - 1, original, target);
    } // if
    // East.
    if ( col < (m_width - 1)
            && (*m_pixel_data)[col+1+row*m_width] == original )
    {
        go_fill(col + 1, row, original, target);
    } // if
    // South.
    if ( row < (m_height - 1)
            && (*m_pixel_data)[col+(row+1)*m_width] == original )
    {
        go_fill(col, row + 1, original, target);
    } // if
    // West.
    if ( col > 0 && (*m_pixel_data)[col-1+row*m_width] == original )
    {
        go_fill(col - 1, row, original, target);
    } // if

} // sprmo::Sprite::go_fill

// **************************************************

void sprmo::Sprite::display_pixel_array() const
{
    for (int i = 0; i < static_cast<int>(m_height); ++i)
    {
        for (int j = 0; j < static_cast<int>(m_width); ++j)
        {
            std::cout << (*m_pixel_data)[i*m_width+j] << "  ";
        } // for
        std::cout << "\n";
    } // for 

} // sprmo::Sprite::display_pixel_array

// **************************************************

void sprmo::Sprite::copy_pixels(const std::vector<int>& data)
{
    std::copy(data.begin(), data.end(), m_pixel_data->begin());

} // sprmo::Sprite::copy_pixels

// **************************************************

sprmo::Palette::C64_color sprmo::Sprite::get_color2()
{
    Palette::C64_color c = static_cast<Palette::C64_color>(m_color);
    //std::cout << "color=" << c << "\n";
    return c;

} // sprmo::Sprite::

// **************************************************

void sprmo::Sprite::refresh_quads(const Palette_panel::Summary& p,
        bool refresh_SPR)
{
    for (size_t row = 0; row < m_height; ++row)
    {
        for (size_t col = 0; col < m_width; ++col)
        {
            int c = (*m_pixel_data)[row*m_width+col];

            if (c == 1 && !refresh_SPR) continue;

            sf::Color mycolor = p.colors[c].first;
            size_t base_i = row*m_width*4+col*4;
            m_cell_quads_pixels[base_i].color = mycolor;
            m_cell_quads_pixels[base_i+1].color = mycolor;
            m_cell_quads_pixels[base_i+2].color = mycolor;
            m_cell_quads_pixels[base_i+3].color = mycolor;
        } // for
    } // for

} // sprmo::Sprite::refresh_quads

// **************************************************

bool sprmo::Sprite::set_pixel(
        size_t col,
        size_t row,
        bool erase,
        const Palette_panel::Summary& s)
{
    bool pixel_changed = false;
    // NOTE: color_type is in range [0,4] - bg,spr,spmc0 or spmc1.
    int color_type = erase ? 0 : s.type;

    if ( color_type != get_pixel(col, row) )
    {
        size_t base_i = row*m_width*4 + col*4;
        for (size_t i = base_i; i < (base_i + 4); ++i)
        {
            m_cell_quads_pixels[i].color = s.colors[color_type].first;
        } // for
        (*m_pixel_data)[ row*m_width+col ] = color_type;
        pixel_changed = true;
    } // if

    return pixel_changed;

} // sprmo::Sprite::set_pixel

// **************************************************

std::shared_ptr<std::vector<int>> sprmo::Sprite::get_C64_data() const
{
    auto mydata_ptr = std::shared_ptr<std::vector<int>>(
            new std::vector<int>(Bytes_per_sprite));
    int resolution = m_multicolor ? 4 : 8;
    int count = 0;

    for (size_t i = 0; i < m_height; ++i)
    {
        // Extract 3 bytes of data.
        for (int k = 0; k < 3; ++k)
        {
            int offset = i*m_width + k*resolution;
            int mybyte = m_multicolor ? calc_multicolor_byte(offset) :
                calc_hires_byte(offset);
            (*mydata_ptr)[count] = mybyte;
            ++count;
        } // for
    } // for

    /*
    for ( auto x : *mydata_ptr )
    {
        std::cout << x << ",";
    } // for
    std::cout << "\n";
    */

    return mydata_ptr;

} // sprmo::Sprite::get_C64_data

// **************************************************

int sprmo::Sprite::calc_hires_byte(int offset) const
{
    int mybyte = 0;

    for (int i = 0; i < 8; ++i)
    {
        if ( (*m_pixel_data)[ offset+i ] == SPR )
        {
            mybyte += static_cast<int>(pow(2, 7-i));
        } // if
    } // for

    return mybyte;

} // sprmo::Sprite::calc_hires_byte

// **************************************************

int sprmo::Sprite::calc_multicolor_byte(int offset) const
{
    static int Exponents[] = { 6, 4, 2, 0 };

    int mybyte = 0;
    for (int i = 0; i < 4; ++i)
    {
        switch ( (*m_pixel_data)[offset+i] )
        {
            case BG:
                // OK - just zero.
                break;
            case SPR:
                mybyte += static_cast<int>(2 * pow(2, Exponents[i]));
                break;
            case SPMC0:
                mybyte += static_cast<int>(1 * pow(2, Exponents[i]));
                break;
            case SPMC1:
                mybyte += static_cast<int>(3 * pow(2, Exponents[i]));
                break;
        } // switch
    } // for

    return mybyte;

} // sprmo::Sprite::calc_multicolor_byte

// **************************************************

/*
void sprmo::Sprite::init_base_x_coords()
{
    // Hi-res:
    for (int i = 0; i < 24; ++i)
    {
        int dest_base = i * 4;
        quads_base_x_coords2[dest_base]     = i * s_cell_size;
        quads_base_x_coords2[dest_base+1]   = (i+1) * s_cell_size;
        quads_base_x_coords2[dest_base+2]   = (i+1) * s_cell_size;
        quads_base_x_coords2[dest_base+3]   = i * s_cell_size;
    } // for

    // Multicolor:
    for (int i = 0; i < 12; ++i)
    {
        int dest_base = i * 4;
        quads_base_x_coords_mc2[dest_base]      = i * s_cell_size * 2;
        quads_base_x_coords_mc2[dest_base+1]    = (i+1) * s_cell_size * 2;
        quads_base_x_coords_mc2[dest_base+2]    = (i+1) * s_cell_size * 2;
        quads_base_x_coords_mc2[dest_base+3]    = i * s_cell_size * 2;
    } // for

} // sprmo::Sprite::
*/

// **************************************************

// sprmo::Sprite::
// {
// } // sprmo::Sprite::
// **************************************************

// sprmo::Sprite::
// {
// } // sprmo::Sprite::
// **************************************************

// sprmo::Sprite::
// {
// } // sprmo::Sprite::
// **************************************************

// sprmo::Sprite::
// {
// } // sprmo::Sprite::
// **************************************************

// sprmo::Sprite::
// {
// } // sprmo::Sprite::
// **************************************************

// sprmo::Sprite::
// {
// } // sprmo::Sprite::
// **************************************************

// sprmo::Sprite::
// {
// } // sprmo::Sprite::
// **************************************************

// sprmo::Sprite::
// {
// } // sprmo::Sprite::
// **************************************************

// sprmo::Sprite::
// {
// } // sprmo::Sprite::
