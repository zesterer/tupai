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

#include <tupai/vfs/vfs.hpp>
#include <tupai/vfs/vtable.hpp>
#include <tupai/vfs/path.hpp>

#include <tupai/util/tar.hpp>
#include <tupai/util/hashtable.hpp>

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
		static initrd_t initrds[INITRD_MAX];

		static vfs::vtable_t initrd_vtable;

		static util::hashtable_t<util::tar_header_t*> inodes;

		void initrd_create(initrd_t* initrd, const char* name);

		/*
		id_t    initrd_open_call (id_t pid, fs::inode_t* inode);
		int     initrd_close_call(id_t pid, fs::desc_t* desc);
		ssize_t initrd_read_call (id_t pid, fs::desc_t* desc, size_t n, void* ret_buff);
		ssize_t initrd_write_call(id_t pid, fs::desc_t* desc, const void* buff, size_t n);
		*/

		int     initrd_open_call (vfs::inode_ptr_t inode);
		int     initrd_close_call(vfs::fd_ptr_t fd);
		ssize_t initrd_read_call (vfs::fd_ptr_t fd, size_t n, void* rbuff);
		ssize_t initrd_write_call(vfs::fd_ptr_t fd, const void* buff, size_t n);

		void initrd_add(void* start, size_t size, const char* args)
		{
			// Search for empty cache
			for (size_t i = 0; i < INITRD_MAX; i ++)
			{
				if (initrds[i].size == 0)
				{
					initrds[i] = initrd_t();

					initrds[i].start = start;
					initrds[i].size  = size;
					initrds[i].args  = args;
					break;
				}
			}
		}

		void initrd_init()
		{
			initrd_vtable.open  = initrd_open_call;
			initrd_vtable.close = initrd_close_call;
			initrd_vtable.read  = initrd_read_call;

			for (size_t i = 0; i < INITRD_MAX; i ++)
			{
				if (initrds[i].size != 0)
					initrd_create(&initrds[i], "initrd");
			}
		}

		void initrd_create(initrd_t* initrd, const char* name)
		{
			// Create a filesystem for the initrd
			vfs::fs_ptr_t fs = vfs::vfs_create_fs(name);

			// Set the VFS's root to the initrd fs
			vfs::vfs_set_root(fs.get_root());

			initrd->fs = fs;

			// TODO : re-add this
			mmap_reserve((size_t)initrd->start, (size_t)initrd->size, KERNEL_PROC_ID); // Reserve the memory

			util::tar_header_t* cheader = (util::tar_header_t*)initrd->start;
			while (cheader != nullptr)
			{
				const char* filename = cheader->filename;
				size_t n = vfs::path_element_count(filename);

				vfs::inode_ptr_t cinode = vfs::vfs_get_root();

				for (size_t i = 0; i < n; i ++)
				{
					char buff[256];
					vfs::path_element_get(filename, buff, i);

					vfs::inode_ptr_t tryinode = cinode.get_child(buff);

					if (tryinode != ID_INVALID)
						cinode = tryinode;
					else
					{
						vfs::inode_type type = vfs::inode_type::DIRECTORY;
						if (i + 1 == n)
						{
							switch (cheader->typeflag)
							{
							case '0':
								type = vfs::inode_type::NORMAL_FILE;
								break;
							case '1':
								type = vfs::inode_type::HARD_LINK;
								break;
							case '2':
								type = vfs::inode_type::SYM_LINK;
								break;
							case '3':
								type = vfs::inode_type::CHAR_DEVICE;
								break;
							case '4':
								type = vfs::inode_type::BLOCK_DEVICE;
								break;
							case '6':
								type = vfs::inode_type::FIFO;
								break;
							case '5':
							default:
								break;
							}
						}

						vfs::inode_ptr_t ninode = vfs::vfs_create_inode(type);
						ninode.set_fs(fs);
						cinode.mount_child(ninode, buff);
						cinode = ninode;

						if (type == vfs::inode_type::NORMAL_FILE)
						{
							ninode.set_vtable(&initrd_vtable);
							inodes.add(ninode, cheader);
						}
					}
				}

				cheader = tar_next(cheader);
			}
		}

		int initrd_open_call (vfs::inode_ptr_t inode)
		{
			return 0;
		}

		int initrd_close_call(vfs::fd_ptr_t fd)
		{
			return 0;
		}

		ssize_t initrd_read_call (vfs::fd_ptr_t fd, size_t n, void* rbuff)
		{
			util::tar_header_t* header = *inodes[fd.get_inode()];

			if (header != nullptr)
			{
				size_t filesize = util::tar_size(header);
				uint8_t* data = (uint8_t*)util::tar_data(header);

				size_t i;
				vfs::fd_offset offset = fd.get_offset();
				for (i = 0; i + 1 < n && offset < filesize; i ++)
				{
					((uint8_t*)rbuff)[i] = data[offset];
					offset ++;
				}
				((uint8_t*)rbuff)[i] = '\0';
				fd.set_offset(offset);

				return i;
			}
			else
				return -1;
		}

		ssize_t       initrd_write_call(vfs::fd_ptr_t desc, const void* buff, size_t n);

		/*
		id_t initrd_open_call(id_t pid, fs::inode_t* inode)
		{
			return proc_create_desc(pid, inode);
		}

		int initrd_close_call(id_t pid, fs::desc_t* desc)
		{
			return proc_close_desc(pid, desc->id);
		}

		ssize_t initrd_read_call(id_t pid, fs::desc_t* desc, size_t n, void* ret_buff)
		{
			util::tar_header_t* header = *inodes[desc->inode];

			if (header != nullptr)
			{
				size_t filesize = util::tar_size(header);
				uint8_t* data = (uint8_t*)util::tar_data(header);

				size_t i;
				for (i = 0; i + 1 < n && desc->offset < filesize; i ++)
				{
					((uint8_t*)ret_buff)[i] = data[desc->offset];
					desc->offset ++;
				}
				((uint8_t*)ret_buff)[i] = '\0';

				return i;
			}
			else
				return -1;
		}
		*/
	}
}
