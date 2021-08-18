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




#include "File_IO.h"




// *******************
// *** CONSTRUCTOR ***
// *******************

sprmo::File_IO::File_IO()
{
    // NYI...

} // sprmo::File_IO::File_IO

// ******************
// *** DESTRUCTOR ***
// ******************

sprmo::File_IO::~File_IO()
{
    // NYI...

} // sprmo::File_IO::~File_IO




// ************************
// *** MEMBER FUNCTIONS ***
// ************************

// **************************************************

void sprmo::File_IO::save_project(const std::string& path, 
        std::shared_ptr<Project> project)
{
    std::ofstream fout(path);
    fout << "<?xml version=\"1.0\"?>\n";
    fout << "<spritissimo64>\n";
    
    //auto v = project->get_palette();
    fout << "  <palette bg=\"" << project->get_bg() << "\" spmc0=\"" <<
        project->get_spmc0() << "\" spmc1=\"" << project->get_spmc1() <<
        "\" />\n";

    size_t n = project->get_num_sprites();
    for (size_t i = 0; i < n; ++i)
    {
        auto spr = project->get_sprite(i);
        auto pixels = spr->get_pixel_data_copy();
        fout << "  <sprite color=\"" << spr->get_color() <<
            "\" multicolor=\"" << spr->is_multicolor() << "\">\n";
        fout << "    <data>";
        for (auto x : *pixels)
        {
            fout << x << " ";
        } // for
        fout << "</data>\n";
        fout << "  </sprite>\n";
    } // for 

    fout << "</spritissimo64>\n\n";
    fout.close();

} // sprmo::File_IO::save_project


// **************************************************

bool sprmo::File_IO::load_project(
        const std::string& path,
        std::shared_ptr<Project> project,
        std::shared_ptr<Palette_panel> pal)
{
    tinyxml2::XMLDocument doc;
    tinyxml2::XMLError err = doc.LoadFile(path.c_str());
    if (err)
    {
        std::cerr << "Cannot load XML file: " << err << "\n";
        return false;
    } // if

    // Palette settings first.
    tinyxml2::XMLElement* el = doc.FirstChildElement("spritissimo64")->
        FirstChildElement("palette");
    int bg = el->IntAttribute("bg");
    int spmc0 = el->IntAttribute("spmc0");
    int spmc1 = el->IntAttribute("spmc1");
    pal->reset_global(bg, spmc0, spmc1);

    project->clear_all_sprites();
    // Get pointer to the first 'sprite' element.
    el = doc.FirstChildElement("spritissimo64")->FirstChildElement("sprite");
    do
    {
        int color = el->IntAttribute("color");
        bool multicolor = el->IntAttribute("multicolor");
        std::string data_str( el->FirstChildElement()->GetText() );
        std::vector<int> data;
        boost::tokenizer<> tok(data_str);
        for (auto x : tok) 
        {
            data.push_back(atoi(x.c_str()));
        } // for
        //std::cout << "got one! " << color << "\n";
        project->add_new_sprite2(multicolor, color, data);

    } while ((el = el->NextSiblingElement()));
    
    // Notify Project that a new file has been loaded so it can reset
    // everything ready for editing.
    project->handle_new_file_loaded();
    return true;

} // sprmo::File_IO::load_project

// **************************************************

void sprmo::File_IO::write_source_code(const std::string& path,
        std::shared_ptr<Project> project, size_t pixels_per_byte)
{
    project->calc_source_code_bytes();
    std::ofstream fout(path);
    
    // NOTE: each sprite can be associated with several 'sprite data' blocks
    // if it's pre-shifted.
    //auto data_block = 0;
    size_t n = project->get_num_sprites();
    for (size_t i = 0; i < n; ++i)
    {
        auto spr = project->get_sprite(i);
        size_t w = spr->get_width();
        // Adjust pixel width if sprite is preshifted and/or masked.
        /*
        if (spr->must_preshift())
        {
            w += pixels_per_byte;
        } // if
        if (spr->is_mask_enabled())
        {
            w *= 2;
        } // if
        */
        size_t bytes_per_row = w / pixels_per_byte;
        size_t total_bytes = bytes_per_row * spr->get_height();
        auto bytes = spr->get_bytes();
        size_t num_shifts = bytes.size() / total_bytes;

        // Information pre-amble for each sprite.  Actual data follows.
        // NOTE: format for label is: spr_data_<sprite#>
        fout << boost::str( boost::format("spr_data_%03d\n") % i );
        fout << boost::str( boost::format("; Sprite #%d\n") % i );
        fout << boost::str( boost::format(";   dimensions: %d*%d\n") %
                spr->get_width() % spr->get_height());
        /*
        fout << boost::str( boost::format(";   bytes per row on screen: %d\n") % 
                (spr->is_mask_enabled() ? bytes_per_row/2 : bytes_per_row) );
        */
        fout << boost::str( boost::format(";   bytes per 'frame': %d\n") %
                total_bytes);
        /*
        if (spr->must_preshift())
        {
            fout << ";   *** pre-shifted ***\n";
        } // if
        if (spr->is_mask_enabled())
        {
            fout << ";   *** masks interleaved ***\n";
        } // if
        */

        for (size_t shift = 0; shift < num_shifts; ++shift)
        {
            fout << boost::str( boost::format("; ->shift #%d (%d)\n") %
                    shift % i );
            for (size_t row = 0; row < spr->get_height(); ++row)
            {
                // Use Boost Format to build correct label for each block
                // of data bytes.
                fout << "  fcb  ";
                for (size_t b = 0; b < bytes_per_row; ++b)
                {
                    if (b > 0) { fout << ","; }
                    fout << static_cast<int>(
                            bytes[ shift*total_bytes+row*bytes_per_row+b ]);
                } // for
                fout << "\n";
            } // for
        } // for
        fout << "\n\n";
    } // for

    fout.close();

} // sprmo::File_IO::write_source_code

// **************************************************

void sprmo::File_IO::write_png(const std::string& path, Project& project)
{
    // FIXME: should get colors from Resolution object.
    png::rgba_pixel pixel_colors[] = {
        png::rgba_pixel(8,255,8,255),
        png::rgba_pixel(255,255,66,255),
        png::rgba_pixel(33,16,181,255),
        png::rgba_pixel(181,0,33,255),
        png::rgba_pixel(0,0,0,0)
    };

    // FIXME: we assume maximum height is 16...
    size_t h = 16;
    size_t w = 0;
    // Work out total width of full set of sprites.
    size_t n = project.get_num_sprites();
    for (size_t i = 0; i < n; ++i)
    {
        w += project.get_sprite(i)->get_width();
    } // for
    
    // Create a PNG image of the appropriate size.
    png::image<png::rgba_pixel> mypng(w, h);
    size_t current_x = 0;
    for (size_t i = 0; i < n; ++i)
    {
        std::shared_ptr<std::vector<int>> pixels =
            project.get_sprite(i)->get_pixel_data_copy();
        size_t spr_w = project.get_sprite(i)->get_width();
        size_t spr_x = current_x;
        size_t spr_y = 0;
        for (auto p : *pixels)
        {
            // Pixel colors for mask are in slot #4 of our array (see above).
            //if (p == Sprite::Mask_on) { p = 4; }
            mypng.set_pixel(spr_x, spr_y, pixel_colors[p]); 
            // Go to the next pixel in the png image, resetting x and
            // incrementing y when reach end of row.
            ++spr_x;
            if (spr_x == current_x + spr_w)
            {
                spr_x = current_x;
                ++spr_y;
            } // if
        } // for

        current_x += spr_w;
    } // for

    mypng.write(path);

} // sprmo::File_IO::write_png

// **************************************************

void sprmo::File_IO::write_prg(const std::string& path, Project& project)
{
    // Extract hex destination address from project name.  Should be last four
    // characters, separated by an underscore.
    size_t pos = path.find_last_of('_');
    if (pos == std::string::npos)
    {
        std::cerr << "Cannot find destination address: " << path << "\n";
        return;
    } // if

    int dest_addr = 0;
    try
    {
        dest_addr = std::stoi( path.substr(pos+1, 4), nullptr, 16 );
    }
    catch (const std::logic_error& e)
    {
        std::cerr << "Cannot parse hex!\n";
        return;
    } // try ... catch

    std::cout << "dest_addr=" << dest_addr << "\n";

    auto data_ptr = project.write_prg_data(dest_addr);

    std::string filename( path.substr(0, pos) );
    std::ofstream fout(filename, std::ios_base::out|std::ios_base::binary);

    for (auto x : *data_ptr)
    {
        fout.put( static_cast<char>(x) );
    } // for

    fout.close();

} // sprmo::File_IO::write_prg

// **************************************************

// sprmo::File_IO::
// {
// } // sprmo::File_IO::

// **************************************************

// sprmo::File_IO::
// {
// } // sprmo::File_IO::

// **************************************************

// sprmo::File_IO::
// {
// } // sprmo::File_IO::

// **************************************************

// sprmo::File_IO::
// {
// } // sprmo::File_IO::

// **************************************************

// sprmo::File_IO::
// {
// } // sprmo::File_IO::

// **************************************************

// sprmo::File_IO::
// {
// } // sprmo::File_IO::

// **************************************************

// sprmo::File_IO::
// {
// } // sprmo::File_IO::

// **************************************************

// sprmo::File_IO::
// {
// } // sprmo::File_IO::

// **************************************************

// sprmo::File_IO::
// {
// } // sprmo::File_IO::

// **************************************************

// sprmo::File_IO::
// {
// } // sprmo::File_IO::

// **************************************************

// sprmo::File_IO::
// {
// } // sprmo::File_IO::

// **************************************************

// sprmo::File_IO::
// {
// } // sprmo::File_IO::

// **************************************************

// sprmo::File_IO::
// {
// } // sprmo::File_IO::

// **************************************************

// sprmo::File_IO::
// {
// } // sprmo::File_IO::

// **************************************************

// sprmo::File_IO::
// {
// } // sprmo::File_IO::

// **************************************************

// sprmo::File_IO::
// {
// } // sprmo::File_IO::

// **************************************************

// sprmo::File_IO::
// {
// } // sprmo::File_IO::

// **************************************************

// sprmo::File_IO::
// {
// } // sprmo::File_IO::

// **************************************************

// sprmo::File_IO::
// {
// } // sprmo::File_IO::

// **************************************************

// sprmo::File_IO::
// {
// } // sprmo::File_IO::

// **************************************************

// sprmo::File_IO::
// {
// } // sprmo::File_IO::

// **************************************************

// sprmo::File_IO::
// {
// } // sprmo::File_IO::

// **************************************************

// sprmo::File_IO::
// {
// } // sprmo::File_IO::

// **************************************************

// sprmo::File_IO::
// {
// } // sprmo::File_IO::

// **************************************************

// sprmo::File_IO::
// {
// } // sprmo::File_IO::

