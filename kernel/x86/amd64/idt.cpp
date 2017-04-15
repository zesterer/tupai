//
// file : idt.cpp
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
#include <tupai/x86/amd64/idt.hpp>
#include <tupai/x86/pic.hpp>
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
			extern "C" void isr_0();
			extern "C" void isr_1();
			extern "C" void isr_2();
			extern "C" void isr_3();
			extern "C" void isr_4();
			extern "C" void isr_5();
			extern "C" void isr_6();
			extern "C" void isr_7();
			extern "C" void isr_8();
			extern "C" void isr_9();
			extern "C" void isr_10();
			extern "C" void isr_11();
			extern "C" void isr_12();
			extern "C" void isr_13();
			extern "C" void isr_14();
			extern "C" void isr_15();
			extern "C" void isr_16();
			extern "C" void isr_17();
			extern "C" void isr_18();

			struct idt_desc_t
			{
				enum class attr : uint8_t
				{
					GATE_TASK = 0b0101,
					GATE_INT  = 0b1110,
					GATE_TRAP = 0b1111,

					STORAGE_SEG = 0b1'0000,

					DPL_0 = 0b00'00000,
					DPL_1 = 0b01'00000,
					DPL_2 = 0b10'00000,
					DPL_3 = 0b11'00000,

					PRESENT = 0b1'0000000,
				};

				uint16_t offset_lo;
				uint16_t selector;
				uint8_t  ist;
				uint8_t  type_attr;
				uint16_t offset_mid;
				uint32_t offset_hi;
				uint32_t zero;
			} __attribute__((packed));

			struct idt_ptr_t
			{
				uint16_t size;
				uint64_t offset;
			} __attribute__((packed));

			// The IDT
			const size_t IDT_LENGTH = 256 + PIC_REMAP_OFFSET;
			idt_desc_t idt[IDT_LENGTH] __attribute__((aligned(ARCH_PAGE_SIZE)));

			// IDT Pointer
			extern "C" idt_ptr_t idt_ptr;
			idt_ptr_t idt_ptr;

			// IDT default handler
			extern "C" void isr_handler_stub();
			asm volatile
			(
				".section .text\n"
				"	.align 8\n"
				"	isr_handler_stub:\n"
				//"		pushal\n"
				"		call isr_handler_stub_main\n"
				//"		popal\n"
				"		iretq\n"
			);

			extern "C" void isr_handler_stub_main()
			{
				panic("Undefined interrupt occured");
			}

			void idt_init()
			{
				// Set exception ISR entries
				idt_set_entry( 0, (void*)isr_0);
				idt_set_entry( 1, (void*)isr_1);
				idt_set_entry( 2, (void*)isr_2);
				idt_set_entry( 3, (void*)isr_3);
				idt_set_entry( 4, (void*)isr_4);
				idt_set_entry( 5, (void*)isr_5);
				idt_set_entry( 6, (void*)isr_6);
				idt_set_entry( 7, (void*)isr_7);
				idt_set_entry( 8, (void*)isr_8);
				idt_set_entry( 9, (void*)isr_9);
				idt_set_entry(10, (void*)isr_10);
				idt_set_entry(11, (void*)isr_11);
				idt_set_entry(12, (void*)isr_12);
				idt_set_entry(13, (void*)isr_13);
				idt_set_entry(14, (void*)isr_14);
				idt_set_entry(15, (void*)isr_15);
				idt_set_entry(16, (void*)isr_16);
				idt_set_entry(17, (void*)isr_17);
				idt_set_entry(18, (void*)isr_18);

				// Assign IDT entries to the stub handler for now
				for (size_t i = PIC_REMAP_OFFSET; i < IDT_LENGTH; i ++)
					idt_set_entry(i, (void*)&isr_handler_stub);

				debug_println("IDT initiated");
			}

			void idt_install()
			{
				idt_ptr.size = sizeof(idt_desc_t) * IDT_LENGTH - 1;
				idt_ptr.offset = (uint64_t)&idt;

				debug_print(
					"IDT has the following properties:\n",
					"  location -> ", &idt, '\n',
					"  size     -> ", sizeof(idt_desc_t) * IDT_LENGTH, '\n'
				);

				asm volatile
				(
					"movabs $idt_ptr, %rax\n"
					"lidt (%rax)\n"
					//"ljmp $8, $idt_install_ljmp\n"
					//"idt_install_ljmp:\n"
				);
			}

			void idt_set_entry(size_t irq, void* address, uint16_t selector_id)
			{
				// Set address (lo and hi parts)
				idt[irq].offset_lo  = ((uint64_t)address >>  0) & 0xFFFF;
				idt[irq].offset_mid = ((uint64_t)address >> 16) & 0xFFFF;
				idt[irq].offset_hi  = ((uint64_t)address >> 32) & 0xFFFFFFFF;

				// Set the code selector from the GDT. It's measured in bytes, so x8 (since GDT entries are 8 bytes big)
				idt[irq].selector = selector_id * 8;

				// Obvious
				idt[irq].zero = 0;

				// Set 'interrupt' and 'present' flags
				idt[irq].type_attr =
					(uint8_t)idt_desc_t::attr::GATE_INT |
					(uint8_t)idt_desc_t::attr::PRESENT;

				//debug_println("Set IDT IRQ enty ", irq, " to ", address);
			}
		}
	}
}
