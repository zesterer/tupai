//
// file : initrd.cpp
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
#include <tupai/sys/initrd.hpp>
#include <tupai/util/out.hpp>
#include <tupai/util/tar.hpp>
#include <tupai/sys/thread.hpp>
#include <tupai/fs/vfs.hpp>

namespace tupai
{
	namespace sys
	{
		struct initrd_cache_t
		{
			void*       start = nullptr;
			size_t      size = 0;
			const char* args = nullptr;
		};

		static const size_t INITRD_MAX = 32;
		initrd_cache_t initrd_cache[INITRD_MAX];

		static void initrd_thread(int argc, char* argv[]);

		void initrd_cache_add(void* start, size_t size, const char* args)
		{
			// Search for empty cache
			for (size_t i = 0; i < INITRD_MAX; i ++)
			{
				if (initrd_cache[i].size == 0)
				{
					initrd_cache[i].start = start;
					initrd_cache[i].size  = size;
					initrd_cache[i].args  = args;
					break;
				}
			}
		}

		void initrd_init()
		{
			for (size_t i = 0; i < INITRD_MAX; i ++)
			{
				if (initrd_cache[i].size != 0)
				{
					util::println("Found initrd!");
					util::tar_print_all((util::tar_header_t*)initrd_cache[i].start);

					sys::thread_create(initrd_thread, 1, (char**)&initrd_cache[i], "initrd");
				}
			}
		}

		void initrd_thread(int argc, char* argv[])
		{
			const initrd_cache_t* initrd = (const initrd_cache_t*)argv;

			fs::fs_t* fs = fs::vfs_create_fs();
			fs::vfs_set_root(fs->root);

			fs::inode_t* inode0 = fs::fs_create_inode(fs, fs::inode_type::DIRECTORY);
			fs::inode_add_child(fs->root, inode0, "bin");
			fs::inode_t* inode1 = fs::fs_create_inode(fs, fs::inode_type::DIRECTORY);
			fs::inode_add_child(fs->root, inode1, "dev");
			fs::inode_t* inode2 = fs::fs_create_inode(fs, fs::inode_type::DIRECTORY);
			fs::inode_add_child(fs->root, inode2, "conf");

			fs::inode_t* inode3 = fs::fs_create_inode(fs, fs::inode_type::BLOCK_FILE);
			fs::inode_add_child(inode1, inode3, "tty");
			fs::inode_t* inode4 = fs::fs_create_inode(fs, fs::inode_type::BLOCK_FILE);
			fs::inode_add_child(inode1, inode4, "com1");

			fs::inode_t* inode5 = fs::fs_create_inode(fs, fs::inode_type::FIFO_BUFFER);
			fs::inode_add_child(inode2, inode5, "test.txt");
			fs::inode_t* inode6 = fs::fs_create_inode(fs, fs::inode_type::FIFO_BUFFER);
			fs::inode_add_child(inode2, inode6, "myfile.txt");

			while(1);
		}
	}
}
