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

namespace tupai::x86::i386
{
	const size_t GDT_SIZE = 5;
	static util::Box<util::Arr<GDTEntry, GDT_SIZE>> gdt;

	struct GDTPtr
	{
	private:
		uint16_t size;
		uint32_t off;

	public:
		GDTPtr() {}
		GDTPtr(util::Arr<GDTEntry, GDT_SIZE>& gdt, uint16_t size) : size(size), off((uint32_t)&gdt[0]) {}
	} ATTR_PACKED;

	extern "C" GDTPtr gdt_ptr;
	GDTPtr gdt_ptr ATTR_ALIGNED(4);

	void gdt_init()
	{
		gdt.create();

		uint8_t kcode_access = GDTAccess::KERNEL | GDTAccess::READ  | GDTAccess::ONE | GDTAccess::PRESENT | GDTAccess::EXEC;
		uint8_t kdata_access = GDTAccess::KERNEL | GDTAccess::WRITE | GDTAccess::ONE | GDTAccess::PRESENT;
		uint8_t ucode_access = GDTAccess::USER   | GDTAccess::READ  | GDTAccess::ONE | GDTAccess::PRESENT | GDTAccess::EXEC;
		uint8_t udata_access = GDTAccess::USER   | GDTAccess::WRITE | GDTAccess::ONE | GDTAccess::PRESENT;
		uint8_t gran = GDTGran::PAGE | GDTGran::P32;

		gdt_set_entry(0, GDTEntry()); // Null segment
		util::bootlog("Null GDT entry set");
		gdt_set_entry(GDT_KCODE_SELECTOR, GDTEntry(0x0, 0xFFFFF, kcode_access, gran)); // Kernel code segment
		util::bootlog("Kernel code GDT entry set");
		gdt_set_entry(GDT_KDATA_SELECTOR, GDTEntry(0x0, 0xFFFFF, kdata_access, gran)); // Kernel data segment
		util::bootlog("Kernel data GDT entry set");
		gdt_set_entry(GDT_UCODE_SELECTOR, GDTEntry(0x0, 0xFFFFF, ucode_access, gran)); // User code segment
		util::bootlog("User code GDT entry set");
		gdt_set_entry(GDT_UDATA_SELECTOR, GDTEntry(0x0, 0xFFFFF, udata_access, gran)); // User data segment
		util::bootlog("User data GDT entry set");

		gdt_flush();
		util::bootlog("Flushed GDT");
	}

	void gdt_set_entry(size_t desc, GDTEntry entry)
	{
		(*gdt)[desc] = entry;
	}

	void gdt_flush()
	{
		gdt_ptr = GDTPtr(*gdt, sizeof(GDTEntry) * GDT_SIZE - 1);

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

	GDTEntry::GDTEntry(uint32_t offset, uint32_t size, uint8_t access, uint8_t gran) :
		size_lo(size & 0xFFFF),
		off_lo ((offset >> 0) & 0xFFFF),
		off_mid((offset >> 16) & 0xFF),
		access (access),
		gran   (((size >> 16) & 0xF) | ((gran << 4) & 0xF0)),
		off_hi ((offset >> 24) & 0xFF)
	{}
}
