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




#ifndef File_IO_h
#define File_IO_h

#include <string>
#include <iostream>
#include "Project.h"
#include "Palette_panel.h"
#include <fstream>
#include <tinyxml2.h>
#include <boost/tokenizer.hpp>
#include <boost/format.hpp>
#include <vector>
#include <cstdlib>
#include <png++/png.hpp>
#include <stdexcept>




namespace sprmo
{

class File_IO
{

    public:

        File_IO();
        ~File_IO();

        static void save_project(const std::string& path, 
                std::shared_ptr<Project> project);
        // Returns true if load successful or false if something went wrong.
        static bool load_project(const std::string& path,
                std::shared_ptr<Project> project,
                std::shared_ptr<Palette_panel> pal);
        static void write_source_code(const std::string& path,
                std::shared_ptr<Project> project, size_t pixels_per_byte);
        static void write_png(const std::string& path, Project& project);
        static void write_prg(const std::string& path, Project& project);

    protected:

    private:

}; // class File_IO

} // namespace sprmo




#endif

