/*
* 	file : ssp.cpp
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

// Tupai
#include <tupai/kpanic.hpp>

// GCC
#include <stdint.h>

#if UINT32_MAX == UINTPTR_MAX
	const uintptr_t STACK_CHK_GUARD = 0xe2dee396;
#else
	const uintptr_t STACK_CHK_GUARD = 0x595e9fbd94fda766;
#endif

uintptr_t __stack_chk_guard = STACK_CHK_GUARD;

extern "C" __attribute__((noreturn)) void __stack_chk_fail()
{
	tupai::kpanic("Stack smashing detected");
}

extern "C" __attribute__((noreturn)) void __stack_chk_fail_local()
{
	__stack_chk_fail();
}
