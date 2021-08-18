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




#include <SFML/Graphics.hpp>
#include <cstdlib>
#include "Application.h"
#include "Globals.h"
#include <iostream>
#include "Panel_dims.h"
//#include "Sprite_dims.h"
#include <vector>
#include <memory>
#include "Sprite.h"
#include "Sprite_dims.h"
#include <boost/program_options.hpp>
#include "Config.h"




namespace po = boost::program_options;


float winwf;
float winhf;
void build_app_dimensions(const sprmo::Config&);
std::vector<sprmo::Panel_dims> dims;
//std::shared_ptr<sprmo::Sprite_dims> sprite_dims_ptr;
size_t font_size;
//size_t cell_size;

int main(int argc, char* argv[])
{
    po::options_description desc("Allowed options");
    desc.add_options()
        ("help", "display options and quit")
        ("ww", po::value<int>(), "window width in pixels")
        ("csz", po::value<int>(), "cell size in pixels");
    po::variables_map vm;
    po::store(po::parse_command_line(argc, argv, desc), vm);
    po::notify(vm);


    // Check for presence of options:
    if (vm.count("help"))
    {
        std::cout << desc << "\n";
        return EXIT_SUCCESS;
    } // if

    /*
    if (vm.count("ww"))
    {
        std::cout << "ww => " << vm["ww"].as<int>() << "\n";
    } // if
    if (vm.count("csz"))
    {
        std::cout << "csz => " << vm["csz"].as<int>() << "\n";
    } // if
    */
    sprmo::Config myconfig(vm);

    //return EXIT_SUCCESS;
    /*
    if (argc != 3)
    {
        std::cerr << "Usage: spritissimo64 <window width> <cell size>\n";
        exit(EXIT_FAILURE);
    } // if

    int window_w = atoi(argv[1]);
    cell_size = atoi(argv[2]);
    std::cout << "window_w=" << window_w << "\n";
    std::cout << "cell_size=" << cell_size << "\n";
    */
    build_app_dimensions(myconfig);

    int winw_as_int = static_cast<int>(winwf);
    int winh_as_int = static_cast<int>(winhf);

    /*sf::RenderWindow rwin(sf::VideoMode(sprmo::Globals::Window_w,
            sprmo::Globals::Window_h), PACKAGE_STRING);*/
    sprmo::Sprite_dims sprdims( myconfig.get_cell_size() );
    sf::RenderWindow rwin(sf::VideoMode(winw_as_int, winh_as_int),
            PACKAGE_STRING);
    rwin.setVerticalSyncEnabled(true);
    rwin.setKeyRepeatEnabled(false);
    sprmo::Application my_app(rwin, dims, font_size, sprdims, myconfig);
    my_app.start();

    return EXIT_SUCCESS;

} // main

void build_app_dimensions(const sprmo::Config& cfg)
{
    int winw = cfg.get_window_width();
    int clsz = cfg.get_cell_size();

    // Must be a multiple of 50.
    if (winw % 50)
    {
        std::cerr << "Window width must be a multiple of 50: " << winw << "\n";
        exit(EXIT_FAILURE);
    } // if

    winwf = winw;
    winhf = 0;

    // Sprite cell size.
    /*
    float cell_size = winwf/(24*4);
    std::cout << "cell_size=" << cell_size << "\n";
    sprite_dims_ptr = std::shared_ptr<sprmo::Sprite_dims>(
            new sprmo::Sprite_dims(winwf/100) );
    */

    // Editing panel:
    sf::Vector2f editpanel_size(winwf/2, winwf/2);
    sf::Vector2f editpanel_center(winwf/4, winwf/4);
    winhf += editpanel_size.y;

    // Animation panel:
    sf::Vector2f animpanel_size(winwf/2, winwf/2);
    sf::Vector2f animpanel_center(winwf/4, winwf/4);

    // Editing status panel:
    sf::Vector2f editstatus_size(winwf/4, winwf/16);
    sf::Vector2f editstatus_center(winwf/8, winwf/32);
    winhf += editstatus_size.y;

    // Palette panel:
    sf::Vector2f palettepanel_size(winwf/4, winwf/16);
    sf::Vector2f palettepanel_center(winwf/8, winwf/32);

    // Animation status panel:
    sf::Vector2f animstatus_size(winwf/2, winwf/16);
    sf::Vector2f animstatus_center(winwf/4, winwf/32);

    // Preview panel:
    sf::Vector2f previewpanel_size(winwf, 23*clsz);
    sf::Vector2f previewpanel_center(previewpanel_size.x/2,
            previewpanel_size.y/2);
    winhf += previewpanel_size.y;

    // Status bar:
    sf::Vector2f statusbar_size(winwf, winwf/16);
    sf::Vector2f statusbar_center(statusbar_size.x/2, statusbar_size.y/2);
    winhf += statusbar_size.y;

    sf::FloatRect editpanel_viewport(0, 0, editpanel_size.x/winwf,
            editpanel_size.y/winhf);
    sf::FloatRect animpanel_viewport(animpanel_size.x/winwf, 0,
            animpanel_size.x/winwf, animpanel_size.y/winhf);
    sf::FloatRect editstatus_viewport(0, editpanel_size.y/winhf,
            editstatus_size.x/winwf, editstatus_size.y/winhf);
    sf::FloatRect palettepanel_viewport(editstatus_size.x/winwf,
            editpanel_size.y/winhf, palettepanel_size.x/winwf,
            palettepanel_size.y/winhf);
    sf::FloatRect previewpanel_viewport(0,
            (editpanel_size.y + editstatus_size.y)/winhf,
            previewpanel_size.x/winwf, previewpanel_size.y/winhf);
    sf::FloatRect statusbar_viewport(0,
            (editpanel_size.y+editstatus_size.y+previewpanel_size.y)/winhf,
            statusbar_size.x/winwf, statusbar_size.y/winhf);
    sf::FloatRect animstatus_viewport(animpanel_size.x/winwf,
            animpanel_size.y/winhf, animstatus_size.x/winwf,
            animstatus_size.y/winhf);

    dims.push_back( sprmo::Panel_dims(editpanel_size, editpanel_center,
                editpanel_viewport, sf::Vector2f(0,0)) );
    dims.push_back( sprmo::Panel_dims(animpanel_size, animpanel_center,
                animpanel_viewport, sf::Vector2f(animpanel_size.x, 0)) );
    dims.push_back( sprmo::Panel_dims(editstatus_size, editstatus_center,
                editstatus_viewport, sf::Vector2f(0, editpanel_size.y)) );
    dims.push_back( sprmo::Panel_dims(palettepanel_size, palettepanel_center,
                palettepanel_viewport,
                sf::Vector2f(editstatus_size.x, editpanel_size.y)) );
    dims.push_back( sprmo::Panel_dims(previewpanel_size, previewpanel_center,
                previewpanel_viewport,
                sf::Vector2f(0, editpanel_size.y+editstatus_size.y)) );
    dims.push_back( sprmo::Panel_dims(statusbar_size, statusbar_center,
                statusbar_viewport,
                sf::Vector2f(0, editpanel_size.y+editstatus_size.y+previewpanel_size.y)) );
    dims.push_back( sprmo::Panel_dims(animstatus_size, animstatus_center,
                animstatus_viewport,
                sf::Vector2f(animpanel_size.x, animpanel_size.y)) );

    font_size = (int) (winwf * (24/800.0));
    //std::cout << "font_size=" << font_size << "\n";

    for (auto d : dims) d.display();

} // build_app_dimensions


