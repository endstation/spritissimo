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




#include "Undo_stack.h"




// *******************
// *** CONSTRUCTOR ***
// *******************

sprmo::Undo_stack::Undo_stack()
{
    // NYI...

} // sprmo::Undo_stack::Undo_stack

// ******************
// *** DESTRUCTOR ***
// ******************

sprmo::Undo_stack::~Undo_stack()
{
    // NYI...

} // sprmo::Undo_stack::~Undo_stack




// ************************
// *** MEMBER FUNCTIONS ***
// ************************

// **************************************************

void sprmo::Undo_stack::clear_stacks()
{
    m_undo_stack.clear();
    m_redo_stack.clear();

} // sprmo::Undo_stack::clear_stacks

// **************************************************

// We'll add the new item in any case, but if undo stack is already at 
// maximum size, lose the oldest item.
void sprmo::Undo_stack::push_undo(const std::vector<int>& data)
{
    if (m_undo_stack.size() == Max_size)
    {
        m_undo_stack.pop_front();
    } // if
    m_undo_stack.push_back(data);

} // sprmo::Undo_stack::push_undo

// **************************************************

void sprmo::Undo_stack::pop_undo()
{
    m_undo_stack.pop_back();

} // sprmo::Undo_stack::pop_undo

// **************************************************

void sprmo::Undo_stack::push_redo(const std::vector<int>& data)
{
    if (m_redo_stack.size() == Max_size)
    {
        m_redo_stack.pop_front();
    } // if
    m_redo_stack.push_back(data);

} // sprmo::Undo_stack::push_redo

// **************************************************

void sprmo::Undo_stack::pop_redo()
{
    m_redo_stack.pop_back();

} // sprmo::Undo_stack::pop_redo

// **************************************************

const std::vector<int>& sprmo::Undo_stack::back_undo() const
{
    return m_undo_stack.back();

} // sprmo::Undo_stack::back_undo

// **************************************************

const std::vector<int>& sprmo::Undo_stack::back_redo() const
{
    return m_redo_stack.back();

} // sprmo::Undo_stack::back_redo

// **************************************************

// sprmo::Undo_stack::
// {
// } // sprmo::Undo_stack::

// **************************************************

// sprmo::Undo_stack::
// {
// } // sprmo::Undo_stack::

// **************************************************

// sprmo::Undo_stack::
// {
// } // sprmo::Undo_stack::

// **************************************************

// sprmo::Undo_stack::
// {
// } // sprmo::Undo_stack::

// **************************************************

// sprmo::Undo_stack::
// {
// } // sprmo::Undo_stack::

// **************************************************

// sprmo::Undo_stack::
// {
// } // sprmo::Undo_stack::

// **************************************************

// sprmo::Undo_stack::
// {
// } // sprmo::Undo_stack::

// **************************************************

// sprmo::Undo_stack::
// {
// } // sprmo::Undo_stack::

// **************************************************

// sprmo::Undo_stack::
// {
// } // sprmo::Undo_stack::

// **************************************************

// sprmo::Undo_stack::
// {
// } // sprmo::Undo_stack::

// **************************************************

// sprmo::Undo_stack::
// {
// } // sprmo::Undo_stack::

// **************************************************

// sprmo::Undo_stack::
// {
// } // sprmo::Undo_stack::

// **************************************************

// sprmo::Undo_stack::
// {
// } // sprmo::Undo_stack::
