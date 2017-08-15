//
// file : idt.cpp
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
#include <tupai/x86/i386/idt.hpp>
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
		namespace i386
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
				uint8_t  zero;
				uint8_t  type_attr;
				uint16_t offset_hi;
			} __attribute__((packed));

			struct idt_ptr_t
			{
				uint16_t size;
				uint32_t offset;
			} __attribute__((packed));

			// The IDT
			const size_t IDT_LENGTH = 256 + PIC_REMAP_OFFSET;
			idt_desc_t idt[IDT_LENGTH] __attribute__((aligned(ARCH_PAGE_SIZE)));

			// IDT Pointer
			extern "C" idt_ptr_t idt_ptr;
			idt_ptr_t idt_ptr;

			// IDT default handler
			extern "C" void isr_stub();
			extern "C" size_t stub_isr_main(size_t stack)
			{
				panic("Undefined interrupt occured");

				return stack;
			}

			extern "C" void isr_spurious();
			extern "C" size_t spurious_isr_main(size_t stack)
			{
				debug_println("Spurious interrupt occured");

				return stack;
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
					idt_set_entry(i, (void*)&isr_stub);

				// Set the spurious interrupt handler
				idt_set_entry(PIC_REMAP_OFFSET + 7, (void*)isr_spurious);

				debug_println("IDT initiated");
			}

			void idt_install()
			{
				idt_ptr.size = sizeof(idt_desc_t) * IDT_LENGTH - 1;
				idt_ptr.offset = (uint32_t)&idt;

				debug_print(
					"IDT has the following properties:\n",
					"  location -> ", &idt, '\n',
					"  size     -> ", sizeof(idt_desc_t) * IDT_LENGTH, '\n'
				);

				asm volatile
				(
					"lidt (idt_ptr)\n"
				);
			}

			void idt_set_entry(size_t irq, void* address, uint16_t selector_id)
			{
				// Set address (lo and hi parts)
				idt[irq].offset_lo = ((uint32_t)address >>  0) & 0xFFFF;
				idt[irq].offset_hi = ((uint32_t)address >> 16) & 0xFFFF;

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
