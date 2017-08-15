//
// file : proc.cpp
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
#include <tupai/task/scheduler.hpp>
#include <tupai/util/queue.hpp>
#include <tupai/util/hwlock.hpp>

namespace tupai
{
	namespace task
	{
		struct task_t
		{
			thrd_ptr_t thread = ID_INVALID;
			short priority = ID_INVALID;
			short cpriority = ID_INVALID;
		};

		static util::queue_t<task_t, 256> schedule;
		static task_t ctask;

		void scheduler_bootstrap_task(size_t entry, size_t stack, int argc = 0, char* argv[] = nullptr);
		extern "C" void proc_thread_finish();

		void scheduler_init()
		{
			util::hwlock_acquire(); // Begin critical section

			// Nothing yet

			util::hwlock_release(); // End critical section
		}

		void scheduler_schedule(thrd_ptr_t thread, int priority)
		{
			(void)priority;

			util::hwlock_acquire(); // Begin critical section

			task_t ntask;
			ntask.thread = thread;
			ntask.priority = priority;
			ntask.cpriority = priority;
			schedule.push(ntask);

			util::hwlock_release(); // End critical section
		}

		void scheduler_increment()
		{
			util::hwlock_acquire(); // Begin critical section

			if (ctask.thread != ID_INVALID)
			{
				if (ctask.thread.get_state() == thread_state::DEAD)
					ctask.thread.get_process().destroy_thread(ctask.thread);
				else
				{
					ctask.cpriority = ctask.priority;
					schedule.push(ctask);
				}
			}

			while (true)
			{
				ctask = schedule.pop();

				if (ctask.cpriority <= 0)
					break;
				else
				{
					ctask.cpriority --;
					schedule.push(ctask);
				}
			}

			set_current_thread(ctask.thread);

			util::hwlock_release(); // End critical section
		}

		thrd_ptr_t scheduler_current()
		{
			util::hwlock_acquire(); // Begin critical section

			thrd_ptr_t val = ctask.thread;

			util::hwlock_release(); // End critical section
			return val;
		}

		size_t scheduler_preempt(size_t old_stack)
		{
			if (ctask.thread != ID_INVALID)
				ctask.thread.set_stack(old_stack);

			scheduler_increment();
			size_t new_stack = ctask.thread.get_stack();

			switch (ctask.thread.get_state())
			{
			case thread_state::NEW:
				{
					ctask.thread.set_state(thread_state::ACTIVE);
					scheduler_bootstrap_task(ctask.thread.get_entry(), ctask.thread.get_stack());
				}
				break;
			default:
				return new_stack;
			}

			return 0;
		}

		void proc_thread_finish()
		{
			get_current_thread().kill();
			while (true);
		}

		void scheduler_bootstrap_task(size_t entry, size_t stack, int argc, char* argv[])
		{
			#if defined(ARCH_i386)
			{
				asm volatile (
					"mov %0, %%esp \n\
					 push %1 \n \
					 push %2 \n \
					 movl $0, (in_irq) \n \
					 sti \n \
					 call *%3 \n \
					 call proc_thread_finish \n"
					 : : "r" (stack), "m" (argv), "m" (argc), "r" (entry)
				);
			}
			#elif defined(ARCH_amd64)
			{
				asm volatile (
					"mov %0, %%rsp \n \
					 mov %1, %%rdi \n \
					 mov %2, %%rsi \n \
					 movq $0, (in_irq) \n \
					 sti \n \
					 call *%3 \n \
					 call proc_thread_finish \n"
					 : : "r" (stack), "m" (argc), "m" (argv), "r" (entry)
					 : "%rdi", "%rsi", "%rsp"
				);
			}
			#endif
		}
	}
}
