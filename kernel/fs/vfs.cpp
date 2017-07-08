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
#include <tupai/fs/file.hpp>

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

		util::table_t<fs_t   , util::spinlock_t> fs;
		util::table_t<inode_t, util::spinlock_t> inodes;
		util::table_t<file_t , util::spinlock_t> files;

		id_t root = ID_INVALID;

		id_t fs_counter    = 0;
		id_t inode_counter = 0;
		id_t file_counter  = 0;

		fs_t& vfs_get_fs(id_t id)
		{
			return fs[id];
		}

		inode_t& vfs_get_inode(id_t id)
		{
			return inodes[id];
		}

		file_t& vfs_get_file(id_t id)
		{
			return files[id];
		}

		id_t vfs_get_root()
		{
			return root;
		}

		id_t vfs_get_inode(const char* path)
		{
			size_t element_count = path_element_count(path);
			id_t cinode = root;
			for (size_t i = 0; i < element_count; i ++)
			{
				char element[FILENAME_SIZE];
				path_element_get(path, element, i);

				cinode = inode_get_child(cinode, element);

				if (cinode == ID_INVALID)
					break;
			}
			return cinode;
		}

		void vfs_set_root(id_t inode)
		{
			root = inode;
		}

		id_t vfs_create_fs(const char* name)
		{
			fs_t nfs(name);
			nfs.id = fs_counter++;

			// Create a root inode for the filesystem
			id_t nroot = vfs_create_inode(inode_type::DIRECTORY);
			nfs.root = nroot;

			fs.add(nfs.id, nfs);
			return nfs.id;
		}

		id_t vfs_create_inode(inode_type type)
		{
			inode_t ninode;
			ninode.type = type;
			ninode.id = inode_counter++;

			inodes.add(ninode.id, ninode);
			return ninode.id;
		}

		void vfs_init()
		{
			spinlock.lock(); // Begin critical section

			//fs     = util::table_t<id_t, fs_t   , util::spinlock_t>();
			//inodes = util::table_t<id_t, inode_t, util::spinlock_t>();
			//files  = util::table_t<id_t, file_t , util::spinlock_t>();

			spinlock.unlock(); // End critical section
		}

		void vfs_print_inode(id_t inode, const char* name, size_t depth = 0)
		{
			if (inode == ID_INVALID)
				return;

			// Indentation
			for (size_t i = 0; i < depth; i ++)
				util::print((i == depth - 1) ? "|--" : "|  ");
			// Name
			//util::print('[', inode->id, "] ", name, (inode->type == inode_type::DIRECTORY) ? "/" : "", '\n');
			util::print(name, (inode_get_type(inode) == inode_type::DIRECTORY) ? "/" : "", '\n');

			for (size_t i = 0; i < inode_child_count(inode); i ++)
			{
				id_t ninode = inode_get_nth_child(inode, i);
				vfs_print_inode(ninode, inode_get_child_name(inode, ninode), depth + 1);
			}
		}

		void vfs_display()
		{
			util::println("--- Virtual Filesystem ---");
			vfs_print_inode(root, "");

			util::print('\n');

			util::println("--- Filesystem Devices ---");
			for (size_t i = 0; i < fs.size(); i ++)
				util::println(fs.nth(i).id, " : ", fs.nth(i).name);
		}
	}
}
