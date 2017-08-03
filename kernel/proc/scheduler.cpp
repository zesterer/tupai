//
// file : proc.cpp
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
#include <tupai/proc/scheduler.hpp>
#include <tupai/util/queue.hpp>
#include <tupai/util/hwlock.hpp>

namespace tupai
{
	namespace proc
	{
		struct task_t
		{
			thread_ptr_t thread = ID_INVALID;
			short priority = ID_INVALID;
			short cpriority = ID_INVALID;
		};

		static util::queue_t<task_t, 256> schedule;
		static task_t ctask;

		static util::hwlock_t hwlock;

		void scheduler_bootstrap_task(size_t entry, size_t stack, int argc = 0, char* argv[] = nullptr);
		extern "C" void proc_thread_finish();

		void scheduler_init()
		{
			// Nothing yet
		}

		void scheduler_schedule(thread_ptr_t thread, int priority)
		{
			(void)priority;

			hwlock.lock(); // Begin critical section

			task_t ntask;
			ntask.thread = thread;
			ntask.priority = priority;
			ntask.cpriority = priority;
			schedule.push(ntask);

			hwlock.unlock(); // End critical section
		}

		void scheduler_increment()
		{
			hwlock.lock(); // Begin critical section

			if (ctask.thread != ID_INVALID && ctask.thread.get_state() != thread_state::DEAD)
			{
				ctask.cpriority = ctask.priority;
				schedule.push(ctask);
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

			hwlock.unlock(); // End critical section
		}

		thread_ptr_t scheduler_current()
		{
			hwlock.lock(); // Begin critical section
			thread_ptr_t val = ctask.thread;
			hwlock.unlock(); // End critical section

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
