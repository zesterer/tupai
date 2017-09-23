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

#include <tupai/x86/i386/idt.h>

#include <tupai/util/log.h>
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

#define IDT_LEN 256

// IDT
idt_desc_t idt[IDT_LEN] __attribute__((aligned(PAGE_SIZE)));

// IDT pointer
idt_ptr_t idt_ptr __attribute__((aligned(4)));

static void idt_set(size_t irq, void* address, uint16_t selector_id);

extern void _exception_0();
extern void _exception_1();
extern void _exception_2();
extern void _exception_3();
extern void _exception_4();
extern void _exception_5();
extern void _exception_6();
extern void _exception_7();
extern void _exception_8();
extern void _exception_9();
extern void _exception_10();
extern void _exception_11();
extern void _exception_12();
extern void _exception_13();
extern void _exception_14();
extern void _exception_15();
extern void _exception_16();
extern void _exception_17();
extern void _exception_18();

extern void _isr_default_stub();
size_t isr_default(size_t stack);

extern void _isr_spurious_stub();
size_t isr_spurious(size_t stack);

void idt_init()
{
	// Set exception routine entries
	idt_set( 0, (void*)_exception_0, 1);
	idt_set( 1, (void*)_exception_1, 1);
	idt_set( 2, (void*)_exception_2, 1);
	idt_set( 3, (void*)_exception_3, 1);
	idt_set( 4, (void*)_exception_4, 1);
	idt_set( 5, (void*)_exception_5, 1);
	idt_set( 6, (void*)_exception_6, 1);
	idt_set( 7, (void*)_exception_7, 1);
	idt_set( 8, (void*)_exception_8, 1);
	idt_set( 9, (void*)_exception_9, 1);
	idt_set(10, (void*)_exception_10, 1);
	idt_set(11, (void*)_exception_11, 1);
	idt_set(12, (void*)_exception_12, 1);
	idt_set(13, (void*)_exception_13, 1);
	idt_set(14, (void*)_exception_14, 1);
	idt_set(15, (void*)_exception_15, 1);
	idt_set(16, (void*)_exception_16, 1);
	idt_set(17, (void*)_exception_17, 1);
	idt_set(18, (void*)_exception_18, 1);
	log("[ OK ] Exception IDT entries set\n");

	// Assign IDT entries to the stub handler for now
	for (size_t i = IDT_REMAP_OFFSET; i < IDT_LEN; i ++)
		idt_set(i, (void*)&_isr_default_stub, 1);
	log("[ OK ] Default IDT entries set\n");

	idt_install();
	log("[ OK ] IDT installed\n");
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

size_t isr_default(size_t stack)
{
	return stack;
}

size_t isr_spurious(size_t stack)
{
	log("Spurious interrupt occured\n");
	return stack;
}
