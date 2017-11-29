//
// file : irq.cpp
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

#include <tupai/cpu/irq.hpp>
#include <tupai/x86/pic.hpp>
#include <tupai/x86/i386/idt.hpp>

namespace tupai::cpu::irq
{
	void unmask_irq(uint8_t vec)
	{
		x86::pic::unmask_irq(vec);
	}

	void mask_irq(uint8_t vec)
	{
		x86::pic::mask_irq(vec);
	}

	void ack(uint8_t vec)
	{
		x86::pic::ack(vec);
	}

	void set_handler(uint8_t vec, uintptr_t handler)
	{
		x86::i386::idt::set(x86::pic::IRQ_REMAP + vec, handler);
		x86::i386::idt::flush();
	}
}
