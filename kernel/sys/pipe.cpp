//
// file : pipe.cpp
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
#include <tupai/sys/pipe.hpp>
#include <tupai/vfs/vtable.hpp>
#include <tupai/util/hashtable.hpp>

// Standard
#include <stddef.h>
#include <stdint.h>

namespace tupai
{
	namespace sys
	{
		namespace pipe
		{
			static util::hashtable_t<pipe_t*> pipes;
			static vfs::vtable_t vtable;
			static vfs::fs_ptr_t fs;

			static int     open_call (vfs::inode_ptr_t inode);
			static int     close_call(vfs::fd_ptr_t fd);
			static ssize_t read_call (vfs::fd_ptr_t fd, void* rbuff, size_t n);
			static ssize_t write_call(vfs::fd_ptr_t fd, const void* buff, size_t n);

			void init()
			{
				vtable.open  = open_call;
				vtable.close = close_call;
				vtable.read  = read_call;
				vtable.write = write_call;

				fs = vfs::create_fs("pipe");
			}

			vfs::inode_ptr_t mount(pipe_t* pipe, const char* path, bool create)
			{
				vfs::inode_ptr_t ninode = vfs::create_inode(vfs::inode_type::PIPE, fs);
				ninode.set_vtable(&vtable);
				vfs::mount(ninode, path, create);

				pipes.add(ninode, pipe);

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

			static ssize_t read_call(vfs::fd_ptr_t fd, void* rbuff, size_t n)
			{
				(void)n;

				pipe_t** pipe = pipes[fd.get_inode()];

				if (pipe != nullptr)
				{
					// TODO : Fix this
					((uint8_t*)rbuff)[0] = (*pipe)->read();
					((uint8_t*)rbuff)[1] = '\0';
					return 1;
				}
				else
					return 0;
			}

			static ssize_t write_call(vfs::fd_ptr_t fd, const void* buff, size_t n)
			{
				pipe_t** pipe = pipes[fd.get_inode()];

				if (pipe != nullptr)
				{
					size_t i;
					for (i = 0; i < n; i ++)
						(*pipe)->write(((uint8_t*)buff)[i]);
					return i;
				}
				else
					return 0;
			}
		}
	}
}
