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
#include <tupai/x86/amd64/gdt.hpp>
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
		namespace amd64
		{
			struct gdt_desc_t
			{
				enum class access : uint8_t
				{
					READABLE   = 0b0000001'0,
					WRITABLE   = 0b0000001'0,
					EXECUTE    = 0b00001'000,
					PRESENT    = 0b1'0000000,
					ONE        = 0b0001'0000,
					CONFORMING = 0b000001'00,
				};

				enum class granularity : uint8_t
				{
					PAGE         = 0b1'000,
					PROTECTED_32 = 0b01'00,
					LONG_64      = 0b001'0,
				};

				uint16_t size_lo;
				uint16_t offset_lo;
				uint8_t  offset_mid;
				uint8_t  access_flags;
				uint8_t  granularity_flags;
				uint8_t  offset_hi;
			} __attribute__((packed));

			struct gdt_ptr_t
			{
				uint16_t size;
				uint64_t offset;
			} __attribute__((packed));

			// The GDT
			const size_t GDT_LENGTH = 3;
			gdt_desc_t gdt[GDT_LENGTH] __attribute__((aligned(ARCH_PAGE_SIZE)));

			// GDT Pointer
			extern "C" gdt_ptr_t gdt_ptr;
			gdt_ptr_t gdt_ptr __attribute__((aligned(4)));

			void gdt_set_entry(size_t n, uint32_t offset, uint32_t size, uint8_t access_flags, uint8_t granularity_flags);

			void gdt_init()
			{
				// Define flags for the code and data segments, along with granularity for both
				uint8_t code_access_flags =
					(uint8_t)gdt_desc_t::access::READABLE |
					(uint8_t)gdt_desc_t::access::EXECUTE |
					(uint8_t)gdt_desc_t::access::ONE |
					(uint8_t)gdt_desc_t::access::CONFORMING |
					(uint8_t)gdt_desc_t::access::PRESENT;
				uint8_t data_access_flags =
					(uint8_t)gdt_desc_t::access::WRITABLE |
					(uint8_t)gdt_desc_t::access::ONE |
					(uint8_t)gdt_desc_t::access::PRESENT;
				uint8_t granularity_flags =
					(uint8_t)gdt_desc_t::granularity::PAGE |
					(uint8_t)gdt_desc_t::granularity::LONG_64;

				gdt_set_entry(0, 0x0, 0x0, 0, 0); // Null segment entry
				gdt_set_entry(1, 0x0, 0xFFFFF, code_access_flags, granularity_flags); // Code segment entry
				gdt_set_entry(2, 0x0, 0xFFFFF, data_access_flags, granularity_flags); // Data segment entry

				debug_println("GDT initiated");
			}

			//struct gdt_ljmp_ptr_t { uint64_t ptr; uint16_t cs; } __attribute__((packed));
			//extern "C" gdt_ljmp_ptr_t gdt_ljmp_ptr;
			//gdt_ljmp_ptr_t gdt_ljmp_ptr;
			//extern "C" void gdt_ljmp();
			void gdt_install()
			{
				gdt_ptr.size = sizeof(gdt_desc_t) * GDT_LENGTH - 1;
				gdt_ptr.offset = (uint64_t)&gdt;

				debug_println("GDT_PTR is at ", &gdt_ptr);

				//gdt_ljmp_ptr.ptr = (uint64_t)gdt_ljmp;
				//gdt_ljmp_ptr.cs = 0x0008;

				asm volatile
				(
					"movabs $gdt_ptr, %rax\n"
					"lgdt (%rax)\n"

					// Far call
					//"xchg %bx, %bx\n"
					/*
					"movabs $gdt_ljmp, %rax\n"
					"ljmp *(%rax)\n"
					"push $0x8\n"
					"push $0f\n"
					"ljmp *(%rsp)\n"
					"0:\n"
					"add $16, %rsp\n"
					*/
					"gdt_ljmp:\n"

					// Reload segment registers
					"mov $0x10, %ax\n"
					"mov %ax, %ds\n"
					"mov %ax, %es\n"
					"mov %ax, %fs\n"
					"mov %ax, %gs\n"
					"mov %ax, %ss\n"
				);

				debug_print(
					"GDT installed with properties:\n",
					"  location -> ", &gdt, '\n',
					"  size     -> ", sizeof(gdt_desc_t) * GDT_LENGTH, '\n'
				);
				debug_println("Segment registers reset");
			}

			void gdt_set_entry(size_t n, uint32_t offset, uint32_t size, uint8_t access_flags, uint8_t granularity_flags)
			{
				// Offset address
				gdt[n].offset_lo  = (offset >>  0) & 0xFFFF;
				gdt[n].offset_mid = (offset >> 16) & 0xFF;
				gdt[n].offset_hi  = (offset >> 24) & 0xFF;

				// Size
				gdt[n].size_lo = size & 0xFFFF;
				gdt[n].granularity_flags = (size >> 16) & 0xF;

				// Granularity & access
				gdt[n].granularity_flags |= (granularity_flags << 4) & 0xF0;
				gdt[n].access_flags = access_flags;

				debug_print(
					"Set GDT entry ", n, " to:\n",
					//"  offset      -> ", util::fmt_int<uint32_t>(offset, 16, 8), '\n',
					//"  size        -> ", util::fmt_int<uint32_t>(size, 16), '\n',
					"  access      -> ", util::fmt_int<uint8_t>(access_flags, 16), '\n',
					"  granularity -> ", util::fmt_int<uint8_t>(granularity_flags, 16), '\n'
				);
			}
		}
	}
}
