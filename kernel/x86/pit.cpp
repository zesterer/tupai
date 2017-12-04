//
// file : pit.cpp
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

#include <tupai/x86/pit.hpp>
#include <tupai/x86/port.hpp>
#include <tupai/cpu/irq.hpp>
#include <tupai/util/log.hpp>
#include <tupai/util/boot.hpp>

namespace tupai::x86::pit
{
	static const uint8_t IRQ = 0;

	// PIT ports
	static const uint16_t CMD = 0x43;
	static const uint16_t DATA_CH0 = 0x40;
	static const uint16_t DATA_CH1 = 0x41;
	static const uint16_t DATA_CH2 = 0x42;

	// Rate configuration
	static const uint32_t MAX_RATE = 1193180; // Hz
	static uint32_t rate_nhz = 0x20;

	// Extern ISR stub reference
	extern "C" void _isr_pit_stub();

	void init()
	{
		cpu::irq::set_handler(IRQ, reinterpret_cast<uintptr_t>(_isr_pit_stub));
		util::bootlog("PIT handler bound");

		set_rate(1000);
		cpu::irq::unmask(IRQ);
		util::bootlog("PIT configured");
	}

	uint32_t get_rate()
	{
		return rate_nhz;
	}

	void set_rate(uint32_t rate)
	{
		rate_nhz = rate;

		uint16_t div = MAX_RATE / rate_nhz;
		uint8_t cmd = (0x3 << 1) | (0x3 << 4); // Square-wave, lo/hi access
		io::out<8>(CMD, cmd);
		io::out<8>(DATA_CH0, (div >> 0) & 0xFF); // lo
		io::out<8>(DATA_CH0, (div >> 8) & 0xFF); // hi
	}

	extern "C" uintptr_t isr_pit(uintptr_t stack)
	{
		util::log("PIT!");

		cpu::irq::ack(IRQ);
		return stack;
	}
}
