//
// file : idt.cpp
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

#include <tupai/x86/i386/idt.hpp>
#include <tupai/x86/i386/gdt.hpp>
#include <tupai/util/type.hpp>
#include <tupai/util/arr.hpp>
#include <tupai/util/box.hpp>
#include <tupai/util/boot.hpp>

namespace tupai::x86::i386::idt
{
	struct Attr
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

	struct Entry
	{
	private:
		uint16_t off_lo = 0;
		uint16_t select = 0;
		uint8_t  zero   = 0;
		uint8_t  attr   = 0;
		uint16_t off_hi = 0;

	public:
		Entry() {}
		Entry(uintptr_t addr);
	} ATTR_PACKED;

	static util::Box<util::Arr<Entry, LENGTH>> idt ATTR_ALIGNED(4);

	struct Ptr
	{
	private:
		uint16_t size;
		uint32_t off;

	public:
		Ptr() {}
		Ptr(util::Arr<Entry, LENGTH>& idt, uint16_t size) : size(size), off((uint32_t)&idt[0]) {}
	} ATTR_PACKED;

	extern "C" Ptr idt_ptr;
	Ptr idt_ptr ATTR_ALIGNED(4);

	void init()
	{
		idt.create();

		util::bootlog("Initiated IDT");

		flush();
		util::bootlog("Flushed IDT");
	}

	void set(size_t vec, uintptr_t handler)
	{
		(*idt)[vec] = Entry(handler);
	}

	void flush()
	{
		idt_ptr = Ptr(*idt, sizeof(Entry) * LENGTH - 1);

		asm volatile ("lidt (idt_ptr)\n");
	}

	Entry::Entry(uintptr_t addr) :
		off_lo(((uint32_t)addr >> 0) & 0xFFFF),
		select(gdt::KCODE_SELECTOR * 8),
		zero  (0),
		attr  (Attr::INT | Attr::PRESENT),
		off_hi(((uint32_t)addr >> 16) & 0xFFFF)
	{}
}
