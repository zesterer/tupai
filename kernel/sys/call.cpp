//
// file : call.cpp
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
#include <tupai/sys/call.hpp>
#include <tupai/interrupt.hpp>
#include <tupai/sys/thread.hpp>
#include <tupai/util/out.hpp>

#include <tupai/x86/textmode.hpp>

namespace tupai
{
	namespace sys
	{
		extern "C" void isr_syscall();
		extern "C" size_t syscall_isr_main(size_t stack_ptr, size_t arg0, size_t arg1, size_t arg2, size_t arg3);

		void call_bind()
		{
			// Bind the interrupt
			interrupt_bind(CALL_IRQ, (void*)isr_syscall);
		}

		void call_init()
		{
			// Do nothing yet
		}

		size_t syscall_isr_main(size_t stack_ptr, size_t arg0, size_t arg1, size_t arg2, size_t arg3)
		{
			char buff[1024];
			util::fmt(buff, "SYSCALL (arg0 = ", arg0, ", arg1 = ", arg1, ", arg2 = ", arg2, ", arg3 = ", arg3, ")");
			//for (size_t i = 0; buff[i] != '\0'; i ++)
			//	x86::textmode_write(buff[i]);

			if (sys::threading_enabled())
				stack_ptr = sys::thread_next_stack(stack_ptr);
			return stack_ptr;
		}
	}
}
