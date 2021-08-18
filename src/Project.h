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




#ifndef Project_h
#define Project_h

#include <string>
#include <iostream>
#include <memory>
#include "Sprite.h"
#include <sigc++/sigc++.h>
#include "Undo_stack.h"
//#include "Resolution.h"
#include <algorithm>
#include "Globals.h"
#include <vector>
#include "Clipboard.h"
#include "Palette.h"
#include "Palette_panel.h"
#include "Sprite_dims.h"




namespace sprmo
{

class Project
{

    public:

        Project(Palette& pal, const Palette_panel::Summary& summary,
                const Sprite_dims& sprdims);
        ~Project();

        std::shared_ptr<Sprite> get_current_sprite();
        size_t get_current_sprite_i() const { return m_current_sprite; }
        void add_new_sprite(int w, int h, bool insert = false);
        // Add a new 'ready-made' sprite (in response to user loading in an
        // existing project).
        void add_new_sprite(int w, int h, bool is_mask, bool preshift,
                const std::vector<int>& data);

        // NOTE: for new C64 version!
        void add_new_sprite2(bool multicolor, bool insert = false);
        void add_new_sprite2(bool multicolor, int color,
                const std::vector<int>& data);

        // NOTE: no wraparound for these functions.
        void next_sprite();
        void previous_sprite();
        void first_sprite();
        void last_sprite();
        size_t get_num_sprites() const { return m_sprites.size(); }
        std::shared_ptr<Sprite> get_sprite(size_t i);
        void set_current_sprite(size_t i);
        //sigc::signal<void,bool,Resolution::C64_color>& signal_new_current_sprite();
        sigc::signal<void,bool,Palette::C64_color>& signal_new_current_sprite();
        sigc::signal<void,int>& signal_new_sprite_added();
        sigc::signal<void,Palette::C64_color>& signal_loading_sprite();
        // Specify where the sprite moved from (index position).
        sigc::signal<void,size_t>& signal_sprite_moved();
        sigc::signal<void,size_t>& signal_sprite_removed();
        // Signals from editing panel.
        void handle_sprite_modified();
        void undo();
        void redo();
        void copy();
        void paste();
        void handle_add_new_sprite();
        void remove();
        void flip_vertical();
        void flip_horizontal();
        void rotate_pixels_left();
        void rotate_pixels_right();
        void rotate_pixels_up();
        void rotate_pixels_down();
        void delete_pixels();
        // Called in response to a 'load' request.
        void clear_all_sprites();
        void handle_new_file_loaded();
        void calc_source_code_bytes();
        void move_sprite_left();
        void move_sprite_right();
        void bucket_fill(size_t col, size_t row, bool erase);
        bool unsaved_changes() const { return m_unsaved_changes; }
        const sf::Texture* get_mask_texture() const { return &m_mask_texture; }
        void handle_project_saved() { m_unsaved_changes = false; }
        void handle_color_cycled(size_t i);
        // For your convenience (used by Editing_panel when user wants to
        // erase pixels):
        //const Resolution::Palette_color& get_clear_color() const;
        bool is_current_sprite_multicolor() const;
        std::vector<int> get_palette() const;
        bool set_sprite_pixel(size_t col, size_t row, bool erase);

        // Some convenience methods for file i/o.
        int get_bg() const { return m_summary_ref.colors[0].second; }
        int get_spmc0() const { return m_summary_ref.colors[2].second; }
        int get_spmc1() const { return m_summary_ref.colors[3].second; }

        std::shared_ptr<std::vector<int>> write_prg_data(int dest_addr) const;

    protected:

    private:

        void copy_current_sprite_pixels();
        void to_console(const std::vector<int>& v) const;
        void handle_new_current_sprite();

        std::string                             m_name;
        std::string                             m_path;
        bool                                    m_unsaved_changes;
        size_t                                  m_current_sprite;
        std::vector<std::shared_ptr<Sprite>>    m_sprites;
        //sigc::signal<void,bool,Resolution::C64_color>
        sigc::signal<void,bool,Palette::C64_color>
                                                m_signal_new_current_sprite;
        sigc::signal<void,int>                  m_signal_new_sprite_added;
        sigc::signal<void,bool>                 m_signal_mask_enabled_toggled;
        sigc::signal<void,bool>                 m_signal_must_preshift_toggled;
        sigc::signal<void,size_t>               m_signal_sprite_moved;
        sigc::signal<void,size_t>               m_signal_sprite_removed;
        sigc::signal<void,Palette::C64_color>   m_signal_loading_sprite;
        Undo_stack                              m_undo_stack;
        std::shared_ptr<std::vector<int>>       m_current_sprite_pixels_copy;
        std::shared_ptr<Clipboard>              m_clipboard_ptr;
        sf::Texture                             m_mask_texture;
        Palette&                                m_palette_ref;
        const Palette_panel::Summary&           m_summary_ref;
        size_t                                  m_prg_address;
        const Sprite_dims&                      m_sprdims_ref;

}; // class Project

} // namespace sprmo




#endif

