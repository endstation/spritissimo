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




#ifndef Config_h
#define Config_h

#include <string>
#include <iostream>
#include <tinyxml2.h>
// FIXME: *nix only!
#include <unistd.h>
#include <boost/format.hpp>
#include <boost/filesystem.hpp>
#include <memory>
#include <cstdlib>
#include <boost/program_options.hpp>
#include <fstream>




namespace sprmo
{

class Config
{

    public:

        Config(boost::program_options::variables_map& vm);
        ~Config();

        const std::string& get_last_file() const { return *m_last_file; }
        const std::string& get_projects_dir() const { return *m_projects_dir; }
        void set_last_file(const std::string& file);
        int get_window_width() const { return m_window_width; }
        int get_cell_size() const { return m_cell_size; }

    protected:

    private:

        void init_dotdir();
        void load();
        void save();
        void check_cmd_line_options(boost::program_options::variables_map& vm,
                int* ww, int* csz);

        std::shared_ptr<std::string>    m_projects_dir;
        std::shared_ptr<std::string>    m_dotdir;
        std::shared_ptr<std::string>    m_config_file_path;
        std::shared_ptr<std::string>    m_last_file;
        int                             m_window_width;
        int                             m_cell_size;
        // 'Changes' refers here to the config file, not any Spritissimo 
        // project.
        bool                            m_unsaved_changes;

}; // class Config

} // namespace sprmo




#endif

