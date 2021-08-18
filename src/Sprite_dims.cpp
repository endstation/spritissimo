// Spritissimo64
// Copyright 2016-2021 Matthew Clarke <pclar7@yahoo.co.uk>

// This file is part of Spritissimo64.

// Spritissimo64 is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.

// Spritissimo64 is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.

// You should have received a copy of the GNU General Public License
// along with Spritissimo64.  If not, see <http://www.gnu.org/licenses/>.




#include "Sprite_dims.h"




// *******************
// *** CONSTRUCTOR ***
// *******************

sprmo::Sprite_dims::Sprite_dims(size_t cell_size) :
    Cell_size(cell_size),
    Img_width(cell_size * 24),
    Img_height(cell_size * 21)
{
    init();

} // sprmo::Sprite_dims::Sprite_dims

// ******************
// *** DESTRUCTOR ***
// ******************

sprmo::Sprite_dims::~Sprite_dims()
{
    // NYI...

} // sprmo::Sprite_dims::~Sprite_dims




// ************************
// *** MEMBER FUNCTIONS ***
// ************************

// **************************************************

void sprmo::Sprite_dims::init()
{
    // Hi-res:
    for (int i = 0; i < 24; ++i)
    {
        int dest_base = i * 4;
        Quads_base_x_coords[dest_base]      = i * Cell_size;
        Quads_base_x_coords[dest_base+1]    = (i+1) * Cell_size;
        Quads_base_x_coords[dest_base+2]    = (i+1) * Cell_size;
        Quads_base_x_coords[dest_base+3]    = i * Cell_size;
    } // for

    // Multicolor:
    for (int i = 0; i < 12; ++i)
    {
        int dest_base = i * 4;
        Quads_base_x_coords_mc[dest_base]   = i * Cell_size * 2;
        Quads_base_x_coords_mc[dest_base+1] = (i+1) * Cell_size * 2;
        Quads_base_x_coords_mc[dest_base+2] = (i+1) * Cell_size * 2;
        Quads_base_x_coords_mc[dest_base+3] = i * Cell_size * 2;
    } // for

} // sprmo::Sprite_dims::init

// **************************************************

// sprmo::Sprite_dims::
// {
// } // sprmo::Sprite_dims::

// **************************************************

// sprmo::Sprite_dims::
// {
// } // sprmo::Sprite_dims::

// **************************************************

// sprmo::Sprite_dims::
// {
// } // sprmo::Sprite_dims::

// **************************************************

// sprmo::Sprite_dims::
// {
// } // sprmo::Sprite_dims::

