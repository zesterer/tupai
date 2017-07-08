//
// file : fs.hpp
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

#ifndef TUPAI_VFS_FS_HPP
#define TUPAI_VFS_FS_HPP

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
		const size_t FS_NAME_MAX = 256;

		struct fs_t
		{
			id_t id = ID_INVALID;

			char name[FS_NAME_MAX] = { '\0', };

			id_t root = ID_INVALID;
		};
	}
}

#endif
