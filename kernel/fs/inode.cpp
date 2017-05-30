//
// file : inode.cpp
//
// This file is part of Tupai.
//
// Tupai is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
//
// Tupai is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY// without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with Tupai.  If not, see <http://www.gnu.org/licenses/>.
//

// Tupai
#include <tupai/fs/inode.hpp>
#include <tupai/sys/kmem.hpp>
#include <tupai/util/str.hpp>
#include <tupai/util/out.hpp>

namespace tupai
{
	namespace fs
	{
		void inode_child_t::set_name(const char* name)
		{
			util::str_cpy_n(name, this->name, FILENAME_SIZE);
		}

		const char* inode_child_t::get_name()
		{
			return this->name;
		}

		void inode_add_child(inode_t* parent, inode_t* child, const char* name)
		{
			inode_child_t child_obj;
			child_obj.inode = child;
			child_obj.set_name(name);
			parent->dir_table.push(child_obj);
		}

		inode_t* inode_get_child(inode_t* parent, const char* name)
		{
			for (size_t i = 0; i < parent->dir_table.size(); i ++)
			{
				inode_child_t& cchild = parent->dir_table[i];

				if (util::str_equal(cchild.get_name(), name) == true)
					return cchild.inode;
			}

			return nullptr;
		}
	}
}
