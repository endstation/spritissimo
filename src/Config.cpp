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




#include "Config.h"




// *******************
// *** CONSTRUCTOR ***
// *******************

sprmo::Config::Config(boost::program_options::variables_map& vm) : 
    m_unsaved_changes(false)
{
    // Initial values of (-1) - unless overwritten (via command-line options),
    // means use whatever's in config file.
    int ww = (-1);
    int csz = (-1);
    check_cmd_line_options(vm, &ww, &csz);
    init_dotdir();
    load();
    // Overwrite window/cell dimensions if values were specified on command line.
    // These will be written back to the config file when application is closed.
    if (ww > 0)     { m_window_width = ww; }
    if (csz > 0)    { m_cell_size = csz; }
    if (ww > 0 || csz > 0) { m_unsaved_changes = true; }

} // sprmo::Config::Config()


// ******************
// *** DESTRUCTOR ***
// ******************

sprmo::Config::~Config()
{
    save();

} // sprmo::Config::~Config




// ************************
// *** MEMBER FUNCTIONS ***
// ************************

// **************************************************

void sprmo::Config::init_dotdir()
{
    //std::string login( getlogin() );
    std::string login("matthew");

    m_dotdir = std::shared_ptr<std::string>(new std::string(
            boost::str(boost::format("/home/%s/.%s") % login %
                PACKAGE_TARNAME)));
    m_config_file_path = std::shared_ptr<std::string>(new std::string(
            *m_dotdir));
    m_config_file_path->append("/config.xml");

    boost::filesystem::path p { *m_config_file_path };
    if ( !boost::filesystem::exists(p) )
    {
        // Create dotdir and copy default config file there.
        boost::filesystem::create_directory(*m_dotdir);
        boost::filesystem::path default_config_file(
                std::string(PKG_DATA_DIR "/resolutions/config.xml"));
        boost::filesystem::copy_file(default_config_file, *m_config_file_path);

        // Create default projects dir.
        m_projects_dir = std::shared_ptr<std::string>(new std::string(
                boost::str(boost::format("/home/%s/spritissimo_projects") 
                    % login)));
        boost::filesystem::create_directory(*m_projects_dir);
        m_unsaved_changes = true;
    } // if ... else

} // sprmo::Config::init_dotdir

// **************************************************

void sprmo::Config::load()
{
    tinyxml2::XMLDocument doc;
    tinyxml2::XMLError err = doc.LoadFile(m_config_file_path->c_str());
    if (err)
    {
        std::cerr << "Cannot load config file: " << 
                *m_config_file_path << "\n";
        exit(EXIT_FAILURE);
    } // if

    // If we got this far, assume 'projects_dir' exists.  But still need to
    // check for existence of 'last_file'.
    const char* s = doc.FirstChildElement("config")->FirstChildElement(
            "projects_dir")->GetText();
    if (!s) { s = ""; }
    m_projects_dir = std::shared_ptr<std::string>(new std::string(s));
    s = doc.FirstChildElement("config")->FirstChildElement("last_file")->
            GetText();
    if (!s) { s = ""; }
    m_last_file = std::shared_ptr<std::string>(new std::string(s));

    auto el = doc.FirstChildElement("config")->FirstChildElement("window_width");
    //int ww, cs;
    el->QueryIntText(&m_window_width);
    el = doc.FirstChildElement("config")->FirstChildElement("cell_size");
    el->QueryIntText(&m_cell_size);
    std::cout << "ww=" << m_window_width << "; cs=" << m_cell_size << "\n";

} // sprmo::Config::load

// **************************************************

void sprmo::Config::save()
{
    if (!m_unsaved_changes) { return; }

    tinyxml2::XMLDocument doc;
    tinyxml2::XMLError err = doc.LoadFile(m_config_file_path->c_str());
    if (err)
    {
        std::cerr << "Cannot load config file: " << 
                *m_config_file_path << "\n";
        exit(EXIT_FAILURE);
    } // if

    tinyxml2::XMLElement* el = doc.FirstChildElement("config")->
        FirstChildElement("projects_dir");
    el->SetText(m_projects_dir->c_str());
    el = doc.FirstChildElement("config")->FirstChildElement("last_file");
    el->SetText(m_last_file->c_str());
    el = doc.FirstChildElement("config")->FirstChildElement("window_width");
    el->SetText(m_window_width);
    el = doc.FirstChildElement("config")->FirstChildElement("cell_size");
    el->SetText(m_cell_size);
    doc.SaveFile(m_config_file_path->c_str());

} // sprmo::Config::save

// **************************************************

void sprmo::Config::set_last_file(const std::string& file)
{
    if (file.compare(*m_last_file))
    {
        //std::cout << "changed!\n";
        m_last_file->assign(file);
        m_unsaved_changes = true;
    }
    else
    { 
        //std::cout << "the same!\n";
    } // if ... else

} // sprmo::Config::set_last_file

// **************************************************

// TODO: check whether values are valid.
void sprmo::Config::check_cmd_line_options(
        boost::program_options::variables_map& vm, int* ww, int* csz)
{
    if (vm.count("ww"))
    {
        std::cout << "ww=" << vm["ww"].as<int>() << "\n";
        *ww = vm["ww"].as<int>();
    } // if
    if (vm.count("csz"))
    {
        std::cout << "csz=" << vm["csz"].as<int>() << "\n";
        *csz = vm["csz"].as<int>();
    } // if

} // sprmo::Config::check_cmd_line_options

// **************************************************

// sprmo::Config::
// {
// } // sprmo::Config::

// **************************************************

// sprmo::Config::
// {
// } // sprmo::Config::

// **************************************************

// sprmo::Config::
// {
// } // sprmo::Config::

// **************************************************

// sprmo::Config::
// {
// } // sprmo::Config::

// **************************************************

// sprmo::Config::
// {
// } // sprmo::Config::

// **************************************************

// sprmo::Config::
// {
// } // sprmo::Config::

// **************************************************

// sprmo::Config::
// {
// } // sprmo::Config::

// **************************************************

// sprmo::Config::
// {
// } // sprmo::Config::

// **************************************************

// sprmo::Config::
// {
// } // sprmo::Config::

// **************************************************

// sprmo::Config::
// {
// } // sprmo::Config::

