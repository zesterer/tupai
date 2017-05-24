//
// file : mmap.cpp
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

// Tupai
#include <tupai/sys/mmap.hpp>

namespace tupai
{
	namespace sys
	{
		static const size_t BLOCK_SIZE  = 32;
		static const size_t BLOCK_COUNT = 4096;

		volatile uint8_t head[BLOCK_COUNT] __attribute__((aligned(1024)));
		volatile uint8_t pool[BLOCK_COUNT * BLOCK_SIZE] __attribute__((aligned(1024)));

		void mmap_init()
		{
			// Do nothing yet
		}
	}
}
