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
#include <tupai/util/mutex.hpp>
#include <tupai/util/out.hpp>
#include <tupai/fs/desc.hpp>
#include <tupai/fs/vfs.hpp>

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
			(void)arg0;
			(void)arg1;
			(void)arg2;
			(void)arg3;

			CALL call = (CALL)arg0;

			switch (call)
			{
			case CALL::YIELD:
				stack_ptr = sys::thread_next_stack(stack_ptr);
				break;

			case CALL::LMUTEX:
				{
					util::mutex_t* mutex = (util::mutex_t*)arg0;
					if (mutex->locked) // The mutex is already locked
					{
						sys::thread_wait_mutex(sys::thread_get_id(), mutex);
						sys::thread_update_mutex(mutex);
						stack_ptr = sys::thread_next_stack(stack_ptr);
					}
					else // The mutex is available
					{
						mutex->locked = true;
						sys::thread_wait_mutex(sys::thread_get_id(), nullptr);
						sys::thread_update_mutex(mutex);
					}
				}
				break;

			case CALL::UMUTEX:
				{
					util::mutex_t* mutex = (util::mutex_t*)arg0;
					mutex->locked = false;
					sys::thread_update_mutex(mutex);
				}
				break;

			case CALL::OPEN:
				{
					id_t*       desc_id = (id_t*)arg1;
					const char* path = (const char*)arg2;

					fs::inode_t* inode = fs::vfs_get_inode(path);
					if (inode == nullptr)
						*desc_id = 0;
					else
						*desc_id = fs::desc_open(inode);
				}
				break;

			default:
				break;
			}

			return stack_ptr;
		}
	}
}
