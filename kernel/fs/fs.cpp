//
// file : fs.cpp
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
#include <tupai/fs/fs.hpp>
#include <tupai/fs/vfs.hpp>
#include <tupai/fs/inode.hpp>

#include <tupai/util/str.hpp>

namespace tupai
{
	namespace fs
	{
		fs_t::fs_t(const char* name)
		{
			if (name != nullptr)
				util::str_cpy_n(name, this->name, MAX_FS_NAME);
		}

		id_t fs_create_inode(id_t fs, inode_type type, vtable_t* vtable)
		{
			id_t ninode = vfs_create_inode(type);
			inode_set_fs(ninode, fs);

			if (type != inode_type::DIRECTORY)
				inode_set_vtable(ninode, vtable);

			return ninode;
		}

		id_t fs_get_root(id_t id)
		{
			return vfs_get_fs(id).root;
		}
	}
}
