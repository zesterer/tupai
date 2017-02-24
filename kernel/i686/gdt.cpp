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
#include <tupai/i686/i686.hpp>
#include <tupai/mempool.hpp>

namespace tupai
{
	struct gdt_entry
	{
		uint16 limit_low;
		uint16 base_addr_low;
		uint8  base_addr_mid;
		uint8  access;
		uint8  granularity;
		uint8  base_addr_high;

	} __attribute((packed));

	struct gdt_desc
	{
		uint16 size;
		uint32 offset;

	} __attribute((packed));

	static gdt_entry gdt[GDT_SIZE] __attribute__((aligned(8)));

	extern "C" gdt_desc gdt_ptr;
	gdt_desc gdt_ptr;
	extern "C" umem gdt_ptr_ptr;
	umem gdt_ptr_ptr;

	// Section constraints
	extern "C" umem _text_begin;
	extern "C" umem _text_end;
	extern "C" umem _rodata_begin;
	extern "C" umem _rodata_end;
	extern "C" umem _data_begin;
	extern "C" umem _data_end;
	extern "C" umem _bss_begin;
	extern "C" umem _bss_end;

	void gdt_init()
	{
		// The null gdt entry
		gdt_set_entry(0, 0, 0, 0, 0);

		// Code segments
		gdt_set_entry(1, 0x0, 0xFFFFFF, 0x9A, 0xC0);

		// Data segments
		gdt_set_entry(2, 0x0, 0xFFFFFF, 0x92, 0xC0);

		gdt_install();
	}

	void gdt_install()
	{
		// Set the GDT pointer
		gdt_ptr.size = (sizeof(gdt_entry) * GDT_SIZE) - 1;
		gdt_ptr.offset = (uint32)&gdt;

		gdt_ptr_ptr = (umem)&gdt_ptr;
		asm volatile ("lgdt (((gdt_ptr)))");
	}

	void gdt_set_entry(umem n, uint32 base, uint32 limit, uint8 access, uint8 granularity)
	{
		// Base address
		gdt[n].base_addr_low = (base & 0xFFFF);
		gdt[n].base_addr_mid = ((base >> 16) & 0xFF);
		gdt[n].base_addr_high = ((base >> 24) & 0xFF);

		// Descriptor limits
		gdt[n].limit_low = (limit & 0xFFFF);
		gdt[n].granularity = ((limit >> 16) & 0x0F);

		// Granularity and access flags
		gdt[n].granularity |= (granularity & 0xF0);
		gdt[n].access = access;
	}
}
