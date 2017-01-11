/*
* 	file : kpanic.hpp
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

#ifndef TUPAI_KPANIC_HPP
#define TUPAI_KPANIC_HPP

// Tupai
#include <tupai/type.hpp>

#define KBREAK() asm volatile ("xchgw %bx, %bx")

namespace tupai
{
	extern "C" void kpanic(const char* msg = nullptr, long code = 0x0) __attribute__((noreturn));
	extern "C" void kbreak();
	extern "C" void khalt();

	extern "C" void kfault(uint32 isr_id, uint32 error);
}

#endif
