//
// file : cpu.cpp
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
#include <tupai/cpu.hpp>
#include <tupai/irq.hpp>

namespace tupai
{
	namespace cpu
	{
		static const char* exception_names[] =
		{
			"Division By Zero",            // 0
			"Debug",                       // 1
			"Non-Maskable Interrupt",      // 2
			"Breakpoint",                  // 3
			"Overflow",                    // 4
			"Out of Bounds",               // 5
			"Invalid Opcode",              // 6
			"No FPU Coprocessor",          // 7
			"Double Fault",                // 8
			"Coprocessor Segment Overrun", // 9
			"Bad TSS",                    // 10
			"Segment Not Present",        // 11
			"Stack Fault",                // 12
			"General Protection Fault",   // 13
			"Page Fault",                 // 14
			"Reserved Exception",         // 15
			"x87 Floating-Point",         // 16
			"Alignment Check",            // 17
			"Machine Check",              // 18
		};

		bool exception_critical[] =
		{
			false, // Division By Zero
			false, // Debug
			true,  // Non-Maskable Interrupt
			false, // Breakpoint
			true,  // Overflow
			false, // Out of Bounds
			false, // Invalid Opcode
			false, // No FPU Coprocessor
			true,  // Double Fault
			true,  // Coprocessor Segment Overrun
			true,  // Bad TSS
			false, // Segment not present
			false, // Stack fault
			false, // General Protection Fault
			false, // Page Fault
			true,  // Reserved Exception
			false, // x87 Floating-Point
			false, // Alignment Check
			true,  // Machine Check
		};

		void wait()
		{
			asm volatile ("hlt");
		}

		void halt()
		{
			while (true)
				wait();
		}

		void hang()
		{
			while (true)
			{
				irq::disable();
				wait();
			}
		}

		const char* get_exception_name(size_t code)
		{
			if (code <= 18)
				return exception_names[code];
			else
				return "Unknown Exception";
		}

		bool get_exception_critical(size_t code)
		{
			if (code <= 18)
				return exception_critical[code];
			else
				return true;
		}

		extern "C" size_t* in_irq;
		bool is_irq()
		{
			return *in_irq;
		}

		void end_irq()
		{
			*in_irq = 1;
		}
	}
}
