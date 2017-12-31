//
// file : isr.cpp
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

#include <tupai/x86/i386/isr.hpp>
#include <tupai/x86/i386/idt.hpp>
#include <tupai/x86/pic.hpp>
#include <tupai/util/boot.hpp>
#include <tupai/util/panic.hpp>

namespace tupai::x86::i386::isr
{
	extern "C" void _fault_0();
	extern "C" void _fault_1();
	extern "C" void _fault_2();
	extern "C" void _fault_3();
	extern "C" void _fault_4();
	extern "C" void _fault_5();
	extern "C" void _fault_6();
	extern "C" void _fault_7();
	extern "C" void _fault_8();
	extern "C" void _fault_9();
	extern "C" void _fault_10();
	extern "C" void _fault_11();
	extern "C" void _fault_12();
	extern "C" void _fault_13();
	extern "C" void _fault_14();
	extern "C" void _fault_15();
	extern "C" void _fault_16();
	extern "C" void _fault_17();
	extern "C" void _fault_18();

	extern "C" void _isr_default_stub();
	extern "C" void _isr_spurious_stub();

	void init()
	{
		// Set fault handlers
		idt::set(0, (uintptr_t)_fault_0);
		idt::set(1, (uintptr_t)_fault_1);
		idt::set(2, (uintptr_t)_fault_2);
		idt::set(3, (uintptr_t)_fault_3);
		idt::set(4, (uintptr_t)_fault_4);
		idt::set(5, (uintptr_t)_fault_5);
		idt::set(6, (uintptr_t)_fault_6);
		idt::set(7, (uintptr_t)_fault_7);
		idt::set(8, (uintptr_t)_fault_8);
		idt::set(9, (uintptr_t)_fault_9);
		idt::set(10, (uintptr_t)_fault_10);
		idt::set(11, (uintptr_t)_fault_11);
		idt::set(12, (uintptr_t)_fault_12);
		idt::set(13, (uintptr_t)_fault_13);
		idt::set(14, (uintptr_t)_fault_14);
		idt::set(15, (uintptr_t)_fault_15);
		idt::set(16, (uintptr_t)_fault_16);
		idt::set(17, (uintptr_t)_fault_17);
		idt::set(18, (uintptr_t)_fault_18);

		util::bootlog("Set fault ISRs");

		for (size_t i = pic::IRQ_REMAP; i < idt::LENGTH; i ++)
			idt::set(i, (uintptr_t)_isr_default_stub);

		util::bootlog("Set default ISRs");

		idt::set(pic::IRQ_REMAP + 7, (uintptr_t)_isr_spurious_stub);
		util::bootlog("Spurious ISR set");

		idt::flush();
		util::bootlog("ISRs installed, IDT flushed");
	}

	extern "C" uintptr_t isr_stub(uintptr_t stack)
	{
		(void)stack;

		util::panic("Unhandled interrupt");
	}

	extern "C" uintptr_t isr_spurious(uintptr_t stack)
	{
		return stack;
	}
}
