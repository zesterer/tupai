/*
* 	file : pit.cpp
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
#include <tupai/i686/pit.hpp>
#include <tupai/i686/idt.hpp>
#include <tupai/i686/gdt.hpp>
#include <tupai/i686/port.hpp>
#include <tupai/i686/interrupt.hpp>
#include <tupai/i686/pic.hpp>

#include <tupai/task.hpp>

namespace tupai
{
	const uint32 PIT_MIN_RATE = 1193180;

	const uint16 PIT_CMD_PORT = 0x43;
	const uint16 PIT_DATA_CH0_PORT = 0x40;
	const uint16 PIT_DATA_CH1_PORT = 0x41;
	const uint16 PIT_DATA_CH2_PORT = 0x42;

	volatile counter_t pit_count = 0;
	volatile uint16    pit_rate = 0;

	void (*pit_tick_func)(cpu_pushal, cpu_int) = nullptr;

	// PIT interrupt handler
	extern "C" void pit_irq_handler();
	//asm volatile ("pit_irq_handler: \n call pit_irq_handler_main \n iret");
	asm volatile (
					".section .text \n"
					"	.align 4 \n"
	 				"	pit_irq_handler: \n"
					"		pushal \n"
					"		call pit_irq_handler_main \n"
					"		popal \n"
					"		iret \n"
					);

	void pit_init()
	{
		// Set the PIT IRQ handler
		idt_set_entry(IDT_REMAP_OFFSET + 0x0, (uint32)pit_irq_handler, 1);

		// Every 1 milliseconds
		pit_set_rate(1000);
	}

	void pit_enable()
	{
		// Enable IRQ0 (PIT)
		pic_set_mask(0, true);
	}

	void pit_set_rate(uint16 rate)
	{
		pit_rate = rate;

		uint16 div = PIT_MIN_RATE / pit_rate;

		byte cmd_byte = (0x0 << 0) | (0x3 << 1) | (0x3 << 4) | (0x0 << 6);
		port_out8(PIT_CMD_PORT, cmd_byte);
		port_out8(PIT_DATA_CH0_PORT, (div >> 0) & 0xFF); // LSB
		port_out8(PIT_DATA_CH0_PORT, (div >> 8) & 0xFF); // MSB
	}

	void pit_set_tick_func(void (*func)(cpu_pushal, cpu_int))
	{
		pit_tick_func = func;
	}

	extern "C" void pit_irq_handler_main(cpu_pushal state_pushal, cpu_int state_int)
	{
		interrupt_ack(IDT_REMAP_OFFSET + 0x0);

		pit_count += 1000000 / pit_rate;

		if (pit_tick_func != nullptr)
			pit_tick_func(state_pushal, state_int);
	}
}
