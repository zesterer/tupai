//
// file : kentry.cpp
//
// Copyright (c) 2017 Joshua Barretto <joshua.s.barretto@gmail.com>
//
// This file is part of Tupai.
//
// Permission to use, copy, modify, and distribute this software for any
// purpose with or without fee is hereby granted, provided that the above
// copyright notice and this permission notice appear in all copies.
//
// THE SOFTWARE IS PROVIDED "AS IS" AND THE AUTHOR DISCLAIMS ALL WARRANTIES
// WITH REGARD TO THIS SOFTWARE INCLUDING ALL IMPLIED WARRANTIES OF
// MERCHANTABILITY AND FITNESS. IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR
// ANY SPECIAL, DIRECT, INDIRECT, OR CONSEQUENTIAL DAMAGES OR ANY DAMAGES
// WHATSOEVER RESULTING FROM LOSS OF USE, DATA OR PROFITS, WHETHER IN AN
// ACTION OF CONTRACT, NEGLIGENCE OR OTHER TORTIOUS ACTION, ARISING OUT OF
// OR IN CONNECTION WITH THE USE OR PERFORMANCE OF THIS SOFTWARE.
//

// Tupai
#include <tupai/main.hpp>
#include <tupai/debug.hpp>
#include <tupai/panic.hpp>
#include <tupai/arm/mmio.hpp>
#include <tupai/arm/delay.hpp>
#include <tupai/arch.hpp>
#include <tupai/sys/call.hpp>

// Standard
#include <stddef.h>
#include <stdint.h>

namespace tupai
{
	namespace arm
	{
		namespace rpi2
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

				// Initiate the syscall routine
				sys::call_init();

				debug_print("Finished rpi2 initiation\n");

				early(); // Core system setup
			}

			extern "C" void kmain()
			{
				main(); // Main kernel entry
			}

			void core_hang()
			{
				hang(); // This core only!
			}
		}
	}
}
