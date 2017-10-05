//
// file : isr.c
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

#include <tupai/arch/x86/i386/isr.h>
#include <tupai/arch/x86/i386/idt.h>
#include <tupai/util/log.h>
#include <tupai/util/panic.h>
#include <tupai/def.h>

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
ptr_t isr_default(ptr_t stack);

extern void _isr_spurious_stub();
ptr_t isr_spurious(ptr_t stack);

void isr_init()
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
	log("[ OK ] Exception ISRs set\n");

	// Assign IDT entries to the stub handler for now
	for (size_t i = IDT_REMAP_OFFSET; i < IDT_LEN; i ++)
		idt_set(i, (void*)&_isr_default_stub, 1);
	log("[ OK ] Default ISRs set\n");

	idt_install();
	log("[ OK ] ISRs installed\n");
}

ptr_t isr_default(ptr_t stack)
{
	panic("Unspecified interrupt occured!\n");
	return stack;
}

ptr_t isr_spurious(ptr_t stack)
{
	panic("Spurious interrupt occured!\n");
	return stack;
}
