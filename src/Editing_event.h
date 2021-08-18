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




#ifndef Editing_event_h
#define Editing_event_h

#include <vector>
#include <cstdlib>




namespace sprmo
{

class Editing_event
{

    public:

        enum Type { Pixels, Mask_toggle, Preshift_toggle, Sprite_move,
            Sprite_add, Sprite_remove };

        Editing_event(Type t, size_t index, 
                const std::vector<int>* pixels = nullptr,
                bool mask_changed = false);
        ~Editing_event();

        Type get_type() const { return m_type; }
        size_t get_index() const { return m_index; }
        bool did_mask_change() const { return m_mask_changed; }
        // TODO: pixels.
        
    protected:

    private:
        
        Type                m_type;
        size_t              m_index;
        std::vector<int>    m_pixel_data;
        bool                m_mask_changed;
        // TODO: define Dims somewhere (in Sprite class?).
        // Dims                m_dims;

}; // class Editing_event

} // namespace sprmo




#endif
