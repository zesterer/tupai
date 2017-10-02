//
// file : pic.c
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

#include <tupai/arch/x86/pic.h>
#include <tupai/arch/x86/port.h>
#include <tupai/util/log.h>
#include <tupai/cpu.h>

#ifdef ARCH_TARGET_i386
	#include <tupai/arch/x86/i386/idt.h>
#endif

#define PIC1 0x20 // I/O offset address for master PIC
#define PIC2 0xA0 // I/O offset address for slave PIC

#define PIC1_CMD PIC1 // I/O offset address for master PIC commands
#define PIC2_CMD PIC2 // I/O offset address for slave PIC commands

#define PIC1_DATA (PIC1 + 1) // I/O offset address for master PIC data
#define PIC2_DATA (PIC2 + 1) // I/O offset address for slave PIC data

#define ICW1_ICW4 0x01 // ICW4 (Interrupt Command Word)
#define ICW1_SINGLE 0x02 // Single (cascade) mode
#define ICW1_INTERVAL4 0x04 // Call address interval 4 (8)
#define ICW1_LEVEL 0x08 // Level triggered (edge) mode
#define ICW1_INIT 0x10 // Initialization - required!

#define ICW4_8086 0x01 // 8086/88 (MCS-80/85) mode
#define ICW4_AUTO 0x02 // Auto (normal) EOI
#define ICW4_BUFF_SLAVE 0x08 // Buffered mode / slave
#define ICW4_BUFF_MASTER 0x0C // Buffered mode / master
#define ICW4_SFNM 0x10 // Special fully nested (not)

#define OCW3_READ_IRR 0x0A // OCW3 irq ready next CMD read
#define OCW3_READ_ISR 0x0B // OCW3 irq service next CMD read

#define PIC_EOI 0x20 // PIC EOI byte

void pic_init()
{
	outb(PIC1_CMD, ICW1_INIT | ICW1_ICW4);
	cpu_wait(150);
	outb(PIC2_CMD, ICW1_INIT | ICW1_ICW4);
	cpu_wait(150);

	// Initiation has begun. The PIC now waits for 3 'initiation bytes' on the data port
	// These 'initiation bytes' are:
	//   -> Vector offset (ICW2) (for remapping)
	//   -> Wiring info (ICW3) (masters / slaves)
	//   -> Additional environment info (ICW4)

	outb(PIC1_DATA, IDT_REMAP_OFFSET); // ICW2 - Master PIC vector offset
	cpu_wait(150);
	outb(PIC2_DATA, IDT_REMAP_OFFSET + 0x8); // ICW2 - Slave PIC vector offset
	cpu_wait(150);

	outb(PIC1_DATA, 0x4); // ICW3 - Master PIC needs to know it has a slave PIC at IRQ2 (0x00000100)
	cpu_wait(150);
	outb(PIC2_DATA, 0x2); // ICW3 - Slave PIC needs to know it's cascade identity (0x00000010)
	cpu_wait(150);

	outb(PIC1_DATA, ICW4_8086); // ICW4 - Tell the PIC to operate in 8086 mode
	cpu_wait(150);
	outb(PIC2_DATA, ICW4_8086); // ICW4 - Tell the PIC to operate in 8086 mode
	cpu_wait(150);
	log("[ OK ] PIC configured\n");

	// Mask all interrupts
	outb(PIC1_DATA, 0xFF);
	outb(PIC2_DATA, 0xFF);
	log("[ OK ] All PIC interrupts masked by default\n");
}

void pic_ack(uint8_t irq) // (send EOI)
{
	if (irq >= 16) // It's not a valid irq
		return;

	if (irq >= 8) // It's a PIC2
		outb(PIC2_CMD, PIC_EOI);

	// PIC1 EOI must be called anyway
	outb(PIC1_CMD, PIC_EOI);
}

void pic_mask(uint8_t irq, bool mask)
{
	uint16_t port;

	if (irq < 8)
		port = PIC1_DATA;
	else if (irq < 16)
	{
		port = PIC2_DATA;
		irq -= 8;
	}
	else
		return;

	uint8_t value = inb(port);
	cpu_wait(150);

	if (mask)
		value = value | (1 << irq);
	else
		value = value & ~(1 << irq);

	outb(port, value);
	cpu_wait(150);
}
