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




#ifndef Sprite_h
#define Sprite_h

#include <vector>
#include <cassert>
#include <memory>
#include <SFML/Graphics.hpp>
#include <algorithm>
#include "Globals.h"
#include "Clipboard.h"
#include <boost/utility/binary.hpp>
#include "Palette.h"
#include "Palette_panel.h"
#include <cmath>
#include "Sprite_dims.h"




namespace sprmo
{

class Sprite
{

    public:

        // Pixel data is composed of these.  Which colors they correspond to
        // depends on the current palette choice.
        enum Pixel_codes { BG, SPR, SPMC0, SPMC1 };

        static const int Bytes_per_sprite = 64;
        static const int Cell_height = 8;
        static const int Img_width = Cell_height*24;
        static const int Img_height = Cell_height*21;
        static const int s_cell_size = 8;
        // This is used by Preview_panel.
        // FIXME: also need value for HEIGHT!
        //static const int Size = Cell_size * 24;

        Sprite(bool multicolor, const Palette_panel::Summary& s,
                const Sprite_dims& sprdims);
        Sprite(bool multicolor, const Palette_panel::Summary& s,
                Palette::C64_color color, const std::vector<int>& v,
                const Sprite_dims& sprdims);
        ~Sprite();

        bool set_pixel(size_t col, size_t row, size_t color_selection,
                const Palette& r);
        bool set_pixel(size_t col, size_t row, bool erase,
                const Palette_panel::Summary& s);
        //bool set_mask(size_t col, size_t row);
        const sf::VertexArray& get_quads_pixels() const;
        const sf::VertexArray& get_bg_quad() const { return m_bg_quad; }
        std::shared_ptr<std::vector<int>> get_pixel_data_copy() const;
        // Called as part of an undo/redo operation.
        void set_all_pixels(const std::vector<int>& v);
        // Pixel adjustment.
        void flip_vertical(const Palette_panel::Summary& s);
        void flip_horizontal(const Palette_panel::Summary& s);
        void rotate_pixels_left(const Palette_panel::Summary& s);
        void rotate_pixels_right(const Palette_panel::Summary& s);
        void rotate_pixels_up(const Palette_panel::Summary& s);
        void rotate_pixels_down(const Palette_panel::Summary& s);
        void delete_pixels(const Palette_panel::Summary& s);
        size_t get_width() const { return m_width; }
        size_t get_height() const { return m_height; }
        void calc_source_code_bytes(size_t pixels_per_byte);
        const std::vector<sf::Uint8>& get_bytes() const { return m_bytes; }
        void refresh_quads(const Palette_panel::Summary& p,
                bool refresh_SPR = true);
        void bucket_fill(size_t col, size_t row, bool erase,
                const Palette_panel::Summary& s);
        void set_color(Palette::C64_color c) { m_color = c; }
        Palette::C64_color get_color() const { return m_color; }
        Palette::C64_color get_color2();
        bool is_multicolor() const { return m_multicolor; }
        int get_cell_width() const { return m_cell_width; }
        void copy_pixels(const std::vector<int>& data);
        std::shared_ptr<std::vector<int>> get_C64_data() const;

    protected:

    private:

        // NOTE: y-coords don't change.
        static const float Quads_base_x_coords[];
        static const float Quads_base_x_coords_mc[];
        //static const sf::Color Mask_color;

        float quads_base_x_coords2[96];
        float quads_base_x_coords_mc2[48];

        void init_quads();
        int get_pixel(size_t col, size_t row) const;
        //int get_mask(size_t col, size_t row) const;
        void calc_bytes(const std::vector<int>& pixel_data, 
                size_t width_pixels, size_t pixels_per_byte);
        void go_fill(size_t col, size_t row, int source, int target);
        //void remove_mask();
        void display_pixel_array() const;
        int calc_hires_byte(int offset) const;
        int calc_multicolor_byte(int offset) const;
        //void init_base_x_coords();

        // NOTE: width and height measured in pixels.
        size_t m_width;
        size_t m_height;
        std::unique_ptr<std::vector<int>> m_pixel_data;
        sf::VertexArray m_cell_quads_pixels;
        sf::VertexArray m_bg_quad;
        //bool m_mask_enabled;
        //bool m_must_preshift;
        std::vector<sf::Uint8> m_bytes;
        // Individual sprite color (0-15).
        Palette::C64_color m_color;
        bool m_multicolor;
        int m_cell_width;
        const Sprite_dims& m_sprdims_ref;

}; // class Sprite

} // namespace sprmo




#endif

