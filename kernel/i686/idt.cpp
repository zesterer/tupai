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
#include <tupai/i686/port.hpp>
#include <tupai/tty.hpp>

namespace tupai
{
	namespace i686
	{
		struct idt_entry
		{
			uint16 base_addr_low;
			uint16 selector;
			uint8  zero;
			uint8  type_attr;
			uint16 base_addr_high;

		} __attribute((packed));

		struct idt_desc
		{
			uint16 size;
			uint32 offset;

		} __attribute((packed));

		const umem IDT_SIZE = 256;
		const umem IDT_REMAP_OFFSET = 0x20;

		idt_entry idt[IDT_SIZE];

		extern "C" idt_desc idt_ptr;
		idt_desc idt_ptr;
		extern "C" umem idt_ptr_ptr;
		umem idt_ptr_ptr;

		// Default interrupt handler
		extern "C" void default_irq_handler();
		asm volatile ("default_irq_handler: \n cli \n call default_irq_handler_main \n sti \n iret");

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

			// Fill with the blank interrupt handler for now
			for (uint16 i = 0; i < IDT_SIZE - IDT_REMAP_OFFSET; i ++)
				idt_set_entry(i, (uint32)default_irq_handler, 0x01);
		}

		void idt_install()
		{
			idt_ptr.size = (sizeof(idt_entry) * IDT_SIZE) + (((umem)idt & 0xFFFF) << 16);
			idt_ptr.offset = (umem)idt >> 16;

			idt_ptr_ptr = (umem)&idt_ptr;
			asm volatile ("lidt (((idt_ptr)))");
		}

		void idt_set_entry(umem irq, uint32 address, uint16 selector)
		{
			// Base address
			idt[IDT_REMAP_OFFSET + irq].base_addr_low = address & 0xFFFF;
			idt[IDT_REMAP_OFFSET + irq].base_addr_high = (address & 0xFFFF0000) >> 16;

			idt[IDT_REMAP_OFFSET + irq].selector = selector;
			idt[IDT_REMAP_OFFSET + irq].zero = 0;
			idt[IDT_REMAP_OFFSET + irq].type_attr = 0x8E;

			idt_install();
		}

		extern "C" void default_irq_handler_main()
		{
			tty_write_str("Interrupt!\n");
		}
	}
}
