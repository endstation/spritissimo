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




#ifndef Undo_stack_h
#define Undo_stack_h

#include <deque>
#include <vector>
#include <cstdlib>  // for size_t
#include <iostream>




namespace sprmo
{

class Undo_stack
{

    public:

        const size_t Max_size = 10;

        Undo_stack();
        ~Undo_stack();

        void clear_stacks();
        void push_undo(const std::vector<int>& data);
        void push_redo(const std::vector<int>& data);
        const std::vector<int>& back_undo() const;
        const std::vector<int>& back_redo() const;
        void pop_undo(); 
        void pop_redo();
        bool empty_undo() const { return m_undo_stack.empty(); }
        bool empty_redo() const { return m_redo_stack.empty(); }
        void clear_redo() { m_redo_stack.clear(); }

    protected:

    private:
            
        std::deque<std::vector<int>>    m_undo_stack;
        std::deque<std::vector<int>>    m_redo_stack;

}; // class Undo_stack

} // namespace sprmo




#endif
