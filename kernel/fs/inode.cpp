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
// but WITHOUT ANY WARRANTY without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with Tupai.  If not, see <http://www.gnu.org/licenses/>.
//

// Tupai
#include <tupai/fs/inode.hpp>
#include <tupai/fs/vfs.hpp>

#include <tupai/util/str.hpp>
#include <tupai/util/out.hpp>

#include <tupai/panic.hpp>

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

		void inode_add_child(id_t id, id_t child, const char* name)
		{
			inode_child_t child_obj;
			child_obj.inode = child;
			child_obj.set_name(name);
			vfs_get_inode(id).dir_table.push(child_obj);
		}

		id_t inode_get_child(id_t id, const char* name)
		{
			for (size_t i = 0; i < vfs_get_inode(id).dir_table.size(); i ++)
			{
				inode_child_t& cchild = vfs_get_inode(id).dir_table[i];

				if (util::str_equal(cchild.get_name(), name) == true)
					return cchild.inode;
			}

			return ID_INVALID;
		}

		const char* inode_get_child_name(id_t id, id_t child)
		{
			for (size_t i = 0; i < vfs_get_inode(id).dir_table.size(); i ++)
			{
				inode_child_t& cchild = vfs_get_inode(id).dir_table[i];

				if (cchild.inode == child)
					return cchild.get_name();
			}

			return "null";
		}

		id_t inode_get_nth_child(id_t id, size_t n)
		{
			if (n < vfs_get_inode(id).dir_table.size())
				return vfs_get_inode(id).dir_table[n].inode;
			else
				return ID_INVALID;
		}

		size_t inode_child_count(id_t id)
		{
			return vfs_get_inode(id).dir_table.size();
		}

		vtable_t* inode_get_vtable(id_t id)
		{
			return vfs_get_inode(id).vtable;
		}

		void inode_set_vtable(id_t id, vtable_t* vtable)
		{
			vfs_get_inode(id).vtable = vtable;
		}

		inode_type inode_get_type(id_t id)
		{
			return vfs_get_inode(id).type;
		}

		void inode_set_fs(id_t id, id_t fs)
		{
			vfs_get_inode(id).fs = fs;
		}
	}
}
