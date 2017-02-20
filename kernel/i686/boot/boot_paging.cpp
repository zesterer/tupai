/*
* 	file : boot_paging.cpp
*
* 	This file is part of Tupai.
*
* 	Tupai is free software: you can redistribute it and/or modify
* 	it under the terms of the GNU General Public License as published by
* 	the Free Software Foundation, either version 3 of the License, or
* 	(at your option) any later version.
*
* 	Tupai is distributed in the hope that it will be useful,
* 	but WITHOUT ANY WARRANTY; without even the implied warranty of
* 	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
* 	GNU General Public License for more details.
*
* 	You should have received a copy of the GNU General Public License
* 	along with Tupai.  If not, see <http://www.gnu.org/licenses/>.
*/

// Tupai
#include <tupai/i686/boot/boot_paging.hpp>

namespace tupai
{
	namespace i686
	{
		volatile uint32 boot_page_directory[1024];
		volatile uint32 boot_page_tables[1024][256];

		extern "C" void _boot_load_page_directory();
		extern "C" void _boot_enable_paging();

		void init_boot_paging()
		{
			// --- NOTE: The code in .fini gets called about HERE ---
			asm volatile ("xchg %bx, %bx");

			// Map all of page directory as not present
			for (umem i = 0; i < 1024; i ++)
				boot_page_directory[i] = 0x00000002; // Not present, R/W

			// Map all 256 of the page tables to 0x00000000 - 0x3FFFFFFF (1G)
			for (umem pt = 0; pt < 256; pt ++)
				for (umem i = 0; i < 1024; i ++)
					boot_page_tables[i][pt] = (i * 0x1000) | 0x3; // Present, R/W

			// Map 0x00000000 - 0x3FFFFFFF of page directory to the page tables
			for (umem i = 256 * 0; i < 256; i ++)
				boot_page_directory[i] = ((umem)&boot_page_tables[0][i]) | 0x3; // Present, R/W

			// Map 0xC0000000 - 0xFFFFFFFF of page directory to the page tables
			for (umem i = 256 * 3; i < 1024; i ++)
				boot_page_directory[i] = ((umem)&boot_page_tables[0][i]) | 0x3; // Present, R/W

			// Load page directory
			asm volatile ("xchg %bx, %bx; push %eax; mov $boot_page_directory, %eax; mov %eax, %cr3; pop %eax; xchg %bx, %bx");

			//_boot_load_page_directory();
			_boot_enable_paging();
		}
	}
}
