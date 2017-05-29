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
// but WITHOUT ANY WARRANTY// without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with Tupai.  If not, see <http://www.gnu.org/licenses/>.
//

#ifndef TUPAI_FS_INODE_HPP
#define TUPAI_FS_INODE_HPP

// Tupai
#include <tupai/fs/com.hpp>
#include <tupai/util/vector.hpp>

// Standard
#include <stddef.h>
#include <stdint.h>

namespace tupai
{
	namespace fs
	{
		enum class inode_type
		{
			DIRECTORY,
			FIFO_BUFFER,
			BLOCK_FILE,
		};

		struct inode_t
		{
			id_t id;
			id_t dev;

			inode_type type;
			util::vector_t<id_t> dir_table;

			id_t     owner;
			id_t     group;
			uint16_t mode;

			size_t ref_count = 0;

			uint64_t last_access;
			uint64_t last_modify;
		};
	}
}

#endif
