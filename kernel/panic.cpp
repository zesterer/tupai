//
// file : panic.cpp
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

// Tupai
#include <tupai/panic.hpp>
#include <tupai/cpu.hpp>
#include <tupai/irq.hpp>

#include <tupai/util/fmt.hpp>
#if defined(ARCH_FAMILY_X86)
	#include <tupai/x86/textmode.hpp>
#endif

namespace tupai
{
	void _panic(const char* msg)
	{
		irq::disable();

		char str[1024];

		util::fmt(str,
		 	"Kernel Panic!\n\n",
			"Something went badly wrong and the following error was produced.\n\n",
			msg, "\n\n",
			"Please send this error and all other relevant details concerning\nthis crash to ", KMAINTAINER_EMAIL, '.'
		);

		#if defined(ARCH_FAMILY_X86)
			x86::textmode_clear();
			x86::textmode_cursor_disable();
			x86::textmode_move(0, 0);
			for (size_t i = 0; str[i] != '\0'; i ++)
				x86::textmode_write(str[i]);
		#endif

		cpu::hang();
	}
}
