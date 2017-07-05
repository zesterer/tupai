//
// file : vfs.hpp
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

#ifndef TUPAI_FS_VFS_HPP
#define TUPAI_FS_VFS_HPP

// Tupai
#include <tupai/fs/com.hpp>
#include <tupai/fs/inode.hpp>
#include <tupai/fs/fs.hpp>

// Standard
#include <stddef.h>
#include <stdint.h>

namespace tupai
{
	namespace fs
	{
		void vfs_init();
		void vfs_print();

		void     vfs_set_root(inode_t* inode);
		fs_t*    vfs_create_fs(const char* name);
		inode_t* vfs_create_inode(id_t id, inode_type type);
		inode_t* vfs_find_inode(id_t g_id);
	}
}

#endif
