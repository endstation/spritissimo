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




#ifndef Clipboard_h
#define Clipboard_h

#include <vector>
#include <cstdlib>
#include <iostream>




namespace sprmo
{

class Clipboard
{

    public:

        Clipboard(size_t w, size_t h, bool masked = false);
        ~Clipboard();

        size_t width;
        size_t height;
        std::vector<int> data;
        bool has_mask;

        void to_console();

    protected:

    private:

}; // class Clipboard

} // namespace sprmo




#endif

