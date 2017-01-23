/*
* 	file : idt.cpp
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
#include <tupai/i686/idt.hpp>
#include <tupai/i686/gdt.hpp>
#include <tupai/i686/port.hpp>

#include <tupai/tty.hpp>
#include <tupai/util/conv.hpp>

namespace tupai
{
	extern "C" void _isr_0();
	extern "C" void _isr_1();
	extern "C" void _isr_2();
	extern "C" void _isr_3();
	extern "C" void _isr_4();
	extern "C" void _isr_5();
	extern "C" void _isr_6();
	extern "C" void _isr_7();
	extern "C" void _isr_8();
	extern "C" void _isr_9();
	extern "C" void _isr_10();
	extern "C" void _isr_11();
	extern "C" void _isr_12();
	extern "C" void _isr_13();
	extern "C" void _isr_14();
	extern "C" void _isr_15();
	extern "C" void _isr_16();
	extern "C" void _isr_17();
	extern "C" void _isr_18();

	idt_entry idt[IDT_SIZE] __attribute__((aligned(8)));

	extern "C" idt_desc idt_ptr;
	idt_desc idt_ptr;
	extern "C" umem idt_ptr_ptr;
	umem idt_ptr_ptr;

	// Default interrupt handler
	extern "C" void default_irq_handler();
	asm volatile (
					".section .text \n"
					"	.align 4 \n"
	 				"	default_irq_handler: \n"
					"		pushal \n"
					"		call default_irq_handler_main \n"
					"		popal \n"
					"		iret \n"
					);

	void idt_init()
	{
		// Init ICW1
		port_out8(0x20, 0x11); // Master PIC Command
		port_out8(0xA0, 0x11); // Slave PIC Command

		// Init ICW2 (remap offset address)
		// x86 protected mode requires us to remap the PICs beyond 0x20 since first 32 are reserved
		port_out8(0x21, IDT_REMAP_OFFSET + 0x0); // Master PIC Data
		port_out8(0xA1, IDT_REMAP_OFFSET + 0x8); // Slave PIC Data

		// Init ICW3 (cascading)
		port_out8(0x21, 0x00); // Master PIC Data
		port_out8(0xA1, 0x00); // Slave PIC Data

		// Init ICW4 (environment information)
		port_out8(0x21, 0x01); // Master PIC Data
		port_out8(0xA1, 0x01); // Slave PIC Data

		// Mask interrupts
		port_out8(0x21, 0xFF); // Master PIC Data
		port_out8(0xA1, 0xFF); // Slave PIC Data

		const uint16 code_segment_index = 0x08;//sizeof(gdt_entry) * 1;

		// Assign exception ISRs
		idt_set_entry(0, (uint32)_isr_0, code_segment_index);
		idt_set_entry(1, (uint32)_isr_1, code_segment_index);
		idt_set_entry(2, (uint32)_isr_2, code_segment_index);
		idt_set_entry(3, (uint32)_isr_3, code_segment_index);
		idt_set_entry(4, (uint32)_isr_4, code_segment_index);
		idt_set_entry(5, (uint32)_isr_5, code_segment_index);
		idt_set_entry(6, (uint32)_isr_6, code_segment_index);
		idt_set_entry(7, (uint32)_isr_7, code_segment_index);
		idt_set_entry(8, (uint32)_isr_8, code_segment_index);
		idt_set_entry(9, (uint32)_isr_9, code_segment_index);
		idt_set_entry(10, (uint32)_isr_10, code_segment_index);
		idt_set_entry(11, (uint32)_isr_11, code_segment_index);
		idt_set_entry(12, (uint32)_isr_12, code_segment_index);
		idt_set_entry(13, (uint32)_isr_13, code_segment_index);
		idt_set_entry(14, (uint32)_isr_14, code_segment_index);
		idt_set_entry(15, (uint32)_isr_15, code_segment_index);
		idt_set_entry(16, (uint32)_isr_16, code_segment_index);
		idt_set_entry(17, (uint32)_isr_17, code_segment_index);
		idt_set_entry(18, (uint32)_isr_18, code_segment_index);

		// Fill with the blank interrupt handler for now
		for (int16 i = IDT_REMAP_OFFSET; i < (int16)(IDT_SIZE); i ++)
			idt_set_entry(i, (uint32)default_irq_handler, code_segment_index);
	}

	void idt_install()
	{
		idt_ptr.size = (sizeof(idt_entry) * IDT_SIZE) + (((umem)idt & 0xFFFF) << 16);
		idt_ptr.offset = (umem)idt >> 16;

		idt_ptr_ptr = (umem)&idt_ptr;
		asm volatile (
						"lidt (((idt_ptr))) \n" // Set the IDT
						"ljmp $8, $_idt_longjump \n"
						"_idt_longjump: \n"
						);
	}

	void idt_set_entry(smem irq, uint32 address, uint16 selector)
	{
		// Base address
		idt[irq].base_addr_low = address & 0xFFFF;
		idt[irq].base_addr_high = (address & 0xFFFF0000) >> 16;

		idt[irq].selector = selector;
		idt[irq].zero = 0;
		idt[irq].type_attr = 0x8E;

		idt_install();
	}

	extern "C" void default_irq_handler_main()
	{
		/* write EOI */
		//port_out8(0x20, 0x20);
		//kbreak();

		tty_write_str("Interrupt occured!\n");
	}
}
