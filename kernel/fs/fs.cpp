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
// but WITHOUT ANY WARRANTY// without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with Tupai.  If not, see <http://www.gnu.org/licenses/>.
//

// Tupai
#include <tupai/fs/fs.hpp>
#include <tupai/fs/vfs.hpp>

namespace tupai
{
	namespace fs
	{
		inode_t* fs_create_inode(fs_t* fs, inode_type type)
		{
			fs->inode_id_counter ++; // Increment id counter
			inode_t* ninode = vfs_create_inode(fs->inode_id_counter);
			ninode->type = type;
			return ninode;
		}
	}
}
