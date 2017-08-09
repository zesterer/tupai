//
// file : ramdisk.cpp
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
#include <tupai/sys/ramdisk.hpp>
#include <tupai/mem/mmap.hpp>
#include <tupai/task/task.hpp>
#include <tupai/vfs/vfs.hpp>
#include <tupai/vfs/vtable.hpp>
#include <tupai/vfs/path.hpp>
#include <tupai/util/tar.hpp>
#include <tupai/util/hashtable.hpp>
#include <tupai/arch.hpp>

#include <tupai/util/log.hpp>

namespace tupai
{
	namespace sys
	{
		struct ramdisk_t
		{
			void*       start = nullptr;
			size_t      size = 0;
			const char* args = nullptr;

			id_t fs;
		};

		static const size_t RAMDISK_MAX = 32;
		static ramdisk_t ramdisks[RAMDISK_MAX];

		static vfs::vtable_t ramdisk_vtable;

		static util::hashtable_t<util::tar_header_t*> inodes;

		void ramdisk_create(ramdisk_t* ramdisk, const char* name);

		int     ramdisk_open_call (vfs::inode_ptr_t inode);
		int     ramdisk_close_call(vfs::fd_ptr_t fd);
		ssize_t ramdisk_read_call (vfs::fd_ptr_t fd, void* rbuff, size_t n);
		ssize_t ramdisk_write_call(vfs::fd_ptr_t fd, const void* buff, size_t n);

		void ramdisk_add(void* start, size_t size, const char* args)
		{
			// Search for empty cache
			for (size_t i = 0; i < RAMDISK_MAX; i ++)
			{
				if (ramdisks[i].size == 0)
				{
					ramdisks[i] = ramdisk_t();

					ramdisks[i].start = start;
					ramdisks[i].size  = size;
					ramdisks[i].args  = args;
					break;
				}
			}
		}

		void ramdisk_reserve()
		{
			for (size_t i = 0; i < RAMDISK_MAX; i ++)
			{
				if (ramdisks[i].size != 0)
					mem::mmap::reserve_region((void*)((size_t)ramdisks[i].start - arch_get_offset()), ramdisks[i].size, task::get_kernel(), 0xFF); // Reserve the ramdisk region
			}
		}

		void ramdisk_init()
		{
			ramdisk_vtable.open  = ramdisk_open_call;
			ramdisk_vtable.close = ramdisk_close_call;
			ramdisk_vtable.read  = ramdisk_read_call;

			for (size_t i = 0; i < RAMDISK_MAX; i ++)
			{
				if (ramdisks[i].size != 0)
					ramdisk_create(&ramdisks[i], "initrd");
			}
		}

		void ramdisk_create(ramdisk_t* ramdisk, const char* name)
		{
			//util::logln("Found ramdisk at ", ramdisk->start, " of size ", ramdisk->size);

			// Create a filesystem for the ramdisk
			vfs::fs_ptr_t fs = vfs::create_fs(name);

			// Set the VFS's root to the ramdisk fs
			vfs::set_root(fs.get_root());

			ramdisk->fs = fs;

			util::tar_header_t* cheader = (util::tar_header_t*)ramdisk->start;
			while (cheader != nullptr)
			{
				const char* filename = cheader->filename;
				size_t n = vfs::path_element_count(filename);

				vfs::inode_ptr_t cinode = vfs::get_root();

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
								type = vfs::inode_type::PIPE;
								break;
							case '5':
							default:
								break;
							}
						}

						vfs::inode_ptr_t ninode = vfs::create_inode(type);
						ninode.set_fs(fs);
						cinode.mount_child(ninode, buff);

						if (type == vfs::inode_type::NORMAL_FILE && i + 1 == n)
						{
							ninode.set_vtable(&ramdisk_vtable);
							inodes.add(ninode, cheader);
						}
						else
							ninode.set_vtable(nullptr);

						cinode = ninode;
					}
				}

				cheader = tar_next(cheader);
			}
		}

		int ramdisk_open_call (vfs::inode_ptr_t inode)
		{
			return (inode == ID_INVALID) ? 1 : 0;
		}

		int ramdisk_close_call(vfs::fd_ptr_t fd)
		{
			return (fd == ID_INVALID) ? 1 : 0;
		}

		ssize_t ramdisk_read_call (vfs::fd_ptr_t fd, void* rbuff, size_t n)
		{
			util::tar_header_t** header = inodes[fd.get_inode()];

			if (header != nullptr)
			{
				size_t filesize = util::tar_size(*header);
				uint8_t* data = (uint8_t*)util::tar_data(*header);

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

		ssize_t       ramdisk_write_call(vfs::fd_ptr_t desc, const void* buff, size_t n);

		/*
		id_t ramdisk_open_call(id_t pid, fs::inode_t* inode)
		{
			return create_desc(pid, inode);
		}

		int ramdisk_close_call(id_t pid, fs::desc_t* desc)
		{
			return proc_close_desc(pid, desc->id);
		}

		ssize_t ramdisk_read_call(id_t pid, fs::desc_t* desc, size_t n, void* ret_buff)
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
