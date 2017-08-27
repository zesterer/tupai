//
// file : call.cpp
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
#include <tupai/call/call.hpp>
#include <tupai/irq.hpp>
#include <tupai/cpu.hpp>
#include <tupai/panic.hpp>
#include <tupai/util/mem.hpp>
#include <tupai/call/file.hpp>

namespace tupai
{
	namespace call
	{
		static uint8_t IRQ = 0x80;

		static size_t (*call_table[])(size_t arg0, size_t arg1, size_t arg2, size_t arg3) = {
			[OPEN]  = open,
			[READ]  = read,
			[WRITE] = write,
			[CLOSE] = close,
			[SEEK] = seek,
		};

		extern "C" void isr_syscall();
		extern "C" size_t syscall_isr_main(size_t sp, size_t func, size_t arg0, size_t arg1, size_t arg2, size_t arg3, size_t* status);

		void init()
		{
			// Bind the interrupt
			irq::bind(IRQ, (void*)isr_syscall);
		}

		size_t syscall_isr_main(size_t sp, size_t func, size_t arg0, size_t arg1, size_t arg2, size_t arg3, size_t* status)
		{
			// Avoid warnings
			(void)func;
			(void)arg0;
			(void)arg1;
			(void)arg2;
			(void)arg3;
			(void)status;

			// Re-enable interrupts since the kernel is re-entrant! (This also means we should stop indicating that we're in an IRQ)
			cpu::end_irq();
			irq::enable();

			if (func < util::length(call_table)) // Is the call within the bounds of the call table?
				*status = call_table[func](arg0, arg1, arg2, arg3); // Then call it!
			else
				panic("Unknown syscall with id '", func, "'"); // No it is not

			return sp;
		}
	}
}
