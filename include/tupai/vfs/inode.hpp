//
// file : inode.hpp
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

#ifndef TUPAI_VFS_INODE_HPP
#define TUPAI_VFS_INODE_HPP

// Tupai
#include <tupai/type.hpp>
#include <tupai/vfs/vfs.hpp>
#include <tupai/vfs/vtable.hpp>
#include <tupai/util/vector.hpp>

// Standard
#include <stddef.h>
#include <stdint.h>

namespace tupai
{
	namespace vfs
	{
		enum class inode_mode : uint8_t
		{
			READ    = (1 << 0),
			WRITE   = (1 << 1),
			EXECUTE = (1 << 2),
		};

		struct inode_mode_t
		{
			uint8_t owner_flags = 0xFF; // Allow all by default
			uint8_t group_flags = 0xFF; // Allow all by default
			uint8_t other_flags = 0xFF; // Allow all by default
		};

		struct inode_child_t
		{
			id_t inode;
			char name[FILENAME_MAX] = { '\0', };
		};

		struct inode_t
		{
			id_t id = ID_INVALID;
			id_t fs = ID_INVALID;

			util::vector_t<inode_child_t> children;

			inode_type type;
			vtable_t*  vtable = nullptr;

			id_t         owner = ID_INVALID;
			id_t         group = ID_INVALID;
			inode_mode_t mode;

			uint64_t last_access = 0;
			uint64_t last_modify = 0;

			size_t refc = 0;
		};
	}
}

#endif
