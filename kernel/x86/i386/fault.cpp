//
// file : fault.cpp
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

#include <tupai/x86/i386/fault.hpp>
#include <tupai/util/panic.hpp>

namespace tupai::x86::i386
{
	static const char* fault_names[] =
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

	uintptr_t fault_handler(uintptr_t stack, uint32_t code, uint32_t err)
	{
		(void)stack;

		util::panic("Fault '{}' [{}] occured", fault_names[code], err);

		//return stack;
	}
}
