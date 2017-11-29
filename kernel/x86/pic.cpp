//
// file : pic.cpp
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

#include <tupai/x86/pic.hpp>
#include <tupai/x86/port.hpp>
#include <tupai/util/boot.hpp>
#include <tupai/util/assert.hpp>

namespace tupai::x86::pic
{
	// PIC ports
	static const uint16_t MASTER_CMD = 0x20;
	static const uint16_t SLAVE_CMD = 0xA0;
	static const uint16_t MASTER_DATA = MASTER_CMD + 1;
	static const uint16_t SLAVE_DATA = SLAVE_CMD + 1;

	// ICW (Interrupt Command Word) flags
	static const uint8_t ICW1_INIT = 0x10;
	static const uint8_t ICW1_ICW4 = 0x01;
	static const uint8_t ICW4_8086 = 0x01;

	// PIC commands
	static const uint8_t PIC_EOI = 0x20;

	void init()
	{
		// Tell the PICs to begin initiation
		io::out<8>(MASTER_CMD, ICW1_INIT | ICW1_ICW4);
		io::out<8>(SLAVE_CMD, ICW1_INIT | ICW1_ICW4);

		// Initiation has begun. The PIC now waits for 3 'initiation bytes' on the data port
		// These 'initiation bytes' are:
		//   -> Vector offset (ICW2) (for remapping)
		//   -> Wiring info (ICW3) (masters / slaves)
		//   -> Additional environment info (ICW4)

		// Inform the PICs that their IRQs should be offset (to make space for exception handling)
		io::out<8>(MASTER_DATA, 0 + IRQ_REMAP);
		io::out<8>(SLAVE_DATA, 8 + IRQ_REMAP);

		// Inform the master PIC that it has a slave on IRQ 2, and make the slave aware of its cascade identity
		io::out<8>(MASTER_DATA, 4);
		io::out<8>(SLAVE_DATA, 2);

		// Tell the PICs to operate in 8086 mode
		io::out<8>(MASTER_DATA, ICW4_8086);
		io::out<8>(SLAVE_DATA, ICW4_8086);

		util::bootlog("PIC configured");

		// Mask all IRQs by default
		io::out<8>(MASTER_DATA, 0xFF);
		io::out<8>(SLAVE_DATA, 0xFF);

		util::bootlog("Masked PIC interrupts");
	}

	void unmask_irq(int irq)
	{
		assert(irq < 16, "Attempted to enable an IRQ not within PIC range");
		uint16_t port = (irq < 8) ? MASTER_DATA : SLAVE_DATA;
		uint8_t nmask = io::in<8>(port) & (~(1 << (irq & 0b111)));
		io::out<8>(port, nmask);
	}

	void mask_irq(int irq)
	{
		assert(irq < 16, "Attempted to disable an IRQ not within PIC range");
		uint16_t port = (irq < 8) ? MASTER_DATA : SLAVE_DATA;
		uint8_t nmask = io::in<8>(port) | (1 << (irq & 0b111));
		io::out<8>(port, nmask);
	}

	void ack(int irq)
	{
		assert(irq < 16, "Attempted to acknowledge an IRQ not within PIC range");

		// If it's a slave IRQ, send an EOI to the slave too
		if (irq >= 8)
			io::out<8>(SLAVE_CMD, PIC_EOI);

		io::out<8>(MASTER_CMD, PIC_EOI);
	}
}
