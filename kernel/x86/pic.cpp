//
// file : pic.cpp
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
#include <tupai/x86/pic.hpp>
#include <tupai/x86/port.hpp>
#include <tupai/debug.hpp>

// Standard
#include <stddef.h>
#include <stdint.h>

namespace tupai
{
	namespace x86
	{
		static const uint16_t PORT_PIC1 = 0x20; // I/O offset address for master PIC
		static const uint16_t PORT_PIC2 = 0xA0; // I/O offset address for slave PIC

		static const uint16_t PORT_PIC1_CMD = PORT_PIC1; // I/O offset address for master PIC commands
		static const uint16_t PORT_PIC2_CMD = PORT_PIC2; // I/O offset address for slave PIC commands

		static const uint16_t PORT_PIC1_DATA = PORT_PIC1 + 1; // I/O offset address for master PIC data
		static const uint16_t PORT_PIC2_DATA = PORT_PIC2 + 1; // I/O offset address for slave PIC data

		static const uint8_t ICW1_ICW4      = 0x01; // ICW4 (Interrupt Command Word)
		static const uint8_t ICW1_SINGLE    = 0x02; // Single (cascade) mode
		static const uint8_t ICW1_INTERVAL4 = 0x04; // Call address interval 4 (8)
		static const uint8_t ICW1_LEVEL     = 0x08; // Level triggered (edge) mode
		static const uint8_t ICW1_INIT      = 0x10; // Initialization - required!

		static const uint8_t ICW4_8086        = 0x01; // 8086/88 (MCS-80/85) mode
		static const uint8_t ICW4_AUTO        = 0x02; // Auto (normal) EOI
		static const uint8_t ICW4_BUFF_SLAVE  = 0x08; // Buffered mode / slave
		static const uint8_t ICW4_BUFF_MASTER = 0x0C; // Buffered mode / master
		static const uint8_t ICW4_SFNM        = 0x10; // Special fully nested (not)

		static const uint8_t OCW3_READ_IRR = 0x0A; // OCW3 irq ready next CMD read
		static const uint8_t OCW3_READ_ISR = 0x0B; // OCW3 irq service next CMD read

		static const uint8_t PIC_EOI = 0x20; // PIC EOI byte

		void pic_bind()
		{
			// Do nothing yet
		}

		void pic_init()
		{
			uint8_t offset = PIC_REMAP_OFFSET;

			// 8259 PIC initiation code

			// Start initiation (in cascade mode)
			outb(PORT_PIC1_CMD, ICW1_INIT | ICW1_ICW4);
			wait(150);
			outb(PORT_PIC2_CMD, ICW1_INIT | ICW1_ICW4);
			wait(150);

			// Initiation has begun. The PIC now waits for 3 'initiation bytes' on the data port
			// These 'initiation bytes' are:
			//   -> Vector offset (ICW2) (for remapping)
			//   -> Wiring info (ICW3) (masters / slaves)
			//   -> Additional environment info (ICW4)

			outb(PORT_PIC1_DATA, offset); // ICW2 - Master PIC vector offset
			wait(150);
			outb(PORT_PIC2_DATA, offset + 0x8); // ICW2 - Slave PIC vector offset
			wait(150);

			outb(PORT_PIC1_DATA, 0x4); // ICW3 - Master PIC needs to know it has a slave PIC at IRQ2 (0x00000100)
			wait(150);
			outb(PORT_PIC2_DATA, 0x2); // ICW3 - Slave PIC needs to know it's cascade identity (0x00000010)
			wait(150);

			outb(PORT_PIC1_DATA, ICW4_8086); // ICW4 - Tell the PIC to operate in 8086 mode
			wait(150);
			outb(PORT_PIC2_DATA, ICW4_8086); // ICW4 - Tell the PIC to operate in 8086 mode
			wait(150);

			// Mask all interrupts
			outb(PORT_PIC1_DATA, 0xFF);
			outb(PORT_PIC2_DATA, 0xFF);

			debug_print(
				"PIC initiated with properties:", '\n',
				"  IRQ offset -> ", offset, '\n'
			);
		}

		uint16_t __pic_get_irq_reg(uint8_t ocw3)
		{
			/* OCW3 to PIC CMD to get the register values.  PIC2 is chained, and
			 * represents IRQs 8-15.  PIC1 is IRQs 0-7, with 2 being the chain */
			outb(PORT_PIC1_CMD, ocw3);
			outb(PORT_PIC2_CMD, ocw3);
			return (inb(PORT_PIC2_CMD) << 8) | inb(PORT_PIC1_CMD);
		}

		uint16_t pic_get_irr()
		{
			return __pic_get_irq_reg(OCW3_READ_IRR);
		}

		uint16_t pic_get_isr()
		{
			return __pic_get_irq_reg(OCW3_READ_ISR);
		}

		void pic_ack(uint8_t irq)
		{
			/* write EOI */

			if (irq >= 16) // It's not a valid irq
				return;

			if (irq >= 8) // It's a PIC2
				outb(PORT_PIC2_CMD, PIC_EOI);

			// PIC1 EOI must be called anyway
			outb(PORT_PIC1_CMD, PIC_EOI);
		}

		void pic_mask(uint8_t irq, bool enable)
		{
			uint16_t port;

			if (irq < 8)
				port = PORT_PIC1_DATA;
			else if (irq < 16)
			{
				port = PORT_PIC2_DATA;
				irq -= 8;
			}
			else
				return;

			uint8_t value = inb(port);
			wait(150);

			if (enable)
				value = value & ~(1 << irq);
			else
				value = value | (1 << irq);

			outb(port, value);
			wait(150);
		}
	}
}
