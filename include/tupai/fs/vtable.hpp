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

#ifndef TUPAI_FS_VTABLE_HPP
#define TUPAI_FS_VTABLE_HPP

// Tupai
#include <tupai/fs/com.hpp>
#include <tupai/fs/inode.hpp>
#include <tupai/fs/desc.hpp>
#include <tupai/fs/fs.hpp>

// Standard
#include <stddef.h>
#include <stdint.h>

namespace tupai
{
	namespace fs
	{
		struct vtable_t
		{
			int (*open) (const inode_t*, id_t proc, desc_t* ret_desc)                           = nullptr;
			int (*read) (const inode_t*, id_t proc, size_t n, void* ret_buff, ssize_t* ret_n)   = nullptr;
			int (*write)(const inode_t*, id_t proc, const void* buff, size_t n, ssize_t* ret_n) = nullptr;
			int (*close)(const inode_t*, id_t proc, desc_t* desc)                               = nullptr;
		};
	}
}

#endif
