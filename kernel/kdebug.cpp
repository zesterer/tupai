/*
* 	file : kdebug.cpp
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
#include <tupai/kdebug.hpp>

#include <tupai/util/out.hpp>
#include <tupai/util/ansi.hpp>

#include <tupai/i686/port.hpp>

// Libk
#include <libk/stdio.hpp>

namespace tupai
{
	extern "C" void _kpanic(const char* msg, uint32 code) __attribute__((noreturn));

	void klog(const char* msg, klog_level level)
	{
		switch (level)
		{
		case klog_level::INFO:
			{
				util::ansi_set_fg_color(util::ansi::LIGHT_CYAN);
				util::print("INFO");
			}
			break;
		case klog_level::WARNING:
			{
				util::ansi_set_fg_color(util::ansi::LIGHT_YELLOW);
				util::print("WARNING");
			}
			break;
		case klog_level::ERROR:
			{
				util::ansi_set_fg_color(util::ansi::LIGHT_RED);
				util::print("ERROR");
			}
			break;
		case klog_level::PANIC:
			{
				util::ansi_set_fg_color(util::ansi::RED);
				util::print("PANIC");
			}
			break;
		default:
			{
				util::ansi_set_fg_color(util::ansi::YELLOW);
				util::print("LOG");
			}
			break;
		}

		util::ansi_reset();
		util::print(" : ");
		util::println(msg);
	}

	void klog_init(const char* msg, bool success)
	{
		klog(msg, success ? klog_level::INFO : klog_level::ERROR);
	}

	void kpanic(const char* msg, uint32 error)
	{
		klog(msg, klog_level::PANIC);
		_kpanic(msg, error);
	}

	bool __kassert__(bool expr, const char* expr_str, int line, const char* func, const char* file)
	{
		if (!expr)
		{
			klog("Warning: assertion '");
			klog(expr_str);
			klog("' at line ");

			bool start = false;
			for (int pow = 1000000; line > 0; pow /= 10)
			{
				if (line >= pow)
				{
					char num[] = "0";
					num[0] = '0' + (line / pow) % 10;
					klog(num);
					line = line % pow;
					start = true;
				}
				else if (start)
					klog("0");
			}

			klog(" in function '");
			klog(func);
			klog("' in file '");
			klog(file);
			klog("' failed!\n");
		}

		return expr;
	}

	/* Kfault */

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

		// Breakpoint
		asm volatile ("xchg %bx, %bx");

		const char* exception_msg = exceptions[isr_id % (sizeof(exceptions) / sizeof(char*))];
		kpanic(exception_msg, error);
	}
}
