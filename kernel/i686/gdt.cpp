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

// Libk
#include <libk/stdio.hpp>

namespace tupai
{
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

	static umem size_in_4kb(umem start, umem end)
	{
		if ((end - start) % 4096 == 0) // Perfectly aligned
			return (end - start) / 4096;
		else
			return (end - start) / 4096 + 1;
	}

	void gdt_init()
	{
		// The null gdt entry
		gdt_set_entry(0, 0, 0, 0, 0);

		// Segments - all using 4Kb alignment

		//KBREAK();
		//libk::printf("CODE SEGMENT : START = 0x%X, END = 0x%X, SIZE(4K) = 0x%X\n", _text_begin, _text_end, size_in_4kb(_text_begin, _text_end));

		// Code segments
		gdt_set_entry(1, 0x0, 0xFFFFFF, 0x9A, 0xC0);

		// Data segments
		gdt_set_entry(2, 0x0, 0xFFFFFF, 0x92, 0xC0);

		/*
		// Code segment
		gdt_set_entry(1, 0x0, 0x180, 0x9A, 0xC0);
		//gdt_set_entry(1, _text_begin / 4096, size_in_4kb(_text_begin, _text_end)*, 0x9A, 0xC0);
		// Rodata segment
		gdt_set_entry(2, _rodata_begin / 4096, size_in_4kb(_rodata_begin, _rodata_end), 0x92, 0xC0);
		// Data segment
		gdt_set_entry(3, _data_begin / 4096, size_in_4kb(_data_begin, _data_end), 0x92, 0xC0);
		// Bss segment
		gdt_set_entry(4, _bss_begin / 4096, size_in_4kb(_bss_begin, _bss_end), 0x92, 0xC0);
		// Dynamic memory segment
		gdt_set_entry(5, mempool_begin / 4096, size_in_4kb(mempool_begin, mempool_begin + mempool_size), 0x92, 0xC0);
		*/

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
