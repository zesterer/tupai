/*
* 	file : syscall.cpp
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
#include <tupai/syscall.hpp>
#include <tupai/task.hpp>

#include <tupai/util/out.hpp>

#if defined(SYSTEM_ARCH_i686)
	#include <tupai/i686/interrupt.hpp>
	#include <tupai/i686/idt.hpp>
	#include <tupai/i686/gdt.hpp>
	#include <tupai/i686/cpu.hpp>
#endif

#include <tupai/util/conv.hpp>

namespace tupai
{
	extern "C" void syscall_irq_handler_main(cpu_pushal state_pushal, cpu_int state_int);

	#if defined(SYSTEM_ARCH_i686)
		// Syscall interrupt handler
		extern "C" void syscall_irq_handler();
		//asm volatile ("pit_irq_handler: \n call pit_irq_handler_main \n iret");
		asm volatile (
						".section .text \n"
						"	.align 4 \n"
		 				"	syscall_irq_handler: \n"
						"		pushal \n"
						"		call syscall_irq_handler_main \n"
						"		popal \n"
						"		iret \n"
						);
	#endif

	void syscall_init()
	{
		idt_set_entry(0x80, (uint32)syscall_irq_handler, 1);
	}

	void syscall_irq_handler_main(cpu_pushal state_pushal, cpu_int state_int)
	{
		interrupt_ack(0x80);

		task_save_state(state_pushal, state_int);
		task_preempt();

		util::println("SYSCALL!");
	}
}
