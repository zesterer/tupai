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
#include <tupai/kpanic.hpp>

#if defined(SYSTEM_ARCH_i686)
	#include <tupai/i686/interrupt.hpp>
	#include <tupai/i686/idt.hpp>
	#include <tupai/i686/gdt.hpp>
	#include <tupai/i686/cpu.hpp>
#endif

// Libk
#include <libk/stdio.hpp>

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
		idt_set_entry(0x80, (uint32)syscall_irq_handler, sizeof(gdt_entry) * 1);
	}

	void syscall_irq_handler_main(cpu_pushal state_pushal, cpu_int state_int)
	{
		interrupt_ack(0x80);

		libk::printf("SYSCALL!\n");

		libk::printf("--- State ---\nEAX : 0x%X\nEBX : 0x%X\nECX : 0x%X\nEDX : 0x%X\nESP : 0x%X\nEBP : 0x%X\nESI : 0x%X\nEDI : 0x%X\nEIP : 0x%X\nCS : 0x%X\nEFLAGS : 0x%X\n",
		state_pushal.eax,
		state_pushal.ebx,
		state_pushal.ecx,
		state_pushal.edx,
		state_pushal.esp,
		state_pushal.ebp,
		state_pushal.esi,
		state_pushal.edi,
		state_int.eip,
		state_int.cs,
		state_int.eflags);
	}
}
