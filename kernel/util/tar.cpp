//
// file : tar.cpp
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
#include <tupai/util/tar.hpp>
#include <tupai/util/out.hpp>
#include <tupai/util/mem.hpp>

namespace tupai
{
	namespace util
	{
		void tar_print_all(tar_header_t* header)
		{
			println("--- Tar ---");
			while (header != nullptr)
			{
				tar_print(header);
				header = tar_next(header);
			}
		}

		void tar_print(tar_header_t* header)
		{
			println("File: filename = ", header->filename, " size = ", tar_size(header), " type = ", (char)header->typeflag);
		}

		tar_header_t* tar_next(tar_header_t* header)
		{
			size_t offset = util::align_ceiling(512 + tar_size(header), 512);
			tar_header_t* nheader = (tar_header_t*)((size_t)header + offset);

			if (nheader->filename[0] == '\0')
				return nullptr;
			else
				return nheader;
		}

		size_t tar_size(tar_header_t* header)
		{
			size_t csize = 0;
			for (int i = 0; i < 12 && header->size[i] != '\0'; i ++)
			{
				csize *= 8;
				csize += (size_t)(header->size[i] - '0');
			}

			return csize;
		}
	}
}
