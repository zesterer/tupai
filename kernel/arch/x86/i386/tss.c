//
// file : tss.c
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

#include <tupai/x86/i386/tss.h>
#include <tupai/x86/i386/gdt.h>
#include <tupai/util/log.h>

typedef struct tss_entry
{
	uint32_t prev;
	uint32_t esp0;
	uint32_t ss0;
	uint32_t esp1;
	uint32_t ss1;
	uint32_t esp2;
	uint32_t ss2;
	uint32_t cr3;
	uint32_t eip;
	uint32_t eflags;
	uint32_t eax;
	uint32_t ecx;
	uint32_t edx;
	uint32_t ebx;
	uint32_t esp;
	uint32_t ebp;
	uint32_t esi;
	uint32_t edi;
	uint32_t es;
	uint32_t cs;
	uint32_t ss;
	uint32_t ds;
	uint32_t fs;
	uint32_t gs;
	uint32_t ldt;
	uint16_t trap;
	uint16_t iomap_base;
} __attribute__((packed)) tss_entry_t;

tss_entry_t tss;

static void write_tss(uint32_t ss0, uint32_t esp0);
static void flush();

uint8_t tss_test_stack[1024];

void tss_init()
{
	write_tss(0x10, 0x0);
	log("[ OK ] Wrote TSS\n");
	flush();
	log("[ OK ] Flushed TSS\n");

	tss_set_stack((size_t)&tss_test_stack + 1024);
}

void write_tss(uint32_t ss0, uint32_t esp0)
{
	uint32_t offset = (uint32_t)&tss;
	uint32_t limit = offset + sizeof(tss_entry_t);

	gdt_set(5, offset, limit, 0xE9, 0x00); // TODO : Remove these magic numbers
	gdt_install();

	tss.ss0 = ss0; // Set kernel stack segment
	tss.esp0 = esp0; // Set kernel stack pointer

	// Standard code and data segments (0x08 and 0x10) but with last 3 bits set
	tss.cs = 0x0b;
	tss.ss = 0x13;
	tss.ds = 0x13;
	tss.es = 0x13;
	tss.fs = 0x13;
	tss.gs = 0x13;
}

void tss_set_stack(size_t stack)
{
	tss.esp0 = stack; // Set kernel stack
}

void flush()
{
	asm volatile ("mov %0, %%eax; ltr %%ax" :: "r"(0x2B) : "%eax");
}
