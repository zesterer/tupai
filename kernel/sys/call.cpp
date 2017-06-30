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

namespace tupai
{
	namespace sys
	{
		extern "C" void isr_syscall();
		extern "C" size_t syscall_isr_main(size_t stack_ptr);

		void call_bind()
		{
			// Bind the interrupt
			interrupt_bind(CALL_IRQ, (void*)isr_syscall);
		}

		void call_init()
		{
			// Do nothing yet
		}

		size_t syscall_isr_main(size_t stack_ptr)
		{
			if (sys::threading_enabled())
				stack_ptr = sys::thread_next_stack(stack_ptr);
			return stack_ptr;
		}

		void call(int call)
		{
			asm volatile ("int $0x80");
		}
	}
}
