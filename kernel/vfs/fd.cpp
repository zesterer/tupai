//
// file : fd.cpp
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
#include <tupai/vfs/fd.hpp>
#include <tupai/vfs/vtable.hpp>
#include <tupai/sys/proc.hpp>

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
					return sys::proc_create_fd(sys::proc_get_current(), inode);
				else
					return ID_INVALID;
			}
		}

		int fd_close(id_t lfd)
		{
			id_t cproc = sys::proc_get_current();

			if (cproc == ID_INVALID)
				return 1;
			else
			{
				fd_ptr_t fd = sys::proc_get_fd(cproc, lfd);
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
						vfs_delete_fd(fd);
						sys::proc_remove_fd(cproc, lfd);
						return 0;
					}
				}
			}
		}

		ssize_t fd_read(id_t lfd, size_t n, void* rbuff)
		{
			id_t cproc = sys::proc_get_current();

			if (cproc == ID_INVALID)
				return 1;
			else
			{
				fd_ptr_t fd = sys::proc_get_fd(cproc, lfd);
				vtable_t* vtable;
				fd.get_inode().get_vtable(&vtable);

				if (vtable == nullptr)
					return 1;
				else if (vtable->read == nullptr)
					return 1;
				else
					return vtable->read(fd, n, rbuff);
			}
		}
	}
}
