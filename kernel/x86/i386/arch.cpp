//
// file : arch.cpp
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
#include <tupai/arch.hpp>
#include <tupai/util/log.hpp>

namespace tupai
{

	struct arch_reg_state
	{
		uint32_t edi;
		uint32_t esi;
		uint32_t ebp;
		uint32_t esp;
		uint32_t ebx;
		uint32_t edx;
		uint32_t ecx;
		uint32_t eax;
		uint32_t eip;
		uint32_t eflags;
	} __attribute__((packed));

	const char* arch_get_target()
	{
		return "i386";
	}

	size_t arch_get_offset()
	{
		return 0xC0000000;
	}

	size_t arch_get_reg_state_size() { return sizeof(arch_reg_state); }

	void arch_display_reg_state(arch_reg_state* state)
	{
		util::logln("--- Register Dump ---");
		util::logln("Execution registers:");
		util::logln("  EIP    = ", util::fmt_int<uint32_t>(state->eip, 16, 16));
		util::logln("  EFLAGS = ", util::fmt_int<uint32_t>(state->eflags, 16, 16));
		util::logln("  ESP    = ", util::fmt_int<uint32_t>((uint64_t)state + 4, 16, 16)); // Location of reg block + 4

		util::logln("General-purpose registers:");
		util::logln("  EAX    = ", util::fmt_int<uint32_t>(state->eax, 16, 16));
		util::logln("  EBX    = ", util::fmt_int<uint32_t>(state->ebx, 16, 16));
		util::logln("  ECX    = ", util::fmt_int<uint32_t>(state->ecx, 16, 16));
		util::logln("  EDX    = ", util::fmt_int<uint32_t>(state->edx, 16, 16));
		util::logln("  EBP    = ", util::fmt_int<uint32_t>(state->ebp, 16, 16));
	}
}
