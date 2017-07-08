//
// file : vtable.hpp
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

#ifndef TUPAI_VFS_VTABLE_HPP
#define TUPAI_VFS_VTABLE_HPP

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
		struct vtable_t
		{
			int      (*open) (inode_ptr_t inode)                         = nullptr;
			int      (*close)(fd_ptr_t desc)                             = nullptr;
			ssize_t  (*read) (fd_ptr_t desc, size_t n, void* rbuff)      = nullptr;
			ssize_t  (*write)(fd_ptr_t desc, const void* buff, size_t n) = nullptr;
		};
	}
}

#endif
