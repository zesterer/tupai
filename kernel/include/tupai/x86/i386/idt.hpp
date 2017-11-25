//
// file : idt.hpp
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

#ifndef TUPAI_X86_I386_IDT_HPP
#define TUPAI_X86_I386_IDT_HPP

#include <tupai/util/type.hpp>
#include <tupai/util/attr.hpp>

namespace tupai::x86::i386
{
	struct IDTAttr
	{
		const static uint8_t TASK = 0b0101;
		const static uint8_t INT  = 0b1110;
		const static uint8_t TRAP = 0b1111;

		const static uint8_t STORE = 0b10000;

		const static uint8_t DPL0 = 0b0000000;
		const static uint8_t DPL1 = 0b0100000;
		const static uint8_t DPL2 = 0b1000000;
		const static uint8_t DPL3 = 0b1100000;

		const static uint8_t PRESENT = 0b10000000;
	};

	struct IDTEntry
	{
	private:
		uint16_t off_lo = 0;
		uint16_t select = 0;
		uint8_t  zero   = 0;
		uint8_t  attr   = 0;
		uint16_t off_hi = 0;

	public:
		IDTEntry() {}
		IDTEntry(uintptr_t addr);
	} ATTR_PACKED;

	void idt_init();
	void idt_set_entry(size_t vec, IDTEntry entry);
	void idt_flush();
}

#endif
