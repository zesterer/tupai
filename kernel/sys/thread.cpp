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
// but WITHOUT ANY WARRANTY// without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with Tupai.  If not, see <http://www.gnu.org/licenses/>.
//

// Tupai
#include <tupai/sys/thread.hpp>
#include <tupai/interrupt.hpp>
#include <tupai/util/out.hpp>
#include <tupai/util/mutex.hpp>

namespace tupai
{
	namespace sys
	{
		const size_t MAXTHREADS = 64;
		const size_t STACKSIZE  = 1024;

		volatile thread_t threads[MAXTHREADS];
		#if defined(ARCH_ADDRESS_64)
			volatile uint64_t stacks[STACKSIZE / 8][MAXTHREADS] __attribute__((aligned(16)));
		#elif defined(ARCH_ADDRESS_32)
			volatile uint32_t stacks[STACKSIZE / 4][MAXTHREADS] __attribute__((aligned(16)));
		#endif

		volatile bool threads_enabled = false;
		volatile id_t cthread = -1;

		volatile id_t thread_id_counter = 0;
		static id_t thread_gen_id() { return thread_id_counter++; }

		void threading_init()
		{
			// Clear threads
			for (size_t i = 0; i < MAXTHREADS; i ++)
			{
				threads[i].id     = -1;
				threads[i].cstate = thread_t::state::DEAD;
			}

			// Create the the main thread
			id_t nid = thread_gen_id();
			threads[0].id = nid;
			threads[0].cstate = thread_t::state::WAITING;
			cthread = nid;

			// Set threading to enabled
			threads_enabled = true;
		}

		bool threading_enabled()
		{
			return threads_enabled;
		}

		id_t thread_create(void(*addr)(), bool create_stack)
		{
			id_t nid = thread_gen_id();

			// Search for a free thread space
			for (size_t i = 0; i < MAXTHREADS; i ++)
			{
				if (threads[i].cstate == thread_t::state::DEAD)
				{
					threads[i].id = nid;
					threads[i].cstate = thread_t::state::UNSPAWNED;

					// Create a stack
					if (create_stack)
					{
						threads[i].entry = (size_t)addr;
						threads[i].stack = (size_t)(&stacks[0][0]) + i * STACKSIZE + STACKSIZE;
					}

					break;
				}
			}

			return nid;
		}

		id_t thread_get_id()
		{
			return cthread;
		}

		int c = 0;
		size_t thread_next_stack(size_t ostack)
		{
			size_t found_index = 0;

			for (size_t i = 0; i < MAXTHREADS; i ++)
			{
				if (threads[i].id == cthread)
				{
					found_index = i;
					// Set current thread to waiting
					threads[found_index].cstate = thread_t::state::WAITING;
					threads[found_index].stack = ostack;
					break;
				}
			}

			for (size_t i = 0; i < MAXTHREADS; i ++)
			{
				size_t cindex = (found_index + i + 1) % MAXTHREADS;

				if (threads[cindex].cstate == thread_t::state::WAITING)
				{
					// Make it the active thread
					cthread = threads[cindex].id;
					threads[cindex].cstate = thread_t::state::ACTIVE;

					// Find the new stack
					size_t nstack = threads[cindex].stack;

					return nstack;
				}
				else if (threads[cindex].cstate == thread_t::state::UNSPAWNED) // If it's a new thread, just jump to the location
				{
					// Find the new entry and stack
					size_t nentry = threads[cindex].entry;
					size_t nstack = threads[cindex].stack;

					// Make it the active thread
					cthread = threads[cindex].id;
					threads[cindex].cstate = thread_t::state::WAITING;

					#if defined(ARCH_i386)
					{
						asm volatile (
							"mov %0, %%esp \n\
							 sti \n \
							 jmp *%1\n"
							 : : "r" (nstack), "r" (nentry)
						);
					}
					#elif defined(ARCH_amd64)
					{
						asm volatile (
							"mov %0, %%rsp \n \
							 sti \n \
							 jmp *%1\n"
							 : : "r" (nstack), "r" (nentry)
						);
					}
					#endif
				}
			}
		}
	}
}
