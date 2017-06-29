//
// file : panic.cpp
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
#include <tupai/panic.hpp>
#include <tupai/util/out.hpp>

namespace tupai
{
	const char* exceptions[] =
	{
		"Division By Zero",            // 0
		"Debug",                       // 1
		"Non-Maskable Interrupt",      // 2
		"Breakpoint",                  // 3
		"Into Detected Overflow",      // 4
		"Out of Bounds",               // 5
		"Invalid Opcode",              // 6
		"No Coprocessor",              // 7
		"Double Fault",                // 8
		"Coprocessor Segment Overrun", // 9
		"Bad TSS",                    // 10
		"Segment Not Present",        // 11
		"Stack Fault",                // 12
		"General Protection Fault",   // 13
		"Page Fault",                 // 14
		"Unknown Interrupt",          // 15
		"x87 Floating-Point",         // 16
		"Alignment Check",            // 17
		"Machine Check",              // 18
	};

	void exception_panic(uint32_t id)
	{
		panic(exceptions[id]);
	}

	void panic(const char* msg)
	{
		util::println("[PANIC] ", msg);
		hang();
	}
}
