//
// file : kbd.c
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
#include <tupai/util/char.h>
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

#define KEY_LSHIFT_DOWN 0x2A
#define KEY_LSHIFT_UP   0xAA

#define LOCK_LSHIFT 0
bool lock[1];

unsigned char kbdus[128] =
{
	0, 27, '1', '2', '3', '4', '5', '6', '7', '8',	// 9
	'9', '0', '-', '=', '\b', // Backspace
	'\t', // Tab
	'q', 'w', 'e', 'r',	// 19
	't', 'y', 'u', 'i', 'o', 'p', '[', ']', '\n', // Enter key
	0, // 29 - Control
	'a', 's', 'd', 'f', 'g', 'h', 'j', 'k', 'l', ';', // 39
	'\'', '`',
	0,   // Left shift
	'\\', 'z', 'x', 'c', 'v', 'b', 'n', // 49
	'm', ',', '.', '/',
	0,   // Right shift
	'*',
	0,   // Alt
	' ', // Space bar
	0,   // Caps lock
	0,   // 59 - F1 key ... >
	0,   0,   0,   0,   0,   0,   0,   0,
	0,   // < ... F10
	0,   // 69 - Num lock
	0,   // Scroll Lock
	0,   // Home key
	0,   // Up Arrow
	0,   // Page Up
	'-',
	0,   // Left Arrow
	0,
	0,   // Right Arrow
	'+',
	0,   // 79 - End key
	0,   // Down Arrow
	0,   // Page Down
	0,   // Insert Key
	0,   // Delete Key
	0, 0, 0,
	0,   // F11 Key
	0,   // F12 Key
	0,   // All other keys are undefined
};

void kbd_init()
{
	idt_set(IDT_REMAP_OFFSET + KBD_INT, (void*)_isr_kbd_stub, 1);
	idt_install();
	log("[ OK ] PS2 keyboard ISR installed\n");

	irq_enable(KBD_INT, true); // Unmask the keyboard interrupt
	log("[ OK ] PS2 keyboard IRQ unmasked\n");
}

size_t isr_kbd(size_t stack)
{
	while (true)
	{
		if ((inb(STATUS_PORT) & 1) == 0) // Only read scancodes if the buffer is full
			break;

		uint8_t scancode = inb(DATA_PORT);
		if (scancode & 0x80) // Is it a key release?
		{
			switch (scancode)
			{
				case KEY_LSHIFT_UP:
					lock[LOCK_LSHIFT] = false;
					break;

				default:
					break;
			}
		}
		else // Key press
		{
			switch (scancode)
			{
				case KEY_LSHIFT_DOWN:
					lock[LOCK_LSHIFT] = true;
					break;

				default:
				{
					char c = kbdus[scancode];

					if (lock[LOCK_LSHIFT])
						c = uppercase(c);

					console_write_in(c);
				}
				break;
			}
		}
	}

	irq_ack(KBD_INT); // Send EOI
	return stack;
}
