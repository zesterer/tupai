/*
* 	file : task.cpp
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
#include <tupai/task.hpp>
#include <tupai/generic/ringbuff.hpp>

#if defined(SYSTEM_ARCH_i686)
	#include <tupai/i686/pit.hpp>
#endif

// Libk
#include <libk/stdio.hpp>

namespace tupai
{
	generic::ringbuff<task> tasks;
	const uint32 MAX_TASK_NUM = 64;

	uint32 current_task = 0;
	uint32 task_id_counter = 1;

	static uint32 task_get_new_id();
	static void task_save_state(cpu_pushal state_pushal, cpu_int state_int);

	void task_init()
	{
		tasks.init(64);
		pit_set_tick_func(task_save_state);
	}

	static void task_save_state(cpu_pushal state_pushal, cpu_int state_int)
	{
		task_preempt();
	}

	void task_preempt()
	{
		// Choose a task to run
		if (tasks.length() <= 0)
			return; // Return if no tasks are running. We've probably not enabled multitasking yet!

		current_task = (current_task + 1) % tasks.length();
		libk::printf("Switched to task '%s' (%i)!\n", tasks[current_task].name, current_task);
	}

	static uint32 task_get_new_id()
	{
		task_id_counter ++;
		return task_id_counter - 1;
	}

	task task_create(const char* name, void (*main)(), uint32 eflags, uint32* page_dir)
	{
		task n_task;

		n_task.name = name;
		n_task.id = task_get_new_id();
		n_task.priority = 0;

		n_task.state.eax = 0;
		n_task.state.ebx = 0;
		n_task.state.ecx = 0;
		n_task.state.edx = 0;
		n_task.state.esi = 0;
		n_task.state.edi = 0;
		n_task.state.eflags = eflags;
		n_task.state.eip = (uint32)main;
		n_task.state.cr3 = (uint32)page_dir;
		n_task.state.esp = (uint32)0; // MUST BE FILLED LATER!

		return n_task;
	}
}
