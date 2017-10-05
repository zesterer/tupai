//
// file : kentry.c
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

#include <tupai/kmain.h>
#include <tupai/mem/kheap.h>
#include <tupai/arch/x86/i386/gdt.h>
#include <tupai/arch/x86/i386/idt.h>
#include <tupai/arch/x86/i386/isr.h>
#include <tupai/arch/x86/i386/tss.h>
#include <tupai/arch/x86/mb.h>
#include <tupai/arch/x86/pic.h>
#include <tupai/arch/x86/pit.h>
#include <tupai/arch/x86/vga.h>
#include <tupai/arch/x86/kbd.h>

void kentry(ptr_t mb_header)
{
	// Pre-initiation
	vga_preinit();

	// Kernel early setup
	mb_parse(mb_header);
	kearly();

	// CPU setup
	gdt_init();
	idt_init();
	isr_init();
	tss_init();

	// Interrupt setup
	pic_init();
	pit_init();

	// Hardware setup
	vga_init();
	kbd_init();

	// Kernel main entry
	kmain();
}
