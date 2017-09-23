//
// file : vga.c
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

#include <tupai/x86/kbd.h>
#include <tupai/x86/irq.h>
#include <tupai/x86/port.h>
#include <tupai/dev/console.h>
#include <tupai/util/log.h>
#include <tupai/def.h>

#ifdef ARCH_TARGET_i386
	#include <tupai/x86/i386/idt.h>
#endif

#define KBD_INT 1

#define CMD_PORT 0x60
#define STATUS_PORT 0x64
#define DATA_PORT 0x60

extern void _isr_kbd_stub();

const char scancode_table[] = "!!1234567890-=\b\tqwertyuiop[]\n!asdfghjkl;'#!\\zxcvbnm,./!!! !FFFFFFFFFF!";

void kbd_init()
{
	idt_set(IDT_REMAP_OFFSET + 1, (void*)_isr_kbd_stub, 1);
	idt_install();
	log("[ OK ] PS2 keyboard ISR installed\n");

	irq_enable(KBD_INT, true); // Unmask the keyboard interrupt
}

size_t isr_kbd(size_t stack)
{
	// TODO : Totally refactor this
	while (true)
	{
		// Lowest bit of status will be set if buffer is not empty
		if ((inb(STATUS_PORT) & 0x01) != 0x01) // If the buffer is empty, stop reading scancode bytes
			break;

		uint8_t keycode = inb(DATA_PORT);
		if (keycode >= sizeof(scancode_table) / sizeof(char))
			break;

		char c = scancode_table[(size_t)keycode]; // Find input character using rudimentary table

		console_write_in(c);
	}

	irq_ack(KBD_INT); // Send EOI

	return stack;
}
