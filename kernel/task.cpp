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
#include <tupai/kpanic.hpp>

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
	bool in_task = false;
	uint32 task_id_counter = 1;
	bool task_scheduler_enabled = false;

	static uint32 task_get_new_id();
	static void task_save_state(cpu_pushal state_pushal, cpu_int state_int);

	extern "C" void _task_switch(cpu_task_state* old, cpu_task_state* target);

	void task_init()
	{
		tasks.init(64);
		pit_set_tick_func(task_save_state);
	}

	void task_enable_scheduler(bool enable)
	{
		task_scheduler_enabled = enable;
	}

	static void task_save_state(cpu_pushal state_pushal, cpu_int state_int)
	{
		if (!task_scheduler_enabled) // If the task scheduler is not enabled yet
			return;

		if (in_task)
		{
			tasks[current_task].state.eax    = state_pushal.eax;
			tasks[current_task].state.ebx    = state_pushal.ebx;
			tasks[current_task].state.ecx    = state_pushal.ecx;
			tasks[current_task].state.edx    = state_pushal.edx;
			tasks[current_task].state.esi    = state_pushal.esi;
			tasks[current_task].state.edi    = state_pushal.edi;
			tasks[current_task].state.esp    = state_pushal.esp;
			tasks[current_task].state.ebp    = state_pushal.ebp;
			tasks[current_task].state.eip    = state_int.eip;
			tasks[current_task].state.eflags = state_int.eflags;
			//tasks[current_task].state.cr3    = ?;
		}

		task_preempt();
	}

	void task_preempt()
	{
		if (tasks.length() <= 0)
			return; // Return if no tasks are running. We've probably not enabled multitasking yet!

		current_task = (current_task + 1) % tasks.length();
		libk::printf("Switched to task '%s' (%i)!\n", tasks[current_task].name, current_task);

		cpu_task_state nil;
		cpu_task_state regs = tasks[current_task].state;
		in_task = true;
		//KBREAK();
		_task_switch(&nil, &regs);
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
		n_task.stack = nullptr;

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

	void task_add(const char* name, void (*main)(), uint32 eflags, uint32* page_dir)
	{
		task n_task = task_create(name, main, eflags, page_dir);

		// Create a stack for the task. Make it 4Kb (1024 stack elements) TODO: change this
		uint32* stack = new uint32[1024];
		uint32 stack_ptr = (uint32)stack + (sizeof(uint32) * (1024 - 4)); // The stack grows downwards on x86, so point to the end of the buffer
		n_task.state.esp = stack_ptr;
		n_task.stack = stack;

		tasks.push(n_task);
	}
}
