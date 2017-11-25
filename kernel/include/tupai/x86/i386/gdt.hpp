//
// file : gdt.hpp
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

#ifndef TUPAI_X86_I386_GDT_HPP
#define TUPAI_X86_I386_GDT_HPP

#include <tupai/util/type.hpp>
#include <tupai/util/attr.hpp>

namespace tupai::x86::i386
{
	const size_t GDT_KCODE_SELECTOR = 1;
	const size_t GDT_KDATA_SELECTOR = 2;
	const size_t GDT_UCODE_SELECTOR = 3;
	const size_t GDT_UDATA_SELECTOR = 4;

	struct GDTAccess
	{
		const static uint8_t READ    = 0b00000010;
		const static uint8_t WRITE   = 0b00000010;
		const static uint8_t EXEC    = 0b00001000;
		const static uint8_t PRESENT = 0b10000000;
		const static uint8_t ONE     = 0b00010000;
		const static uint8_t KERNEL  = 0b00000000;
		const static uint8_t USER    = 0b01100000;
	};

	struct GDTGran
	{
		const static uint8_t PAGE = 0b1000;
		const static uint8_t P32  = 0b0100;
	};

	struct GDTEntry
	{
	private:
		uint16_t size_lo = 0;
		uint16_t off_lo  = 0;
		uint8_t  off_mid = 0;
		uint8_t  access  = 0;
		uint8_t  gran    = 0;
		uint8_t  off_hi  = 0;

	public:
		GDTEntry() {}
		GDTEntry(uint32_t offset, uint32_t size, uint8_t access, uint8_t gran);
	} ATTR_PACKED;

	void gdt_init();
	void gdt_set_entry(size_t desc, GDTEntry entry);
	void gdt_flush();
}

#endif
