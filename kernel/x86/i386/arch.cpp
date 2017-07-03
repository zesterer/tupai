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
		util::println("--- Register Dump ---");
		util::println("Execution registers:");
		util::println("  EIP    = ", util::fmt_int<uint32_t>(state->eip, 16, 16));
		util::println("  EFLAGS = ", util::fmt_int<uint32_t>(state->eflags, 16, 16));
		util::println("  ESP    = ", util::fmt_int<uint32_t>((uint64_t)state + 4, 16, 16)); // Location of reg block + 4

		util::println("General-purpose registers:");
		util::println("  EAX    = ", util::fmt_int<uint32_t>(state->eax, 16, 16));
		util::println("  EBX    = ", util::fmt_int<uint32_t>(state->ebx, 16, 16));
		util::println("  ECX    = ", util::fmt_int<uint32_t>(state->ecx, 16, 16));
		util::println("  EDX    = ", util::fmt_int<uint32_t>(state->edx, 16, 16));
		util::println("  EBP    = ", util::fmt_int<uint32_t>(state->ebp, 16, 16));
	}
}
