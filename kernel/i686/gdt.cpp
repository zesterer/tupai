/*
* 	file : gdt.cpp
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
#include <tupai/i686/gdt.hpp>
//#include <tupai/tty.hpp>

namespace tupai
{
	static gdt_entry gdt[GDT_SIZE] __attribute__((aligned(8)));

	extern "C" gdt_desc gdt_ptr;
	gdt_desc gdt_ptr;
	extern "C" umem gdt_ptr_ptr;
	umem gdt_ptr_ptr;

	void gdt_init()
	{
		// The null gdt entry
		gdt_set_entry(0, 0, 0, 0, 0);

		// Code segment
		gdt_set_entry(1, 0x0, 0xFFFFF, 0x9A, 0xCF);
		// Data segment
		gdt_set_entry(2, 0x0, 0xFFFFF, 0x92, 0xCF);

		gdt_install();//&gdt, sizeof(gdt));
	}

	void gdt_install()
	{
		// Set the GDT pointer
		gdt_ptr.size = (sizeof(gdt_entry) * GDT_SIZE) - 1;
		gdt_ptr.offset = (uint32)&gdt;

		gdt_ptr_ptr = (umem)&gdt_ptr;
		asm volatile ("lgdt (((gdt_ptr)))");

		/*asm volatile (
						"lgdt (((gdt_ptr))) \n" // Set the GDT
						"ljmp $0x08, $_gdt_longjump \n" // We need to perform a long jump (just next door! to flush all the GDT-related internal registers)
						"_gdt_longjump: \n"
						);*/
	}

	void gdt_set_entry(umem n, uint32 base, uint32 limit, uint8 access, uint8 gran)
	{
		// Base address
		gdt[n].base_addr_low = (base & 0xFFFF);
		gdt[n].base_addr_mid = ((base >> 16) & 0xFF);
		gdt[n].base_addr_high = ((base >> 24) & 0xFF);

		// Descriptor limits
		gdt[n].limit_low = (limit & 0xFFFF);
		gdt[n].granularity = ((limit >> 16) & 0x0F);

		// Granularity and access flags
		gdt[n].granularity |= (gran & 0xF0);
		gdt[n].access = access;

		gdt_install();//&gdt, sizeof(gdt));

		//tty_write_str("Created GDT entry\n");
	}
}
