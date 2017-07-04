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
// but WITHOUT ANY WARRANTY without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with Tupai.  If not, see <http://www.gnu.org/licenses/>.
//

// Tupai
#include <tupai/sys/initrd.hpp>
#include <tupai/sys/thread.hpp>
#include <tupai/sys/mmap.hpp>
#include <tupai/util/out.hpp>
#include <tupai/util/tar.hpp>
#include <tupai/fs/vfs.hpp>
#include <tupai/fs/path.hpp>

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
					//util::println("Found initrd!");
					//util::tar_print_all((util::tar_header_t*)initrd_cache[i].start);

					sys::thread_create(initrd_thread, 1, (char**)&initrd_cache[i], "initrd");
				}
			}
		}

		void initrd_thread(int argc, char* argv[])
		{
			(void)argc;

			const initrd_cache_t* initrd = (const initrd_cache_t*)argv;

			fs::fs_t* fs = fs::vfs_create_fs("initrd");
			fs::vfs_set_root(fs->root);

			// TODO : re-add this
			//mmap_reserve((size_t)initrd->start, (size_t)initrd->size, KERNEL_PROC_ID); // Reserve the memory

			util::tar_header_t* cheader = (util::tar_header_t*)initrd->start;
			while (cheader != nullptr)
			{
				const char* filename = cheader->filename;

				size_t n = fs::path_element_count(filename);
				fs::inode_t* cinode = fs->root;
				for (size_t i = 0; i < n; i ++)
				{
					char buff[256];
					fs::path_element_get(filename, buff, i);
					fs::inode_t* tryinode = fs::inode_get_child(cinode, buff);
					if (tryinode != nullptr)
						cinode = tryinode;
					else
					{
						fs::inode_type type = fs::inode_type::DIRECTORY;
						if (i + 1 == n)
						{
							switch (cheader->typeflag)
							{
							case '0':
								type = fs::inode_type::NORMAL_FILE;
								break;
							case '1':
								type = fs::inode_type::HARD_LINK;
								break;
							case '2':
								type = fs::inode_type::SYM_LINK;
								break;
							case '3':
								type = fs::inode_type::CHAR_DEVICE;
								break;
							case '4':
								type = fs::inode_type::BLOCK_DEVICE;
								break;
							case '6':
								type = fs::inode_type::FIFO;
								break;
							case '5':
							default:
								break;
							}
						}

						fs::inode_t* ninode = fs::fs_create_inode(fs, type);
						fs::inode_add_child(cinode, ninode, buff);
						cinode = ninode;
					}
				}

				cheader = tar_next(cheader);
			}

			while(1);
		}
	}
}
