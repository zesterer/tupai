//
// file : vfs.cpp
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
#include <tupai/fs/vfs.hpp>
#include <tupai/util/mutex.hpp>
#include <tupai/util/vector.hpp>
#include <tupai/util/out.hpp>

namespace tupai
{
	namespace fs
	{
		struct inode_pair_t
		{
			inode_t* inode;
			id_t     id;

			inode_pair_t(inode_t* inode, id_t id)
			{
				this->inode = inode;
				this->id = id;
			}
		};

		util::mutex fs_mutex;

		util::vector_t<fs_t*>    active_fs;
		util::vector_t<inode_pair_t> active_inodes;
		inode_t*                 root_inode = nullptr;

		id_t g_inode_id = 0;

		static void vfs_print_inode(inode_t* inode, const char* name, size_t depth = 0);

		void vfs_init()
		{
			fs_mutex.lock(); // Begin critical section

			fs_mutex.unlock(); // End critical section
		}

		void vfs_print()
		{
			util::println("--- Virtual Filesystem ---");
			vfs_print_inode(root_inode, "");

			util::println("--- Filesystem Devices ---");
			//for (size_t i = 0; i < active_fs.size(); i ++)
			//	util::println(active_fs[i]->id, " : ", active_fs[i]->name);
		}

		void vfs_print_inode(inode_t* inode, const char* name, size_t depth)
		{
			if (inode == nullptr)
				return;

			// Indentation
			for (size_t i = 0; i < depth; i ++)
				util::print((i == depth - 1) ? "|--" : "|  ");
			// Name
			util::print('[', inode->id, "] ", name, (inode->type == inode_type::DIRECTORY) ? "/" : "", '\n');

			for (size_t i = 0; i < inode->dir_table.size(); i ++)
			{
				inode_child_t* ninode = &inode->dir_table[i];
				vfs_print_inode(ninode->inode, ninode->get_name(), depth + 1);
			}
		}

		void vfs_set_root(inode_t* inode)
		{
			root_inode = inode;
		}

		fs_t* vfs_create_fs(const char* name)
		{
			fs_t* nfs = new fs_t(name);
			inode_t* nroot = fs_create_inode(nfs, inode_type::DIRECTORY);
			nfs->root = nroot;

			active_fs.push(nfs);

			return nfs;
		}

		inode_t* vfs_create_inode(id_t id)
		{
			inode_t* ninode = new inode_t();
			ninode->id = id;

			id_t g_id = ++g_inode_id;
			active_inodes.push(inode_pair_t(ninode, g_id));

			return ninode;
		}

		inode_t* vfs_find_inode(id_t g_id)
		{
			for (size_t i = 0; i < active_inodes.size(); i ++)
			{
				if (active_inodes[i].id == g_id)
					return active_inodes[i].inode;
			}

			return nullptr;
		}
	}
}
