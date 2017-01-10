/*
* 	file : task.hpp
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

#ifndef TUPAI_TASK_HPP
#define TUPAI_TASK_HPP

// Tupai
#include <tupai/type.hpp>

#if defined(SYSTEM_ARCH_i686)
	#include <tupai/i686/cpu.hpp>
#endif

namespace tupai
{
	struct task
	{
		const char* name = nullptr;
		uint32 id = 0;
		uint16 priority = 0;

		cpu_task_state state;
	};

	void task_init();
	void task_preempt();
	task task_create(const char* name, void (*main)(), uint32 eflags, uint32* page_dir);
}

#endif
