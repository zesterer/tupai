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
#include <tupai/fs/fs.hpp>
#include <tupai/fs/inode.hpp>
#include <tupai/fs/path.hpp>

#include <tupai/util/mutex.hpp>
#include <tupai/util/spinlock.hpp>
#include <tupai/util/vector.hpp>
#include <tupai/util/hashtable.hpp>
#include <tupai/util/out.hpp>

namespace tupai
{
	namespace fs
	{
		static util::spinlock_t spinlock;

		static util::hashtable_t<id_t, fs_t*>    fs;
		static util::hashtable_t<id_t, inode_t*> inodes;

		inode_t* root = nullptr;

		id_t g_inode_counter = 0;

		void vfs_init()
		{
			spinlock.lock(); // Begin critical section

			fs     = util::hashtable_t<id_t, fs_t*>();
			inodes = util::hashtable_t<id_t, inode_t*>();

			spinlock.unlock(); // End critical section
		}

		void vfs_set_root(inode_t* inode)
		{
			root = inode;
		}

		id_t vfs_get_root()
		{
			return root->gid;
		}

		fs_t* vfs_create_fs(const char* name)
		{
			fs_t* nfs = new fs_t(name);

			// Create a root inode for the filesystem
			inode_t* nroot = fs_create_inode(nfs, inode_type::DIRECTORY, nullptr);
			nfs->root = nroot;

			fs.add(nfs->id, nfs);

			return nfs;
		}

		inode_t* vfs_create_inode(id_t id, inode_type type)
		{
			id_t gid = ++g_inode_counter;
			inode_t* ninode = new inode_t(type);
			ninode->gid = gid;
			ninode->id = id;

			inodes.add(gid, ninode);

			return ninode;
		}

		inode_t* vfs_get_inode(id_t gid)
		{
			return inodes[gid];
		}

		inode_t* vfs_get_inode(const char* path)
		{
			size_t element_count = path_element_count(path);
			inode_t* cinode = root;
			for (size_t i = 0; i < element_count; i ++)
			{
				char element[FILENAME_SIZE];
				path_element_get(path, element, i);

				cinode = inode_get_child(cinode, element);

				if (cinode == nullptr)
					return nullptr;
			}
			return cinode;
		}

		void vfs_print_inode(inode_t* inode, const char* name, size_t depth = 0)
		{
			if (inode == nullptr)
				return;

			// Indentation
			for (size_t i = 0; i < depth; i ++)
				util::print((i == depth - 1) ? "|--" : "|  ");
			// Name
			//util::print('[', inode->id, "] ", name, (inode->type == inode_type::DIRECTORY) ? "/" : "", '\n');
			util::print(name, (inode->type == inode_type::DIRECTORY) ? "/" : "", '\n');

			for (size_t i = 0; i < inode->dir_table.size(); i ++)
			{
				inode_child_t* ninode = &inode->dir_table[i];
				vfs_print_inode(ninode->inode, ninode->get_name(), depth + 1);
			}
		}

		void vfs_display()
		{
			util::println("--- Virtual Filesystem ---");
			vfs_print_inode(root, "");

			util::print('\n');

			util::println("--- Filesystem Devices ---");
			for (size_t i = 0; i < fs.size(); i ++)
				util::println(fs.nth_key(i), " : ", fs.nth(i)->name);
		}
	}
}
