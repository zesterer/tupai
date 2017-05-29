//
// file : tar.hpp
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

#ifndef TUPAI_UTIL_TAR_HPP
#define TUPAI_UTIL_TAR_HPP

// Standard
#include <stddef.h>
#include <stdint.h>

namespace tupai
{
	namespace util
	{
		struct tar_header_t
		{
			char    filename[100];
			char    mode[8];
			char    uid[8];
			char    gid[8];
			char    size[12];
			char    mtime[12];
			uint8_t checksum[8];
			uint8_t typeflag;
		} __attribute__((packed));

		void tar_print_all(tar_header_t* header);
		void tar_print(tar_header_t* header);
		tar_header_t* tar_next(tar_header_t* header);
		size_t tar_size(tar_header_t* header);
	}
}

#endif
