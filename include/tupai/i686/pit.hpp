/*
* 	file : pit.hpp
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

#ifndef TUPAI_I686_PIT_HPP
#define TUPAI_I686_PIT_HPP

// Tupai
#include <tupai/type.hpp>

#include <tupai/i686/cpu.hpp>

namespace tupai
{
	extern volatile counter_t pit_counter;
	extern volatile uint16    pit_rate;

	void pit_init();
	void pit_set_rate(uint16 rate);
	void pit_set_tick_func(void (*func)(cpu_pushal, cpu_int));
}

#endif
