//
// file : kentry.cpp
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
#include <tupai/main.hpp>
#include <tupai/debug.hpp>
#include <tupai/arm/mmio.hpp>
#include <tupai/arm/delay.hpp>
#include <tupai/arch.hpp>

// Standard
#include <stddef.h>
#include <stdint.h>

namespace tupai
{
	const size_t CORE_OFFSET = 0x4000008C;
	const int    CORE_COUNT = 4;

	void core_hang();

	extern "C" void kentry(uint32_t r0, uint32_t r1, uint32_t atags)
	{
		// Initiate debugging
		debug_init();

		// Passed information
		debug_print("Kernel virtual base is located at offset ", (void*)arch_get_offset(), '\n');
		debug_print( // Display kentry info
			"kentry at ", (void*)&kentry, " called with:\n",
			"  r0    -> ", (void*)r0, '\n',
			"  r1    -> ", (void*)r1, '\n',
			"  atags -> ", (void*)atags, '\n'
		);

		// Wake up other cores and hang them to prevent slowdown
		for (int i = 1; i < CORE_COUNT; i ++)
			arm::mmio_write(CORE_OFFSET * 0x10 * i, (size_t)&core_hang);
		arm::delay(500); // Wait for the cores to wake up
		debug_print("Set cores [2, 3, 4] into idle state\n");

		// Enter the kernel main with a stable environment
		debug_print("Finished rpi2 initiation\n");
		main();
	}

	void core_hang()
	{
		while (1) // Hang forever
			asm volatile ("wfe");
	}
}
