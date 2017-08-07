//
// file : cpu.cpp
//
// This file is part of Tupai.
//
// Tupai is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
//
// Tupai is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with Tupai.  If not, see <http://www.gnu.org/licenses/>.
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
	}
}
