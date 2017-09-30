//
// file : gdt.c
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

#include <tupai/x86/i386/gdt.h>

#include <tupai/util/log.h>
#include <tupai/def.h>

enum
{
	ACCESS_READ    = 0b00000010,
	ACCESS_WRITE   = 0b00000010,
	ACCESS_EXEC    = 0b00001000,
	ACCESS_PRESENT = 0b10000000,
	ACCESS_ONE     = 0b00010000,
	ACCESS_KERNEL  = 0b00000000,
	ACCESS_USER    = 0b01100000,
};

enum
{
	GRAN_PAGE = 0b1000,
	GRAN_P32  = 0b0100,
};

typedef struct gdt_desc
{
	uint16_t size_lo;
	uint16_t offset_lo;
	uint8_t  offset_mid;
	uint8_t  access_flags;
	uint8_t  gran_flags;
	uint8_t  offset_hi;
} __attribute__((packed)) gdt_desc_t;

typedef struct gdt_ptr
{
	uint16_t size;
	uint32_t offset;
} __attribute__((packed)) gdt_ptr_t;

#define GDT_LEN 6

// GDT
gdt_desc_t gdt[GDT_LEN] __attribute__((aligned(PAGE_SIZE)));

// GDT pointer
gdt_ptr_t gdt_ptr __attribute__((aligned(4)));

void gdt_init()
{
	// Flags for various sections
	uint8_t kernel_code_flags = ACCESS_KERNEL | ACCESS_READ | ACCESS_EXEC | ACCESS_ONE | ACCESS_PRESENT;
	uint8_t kernel_data_flags = ACCESS_KERNEL | ACCESS_WRITE | ACCESS_ONE | ACCESS_PRESENT;
	uint8_t user_code_flags = ACCESS_USER | ACCESS_READ | ACCESS_EXEC | ACCESS_ONE | ACCESS_PRESENT;
	uint8_t user_data_flags = ACCESS_USER | ACCESS_WRITE | ACCESS_ONE | ACCESS_PRESENT;
	uint8_t gran_flags = GRAN_PAGE | GRAN_P32;

	gdt_set(0, 0x0, 0x0, 0, 0); // Null segment
	log("[ OK ] Null GDT segment set\n");
	gdt_set(1, 0x0, 0xFFFFF, kernel_code_flags, gran_flags); // Kernel code segment
	log("[ OK ] Kernel code GDT segment set\n");
	gdt_set(2, 0x0, 0xFFFFF, kernel_data_flags, gran_flags); // Kernel data segment
	log("[ OK ] Kernel data GDT segment set\n");
	gdt_set(3, 0x0, 0xFFFFF, user_code_flags, gran_flags); // User code segment
	log("[ OK ] User code GDT segment set\n");
	gdt_set(4, 0x0, 0xFFFFF, user_data_flags, gran_flags); // User data segment
	log("[ OK ] User data GDT segment set\n");

	gdt_install();
	log("[ OK ] GDT installed\n");
}

void gdt_install()
{
	gdt_ptr.size = sizeof(gdt_desc_t) * GDT_LEN - 1;
	gdt_ptr.offset = (uint32_t)&gdt;

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

void gdt_set(size_t n, uint32_t offset, uint32_t size, uint8_t access_flags, uint8_t gran_flags)
{
	// Offset address
	gdt[n].offset_lo  = (offset >>  0) & 0xFFFF;
	gdt[n].offset_mid = (offset >> 16) & 0xFF;
	gdt[n].offset_hi  = (offset >> 24) & 0xFF;

	// Size
	gdt[n].size_lo = size & 0xFFFF;
	gdt[n].gran_flags = (size >> 16) & 0xF;

	// Granularity & access
	gdt[n].gran_flags |= (gran_flags << 4) & 0xF0;
	gdt[n].access_flags = access_flags;
}
