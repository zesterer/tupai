//
// file : pipe.cpp
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
		static util::hashtable_t<pipe_t*> pipes;
		static vfs::vtable_t pipe_vtable;

		int     pipe_open_call (vfs::inode_ptr_t inode);
		int     pipe_close_call(vfs::fd_ptr_t fd);
		ssize_t pipe_read_call (vfs::fd_ptr_t fd, void* rbuff, size_t n);
		ssize_t pipe_write_call(vfs::fd_ptr_t fd, const void* buff, size_t n);

		void pipe_init()
		{
			pipe_vtable.open  = pipe_open_call;
			pipe_vtable.close = pipe_close_call;
			pipe_vtable.read  = pipe_read_call;
			pipe_vtable.write = pipe_write_call;
		}

		vfs::inode_ptr_t mount_pipe(pipe_t* pipe, const char* path)
		{
			vfs::inode_ptr_t ninode = vfs::vfs_create_inode(vfs::inode_type::PIPE);
			ninode.set_vtable(&pipe_vtable);
			vfs::vfs_mount(ninode, path);

			vfs::vtable_t* nv;
			ninode.get_vtable(&nv);

			pipes.add(ninode, pipe);

			return ninode;
		}

		int pipe_open_call (vfs::inode_ptr_t inode)
		{
			return (inode == ID_INVALID) ? 1 : 0;
		}

		int pipe_close_call(vfs::fd_ptr_t fd)
		{
			return (fd == ID_INVALID) ? 1 : 0;
		}

		ssize_t pipe_read_call (vfs::fd_ptr_t fd, void* rbuff, size_t n)
		{
			pipe_t** pipe = pipes[fd.get_inode()];

			if (pipe != nullptr)
			{
				((uint8_t*)rbuff)[0] = (*pipe)->read_unsafe();
				((uint8_t*)rbuff)[1] = '\0';
				return 1;
			}
			else
				return 0;
		}

		ssize_t pipe_write_call(vfs::fd_ptr_t fd, const void* buff, size_t n)
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
