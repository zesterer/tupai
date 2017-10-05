//
// file : pit.c
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

#include <tupai/arch/x86/pit.h>
#include <tupai/arch/x86/irq.h>
#include <tupai/arch/x86/port.h>
#include <tupai/dev/console.h>
#include <tupai/util/log.h>

#ifdef ARCH_TARGET_i386
	#include <tupai/arch/x86/i386/idt.h>
#endif

#define PIT_INT 0

#define PORT_CMD 0x43
#define PORT_DATA_CH0 0x40
#define PORT_DATA_CH1 0x41
#define PORT_DATA_CH2 0x42

#define MAX_RATE 1193180

static volatile uint64_t tick = 0;
static volatile uint64_t time = 0;
static volatile uint32_t rate = 0;

static void set_rate(uint32_t rate);
extern void _isr_pit_stub();

void pit_init()
{
	idt_set(IDT_REMAP_OFFSET + PIT_INT, (void*)_isr_pit_stub, 1);
	idt_install();
	log("[ OK ] PIT ISR installed\n");

	irq_enable(PIT_INT, true); // Unmask PIT interrupt
	log("[ OK ] PIT IRQ unmasked\n");

	set_rate(1000);
	log("[ OK ] PIT rate set\n");
}

void set_rate(uint32_t r)
{
	rate = r;

	uint16_t div = MAX_RATE / rate;

	uint8_t cmd = (0x3 << 1) | (0x3 << 4); // Square wave generator
	outb(PORT_CMD, cmd);
	outb(PORT_DATA_CH0, (div >> 0) & 0xFF); // Low byte
	outb(PORT_DATA_CH0, (div >> 8) & 0xFF); // High byte
}

ptr_t isr_pit(ptr_t stack)
{
	tick ++;
	time += 1000000 / rate;

	// TODO : Add task switching

	irq_ack(PIT_INT); // Send EOI
	return stack;
}
