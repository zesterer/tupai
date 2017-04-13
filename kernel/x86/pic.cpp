//
// file : pic.hpp
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
		const uint16_t PORT_PIC1 = 0x20; // I/O offset address for master PIC
		const uint16_t PORT_PIC2 = 0xA0; // I/O offset address for slave PIC

		const uint16_t PORT_PIC1_CMD = PORT_PIC1; // I/O offset address for master PIC commands
		const uint16_t PORT_PIC2_CMD = PORT_PIC2; // I/O offset address for slave PIC commands

		const uint16_t PORT_PIC1_DATA = PORT_PIC1 + 1; // I/O offset address for master PIC data
		const uint16_t PORT_PIC2_DATA = PORT_PIC2 + 1; // I/O offset address for slave PIC data

		const uint8_t ICW1_ICW4      = 0x01; // ICW4 (Interrupt Command Word)
		const uint8_t ICW1_SINGLE    = 0x02; // Single (cascade) mode
		const uint8_t ICW1_INTERVAL4 = 0x04; // Call address interval 4 (8)
		const uint8_t ICW1_LEVEL     = 0x08; // Level triggered (edge) mode
		const uint8_t ICW1_INIT      = 0x10; // Initialization - required!

		const uint8_t ICW4_8086        = 0x01; // 8086/88 (MCS-80/85) mode
		const uint8_t ICW4_AUTO        = 0x02; // Auto (normal) EOI
		const uint8_t ICW4_BUFF_SLAVE  = 0x08; // Buffered mode / slave
		const uint8_t ICW4_BUFF_MASTER = 0x0C; // Buffered mode / master
		const uint8_t ICW4_SFNM        = 0x10; // Special fully nested (not)

		void pic_init()
		{
			uint8_t offset = PIC_REMAP_OFFSET;

			// 8259 PIC initiation code

			// Preserve masks
			uint8_t masks[2];
			masks[0] = inb(PORT_PIC1_DATA);
			masks[1] = inb(PORT_PIC1_DATA);

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
			outb(PORT_PIC2_DATA, offset); // ICW2 - Slave PIC vector offset
			wait(150);

			outb(PORT_PIC1_DATA, 0x4); // ICW3 - Master PIC needs to know it has a slave PIC at IRQ2 (0x00000100)
			wait(150);
			outb(PORT_PIC2_DATA, 0x2); // ICW3 - Slave PIC needs to know it's cascade identity (0x00000010)
			wait(150);

			outb(PORT_PIC1_DATA, ICW4_8086); // ICW4 - Tell the PIC to operate in 8086 mode
			wait(150);
			outb(PORT_PIC2_DATA, ICW4_8086); // ICW4 - Tell the PIC to operate in 8086 mode
			wait(150);

			// Restore masks
			outb(PORT_PIC1_DATA, masks[0]);
			outb(PORT_PIC2_DATA, masks[1]);

			// Mask all IRQs to prevent them randomly occuring
			// Individual hardware drivers can re-enable them later
			for (int i = 0; i < 16; i ++)
				pic_mask(i, false);

			debug_print(
				"PIC initiated with properties:", '\n',
				"  IRQ offset -> ", offset, '\n'
			);
		}

		void pic_ack(uint8_t irq)
		{
			debug_println("PIC acknowledged IRQ ", irq, " (UNIMPLEMENTED!)");
		}

		void pic_mask(uint8_t irq, bool enable)
		{
			uint16_t port;
			uint8_t  value;

			if (irq < 8)
				port = PORT_PIC1_DATA;
			else
			{
				port = PORT_PIC2_DATA;
				irq -= 8;
			}

			uint8_t val = inb(port);
			wait(150);

			if (enable)
				value = val & ~(1 << irq);
			else
				value = val | (1 << irq);

			outb(port, value);
			wait(150);
		}
	}
}
