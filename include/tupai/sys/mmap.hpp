//
// file : mmap.hpp
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

#ifndef TUPAI_SYS_MMAP_HPP
#define TUPAI_SYS_MMAP_HPP

// Tupai
#include <tupai/sys/proc.hpp>

// Standard
#include <stddef.h>
#include <stdint.h>

namespace tupai
{
	namespace sys
	{
		struct mmap_status_t
		{
			id_t owner = NO_PROC_ID;
			bool valid = false;

			mmap_status_t(id_t owner, bool valid = true)
			{
				this->owner = owner;
				this->valid = valid;
			}

			bool operator==(const mmap_status_t& other)
			{
				return this->owner == other.owner && this->valid == other.valid;
			}

			bool operator!=(const mmap_status_t& other)
			{
				return !(*this == other);
			}
		};

		void mmap_init();
		void mmap_reserve(uint64_t start, uint64_t size, id_t owner);

		void mmap_display();
	}
}

#endif
