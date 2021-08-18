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




#ifndef Sprite_dims_h
#define Sprite_dims_h

#include <cstdlib>




namespace sprmo
{

class Sprite_dims
{

    public:

        Sprite_dims(size_t cell_size);
        ~Sprite_dims();

        const size_t Cell_size;
        const size_t Img_width;
        const size_t Img_height;
        float Quads_base_x_coords[96];
        float Quads_base_x_coords_mc[48];

    protected:

    private:

        void init();

}; // class Sprite_dims

} // namespace sprmo




#endif

