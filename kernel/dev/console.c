//
// file : console.c
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

#include <tupai/dev/console.h>
#include <tupai/util/fifo.h>
#include <tupai/util/log.h>
#include <tupai/cpu.h>

#ifdef ARCH_FAMILY_x86
	#include <tupai/arch/x86/vga.h>
	#include <tupai/arch/x86/kbd.h>
#endif

#define BUFF_LEN 1024

static fifo_t input;
uint8_t input_buff[BUFF_LEN];

static fifo_t output;
uint8_t output_buff[BUFF_LEN];

static bool initiated = false;

char in_buff_char = '\0';
bool in_buff_full = false;

void console_init()
{
	fifo_init(&input, input_buff, BUFF_LEN);
	fifo_init(&output, output_buff, BUFF_LEN);
	log("[ OK ] Initiated console buffers\n");

	initiated = true;
}

void console_puts(const char* str)
{
	#ifdef ARCH_FAMILY_x86
		vga_puts(str);
	#endif
}

void console_putc(char c)
{
	#ifdef ARCH_FAMILY_x86
		vga_putc(c);
	#endif
}

void console_write_in(char c)
{
	/*
	fifo_push(&input_buff, c);

	char nc;
	fifo_pop(&input_buff, &nc);
	*/

	#ifdef ARCH_FAMILY_x86
		in_buff_full = true;
		in_buff_char = c;
	#endif
}

char console_read_in()
{
	#ifdef ARCH_FAMILY_x86
			while (!in_buff_full)
				cpu_halt();

			in_buff_full = false;
			return in_buff_char;
	#endif
}
