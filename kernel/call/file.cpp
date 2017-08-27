//
// file : file.cpp
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
#include <tupai/call/file.hpp>
#include <tupai/panic.hpp>
#include <tupai/vfs/vfs.hpp>
#include <tupai/vfs/vtable.hpp>
#include <tupai/task/task.hpp>

// Standard
#include <stddef.h>
#include <stdint.h>

namespace tupai
{
	namespace call
	{
		enum class mode
		{
			READ   = (0 << 0),
			WRITE  = (1 << 0),
			APPEND = (2 << 0),

			UPDATE = (1 << 2),
		};

		size_t open(size_t arg0, size_t arg1, size_t arg2, size_t arg3)
		{
			(void)arg0; (void)arg1; (void)arg2; (void)arg3; // Avoid warnings

			const char* path = (const char*)arg0;
			id_t*       fd_  = (id_t*)arg1;
			uint8_t     mode = (uint8_t)arg2;

			vfs::inode_ptr_t inode = vfs::get_inode(path);

			if (inode != ID_INVALID)
			{
				vfs::vtable_t* vtable;
				inode.get_vtable(&vtable);

				if (vtable != nullptr)
				{
					if (vtable->open != nullptr)
					{
						int result = vtable->open(inode);

						if (result == 0)
						{
							*fd_ = task::get_current().create_fd(inode);
							return 0;
						}
					}
				}
			}

			*fd_ = ID_INVALID; // Error return value
			return 1;
		}

		size_t read(size_t arg0, size_t arg1, size_t arg2, size_t arg3)
		{
			(void)arg0; (void)arg1; (void)arg2; (void)arg3; // Avoid warnings

			id_t     lfd   = (id_t)arg0;
			void*    buff_ = (void*)arg1;
			ssize_t* n_    = (ssize_t*)arg2;

			if (lfd != ID_INVALID)
			{
				task::proc_ptr_t cproc = task::get_current();

				vfs::fd_ptr_t fd = cproc.get_fd(lfd);

				vfs::vtable_t* vtable;
				fd.get_inode().get_vtable(&vtable);

				if (vtable != nullptr)
				{
					if (vtable->read != nullptr)
					{
						*n_ = vtable->read(fd, buff_, *n_);
						return 0;
					}
				}
			}

			*n_ = 0; // Error return value
			return 1;
		}

		size_t write(size_t arg0, size_t arg1, size_t arg2, size_t arg3)
		{
			(void)arg0; (void)arg1; (void)arg2; (void)arg3; // Avoid warnings

			id_t        lfd  = (id_t)arg0;
			const void* buff = (const void*)arg1;
			ssize_t*    n_   = (ssize_t*)arg2;

			if (lfd != ID_INVALID)
			{
				task::proc_ptr_t cproc = task::get_current();

				vfs::fd_ptr_t fd = cproc.get_fd(lfd);

				vfs::vtable_t* vtable;
				fd.get_inode().get_vtable(&vtable);

				if (vtable != nullptr)
				{
					if (vtable->write != nullptr)
					{
						*n_ = vtable->write(fd, buff, *n_);
						return 0;
					}
				}
			}

			*n_ = 0; // Error return value
			return 1;
		}

		size_t close(size_t arg0, size_t arg1, size_t arg2, size_t arg3)
		{
			(void)arg0; (void)arg1; (void)arg2; (void)arg3; // Avoid warnings

			id_t lfd = (id_t)arg0;

			if (lfd != ID_INVALID)
			{
				task::proc_ptr_t cproc = task::get_current();

				vfs::fd_ptr_t fd = cproc.get_fd(lfd);

				vfs::vtable_t* vtable;
				fd.get_inode().get_vtable(&vtable);

				if (vtable != nullptr)
				{
					if (vtable->close != nullptr)
					{
						int result = vtable->close(fd);

						if (result == 0)
						{
							delete_fd(fd);
							cproc.delete_fd(lfd);
							return 0;
						}
					}
				}
			}

			return 1;
		}

		size_t seek(size_t arg0, size_t arg1, size_t arg2, size_t arg3)
		{
			(void)arg0; (void)arg1; (void)arg2; (void)arg3; // Avoid warnings

			id_t           lfd    = (id_t)arg0;
			int            origin = arg1;
			vfs::fd_offset offset = (vfs::fd_offset)arg2;

			if (lfd != ID_INVALID)
			{
				task::proc_ptr_t cproc = task::get_current();

				vfs::fd_ptr_t fd = cproc.get_fd(lfd);

				vfs::vtable_t* vtable;
				fd.get_inode().get_vtable(&vtable);

				if (vtable != nullptr)
				{
					if (vtable->close != nullptr)
					{
						vtable->seek(fd, origin, offset);
						return 0;
					}
				}
			}

			return 1;
		}
	}
}
