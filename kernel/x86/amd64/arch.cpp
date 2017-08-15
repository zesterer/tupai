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
		uint64_t r15;
		uint64_t r14;
		uint64_t r13;
		uint64_t r12;
		uint64_t r11;
		uint64_t r10;
		uint64_t r9;
		uint64_t r8;
		uint64_t rbp;
		uint64_t rdi;
		uint64_t rsi;
		uint64_t rdx;
		uint64_t rcx;
		uint64_t rbx;
		uint64_t rax;
		uint64_t rip;
		uint64_t cs;
		uint64_t rflags;


	} __attribute__((packed));

	const char* arch_get_target()
	{
		return "amd64";
	}

	size_t arch_get_offset()
	{
		return 0xFFFFFFFF80000000;
	}

	size_t arch_get_reg_state_size() { return sizeof(arch_reg_state); }

	void arch_display_reg_state(arch_reg_state* state)
	{
		util::logln("--- Register Dump ---");
		util::logln("Execution registers:");
		util::logln("  RIP    = ", util::fmt_int<uint64_t>(state->rip, 16, 16));
		util::logln("  RFLAGS = ", util::fmt_int<uint64_t>(state->rflags, 16, 16));
		util::logln("  CS     = ", util::fmt_int<uint64_t>(state->cs, 16, 16));
		util::logln("  RSP    = ", util::fmt_int<uint64_t>((uint64_t)state + 8, 16, 16)); // Location of reg block + 8

		util::logln("General-purpose registers:");
		util::logln("  RAX    = ", util::fmt_int<uint64_t>(state->rax, 16, 16));
		util::logln("  RBX    = ", util::fmt_int<uint64_t>(state->rbx, 16, 16));
		util::logln("  RCX    = ", util::fmt_int<uint64_t>(state->rcx, 16, 16));
		util::logln("  RDX    = ", util::fmt_int<uint64_t>(state->rdx, 16, 16));
		util::logln("  RSI    = ", util::fmt_int<uint64_t>(state->rsi, 16, 16));
		util::logln("  RDI    = ", util::fmt_int<uint64_t>(state->rdi, 16, 16));
		util::logln("  RBP    = ", util::fmt_int<uint64_t>(state->rbp, 16, 16));

		util::logln("Other registers:");
		util::logln("  R8     = ", util::fmt_int<uint64_t>(state->r8, 16, 16));
		util::logln("  R9     = ", util::fmt_int<uint64_t>(state->r9, 16, 16));
		util::logln("  R10    = ", util::fmt_int<uint64_t>(state->r10, 16, 16));
		util::logln("  R11    = ", util::fmt_int<uint64_t>(state->r11, 16, 16));
		util::logln("  R12    = ", util::fmt_int<uint64_t>(state->r12, 16, 16));
		util::logln("  R13    = ", util::fmt_int<uint64_t>(state->r13, 16, 16));
		util::logln("  R14    = ", util::fmt_int<uint64_t>(state->r14, 16, 16));
		util::logln("  R15    = ", util::fmt_int<uint64_t>(state->r15, 16, 16));
	}
}
