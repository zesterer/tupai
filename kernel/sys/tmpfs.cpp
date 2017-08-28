//
// file : tmpfs.cpp
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
#include <tupai/sys/tmpfs.hpp>
#include <tupai/vfs/vtable.hpp>
#include <tupai/util/hashtable.hpp>
#include <tupai/util/dbuffer.hpp>

// Standard
#include <stddef.h>
#include <stdint.h>

namespace tupai
{
	namespace sys
	{
		namespace tmpfs
		{
			static vfs::fs_ptr_t fs;
			static vfs::vtable_t vtable;

			typedef util::dbuffer_t<uint8_t> file_t;
			static util::hashtable_t<file_t> files;

			static int     open_call (vfs::inode_ptr_t inode);
			static int     close_call(vfs::fd_ptr_t fd);
			static ssize_t read_call (vfs::fd_ptr_t fd, void* rbuff, size_t n);
			static ssize_t write_call(vfs::fd_ptr_t fd, const void* buff, size_t n);
			static int     seek_call (vfs::fd_ptr_t fd, int origin, vfs::fd_offset offset);

			void init()
			{
				vtable.open  = open_call;
				vtable.close = close_call;
				vtable.read  = read_call;
				vtable.write = write_call;
				vtable.seek  = seek_call;

				fs = vfs::create_fs("tmpfs");
				vfs::set_root(fs.get_root());
			}

			id_t create(const char* path, vfs::inode_type type, const uint8_t* data, size_t size)
			{
				// Create and mount an inode for the new file
				vfs::inode_ptr_t ninode = vfs::create_inode(type, fs);
				ninode.set_vtable(&vtable);
				fs.get_root().mount_relative(ninode, path, true); // Mount the child, creating the path if necessary

				// Create a buffer for the new file
				file_t nfile;
				nfile.write(data, size, 0);
				files.add(ninode, nfile);

				return ninode;
			}

			static int open_call (vfs::inode_ptr_t inode)
			{
				return (inode == ID_INVALID) ? 1 : 0;
			}

			static int close_call(vfs::fd_ptr_t fd)
			{
				return (fd == ID_INVALID) ? 1 : 0;
			}

			static ssize_t read_call (vfs::fd_ptr_t fd, void* rbuff, size_t n)
			{
				file_t* file = files[fd.get_inode()];

				if (file != nullptr)
				{
					if (fd.get_offset() >= file->size())
						return 0;

					size_t filesize = file->size();
					const uint8_t* data = file->data();

					size_t i;
					vfs::fd_offset offset = fd.get_offset();
					for (i = 0; i + 1 < n && offset < filesize; i ++)
					{
						((uint8_t*)rbuff)[i] = data[offset];
						offset ++;
					}
					((uint8_t*)rbuff)[i] = '\0';

					// Have we reached EOF?
					if (i >= filesize)
						fd.set_flag(vfs::fd_flag::EOF, true);

					fd.set_offset(offset);

					return i;
				}
				else
					return -1;
			}

			static ssize_t write_call(vfs::fd_ptr_t fd, const void* buff, size_t n)
			{
				file_t* file = files[fd.get_inode()];

				if (file != nullptr)
				{
					if (fd.get_offset() > file->size())
						return 0;

					file->write((const uint8_t*)buff, n, fd.get_offset());

					fd.set_offset(fd.get_offset() + n);

					// Have we reached EOF?
					if (fd.get_offset() >= file->size())
						fd.set_flag(vfs::fd_flag::EOF, true);

					return n;
				}
				else
					return -1;
			}

			static int seek_call(vfs::fd_ptr_t fd, int origin, vfs::fd_offset offset)
			{
				file_t* file = files[fd.get_inode()];

				if (file != nullptr)
				{
					switch (origin)
					{
					case 0:
						fd.set_offset(offset);
						break;

					case 1:
						fd.set_offset(fd.get_offset() + offset);
						break;

					case 2:
						fd.set_offset(file->size() + offset);
						break;

					default:
						return 1;
						break;
					};

					// Have we reached EOF?
					if (fd.get_offset() >= file->size())
						fd.set_flag(vfs::fd_flag::EOF, true);

					return 0;
				}
				else
					return 1;
			}
		}
	}
}
