//
// file : thread.cpp
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
#include <tupai/sys/thread.hpp>
#include <tupai/sys/kmem.hpp>
#include <tupai/sys/call.hpp>
#include <tupai/interrupt.hpp>
#include <tupai/util/out.hpp>
#include <tupai/util/hwlock.hpp>
#include <tupai/util/str.hpp>
#include <tupai/panic.hpp>

namespace tupai
{
	namespace sys
	{
		static const size_t MAX_THREADS = 64;
		static const size_t STACK_SIZE  = 64 * 1024; // 64K

		static util::hwlock_t hwlock;

		static volatile thread_t threads[MAX_THREADS];

		static volatile bool   threads_enabled = false;
		static volatile id_t   cthread = -1;
		static volatile size_t cindex = -1;

		static volatile id_t thread_id_counter = 0;
		static id_t thread_gen_id() { return thread_id_counter++; }

		extern "C" void thread_finish();

		void threading_init()
		{
			hwlock.lock(); // Begin critical section

			// Clear threads
			for (size_t i = 0; i < MAX_THREADS; i ++)
			{
				threads[i].id     = -1;
				threads[i].cstate = thread_t::state::DEAD;
			}

			// Create the the main thread
			id_t nid = thread_gen_id();
			cindex = 0;
			threads[cindex].id = nid;
			threads[cindex].cstate = thread_t::state::WAITING;
			threads[cindex].native = false;
			cthread = nid;

			// Set threading to enabled
			threads_enabled = true;

			hwlock.unlock(); // End critical section
		}

		bool threading_enabled()
		{
			return threads_enabled;
		}

		id_t thread_create(void(*addr)(int argc, char* argv[]), int argc, char* argv[], const char* name, bool create_stack)
		{
			hwlock.lock(); // Begin critical section

			id_t nid = thread_gen_id();

			// Search for a free thread space
			for (size_t i = 0; i < MAX_THREADS; i ++)
			{
				if (threads[i].cstate == thread_t::state::DEAD)
				{
					threads[i].id = nid;
					threads[i].cstate = thread_t::state::UNSPAWNED;
					threads[i].native = true;
					threads[i].argc = argc;
					threads[i].argv = argv;

					// Copy the thread name
					size_t j;
					for (j = 0; name[j] != '\0' && j + 1 < thread_t::NAME_MAX_LEN; j ++)
						threads[i].name[j] = name[j];
					threads[i].name[j] = '\0';

					// Create a stack
					if (create_stack)
					{
						threads[i].entry = (size_t)addr;

						size_t nstack = (size_t)kmem_alloc(STACK_SIZE);
						threads[i].stackpos = nstack;
						threads[i].stack = nstack + STACK_SIZE;
					}

					break;
				}
			}

			hwlock.unlock(); // End critical section

			return nid;
		}

		id_t thread_get_id()
		{
			return cthread;
		}

		void thread_kill(id_t id)
		{
			if (id < 0)
				return;

			hwlock.lock(); // Begin critical section

			for (size_t i = 0; i < MAX_THREADS; i ++)
			{
				if (threads[i].id == id)
				{
					if (threads[i].cstate != thread_t::state::ACTIVE && threads[i].cstate != thread_t::state::UNSPAWNED)
						break;

					// Set current thread to dead
					//threads[i].id     = -1;
					threads[i].cstate = thread_t::state::DEAD;

					//if (threads[i].native)
					//	kmem_dealloc((void*)threads[i].stackpos); // Deallocate the stack

					break;
				}
			}

			hwlock.unlock(); // End critical section
		}

		void thread_wait_mutex(id_t id, util::mutex_t* mutex)
		{
			hwlock.lock(); // Begin critical section

			for (size_t i = 0; i < MAX_THREADS; i ++)
			{
				if (threads[i].id == id)
				{
					threads[i].wait_mutex = mutex;

					if (mutex == nullptr)
						continue;

					if (mutex->locked)
						threads[i].cstate = thread_t::state::WAITING;
					else
						threads[i].cstate = thread_t::state::ACTIVE;
				}
			}

			hwlock.unlock(); // End critical section

			thread_update_mutex(mutex);
		}

		void thread_update_mutex(util::mutex_t* mutex)
		{
			hwlock.lock(); // Begin critical section

			for (size_t i = 0; i < MAX_THREADS; i ++)
			{
				if (threads[i].wait_mutex == mutex)
				{
					if (mutex->locked)
						threads[i].cstate = thread_t::state::WAITING;
					else
						threads[i].cstate = thread_t::state::ACTIVE;
				}
			}

			hwlock.unlock(); // End critical section
		}

		void thread_finish()
		{
			thread_kill(thread_get_id());
			while (1);
		}

		size_t thread_next_stack(size_t ostack)
		{
			if (!threading_enabled())
				return ostack;

			threads[cindex].stack = ostack;

			for (size_t i = 0; i < MAX_THREADS; i ++)
			{
				size_t index = (cindex + i + 1) % MAX_THREADS;

				if (threads[index].cstate == thread_t::state::ACTIVE)
				{
					if (threads[index].wait_mutex != nullptr)
					{
						threads[index].wait_mutex->locked = true;
						threads[index].wait_mutex = nullptr;
						thread_update_mutex(threads[index].wait_mutex);
					}

					// Make it the active thread
					cthread = threads[index].id;
					cindex = index;
					threads[index].cstate = thread_t::state::ACTIVE;

					// Find the new stack
					size_t nstack = threads[index].stack;

					return nstack;
				}
				else if (threads[index].cstate == thread_t::state::UNSPAWNED) // If it's a new thread, just jump to the location
				{
					// Find the new entry and stack
					size_t nentry = threads[index].entry;
					size_t nstack = threads[index].stack;

					// Make it the active thread
					cthread = threads[index].id;
					cindex = index;
					threads[index].cstate = thread_t::state::ACTIVE;

					uint64_t argc = (uint64_t)threads[index].argc;
					uint64_t argv = (uint64_t)threads[index].argv;

					#if defined(ARCH_i386)
					{
						asm volatile (
							"mov %0, %%esp \n\
							 sti \n \
							 push %1 \n \
							 push %2 \n \
							 call *%3 \n \
							 call thread_finish \n"
							 : : "r" (nstack), "m" (argv), "m" (argc), "r" (nentry)
						);
					}
					#elif defined(ARCH_amd64)
					{
						asm volatile (
							"mov %0, %%rsp \n \
							 sti \n \
							 mov %1, %%rdi \n \
							 mov %2, %%rsi \n \
							 call *%3 \n \
							 call thread_finish \n"
							 : : "r" (nstack), "m" (argc), "m" (argv), "r" (nentry)
							 : "%rdi", "%rsi", "%rsp"
						);
					}
					#endif
				}
			}

			panic("Cannot determine active thread");
		}

		size_t threads_count()
		{
			size_t n = 0;

			hwlock.lock(); // Begin critical section

			for (size_t i = 0; i < MAX_THREADS; i ++)
			{
				if (threads[i].cstate != thread_t::state::DEAD)
					n ++;
			}

			hwlock.unlock(); // End critical section

			return n;
		}

		id_t thread_get_id(size_t index)
		{
			hwlock.lock(); // Begin critical section

			size_t c = 0;
			for (size_t i = 0; i < MAX_THREADS; i ++)
			{
				if (threads[i].cstate != thread_t::state::DEAD)
				{
					if (c == index)
					{
						hwlock.unlock(); // End critical section
						return threads[i].id;
					}

					c ++;
				}
			}

			hwlock.unlock(); // End critical section
			return 0;
		}

		bool __thread_get_name(id_t id, char* buff, size_t n)
		{
			hwlock.lock(); // Begin critical section

			size_t c = 0;
			for (size_t i = 0; i < MAX_THREADS; i ++)
			{
				if (threads[i].cstate != thread_t::state::DEAD)
				{
					if (threads[i].id == id)
					{
						util::str_cpy_n((const char*)threads[i].name, buff, n);

						hwlock.unlock(); // End critical section
						return true;
					}

					c ++;
				}
			}

			hwlock.unlock(); // End critical section
			return false;
		}
	}
}
