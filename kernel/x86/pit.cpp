//
// file : pit.cpp
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
#include <tupai/x86/pit.hpp>
#include <tupai/x86/pic.hpp>
#include <tupai/x86/port.hpp>
#include <tupai/interrupt.hpp>
#include <tupai/debug.hpp>

// Multithreading
#include <tupai/sys/thread.hpp>

// Standard
#include <stddef.h>
#include <stdint.h>

namespace tupai
{
	namespace x86
	{
		static const uint16_t PORT_PIT_CMD = 0x0043; // I/O offset address for PIT commands

		static const uint16_t PORT_PIT_DATA_CH0 = 0x0040; // I/O offset address for PIT data (CH0)
		static const uint16_t PORT_PIT_DATA_CH1 = 0x0041; // I/O offset address for PIT data (CH1)
		static const uint16_t PORT_PIT_DATA_CH2 = 0x0042; // I/O offset address for PIT data (CH2)

		static const uint32_t PIT_MAX_RATE = 1193180; // 1,193,180 Hz (1.13 MHz)

		static volatile long pit_time = 0; // Nanoseconds
		static volatile int  pit_rate = 0;

		static void pit_set_rate(int rate);
		extern "C" void isr_pit();
		extern "C" void* pit_isr_main(void* stack_ptr);

		void pit_init()
		{
			// Every millisecond
			pit_set_rate(1000);

			// Bind the interrupt
			interrupt_bind(PIC_REMAP_OFFSET + 0, (void*)isr_pit);

			// Unmask the interrupt
			pic_mask(0, true);

			debug_print(
				"PIT initiated with properties:", '\n',
				"  rate -> ", pit_rate, '\n'
			);
		}

		void pit_set_rate(int rate)
		{
			pit_rate = rate;

			// Find the divisor value
			uint16_t div = PIT_MAX_RATE / pit_rate;

			uint8_t cmd = (0x3 << 1) | (0x3 << 4); // square wave generator and lo/hi byte access
			outb(PORT_PIT_CMD, cmd);
			outb(PORT_PIT_DATA_CH0, (div >> 0) & 0xFF); // Lo byte
			outb(PORT_PIT_DATA_CH0, (div >> 8) & 0xFF); // Hi byte

			debug_println("PIT rate set to ", pit_rate);
		}

		void* pit_isr_main(void* stack_ptr)
		{
			// ACK the interrupt
			pic_ack(0);

			pit_time += 1000000 / pit_rate; // Nanoseconds / rate

			// Switch threads

			sys::thread_update(sys::thread_get_id(), stack_ptr);// First, reset the current thread's stack

			if (sys::threading_enabled())
				stack_ptr = sys::thread_next_stack();
			return stack_ptr;
		}
	}
}
