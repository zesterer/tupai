//
// file : pit.cpp
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
#include <tupai/x86/pit.hpp>
#include <tupai/x86/port.hpp>
#include <tupai/irq.hpp>
#include <tupai/task/scheduler.hpp>
#include <tupai/debug.hpp>

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

		static volatile uint64_t pit_time = 0; // Nanoseconds
		static volatile uint32_t pit_rate = 0;

		static void pit_set_rate(int rate);
		extern "C" void isr_pit();
		extern "C" size_t pit_isr_main(size_t sp);

		void pit_bind()
		{
			// Bind the interrupt
			irq::bind(0, (void*)isr_pit, true);
		}

		void pit_init()
		{
			// Every 1,000 nanoseconds
			pit_set_rate(1000);

			// Unmask the interrupt
			irq::mask(0, true);

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

		size_t pit_isr_main(size_t sp)
		{
			// ACK the interrupt
			irq::ack(0);

			pit_time += 1000000 / pit_rate; // Nanoseconds / rate

			// Switch threads

			//if (sys::threading_enabled())
			//	stack_ptr = sys::thread_next_stack(stack_ptr);
			return task::scheduler_preempt(sp);
		}
	}
}
