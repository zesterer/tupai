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
#include <tupai/sys/mmap.hpp>
#include <tupai/sys/proc.hpp>

#include <tupai/fs/vfs.hpp>
#include <tupai/fs/fs.hpp>
#include <tupai/fs/path.hpp>
#include <tupai/fs/vtable.hpp>

#include <tupai/util/tar.hpp>
#include <tupai/util/out.hpp>

namespace tupai
{
	namespace sys
	{
		struct initrd_t
		{
			void*       start = nullptr;
			size_t      size = 0;
			const char* args = nullptr;

			id_t fs;
		};

		static const size_t INITRD_MAX = 32;
		initrd_t initrds[INITRD_MAX];

		fs::vtable_t initrd_vtable;

		void initrd_create(initrd_t* initrd, const char* name);

		id_t    initrd_open_call (id_t pid, fs::inode_t* inode);
		ssize_t initrd_read_call (id_t pid, fs::desc_t* desc, size_t n, void* ret_buff);
		ssize_t initrd_write_call(id_t pid, fs::desc_t* desc, const void* buff, size_t n);
		void    initrd_read_call (id_t pid, fs::desc_t* desc);

		void initrd_add(void* start, size_t size, const char* args)
		{
			// Search for empty cache
			for (size_t i = 0; i < INITRD_MAX; i ++)
			{
				if (initrds[i].size == 0)
				{
					initrds[i].start = start;
					initrds[i].size  = size;
					initrds[i].args  = args;
					break;
				}
			}
		}

		void initrd_init()
		{
			initrd_vtable.open = initrd_open_call;
			initrd_vtable.read = initrd_read_call;

			for (size_t i = 0; i < INITRD_MAX; i ++)
			{
				if (initrds[i].size != 0)
					initrd_create(&initrds[i], "initrd");
			}
		}

		void initrd_create(initrd_t* initrd, const char* name)
		{
			fs::fs_t* fs = fs::vfs_create_fs(name);
			fs::vfs_set_root(fs->root);
			initrd->fs = fs->id;

			// TODO : re-add this
			mmap_reserve((size_t)initrd->start, (size_t)initrd->size, KERNEL_PROC_ID); // Reserve the memory

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

						fs::inode_t* ninode = fs::fs_create_inode(fs, type, &initrd_vtable);
						fs::inode_add_child(cinode, ninode, buff);
						cinode = ninode;
					}
				}

				cheader = tar_next(cheader);
			}
		}

		id_t initrd_open_call(id_t pid, fs::inode_t* inode)
		{
			return proc_create_desc(pid, inode);
		}

		ssize_t initrd_read_call (id_t pid, fs::desc_t* desc, size_t n, void* ret_buff)
		{
			size_t i;
			for (i = 0; i < n && desc->offset < 128; i ++)
				((uint8_t*)ret_buff)[i] = '!';
			return i;
		}
	}
}
