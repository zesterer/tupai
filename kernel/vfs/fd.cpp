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
#include <tupai/proc/proc.hpp>

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
					return proc::get_current().create_fd(inode);
				else
					return ID_INVALID;
			}
		}

		int fd_close(id_t lfd)
		{
			proc::proc_ptr_t cproc = proc::get_current();

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
			proc::proc_ptr_t cproc = proc::get_current();

			if (cproc == ID_INVALID)
				return 1;
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

		ssize_t fd_write(id_t lfd, const void* buff, size_t n)
		{
			proc::proc_ptr_t cproc = proc::get_current();

			if (cproc == ID_INVALID)
				return 1;
			else
			{
				fd_ptr_t fd = cproc.get_fd(lfd);
				vtable_t* vtable;
				fd.get_inode().get_vtable(&vtable);

				if (vtable == nullptr)
					return 1;
				else if (vtable->write == nullptr)
					return 1;
				else
					return vtable->write(fd, buff, n);
			}
		}
	}
}
