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

namespace tupai::x86::i386
{
	const size_t IDT_SIZE = 5;
	static util::Box<util::Arr<IDTEntry, IDT_SIZE>> idt;

	struct IDTPtr
	{
	private:
		uint16_t size;
		uint32_t off;

	public:
		IDTPtr() {}
		IDTPtr(util::Arr<IDTEntry, IDT_SIZE>& idt, uint16_t size) : size(size), off((uint32_t)&idt[0]) {}
	} ATTR_PACKED;

	extern "C" IDTPtr idt_ptr;
	IDTPtr idt_ptr ATTR_ALIGNED(4);

	void idt_init()
	{
		idt.create();

		util::bootlog("Initiated IDT");

		idt_flush();
		util::bootlog("Flushed IDT");
	}

	void idt_set_entry(size_t vec, IDTEntry entry)
	{
		(*idt)[vec] = entry;
	}

	void idt_flush()
	{
		idt_ptr = IDTPtr(*idt, sizeof(IDTEntry) * IDT_SIZE - 1);

		asm volatile ("lidt (idt_ptr)\n");
	}

	IDTEntry::IDTEntry(uintptr_t addr) :
		off_lo(((uint32_t)addr >> 0) & 0xFFFF),
		select(GDT_KCODE_SELECTOR * 8),
		zero  (0),
		attr  (IDTAttr::INT | IDTAttr::PRESENT),
		off_hi(((uint32_t)addr >> 16) & 0xFFFF)
	{}
}
