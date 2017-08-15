//
// file : call.cpp
//
// Copyright (c) 2017 Joshua Barretto <joshua.s.barretto@gmail.com>
//
// This file is part of Tupai.
//
// Permission to use, copy, modify, and distribute this software for any
// purpose with or without fee is hereby granted, provided that the above
// copyright notice and this permission notice appear in all copies.
//
// THE SOFTWARE IS PROVIDED "AS IS" AND THE AUTHOR DISCLAIMS ALL WARRANTIES
// WITH REGARD TO THIS SOFTWARE INCLUDING ALL IMPLIED WARRANTIES OF
// MERCHANTABILITY AND FITNESS. IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR
// ANY SPECIAL, DIRECT, INDIRECT, OR CONSEQUENTIAL DAMAGES OR ANY DAMAGES
// WHATSOEVER RESULTING FROM LOSS OF USE, DATA OR PROFITS, WHETHER IN AN
// ACTION OF CONTRACT, NEGLIGENCE OR OTHER TORTIOUS ACTION, ARISING OUT OF
// OR IN CONNECTION WITH THE USE OR PERFORMANCE OF THIS SOFTWARE.
//

// Tupai
#include <tupai/sys/call.hpp>
#include <tupai/task/task.hpp>
#include <tupai/task/scheduler.hpp>
#include <tupai/vfs/vfs.hpp>
#include <tupai/vfs/fd.hpp>
#include <tupai/util/mutex.hpp>
#include <tupai/irq.hpp>
#include <tupai/cpu.hpp>

namespace tupai
{
	namespace sys
	{
		extern "C" void isr_syscall();
		extern "C" size_t syscall_isr_main(size_t stack_ptr, size_t arg0, size_t arg1, size_t arg2, size_t arg3);

		void call_bind()
		{
			// Bind the interrupt
			irq::bind(CALL_IRQ, (void*)isr_syscall);
		}

		void call_init()
		{
			call_bind();
		}

		size_t syscall_isr_main(size_t stack_ptr, size_t arg0, size_t arg1, size_t arg2, size_t arg3)
		{
			(void)arg0;
			(void)arg1;
			(void)arg2;
			(void)arg3;

			// Re-enable interrupts since the kernel is re-entrant!
			irq::enable();
			cpu::end_irq();

			CALL call = (CALL)arg0;

			switch (call)
			{
			case CALL::YIELD:
				stack_ptr = task::scheduler_preempt(stack_ptr);
				break;

			/*case CALL::LMUTEX:
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
				break;*/

			case CALL::OPEN:
				{
					const char* path  = (const char*)arg1;
					id_t*       rdesc = (id_t*)arg2;

					vfs::inode_ptr_t inode = vfs::get_inode(path);
					if (inode == ID_INVALID)
						*rdesc = 0;
					else
						*rdesc = vfs::fd_open(inode);
				}
				break;

			case CALL::CLOSE:
				{
					id_t lfd = (id_t)arg1;
					vfs::fd_close(lfd);
				}
				break;

			case CALL::READ:
				{
					id_t     lfd   = (id_t)arg1;
					ssize_t* rn    = (ssize_t*)arg2;
					void*    rbuff = (void*)arg3;

					*rn = vfs::fd_read(lfd, rbuff, *rn);
				}
				break;

			case CALL::WRITE:
				{
					id_t        lfd   = (id_t)arg1;
					ssize_t*    rn    = (ssize_t*)arg2;
					const void* rbuff = (const void*)arg3;
					*rn = vfs::fd_write(lfd, rbuff, *rn);
				}
				break;

			default:
				break;
			}

			return stack_ptr;
		}
	}
}
