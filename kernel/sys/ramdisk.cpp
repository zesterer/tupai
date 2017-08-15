//
// file : ramdisk.cpp
//
// Copyright (c) 2017 Joshua Barretto <joshua.s.barretto@gmail.com>
//
// This file is part of Tupai.
//
// Permission to use, copy, modify, and distribute this software for any
// purpose with or without fee is hereby granted, provided that the above
// copyright notice and this permission notice appear in all copies.
//
// THE SOFTWARE IS PROVIDED "AS IS" AND THE AUTHOR DISCLAIMS ALL WARRANTIES
// WITH REGARD TO THIS SOFTWARE INCLUDING ALL IMPLIED WARRANTIES OF
// MERCHANTABILITY AND FITNESS. IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR
// ANY SPECIAL, DIRECT, INDIRECT, OR CONSEQUENTIAL DAMAGES OR ANY DAMAGES
// WHATSOEVER RESULTING FROM LOSS OF USE, DATA OR PROFITS, WHETHER IN AN
// ACTION OF CONTRACT, NEGLIGENCE OR OTHER TORTIOUS ACTION, ARISING OUT OF
// OR IN CONNECTION WITH THE USE OR PERFORMANCE OF THIS SOFTWARE.
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
					mem::mmap::reserve_region((void*)((size_t)ramdisks[i].start - arch_get_offset()), ramdisks[i].size, task::get_kernel(), (uint8_t)mem::mmap::page_flags::STATIC); // Reserve the ramdisk region
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

			//vfs::inode_ptr_t dev_ptr = vfs::create_inode(vfs::inode_type::DIRECTORY);
			//vfs::get_root().mount_child(dev_ptr, "dev");
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
				if (fd.get_offset() >= util::tar_size(*header))
					return 0;
				
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

		// TODO : Implement this
		ssize_t ramdisk_write_call(vfs::fd_ptr_t desc, const void* buff, size_t n);
	}
}
