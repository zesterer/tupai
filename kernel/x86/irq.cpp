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

// Tupai
#include <tupai/irq.hpp>
#include <tupai/x86/pic.hpp>

#if defined(ARCH_amd64)
	#include <tupai/x86/amd64/idt.hpp>
#elif defined(ARCH_i386)
	#include <tupai/x86/i386/idt.hpp>
#endif

namespace tupai
{
	namespace irq
	{
		bool are_enabled()
		{
			#if defined(ARCH_amd64)
				uint64_t flags = 0;
				asm volatile ("pushf; pop %%rax; mov %%rax, %0" : "=r" (flags) : : "%rax");
			#elif defined(ARCH_i386)
				uint32_t flags = 0;
				asm volatile ("pushf; pop %%eax; mov %%eax, %0" : "=r" (flags) : : "%eax");
			#endif

			return (flags & (1 << 9)) > 0;
		}

		void enable()
		{
			asm volatile ("sti");
		}

		void disable()
		{
			asm volatile ("cli");
		}

		void ack(uint8_t irq)
		{
			x86::pic_ack(irq);
		}

		void mask(uint8_t irq, bool enable)
		{
			x86::pic_mask(irq, enable);
		}

		void bind(uint8_t irq, void* address, bool hardware)
		{
			#if defined(ARCH_amd64)
				x86::amd64::idt_set_entry((hardware ? x86::PIC_REMAP_OFFSET : 0) + irq, address);
				x86::amd64::idt_install();
			#elif defined(ARCH_i386)
				x86::i386::idt_set_entry((hardware ? x86::PIC_REMAP_OFFSET : 0) + irq, address);
				x86::i386::idt_install();
			#endif
		}
	}
}
