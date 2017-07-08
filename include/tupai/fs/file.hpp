//
// file : file.hpp
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

#ifndef TUPAI_FS_FILE_HPP
#define TUPAI_FS_FILE_HPP

// Tupai
#include <tupai/util/table.hpp>
#include <tupai/util/spinlock.hpp>

namespace tupai
{
	namespace fs
	{
		enum class file_mode : uint8_t
		{
			READ  = 1 << 0,
			WRITE = 1 << 1,
		};

		struct file_mode_t
		{
			uint8_t mode = 0xFF; // Enable all modes by default
		};

		struct file_t
		{
			id_t        id       = ID_INVALID;
			id_t        inode_id = ID_INVALID;
			file_mode_t mode;
		};
	}
}

#endif
