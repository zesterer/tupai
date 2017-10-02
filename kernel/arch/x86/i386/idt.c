//
// file : idt.c
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

#include <tupai/arch/x86/i386/idt.h>

#include <tupai/util/log.h>
#include <tupai/util/panic.h>
#include <tupai/def.h>

enum
{
	ATTR_GATE_TASK = 0b0101,
	ATTR_GATE_INT  = 0b1110,
	ATTR_GATE_TRAP = 0b1111,

	ATTR_STORAGE_SEG = 0b10000,

	ATTR_DPL_0 = 0b0000000,
	ATTR_DPL_1 = 0b0100000,
	ATTR_DPL_2 = 0b1000000,
	ATTR_DPL_3 = 0b1100000,

	ATTR_PRESENT = 0b10000000,
};

typedef struct idt_desc
{
	uint16_t offset_lo;
	uint16_t selector;
	uint8_t  zero;
	uint8_t  type_attr;
	uint16_t offset_hi;
} __attribute__((packed)) idt_desc_t;

typedef struct idt_ptr
{
	uint16_t size;
	uint32_t offset;
} __attribute__((packed)) idt_ptr_t;

// IDT
idt_desc_t idt[IDT_LEN] __attribute__((aligned(PAGE_SIZE)));

// IDT pointer
idt_ptr_t idt_ptr __attribute__((aligned(4)));

void idt_init()
{
	// Nothing yet
	log("[ OK ] IDT initiated\n");
}

void idt_install()
{
	idt_ptr.size = sizeof(idt_desc_t) * IDT_LEN - 1;
	idt_ptr.offset = (uint32_t)&idt;

	asm volatile ("lidt (idt_ptr)");
}

void idt_set(size_t irq, void* address, uint16_t selector_id)
{
	// Set address (lo and hi parts)
	idt[irq].offset_lo = ((uint32_t)address >>  0) & 0xFFFF;
	idt[irq].offset_hi = ((uint32_t)address >> 16) & 0xFFFF;

	// Set the code selector from the GDT. It's measured in bytes, so x8 (since GDT entries are 8 bytes big)
	idt[irq].selector = selector_id * 8;

	idt[irq].zero = 0;

	// Set 'interrupt' and 'present' flags
	idt[irq].type_attr = ATTR_GATE_INT | ATTR_PRESENT;
}
