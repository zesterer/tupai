//
// file : gdt.cpp
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

#include <tupai/x86/i386/gdt.hpp>
#include <tupai/util/type.hpp>
#include <tupai/util/arr.hpp>
#include <tupai/util/box.hpp>
#include <tupai/util/boot.hpp>

namespace tupai::x86::i386::gdt
{
	const size_t LENGTH = 5;
	static util::Box<util::Arr<Entry, LENGTH>> gdt ATTR_ALIGNED(4);

	struct Ptr
	{
	private:
		uint16_t size;
		uint32_t off;

	public:
		Ptr() {}
		Ptr(util::Arr<Entry, LENGTH>& gdt, uint16_t size) : size(size), off((uint32_t)&gdt[0]) {}
	} ATTR_PACKED;

	extern "C" Ptr gdt_ptr;
	Ptr gdt_ptr ATTR_ALIGNED(4);

	void init()
	{
		gdt.create();

		uint8_t kcode_access = Access::KERNEL | Access::READ  | Access::ONE | Access::PRESENT | Access::EXEC;
		uint8_t kdata_access = Access::KERNEL | Access::WRITE | Access::ONE | Access::PRESENT;
		uint8_t ucode_access = Access::USER   | Access::READ  | Access::ONE | Access::PRESENT | Access::EXEC;
		uint8_t udata_access = Access::USER   | Access::WRITE | Access::ONE | Access::PRESENT;
		uint8_t gran = Gran::PAGE | Gran::P32;

		set_entry(0, Entry()); // Null segment
		util::bootlog("Null GDT entry set");
		set_entry(KCODE_SELECTOR, Entry(0x0, 0xFFFFF, kcode_access, gran)); // Kernel code segment
		util::bootlog("Kernel code GDT entry set");
		set_entry(KDATA_SELECTOR, Entry(0x0, 0xFFFFF, kdata_access, gran)); // Kernel data segment
		util::bootlog("Kernel data GDT entry set");
		set_entry(UCODE_SELECTOR, Entry(0x0, 0xFFFFF, ucode_access, gran)); // User code segment
		util::bootlog("User code GDT entry set");
		set_entry(UDATA_SELECTOR, Entry(0x0, 0xFFFFF, udata_access, gran)); // User data segment
		util::bootlog("User data GDT entry set");

		flush();
		util::bootlog("Flushed GDT");
	}

	void set_entry(size_t desc, Entry entry)
	{
		(*gdt)[desc] = entry;
	}

	void flush()
	{
		gdt_ptr = Ptr(*gdt, sizeof(Entry) * LENGTH - 1);

		asm volatile
		(
			"lgdt (gdt_ptr)\n"

			// Long jump into code segment
			"ljmp $8, $1f\n"
			"1:\n"

			// Reload segment registers
			"mov $0x10, %ax\n"
			"mov %ax, %ds\n"
			"mov %ax, %es\n"
			"mov %ax, %fs\n"
			"mov %ax, %gs\n"
			"mov %ax, %ss\n"
		);
	}

	Entry::Entry(uint32_t offset, uint32_t size, uint8_t access, uint8_t gran) :
		size_lo(size & 0xFFFF),
		off_lo ((offset >> 0) & 0xFFFF),
		off_mid((offset >> 16) & 0xFF),
		access (access),
		gran   (((size >> 16) & 0xF) | ((gran << 4) & 0xF0)),
		off_hi ((offset >> 24) & 0xFF)
	{}
}
