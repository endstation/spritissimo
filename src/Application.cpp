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




#include "Application.h"




// *******************
// *** CONSTRUCTOR ***
// *******************

sprmo::Application::Application(
        sf::RenderWindow& rwin,
        const std::vector<Panel_dims>& pd,
        unsigned int text_size,
        const sprmo::Sprite_dims& sprdims,
        sprmo::Config& cfg) :
    m_current_mode(Start_up),
    m_rwin_ref(rwin),
    m_heartbeat(1.0/60, std::bind(&Application::update, this)),
    m_config_ref(cfg)
{
    init_panel_backgrounds(pd);

    m_bold_font.loadFromFile(PKG_DATA_DIR "/fonts/DejaVuSansMono.ttf");

    m_palette_panel = std::shared_ptr<Palette_panel>(
            new Palette_panel(m_palette, pd[3]));
    m_project = std::shared_ptr<Project>(new Project(m_palette,
                m_palette_panel->get_summary(), sprdims));
    m_editing_panel = std::shared_ptr<Editing_panel>(
            new Editing_panel(m_project, pd[0], sprdims));
    m_preview_panel = std::shared_ptr<Preview_panel>(
            new Preview_panel(m_project, pd[4], m_rwin_ref.getSize().x,
                sprdims));
    m_editing_status_panel = std::shared_ptr<Editing_status_panel>(
            new Editing_status_panel(m_bold_font, m_project, pd[2], text_size));
    m_status_bar = std::shared_ptr<Status_bar>(new Status_bar(m_bold_font,
            m_project, pd[5], text_size));
    m_animation_panel = std::shared_ptr<Animation_panel>(
            new Animation_panel(m_project, pd[1], sprdims));
    m_animation_status_panel = std::shared_ptr<Animation_status_panel>(
            new Animation_status_panel(m_bold_font, m_animation_panel, pd[6], text_size));
    m_help_screen = std::shared_ptr<Help_screen>(
            new Help_screen(m_bold_font, text_size, m_rwin_ref.getSize().x));

    // Set up signal connections.
    m_editing_panel->signal_cell_changed().connect(sigc::mem_fun(
            *m_editing_status_panel, 
            &Editing_status_panel::handle_cell_changed));
    m_editing_panel->signal_sprite_modified().connect(sigc::mem_fun(
            *m_project, &Project::handle_sprite_modified));
    m_palette_panel->signal_pool_changed().connect(sigc::mem_fun(
            *m_editing_panel, &Editing_panel::handle_color_changed));
    /*m_palette_panel->signal_palette_changed().connect(sigc::mem_fun(
            *m_project, &Project::handle_palette_changed));*/
    m_palette_panel->signal_color_cycled().connect(sigc::mem_fun(
                *m_project, &Project::handle_color_cycled));
    m_project->signal_new_current_sprite().connect(sigc::mem_fun(
            *m_editing_panel, &Editing_panel::handle_new_current_sprite));
    m_project->signal_new_current_sprite().connect(sigc::mem_fun(
            *m_editing_status_panel, 
            &Editing_status_panel::handle_new_current_sprite));
    m_project->signal_new_current_sprite().connect(sigc::mem_fun(
            *m_preview_panel, &Preview_panel::handle_new_current_sprite));
    m_project->signal_new_current_sprite().connect(sigc::mem_fun(
            *m_palette_panel, &Palette_panel::handle_new_current_sprite));
    m_project->signal_new_sprite_added().connect(sigc::mem_fun(
            *m_preview_panel, &Preview_panel::handle_new_sprite_added));
    m_project->signal_loading_sprite().connect(sigc::mem_fun(
                *m_palette_panel, &Palette_panel::handle_loading_sprite));
    /*
    m_project->signal_mask_enabled_toggled().connect(sigc::mem_fun(
            *m_editing_status_panel, 
            &Editing_status_panel::handle_mask_enabled_toggled));
    m_project->signal_must_preshift_toggled().connect(sigc::mem_fun(
            *m_editing_status_panel,
            &Editing_status_panel::handle_must_preshift_toggled));
    */
    m_preview_panel->signal_new_sequence().connect(sigc::mem_fun(
            *m_animation_panel, &Animation_panel::handle_new_sequence));
    m_preview_panel->signal_new_sequence().connect(sigc::mem_fun(
            *m_animation_status_panel, 
            &Animation_status_panel::handle_new_sequence));
    m_animation_panel->signal_anim_type_changed().connect(sigc::mem_fun(
            *m_animation_status_panel, 
            &Animation_status_panel::handle_anim_type_changed));
    m_animation_panel->signal_frame_rate_changed().connect(sigc::mem_fun(
            *m_animation_status_panel,
            &Animation_status_panel::handle_frame_rate_changed));
    m_animation_panel->signal_frame_changed().connect(sigc::mem_fun(
            *m_animation_status_panel,
            &Animation_status_panel::handle_frame_changed));
    m_project->signal_sprite_moved().connect(sigc::mem_fun(
            *m_editing_status_panel, 
            &Editing_status_panel::handle_sprite_moved));
    m_project->signal_sprite_moved().connect(sigc::mem_fun(
            *m_preview_panel, &Preview_panel::handle_sprite_moved)); 
    m_project->signal_sprite_removed().connect(sigc::mem_fun(
            *m_preview_panel, &Preview_panel::handle_sprite_removed));
    m_preview_panel->signal_sequence_invalidated().connect(sigc::mem_fun(
            *m_animation_panel, &Animation_panel::handle_sequence_invalidated));
    m_preview_panel->signal_sequence_invalidated().connect(sigc::mem_fun(
            *m_animation_status_panel, 
            &Animation_status_panel::handle_sequence_invalidated));

    // Force palette panel to send 'color changed' signal - otherwise editing
    // panel's color won't be valid when app starts.
    //m_palette_panel->next_pool(false);

    std::string s(m_config_ref.get_last_file());
    if (!s.empty())
    {
        m_current_mode = Loading;
        try_load_save(s);
    }
    else
    {
        //std::cout << "new project\n";
    } // if ... else

    m_editing_panel->handle_new_current_sprite(false, Palette::Default_spr_color);
    m_editing_status_panel->handle_new_current_sprite(false,
            Palette::Default_spr_color);

} // sprmo::Application::Application

// ******************
// *** DESTRUCTOR ***
// ******************

sprmo::Application::~Application()
{
    // NYI...

} // sprmo::Application::~Application




// ************************
// *** MEMBER FUNCTIONS ***
// ************************

// **************************************************

void sprmo::Application::update()
{
    sf::Event my_event;

    while (m_rwin_ref.pollEvent(my_event))
    {
        if (my_event.type == sf::Event::Closed)
        {
            // FIXME: chance to save changes!!!
            handle_closed();
        }
        else if (my_event.type == sf::Event::KeyPressed)
        {
            handle_key_pressed(my_event.key);
        }
        else if (my_event.type == sf::Event::KeyReleased)
        {
            handle_key_released(my_event.key);
        }
        else if (my_event.type == sf::Event::MouseButtonPressed)
        {
            handle_mouse_button_pressed(my_event.mouseButton);
        }
        else if (my_event.type == sf::Event::MouseButtonReleased)
        {
            handle_mouse_button_released(my_event.mouseButton);
        }
        else if (my_event.type == sf::Event::MouseWheelScrolled)
        {
            handle_mouse_wheel_scrolled(my_event.mouseWheelScroll);
        }
        else if (my_event.type == sf::Event::MouseMoved)
        {
            handle_mouse_moved(my_event.mouseMove);
        }
        else if (my_event.type == sf::Event::TextEntered)
        {
            handle_text_entered(my_event.text);
        } // if ... else
    } // while

    m_preview_panel->update();
    m_editing_panel->update();
    m_status_bar->update();
    m_animation_panel->update();
    m_help_screen->update();
    refresh_title();

    m_rwin_ref.clear();
    if (m_current_mode == Display_help)
    {
        m_rwin_ref.draw(*m_help_screen);
    }
    else
    {
        m_rwin_ref.draw(m_panel_backgrounds);
        m_rwin_ref.draw(*m_editing_panel);
        m_rwin_ref.draw(*m_preview_panel);
        m_rwin_ref.draw(*m_editing_status_panel);
        m_rwin_ref.draw(*m_palette_panel);
        m_rwin_ref.draw(*m_status_bar);
        m_rwin_ref.draw(*m_animation_panel);
        m_rwin_ref.draw(*m_animation_status_panel);
    } // if ... else
    m_rwin_ref.display();

} // sprmo::Application::update

// **************************************************

void sprmo::Application::handle_closed()
{
    if (m_project->unsaved_changes())
    {
        std::cout << "prompt for confirmation\n";
        m_status_bar->init_confirm_quit();
        m_current_mode = Confirm_close;
    }
    else
    {
        really_close();
    } // if ... else

} // sprmo::Application::handle_closed

// **************************************************

void sprmo::Application::handle_key_pressed(sf::Event::KeyEvent& e)
{
    if (m_current_mode == Loading || m_current_mode == Saving
            || m_current_mode == Saving_prg
            || m_current_mode == Confirm_close
            || m_current_mode == Saving_png_file)
    {
        if (e.code == sf::Keyboard::Escape)
        {
            // Abort file operation.
            m_current_mode = Editing;
            m_status_bar->reset();
        }
        else if (e.code == sf::Keyboard::Return)
        {
            try_load_save(m_status_bar->get_entry_text());
        } // if ... else
        // MODAL - so no other widgets respond to key press events.
        return;
    }
    else if (m_current_mode == Display_help)
    {
        if (e.code == sf::Keyboard::Escape || e.code == sf::Keyboard::F1)
        {
            m_current_mode = Editing;
        }
        else if (e.code == sf::Keyboard::Up)
        {
            m_help_screen->scroll_up();
        }
        else if (e.code == sf::Keyboard::Down)
        {
            m_help_screen->scroll_down();
        } // if ... else
        return;
    } // if ... else

    if (e.code == sf::Keyboard::N && e.control)
    {
        // FIXME: remove hard-coded size!
        //m_project->add_new_sprite(16, 16);
        // NOTE: if alt key pressed, new sprite will be INSERTED BEFORE
        // the current one, rather than added to the end of the vector.
        //m_project->add_new_sprite(16, 16, e.alt);
        m_project->add_new_sprite2(false, e.alt);
    }
    else if (e.code == sf::Keyboard::M && e.control)
    {
        m_project->add_new_sprite2(true, e.alt);
    }
    else if (e.code == sf::Keyboard::Z && e.alt)
    {
        m_palette_panel->cycle_current_pool(-1);
    }
    else if (e.code == sf::Keyboard::X && e.alt)
    {
        m_palette_panel->cycle_current_pool(1);
    }
    else if (e.code == sf::Keyboard::Z && !e.control)
    {
        m_palette_panel->previous_pool(m_project->is_current_sprite_multicolor());
    }
    else if (e.code == sf::Keyboard::X && !e.control)
    {
        m_palette_panel->next_pool(m_project->is_current_sprite_multicolor());
    }
    else if (e.code == sf::Keyboard::P && e.control)
    {
        init_new_project();
        //m_project = std::shared_ptr<Project>(new Project(m_resolution));
    }
    else if (e.code == sf::Keyboard::L && e.control)
    {
        m_status_bar->init_load();
        m_current_mode = Loading;
    }
    else if (e.code == sf::Keyboard::S && e.control)
    {
        m_status_bar->init_save(m_current_filename);
        m_current_mode = Saving;
    }
    else if (e.code == sf::Keyboard::Equal)
    {
        if (m_animation_panel->is_animating())
        {
            m_animation_panel->zoom_in();
        }
        else
        {
            m_editing_panel->zoom_in();
        } // if ... else
    }
    else if (e.code == sf::Keyboard::Dash)
    {
        if (m_animation_panel->is_animating())
        {
            m_animation_panel->zoom_out();
        }
        else
        {
            m_editing_panel->zoom_out();
        } // if ... else
    }
    else if (e.code == sf::Keyboard::Left && e.control && e.shift)
    {
        m_project->move_sprite_left();
    }
    else if (e.code == sf::Keyboard::Right && e.control && e.shift)
    {
        m_project->move_sprite_right();
    }
    else if (e.code == sf::Keyboard::Left && e.control)
    {
        m_project->previous_sprite();
    }
    else if (e.code == sf::Keyboard::Right && e.control)
    {
        m_project->next_sprite();
    }
    else if (e.code == sf::Keyboard::Left && e.shift)
    {
        m_editing_panel->move_left();
    }
    else if (e.code == sf::Keyboard::Right && e.shift)
    {
        m_editing_panel->move_right();
    }
    else if (e.code == sf::Keyboard::Up && e.shift)
    {
        m_editing_panel->move_up();
    }
    else if (e.code == sf::Keyboard::Down && e.shift)
    {
        m_editing_panel->move_down();
    }
    else if (e.code == sf::Keyboard::Left)
    {
        m_preview_panel->left();
    }
    else if (e.code == sf::Keyboard::Right)
    {
        m_preview_panel->right();
    }
    else if (e.code == sf::Keyboard::G && e.control)
    {
        m_editing_panel->toggle_grid();
    }
    //else if (e.code == sf::Keyboard::T && e.control)
    //{
    //    m_palette_panel->next_palette();
    //}
    else if (e.code == sf::Keyboard::Z && e.control)
    {
        m_project->undo();
    }
    else if (e.code == sf::Keyboard::R && e.control)
    {
        m_project->redo();
    }
    else if (e.code == sf::Keyboard::C && e.control)
    {
        m_project->copy();
    }
    else if (e.code == sf::Keyboard::V && e.control)
    {
        m_project->paste();
    }
    else if (e.code == sf::Keyboard::X && e.control)
    {
        m_project->remove();
    }
    //else if (e.code == sf::Keyboard::Num1 && e.control)
    //{
    //    m_project->add_new_sprite(8, 8, e.alt);
    //}
    //else if (e.code == sf::Keyboard::Num2 && e.control)
    //{
    //    m_project->add_new_sprite(16, 8, e.alt);
    //}
    //else if (e.code == sf::Keyboard::Num3 && e.control)
    //{
    //    m_project->add_new_sprite(8, 16, e.alt);
    //}
    //else if (e.code == sf::Keyboard::Num4 && e.control)
    //{
    //    m_project->add_new_sprite(4, 8, e.alt);
    //}
    //else if (e.code == sf::Keyboard::H && e.control)
    //{
    //    m_project->add_new_sprite(4, 4, e.alt);
    //}
    //else if (e.code == sf::Keyboard::M && !e.control)
    //{
    //    m_editing_panel->toggle_mode();
    //}
    //else if (e.code == sf::Keyboard::M && e.control)
    //{
    //    m_project->toggle_mask_enabled();
    //}
    //else if (e.code == sf::Keyboard::A && e.alt)
    //{
    //    m_project->auto_mask();
    //}
    else if (e.code == sf::Keyboard::W && e.control)
    {
        write_prg_or_png(e.alt);
        /*
        // Only allow this operation if m_current_filename contains something.
        if (m_current_filename.empty())
        {
            std::cerr << "Need a saved project first!\n";
        }
        else
        {
            // Create a copy of the current filename minus the 'xml' suffix.
            size_t len = m_current_filename.size();
            std::string s( m_current_filename.substr(0, len-3) );
            // Then append an 'asm6809' suffix.
            s.append("asm6809");
            m_current_mode = Saving_source_code;
            m_status_bar->init_write_source_code(s);
        } // if ... else
        */
    }
    else if (e.code == sf::Keyboard::Num9 && e.control)
    {
        m_project->flip_vertical();
    }
    else if (e.code == sf::Keyboard::Num0 && e.control)
    {
        m_project->flip_horizontal();
    }
    else if (e.code == sf::Keyboard::Num7 && e.control)
    {
        m_project->rotate_pixels_left();
    }
    else if (e.code == sf::Keyboard::Num8 && e.control)
    {
        m_project->rotate_pixels_right();
    }
    else if (e.code == sf::Keyboard::Num5 && e.control)
    {
        m_project->rotate_pixels_up();
    } 
    else if (e.code == sf::Keyboard::Num6 && e.control)
    {
        m_project->rotate_pixels_down();
    }
    //else if (e.code == sf::Keyboard::Q && e.control)
    //{
    //    m_project->toggle_must_preshift();
    //}
    else if (e.code == sf::Keyboard::Delete)
    {
        m_project->delete_pixels();
    }
    else if (e.code == sf::Keyboard::F10)
    {
        m_animation_panel->start_stop();
        m_editing_panel->stop_all_zoom();
        m_animation_panel->stop_all_zoom();
    }
    else if (e.code == sf::Keyboard::LBracket)
    {
        m_animation_panel->dec_fps();
    }
    else if (e.code == sf::Keyboard::RBracket)
    {
        m_animation_panel->inc_fps();
    }
    else if (e.code == sf::Keyboard::Quote)
    {
        m_animation_panel->next_anim_type();
    }
    else if (e.code == sf::Keyboard::Home && e.control)
    {
        m_project->first_sprite();
    }
    else if (e.code == sf::Keyboard::Home && e.shift)
    {
        m_editing_panel->center_sprite();
    }
    else if (e.code == sf::Keyboard::End && e.control)
    {
        m_project->last_sprite();
    } 
    else if (e.code == sf::Keyboard::Home)
    {
        m_preview_panel->home();
    }
    else if (e.code == sf::Keyboard::End)
    {
        m_preview_panel->end();
    }
    else if (e.code == sf::Keyboard::F1)
    {
        std::cout << "HELP!\n";
        m_current_mode = Display_help;
    }
    else if (e.code == sf::Keyboard::Q)
    {
        m_palette_panel->cycle_current_pool(-1);
    }
    else if (e.code == sf::Keyboard::A)
    {
        m_palette_panel->cycle_current_pool(1);
    }
    /*
    else if (e.code == sf::Keyboard::T)
    {
        File_IO::write_prg("", *m_project);
    } // if ... else
    */

} // sprmo::Application::handle_key_pressed

// **************************************************

void sprmo::Application::handle_mouse_button_pressed(
        sf::Event::MouseButtonEvent& e)
{
    bool result = m_editing_panel->handle_mouse_button_pressed(e);
    // If returns false, give another (preview?) panel chance to process it.
    if (!result)
    {
        // Let preview panel deal with mouse click.
        m_preview_panel->handle_mouse_button_pressed(e, m_rwin_ref);
    } // if

} // sprmo::Application::handle_mouse_button_pressed

// **************************************************

void sprmo::Application::handle_mouse_wheel_scrolled(
        sf::Event::MouseWheelScrollEvent& e)
{
} // sprmo::Application::handle_mouse_wheel_scrolled

// **************************************************

void sprmo::Application::handle_mouse_moved(sf::Event::MouseMoveEvent& e)
{
    m_editing_panel->handle_mouse_moved(e, m_rwin_ref);

} // sprmo::Application::handle_mouse_moved

// **************************************************

void sprmo::Application::start()
{
    bool carry_on = true;
    while (carry_on)
    {
        m_heartbeat.check_beat();
        carry_on = m_rwin_ref.isOpen();
    } // while

} // sprmo::Application::start

// **************************************************

void sprmo::Application::handle_key_released(sf::Event::KeyEvent& e)
{
    if (m_current_mode == Loading || m_current_mode == Saving)
    {
        return;
    }
    else if (m_current_mode == Display_help)
    {
        if (e.code == sf::Keyboard::Up)
        {
            m_help_screen->stop_up();
        }
        else if (e.code == sf::Keyboard::Down)
        {
            m_help_screen->stop_down();
        } // if ... else
        return;
    } // if ... else

    if (e.code == sf::Keyboard::Left)
    {
        m_preview_panel->left_stop();
        m_editing_panel->move_left_stop();
    }
    else if (e.code == sf::Keyboard::Right)
    {
        m_preview_panel->right_stop();
        m_editing_panel->move_right_stop();
    }
    else if (e.code == sf::Keyboard::Up)
    {
        m_editing_panel->move_up_stop();
    }
    else if (e.code == sf::Keyboard::Down)
    {
        m_editing_panel->move_down_stop();
    }
    else if (e.code == sf::Keyboard::Equal)
    {
        if (m_animation_panel->is_animating())
        {
            m_animation_panel->zoom_in_stop();
        }
        else
        {
            m_editing_panel->zoom_in_stop();
        } // if ... else
    }
    else if (e.code == sf::Keyboard::Dash)
    {
        if (m_animation_panel->is_animating())
        {
            m_animation_panel->zoom_out_stop();
        }
        else
        {
            m_editing_panel->zoom_out_stop();
        } // if ... else
    } // if ... else

} // sprmo::Application::handle_key_released

// **************************************************

void sprmo::Application::init_panel_backgrounds(const std::vector<Panel_dims>& pd)
{
    sf::Color grey1(20, 20, 20);
    sf::Color grey2(40, 40, 40);
    sf::Color grey3(60, 60, 60);

    // Animation panels.
    m_panel_backgrounds.setPrimitiveType(sf::Quads);
    const Panel_dims& dims3 = pd[Widget_anim_panel];
    const Panel_dims& dims4 = pd[Widget_anim_status_panel];
    m_panel_backgrounds.append(sf::Vertex(dims3.position, grey1));
    m_panel_backgrounds.append(sf::Vertex(sf::Vector2f(
                    dims3.position.x+dims3.size.x, dims3.position.y), grey1));
    m_panel_backgrounds.append(sf::Vertex(sf::Vector2f(
                    dims3.position.x+dims3.size.x,
                    dims3.position.y+dims3.size.y+dims4.size.y), grey1));
    m_panel_backgrounds.append(sf::Vertex(sf::Vector2f(
                    dims3.position.x, dims3.position.y+dims3.size.y+dims4.size.y), grey1));

    const Panel_dims& dims = pd[Widget_edit_status_panel];
    m_panel_backgrounds.append(sf::Vertex(dims.position, grey2));
    m_panel_backgrounds.append(sf::Vertex(sf::Vector2f(
                    dims.position.x+(dims.size.x*2), dims.position.y), grey2));
    m_panel_backgrounds.append(sf::Vertex(sf::Vector2f(
                    dims.position.x+(dims.size.x*2), dims.position.y+dims.size.y), grey2));
    m_panel_backgrounds.append(sf::Vertex(sf::Vector2f(
                    dims.position.x, dims.position.y+dims.size.y), grey2));

    const Panel_dims& dims2 = pd[Widget_preview_panel];
    m_panel_backgrounds.append(sf::Vertex(dims2.position, grey3));
    m_panel_backgrounds.append(sf::Vertex(sf::Vector2f(
                    dims2.position.x+dims2.size.x, dims2.position.y), grey3));
    m_panel_backgrounds.append(sf::Vertex(sf::Vector2f(
                    dims2.position.x+dims2.size.x, dims2.position.y+dims2.size.y), grey3));
    m_panel_backgrounds.append(sf::Vertex(sf::Vector2f(
                    dims2.position.x, dims2.position.y+dims2.size.y), grey3));

} // sprmo::Application::init_panel_backgrounds

// **************************************************

void sprmo::Application::handle_mouse_button_released(
        sf::Event::MouseButtonEvent& e)
{
    m_editing_panel->handle_mouse_button_released(e);

} // sprmo::Application::handle_mouse_button_released

// **************************************************

void sprmo::Application::handle_text_entered(sf::Event::TextEvent& e)
{
    if (m_current_mode == Loading || m_current_mode == Saving
            || m_current_mode == Confirm_close || m_current_mode == Saving_prg)
    {
        m_status_bar->handle_text_entered(e);
    } // if

} // sprmo::Application::handle_text_entered

// **************************************************

// FIXME: refactor and implement proper error messages (via status bar).
// FIXME: what if trying to load in current file?!
void sprmo::Application::try_load_save(const std::string& filename)
{
    // FIXME: another hack!
    if (m_current_mode == Confirm_close)
    {
        handle_confirm_close(filename);
        return;
    } // if

    if (filename.empty())
    {
        std::cerr << "No filename specified!\n";
        m_current_mode = Editing;
        m_status_bar->reset();
        return;
    } // if

    std::string path( m_config_ref.get_projects_dir() );
    path.append("/");
    path.append(filename);

    if (m_current_mode == Loading)
    {
        bool ok = File_IO::load_project(path, m_project, m_palette_panel);
        m_current_mode = Editing;
        m_status_bar->reset();
        if (ok)
        {
            m_config_ref.set_last_file(filename);
            // Make sure editing panel refreshed.
            //m_editing_panel->handle_new_current_sprite();
            m_current_filename.assign(filename);
            m_preview_panel->refresh_size();
            refresh_title();
        } // if
    }
    else if (m_current_mode == Saving)
    {
        File_IO::save_project(path, m_project);
        m_current_mode = Editing;
        m_status_bar->reset();
        m_config_ref.set_last_file(filename);
        m_current_filename.assign(filename);
        m_project->handle_project_saved();
        refresh_title();
    }
    else if (m_current_mode == Saving_prg)
    {
        std::cout << "save PRG: " << path << "\n";
        /*File_IO::write_source_code(path, m_project,
                m_resolution.get_pixels_per_byte());*/
        File_IO::write_prg(path, *m_project);
        m_current_mode = Editing;
        m_status_bar->reset();
    }
    else    // PNG file!
    {
        // NOTE: '*' in front of shared_ptr object, m_project, returns a
        // REFERENCE to the pointed-to object.
        File_IO::write_png(path, *m_project);
        m_current_mode = Editing;
        m_status_bar->reset();
    } // if ... else

} // sprmo::Application::try_load_save

// **************************************************

void sprmo::Application::refresh_title()
{
    std::string s( m_project->unsaved_changes() ? "*" : "" );
    m_rwin_ref.setTitle(boost::str(boost::format("%s: %s%s%s")
                % PACKAGE_STRING % s % m_current_filename % s));

} // sprmo::Application::refresh_title

// **************************************************

void sprmo::Application::init_new_project()
{
    std::cout << "new project\n";
    // FIXME: check if there are unsaved changes and ask if user wants to save
    // them first!!!
    m_project->clear_all_sprites();
    //m_project->add_new_sprite(16, 16);
    m_project->add_new_sprite2(false);
    m_project->handle_new_file_loaded(); 
    m_current_filename.clear();
    m_preview_panel->refresh_size();
    refresh_title();

} // sprmo::Application::init_new_project

// **************************************************

// Visual feedback to user in window's title bar - '*' next to filename if
// there are unsaved changes.
void sprmo::Application::handle_project_changed()
{
    refresh_title();

} // sprmo::Application::handle_project_changed

// **************************************************

void sprmo::Application::handle_confirm_close(const std::string& response)
{
    // Get a lower-case copy of the response.
    std::locale loc;
    std::string response_copy(response);
    std::transform(response.begin(), response.end(), response_copy.begin(),
            [loc](char c){return std::tolower(c, loc);});

    // Only if response is 'y' or 'yes' will the application be closed.
    if (!response_copy.compare("y") || !response_copy.compare("yes"))
    {
        really_close();
    }
    else
    {
        m_current_mode = Editing;
        m_status_bar->reset();
    } // if ... else

} // sprmo::Application::handle_confirm_close

// **************************************************

void sprmo::Application::really_close()
{
    std::cout << "Goodbye!\n";
    m_rwin_ref.close();

} // sprmo::Application::really_close

// **************************************************

void sprmo::Application::write_prg_or_png(bool alt)
{
    // Only allow this operation if m_current_filename contains something.
    if (m_current_filename.empty())
    {
        std::cerr << "Need a saved project first!\n";
    }
    else
    {
        // Create a copy of the current filename minus the 'xml' suffix.
        size_t len = m_current_filename.size();
        std::string s( m_current_filename.substr(0, len-3) );
        // Then append the appropriate suffix.
        if (alt)
        {
            //std::cout << "write png!\n";
            s.append("png");
            m_current_mode = Saving_png_file;
            m_status_bar->init_write_png(s);
        }
        else
        {
            s.append("prg");
            m_current_mode = Saving_prg;
            m_status_bar->init_write_prg(s);
        } // if ... else
    } // if ... else

} // sprmo::Application::write_prg_or_png

// **************************************************

// sprmo::Application::
//{
//} // sprmo::Application::

// **************************************************

// sprmo::Application::
//{
//} // sprmo::Application::

// **************************************************

// sprmo::Application::
//{
//} // sprmo::Application::

// **************************************************

// sprmo::Application::
//{
//} // sprmo::Application::

// **************************************************

// sprmo::Application::
//{
//} // sprmo::Application::

// **************************************************

// sprmo::Application::
//{
//} // sprmo::Application::

// **************************************************

// sprmo::Application::
//{
//} // sprmo::Application::

// **************************************************

// sprmo::Application::
//{
//} // sprmo::Application::







