/*
* 	file : kpanic.cpp
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
#include <tupai/kpanic.hpp>
#include <tupai/tty.hpp>

#include <tupai/i686/port.hpp>

// Libk
#include <libk/stdio.hpp>

namespace tupai
{
	extern "C" void _kpanic(const char* msg, uint32 code) __attribute__((noreturn));
	extern "C" umem _kpanic_errormsg;
	extern "C" long _kpanic_errorcode;

	extern "C" void _khalt() __attribute__((noreturn));

	void kpanic(const char* msg, uint32 code)
	{
		//_kpanic_errormsg = (umem)msg;
		//_kpanic_errorcode = code;
		_kpanic(msg, code);
	}

	void khalt()
	{
		_khalt();
	}

	void kbreak()
	{
		// Magic bochs breakpoint
		asm volatile ("xchgw %bx, %bx");
	}

	const char* exceptions[] =
	{
		"[0] Division By Zero",            // 0
		"[1] Debug",                       // 1
		"[2] Non-Maskable Interrupt",      // 2
		"[3] Breakpoint",                  // 3
		"[4] Into Detected Overflow",      // 4
		"[5] Out of Bounds",               // 5
		"[6] Invalid Opcode",              // 6
		"[7] No Coprocessor",              // 7
		"[8] Double Fault",                // 8
		"[9] Coprocessor Segment Overrun", // 9
		"[10] Bad TSS",                    // 10
		"[11] Segment Not Present",        // 11
		"[12] Stack Fault",                // 12
		"[13] General Protection Fault",   // 13
		"[14] Page Fault",                 // 14
		"[15] Unknown Interrupt",          // 15
		"[16] x87 Floating-Point",         // 16
		"[17] Alignment Check",            // 17
		"[18] Machine Check",              // 18
	};

	extern "C" void kfault(uint32 isr_id, uint32 error) // To be called by ISRs only!
	{
		/* write EOI */
		port_out8(0x20, 0x20);

		const char* exception_msg = exceptions[isr_id % (sizeof(exceptions) / sizeof(char*))];
		libk::printf("%s exception occured (code = %X)!\n", isr_id, exception_msg, error);
		kpanic(exception_msg, error);
	}
}
