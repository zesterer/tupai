//
// file : kmain.cpp
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
#include <tupai/common/kmain.hpp>

// Standard
#include <stddef.h>
#include <stdint.h>

namespace tupai
{
	extern "C" void rpi2_kmain(uint32_t r0, uint32_t r1, uint32_t atags)
	{
		// Declare as unused
		(void) r0;
		(void) r1;
		(void) atags;

		kmain();
	}
}
