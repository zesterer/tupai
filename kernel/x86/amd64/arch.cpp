//
// file : arch.cpp
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
#include <tupai/arch.hpp>
#include <tupai/util/out.hpp>

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
		util::println("--- Register Dump ---");
		util::println("Execution registers:");
		util::println("  RIP    = ", util::fmt_int<uint64_t>(state->rip, 16, 16));
		util::println("  RFLAGS = ", util::fmt_int<uint64_t>(state->rflags, 16, 16));
		util::println("  CS     = ", util::fmt_int<uint64_t>(state->cs, 16, 16));
		util::println("  RSP    = ", util::fmt_int<uint64_t>((uint64_t)state + 8, 16, 16)); // Location of reg block + 8

		util::println("General-purpose registers:");
		util::println("  RAX    = ", util::fmt_int<uint64_t>(state->rax, 16, 16));
		util::println("  RBX    = ", util::fmt_int<uint64_t>(state->rbx, 16, 16));
		util::println("  RCX    = ", util::fmt_int<uint64_t>(state->rcx, 16, 16));
		util::println("  RDX    = ", util::fmt_int<uint64_t>(state->rdx, 16, 16));
		util::println("  RSI    = ", util::fmt_int<uint64_t>(state->rsi, 16, 16));
		util::println("  RDI    = ", util::fmt_int<uint64_t>(state->rdi, 16, 16));
		util::println("  RBP    = ", util::fmt_int<uint64_t>(state->rbp, 16, 16));

		util::println("Other registers:");
		util::println("  R8     = ", util::fmt_int<uint64_t>(state->r8, 16, 16));
		util::println("  R9     = ", util::fmt_int<uint64_t>(state->r9, 16, 16));
		util::println("  R10    = ", util::fmt_int<uint64_t>(state->r10, 16, 16));
		util::println("  R11    = ", util::fmt_int<uint64_t>(state->r11, 16, 16));
		util::println("  R12    = ", util::fmt_int<uint64_t>(state->r12, 16, 16));
		util::println("  R13    = ", util::fmt_int<uint64_t>(state->r13, 16, 16));
		util::println("  R14    = ", util::fmt_int<uint64_t>(state->r14, 16, 16));
		util::println("  R15    = ", util::fmt_int<uint64_t>(state->r15, 16, 16));
	}
}
