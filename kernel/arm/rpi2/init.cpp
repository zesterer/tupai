//
// file : init.cpp
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
#include <tupai/arm/rpi2/init.hpp>
#include <tupai/arm/mmio.hpp>

namespace tupai
{
	namespace arm
	{
		namespace rpi2
		{
			const size_t CORE_OFFSET = 0x4000008C;
			const int    CORE_COUNT = 4;

			void core_hang();

			int init()
			{
				// Wake up other cores and hang them to prevent slowdown
				for (int i = 1; i < CORE_COUNT; i ++)
					mmio_write(CORE_OFFSET * 0x10 * i, (size_t)&core_hang);

				return 0;
			}

			void core_hang()
			{
				while (1) // Hang forever
					asm volatile ("wfe");
			}
		}
	}
}
