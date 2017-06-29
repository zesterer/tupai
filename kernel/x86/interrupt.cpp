//
// file : interrupt.cpp
//
// This file is part of Tupai.
//
// Tupai is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
//
// Tupai is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with Tupai.  If not, see <http://www.gnu.org/licenses/>.
//

// Tupai
#include <tupai/interrupt.hpp>
#include <tupai/debug.hpp>
#include <tupai/x86/pic.hpp>

//#include <tupai/util/out.hpp>

#if defined(ARCH_amd64)
	#include <tupai/x86/amd64/idt.hpp>
#elif defined(ARCH_i386)
	#include <tupai/x86/i386/idt.hpp>
#endif

namespace tupai
{
	void interrupt_enable(bool enable)
	{
		if (enable)
			asm volatile ("sti");
		else
			asm volatile ("cli");
	}

	bool interrupt_enabled()
	{
		#if defined(ARCH_amd64)
			uint64_t flags = 0;
			asm volatile ("pushf; pop %%rax; mov %%rax, %0" : "=r" (flags) : : "%rax");
		#elif defined(ARCH_i386)
			uint32_t flags = 0;
			asm volatile ("pushf; pop %%eax; mov %%eax, %0" : "=r" (flags) : : "%eax");
		#endif
		//util::println(util::fmt_int<uint64_t>(eflags, 2, 16));
		return (flags & (1 << 9)) > 0;
	}

	void interrupt_bind(uint8_t irq, void* address, bool hardware)
	{
		#if defined(ARCH_amd64)
			x86::amd64::idt_set_entry(hardware ? x86::PIC_REMAP_OFFSET : 0 + irq, address);
		#elif defined(ARCH_i386)
			x86::i386::idt_set_entry(hardware ? x86::PIC_REMAP_OFFSET : 0 + irq, address);
		#endif
	}

	void interrupt_ack(uint8_t irq)
	{
		x86::pic_ack(irq);
	}

	void interrupt_mask(uint8_t irq, bool enable)
	{
		x86::pic_mask(irq, enable);
	}
}
