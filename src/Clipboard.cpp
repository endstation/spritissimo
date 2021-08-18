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




#include "Clipboard.h"




// *******************
// *** CONSTRUCTOR ***
// *******************

sprmo::Clipboard::Clipboard(size_t w, size_t h, bool masked) :
        width(w),
        height(h),
        data(w*h),
        has_mask(masked)
{
    // NYI...

} // sprmo::Clipboard::Clipboard

// ******************
// *** DESTRUCTOR ***
// ******************

sprmo::Clipboard::~Clipboard()
{
    // NYI...

} // sprmo::Clipboard::~Clipboard




// ************************
// *** MEMBER FUNCTIONS ***
// ************************

// **************************************************

void sprmo::Clipboard::to_console()
{
    for (size_t i = 0; i < height; ++i)
    {
        for (size_t j = 0; j < width; ++j)
        {
            std::cout << data[i*width+j] << "  ";
        } // for
        std::cout << "\n";
    } // for

} // sprmo::Clipboard::to_console

// **************************************************

// sprmo::Clipboard::
// {
// } // sprmo::Clipboard::

// **************************************************

// sprmo::Clipboard::
// {
// } // sprmo::Clipboard::

// **************************************************

// sprmo::Clipboard::
// {
// } // sprmo::Clipboard::

// **************************************************

// sprmo::Clipboard::
// {
// } // sprmo::Clipboard::
