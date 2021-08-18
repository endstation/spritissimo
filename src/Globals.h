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




#ifndef Globals_h
#define Globals_h




namespace sprmo
{

class Globals
{

    public:

        Globals();
        ~Globals();

        static const int Window_w = 800;
        static const int Window_h = 625;//600;
        // Size (in pixels) of one Dragon 32 hardware pixel.  Depending
        // on resolution, 'virtual' pixels may consist of more than one
        // 'hardware' pixel on both axes.
        static const int HW_pixel_size = 4;

    protected:

    private:

}; // class Globals

} // namespace sprmo




#endif
