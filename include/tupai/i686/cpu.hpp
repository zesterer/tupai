/*
* 	file : cpu.hpp
*
* 	This file is part of Tupai.
*
* 	Tupai is free software: you can redistribute it and/or modify
* 	it under the terms of the GNU General Public License as published by
* 	the Free Software Foundation, either version 3 of the License, or
* 	(at your option) any later version.
*
* 	Tupai is distributed in the hope that it will be useful,
* 	but WITHOUT ANY WARRANTY; without even the implied warranty of
* 	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
* 	GNU General Public License for more details.
*
* 	You should have received a copy of the GNU General Public License
* 	along with Tupai.  If not, see <http://www.gnu.org/licenses/>.
*/

#ifndef TUPAI_I686_CPU_HPP
#define TUPAI_I686_CPU_HPP

// Tupai
#include <tupai/type.hpp>

namespace tupai
{
	struct cpu_state
	{
		// Segment registers
		uint32 gs, fs, es, ds;

		// General-purpose registers
		uint32 edi, esi, ebp, esp, ebx, edx, ecx, eax;

		// Method state stuff
		uint32 eip, cs, eflags, useresp, ss;
	} __attribute__((__packed__));

	struct cpu_task_state
	{
		uint32 eax, ebx, ecx, edx, esi, edi, esp, ebp, eip, eflags, cr3;
	} __attribute__((__packed__));

	struct cpu_pushal
	{
		uint32 edi, esi, ebp, esp, ebx, edx, ecx, eax;
	} __attribute__((__packed__));

	struct cpu_int
	{
		uint32 eip, cs, eflags;
	} __attribute__((__packed__));
}

#endif
