//
// file : cpu.cpp
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

#include <tupai/cpu/cpu.hpp>
#include <tupai/cpu/irq.hpp>

namespace tupai::cpu
{
	void enable_int()
	{
		asm volatile ("sti");
	}

	void disable_int()
	{
		asm volatile ("cli");
	}

	void halt()
	{
		asm volatile ("hlt");
	}

	ATTR_NORETURN void hang()
	{
		disable_int();

		while (true)
			halt();
	}

	ATTR_NORETURN void wait()
	{
		enable_int();

		while (true)
			halt();
	}
}
