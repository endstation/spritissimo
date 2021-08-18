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




#ifndef Animation_panel_h
#define Animation_panel_h

#include <SFML/Graphics.hpp>
#include <cstddef>
#include <iostream>
#include "Sprite.h"
#include "Project.h"
#include <memory>
#include "Globals.h"
#include <sigc++/sigc++.h>
#include "Panel_dims.h"
#include "Zoomable.h"
#include "Sprite_dims.h"




namespace sprmo
{

class Animation_panel : public sf::Drawable
{

    public:

        const size_t Default_fps = 15;
        const size_t Min_fps = 5;
        const size_t Max_fps = 60;
        // Pause between 'one time' animations.
        const size_t Pause_frames = 60;
        enum Anim_type { Loop, Ping_pong, One_time };

        Animation_panel(std::shared_ptr<Project> p, const Panel_dims& dims,
                const Sprite_dims& sprdims);
        ~Animation_panel();

        void handle_new_sequence(size_t start, size_t end);
        void handle_sequence_invalidated();
        void update();
        void start_stop();
        void dec_fps();
        void inc_fps();
        Anim_type get_anim_type() const { return m_anim_type; }
        size_t get_frame_rate() const { return m_fps; }
        size_t get_sequence_start() const { return m_sequence_start; }
        size_t get_sequence_end() const { return m_sequence_end; }
        void next_anim_type();
        sigc::signal<void, Anim_type>& signal_anim_type_changed();
        sigc::signal<void, size_t>& signal_frame_rate_changed();
        sigc::signal<void, size_t>& signal_frame_changed();
        bool is_animating() const { return m_animating; }

        // Zoom stuff.
        void zoom_in()          { m_zoomable->zoom_in(); }
        void zoom_out()         { m_zoomable->zoom_out(); }
        void zoom_in_stop()     { m_zoomable->zoom_in_stop(); }
        void zoom_out_stop()    { m_zoomable->zoom_out_stop(); }
        void stop_all_zoom()    { m_zoomable->stop_all(); }

    protected:

    private:
        
        void init_view(const Panel_dims& dims, const Sprite_dims& sprdims);
        virtual void draw(sf::RenderTarget& target, 
                sf::RenderStates states) const;

        std::shared_ptr<Project>        m_project_ptr;
        size_t                          m_sequence_start;
        size_t                          m_sequence_end;
        bool                            m_animating;
        sf::View                        m_view;
        size_t                          m_fps;
        size_t                          m_count;
        // This is the index of the first sprite in the sequence.
        int                             m_current_frame;
        Anim_type                       m_anim_type;
        int                             m_delta;
        sigc::signal<void, Anim_type>   m_signal_anim_type_changed;
        sigc::signal<void, size_t>      m_signal_frame_rate_changed;
        sigc::signal<void, size_t>      m_signal_frame_changed;
        // 'Ready' if have a valid sequence that we can animate.
        bool                            m_ready;
        std::unique_ptr<Zoomable>       m_zoomable;

}; // class Animation_panel

} // namespace sprmo




#endif
