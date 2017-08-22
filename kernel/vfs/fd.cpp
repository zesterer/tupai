//
// file : fd.cpp
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
#include <tupai/vfs/fd.hpp>
#include <tupai/vfs/vtable.hpp>
#include <tupai/task/task.hpp>

namespace tupai
{
	namespace vfs
	{
		id_t fd_open(inode_ptr_t inode)
		{
			vtable_t* vtable;
			inode.get_vtable(&vtable);

			if (vtable == nullptr)
				return ID_INVALID;
			else if (vtable->open == nullptr)
				return ID_INVALID;
			else
			{
				int result = vtable->open(inode);

				if (result == 0)
					return task::get_current().create_fd(inode);
				else
					return ID_INVALID;
			}
		}

		int fd_close(id_t lfd)
		{
			task::proc_ptr_t cproc = task::get_current();

			if (cproc == ID_INVALID)
				return 1;
			else
			{
				fd_ptr_t fd = cproc.get_fd(lfd);
				vtable_t* vtable;
				fd.get_inode().get_vtable(&vtable);

				if (vtable == nullptr)
					return 1;
				else if (vtable->close == nullptr)
					return 1;
				else
				{
					int result = vtable->close(fd);

					if (result != 0)
						return 1;
					else
					{
						delete_fd(fd);
						cproc.delete_fd(lfd);
						return 0;
					}
				}
			}
		}

		ssize_t fd_read(id_t lfd, void* rbuff, size_t n)
		{
			task::proc_ptr_t cproc = task::get_current();

			if (cproc == ID_INVALID)
				return 0;
			else
			{
				if (lfd == ID_INVALID)
					return 0;
				else
				{
					fd_ptr_t fd = cproc.get_fd(lfd);
					vtable_t* vtable;
					fd.get_inode().get_vtable(&vtable);

					if (vtable == nullptr)
						return 0;
					else if (vtable->read == nullptr)
						return 0;
					else
						return vtable->read(fd, rbuff, n);
				}
			}
		}

		ssize_t fd_write(id_t lfd, const void* buff, size_t n)
		{
			task::proc_ptr_t cproc = task::get_current();

			if (cproc == ID_INVALID)
				return 0;
			else
			{
				if (lfd == ID_INVALID)
					return 0;
				else
				{
					fd_ptr_t fd = cproc.get_fd(lfd);
					vtable_t* vtable;
					fd.get_inode().get_vtable(&vtable);

					if (vtable == nullptr)
						return 0;
					else if (vtable->write == nullptr)
						return 0;
					else
						return vtable->write(fd, buff, n);
				}
			}
		}
	}
}
