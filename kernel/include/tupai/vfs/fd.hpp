//
// file : fd.hpp
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

#ifndef TUPAI_VFS_FD_HPP
#define TUPAI_VFS_FD_HPP

// Tupai
#include <tupai/type.hpp>
#include <tupai/vfs/vfs.hpp>

// Standard
#include <stddef.h>
#include <stdint.h>

namespace tupai
{
	namespace vfs
	{
		struct fd_t
		{
			id_t id = ID_INVALID;

			id_t      inode = ID_INVALID;
			fd_mode_t mode;

			fd_offset offset = 0;
		};

		id_t    fd_open (inode_ptr_t inode);
		int     fd_close(id_t lfd);
		ssize_t fd_read (id_t lfd, void* rbuff, size_t n);
		ssize_t fd_write(id_t lfd, const void* buff, size_t n);
	}
}

#endif
