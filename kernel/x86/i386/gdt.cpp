//
// file : gdt.cpp
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
#include <tupai/x86/i386/gdt.hpp>
#include <tupai/arch.hpp>
#include <tupai/debug.hpp>
#include <tupai/panic.hpp>

// Standard
#include <stddef.h>
#include <stdint.h>

namespace tupai
{
	namespace x86
	{
		namespace i386
		{
			struct gdt_desc_t
			{
				enum class attr : uint8_t
				{

				};

				uint16_t size_lo;
				uint16_t offset_lo;
				uint8_t  offset_mid;
				uint8_t  access;
				uint8_t  granularity;
				uint8_t  offset_hi;
			} __attribute__((packed));

			struct gdt_ptr_t
			{
				uint16_t size;
				uint32_t offset;
			} __attribute__((packed));

			// The GDT
			const size_t GDT_LENGTH = 3;
			gdt_desc_t gdt[GDT_LENGTH] __attribute__((aligned(ARCH_PAGE_SIZE)));

			// GDT Pointer
			extern "C" gdt_ptr_t gdt_ptr;
			gdt_ptr_t gdt_ptr __attribute__((aligned(4)));

			void gdt_set_entry(size_t n, uint32_t offset, uint32_t size, uint8_t access, uint8_t granularity);

			void gdt_init()
			{
				gdt_set_entry(0, 0x0, 0x0, 0, 0); // Null segment entry
				gdt_set_entry(1, 0x0, 0xFFFFFF, 0x9A, 0xC); // Code segment entry
				gdt_set_entry(2, 0x0, 0xFFFFFF, 0x92, 0xC); // Data segment entry

				debug_println("GDT initiated!");
			}

			void gdt_install()
			{
				gdt_ptr.size = sizeof(gdt_desc_t) * GDT_LENGTH - 1;
				gdt_ptr.offset = (uint32_t)&gdt;

				debug_println("GDT_PTR is at ", &gdt_ptr);
				debug_print(
					"GDT has the following properties:\n",
					"  location -> ", &gdt, '\n',
					"  size     -> ", sizeof(gdt_desc_t) * GDT_LENGTH, '\n'
				);

				asm volatile ("xchg %bx, %bx");

				asm volatile
				(
					"lgdt (((gdt_ptr)))\n"
					//"ljmp $8, $idt_install_ljmp\n"
					//"idt_install_ljmp:\n"
				);

				asm volatile ("xchg %bx, %bx");
			}

			void gdt_set_entry(size_t n, uint32_t offset, uint32_t size, uint8_t access, uint8_t granularity)
			{
				// Offset address
				gdt[n].offset_lo  = (offset >>  0) & 0xFFFF;
				gdt[n].offset_mid = (offset >> 16) & 0xFF;
				gdt[n].offset_hi  = (offset >> 24) & 0xFF;

				// Size
				gdt[n].size_lo = size & 0xFFFF;
				gdt[n].granularity = (size >> 16) & 0xF;

				// Granularity & access
				gdt[n].granularity |= (granularity << 4) & 0xF0;
				gdt[n].access = access;

				debug_print(
					"Set GDT entry ", n, " to:\n",
					"  offset      -> ", (void*)offset, '\n',
					"  size        -> ", (void*)size, '\n',
					"  access      -> ", access, '\n',
					"  granularity -> ", granularity, '\n'
				);
			}
		}
	}
}
