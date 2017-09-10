//
// file : task.cpp
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
#include <tupai/task/task.hpp>
#include <tupai/task/thread.hpp>
#include <tupai/task/process.hpp>
#include <tupai/task/scheduler.hpp>
#include <tupai/util/str.hpp>
#include <tupai/util/mem.hpp>
#include <tupai/util/hwlock.hpp>

#include <tupai/util/log.hpp>
#include <tupai/util/math.hpp>

namespace tupai
{
	namespace task
	{
		static proc_ptr_t   cproc = ID_INVALID;   // The current running process
		static proc_ptr_t   kproc = 0;            // The kernel process
		static thrd_ptr_t cthread = ID_INVALID; // The current running thread

		static util::hashtable_t<process_t> proc_table;
		static id_t proc_counter = (id_t)kproc;

		static util::hashtable_t<thread_t> thread_table;
		static id_t thread_counter = 0;

		thrd_ptr_t create_thread(proc_ptr_t proc, void (*entry)(int argc, char* argv[]));

		/* Process control functions */

		proc_ptr_t get_kernel()
		{
			util::hwlock_acquire(); // Begin critical section

			proc_ptr_t val = kproc;

			util::hwlock_release(); // End critical section
			return val;
		}

		proc_ptr_t get_current()
		{
			util::hwlock_acquire(); // Begin critical section

			proc_ptr_t val = cproc;

			util::hwlock_release(); // End critical section
			return val;
		}

		thrd_ptr_t get_current_thread()
		{
			util::hwlock_acquire(); // Begin critical section

			thrd_ptr_t val = cthread;

			util::hwlock_release(); // End critical section
			return val;
		}

		void set_current(proc_ptr_t proc)
		{
			util::hwlock_acquire(); // Begin critical section

			cproc = proc;

			util::hwlock_release(); // End critical section
		}

		void set_current_thread(thrd_ptr_t thread)
		{
			util::hwlock_acquire(); // Begin critical section

			cthread = thread;
			cproc = thread.get_process();

			util::hwlock_release(); // End critical section
		}

		void init()
		{
			util::hwlock_acquire(); // Begin critical section

			//proc_table   = util::hashtable_t<process_t>();
			//thread_table = util::hashtable_t<thread_t>();

			// Create the initial kernel process
			kproc = create_process("kernel", vfs::get_root());
			cproc = kproc;

			util::hwlock_release(); // End critical section
		}

		proc_ptr_t create_process(const char* name, vfs::inode_ptr_t dir)
		{
			util::hwlock_acquire(); // Begin critical section

			process_t nproc;
			nproc.id = proc_counter ++;
			util::str_cpy_n(name, nproc.name);
			nproc.dir = dir;

			proc_table.add(nproc.id, nproc);

			util::hwlock_release(); // End critical section
			return nproc.id;
		}

		void display()
		{
			for (size_t i = 0; i < proc_table.size(); i ++)
			{
				process_t* proc = proc_table.nth(i);
				util::logln((long)i, " -> ", proc->name, " (", (long)proc->id, ")");

				for (size_t j = 0; j < proc->threads.size(); j ++)
					util::logln("    ", (long)j, " -> Thread ", (long)proc->threads.nth(j)->get_lid());
			}
		}

		/* Thread functions */

		id_t thrd_ptr_t::get_lid()
		{
			util::hwlock_acquire(); // Begin critical section

			thread_t* thread = thread_table[this->id];

			id_t val = ID_INVALID;
			if (thread != nullptr)
				val = thread->lid;

			util::hwlock_release(); // End critical section
			return val;
		}

		thread_state thrd_ptr_t::get_state()
		{
			util::hwlock_acquire(); // Begin critical section

			thread_t* thread = thread_table[this->id];

			thread_state val = thread_state::DEAD;
			if (thread != nullptr)
				val = thread->state;

			util::hwlock_release(); // End critical section
			return val;
		}

		proc_ptr_t thrd_ptr_t::get_process()
		{
			util::hwlock_acquire(); // Begin critical section

			thread_t* thread = thread_table[this->id];

			proc_ptr_t val = ID_INVALID;
			if (thread != nullptr)
				val = thread->proc;

			util::hwlock_release(); // End critical section
			return val;
		}

		size_t thrd_ptr_t::get_entry()
		{
			util::hwlock_acquire(); // Begin critical section

			thread_t* thread = thread_table[this->id];

			size_t val = 0;
			if (thread != nullptr)
				val = thread->entry;

			util::hwlock_release(); // End critical section
			return val;
		}

		size_t thrd_ptr_t::get_stack()
		{
			util::hwlock_acquire(); // Begin critical section

			thread_t* thread = thread_table[this->id];

			size_t val = 0;
			if (thread != nullptr)
				val = thread->stack;

			util::hwlock_release(); // End critical section
			return val;
		}

		short thrd_ptr_t::get_priority()
		{
			util::hwlock_acquire(); // Begin critical section

			thread_t* thread = thread_table[this->id];

			short result = -1;
			if (thread != nullptr)
				result = thread->priority;

			util::hwlock_release(); // End critical section
			return result;
		}

		short thrd_ptr_t::get_effective_priority()
		{
			util::hwlock_acquire(); // Begin critical section

			thread_t* thread = thread_table[this->id];

			short result = -1;
			if (thread != nullptr)
			{
				process_t* proc = proc_table[thread->proc];
				if (proc != nullptr)
					result = thread->priority + proc->priority;
			}

			util::hwlock_release(); // End critical section
			return result;
		}

		void thrd_ptr_t::set_lid(id_t lid)
		{
			util::hwlock_acquire(); // Begin critical section

			thread_t* thread = thread_table[this->id];

			if (thread != nullptr)
				thread->lid = lid;

			util::hwlock_release(); // End critical section
		}

		void thrd_ptr_t::set_state(thread_state state)
		{
			util::hwlock_acquire(); // Begin critical section

			thread_t* thread = thread_table[this->id];

			if (thread != nullptr)
				thread->state = state;

			util::hwlock_release(); // End critical section
		}

		void thrd_ptr_t::set_stack(size_t stack)
		{
			util::hwlock_acquire(); // Begin critical section

			thread_t* thread = thread_table[this->id];

			if (thread != nullptr)
				thread->stack = stack;

			util::hwlock_release(); // End critical section
		}

		int thrd_ptr_t::kill()
		{
			this->set_state(thread_state::DEAD);

			return 0;
		}

		/* Process functions */

		int proc_ptr_t::get_name(char* rbuff, size_t n)
		{
			util::hwlock_acquire(); // Begin critical section

			process_t* proc = proc_table[this->id];

			int err = 1;
			if (proc != nullptr)
			{
				util::str_cpy_n(proc->name, rbuff, n);
				err = 0;
			}

			util::hwlock_release(); // End critical section
			return err;
		}

		vfs::fd_ptr_t proc_ptr_t::get_fd(id_t lfd)
		{
			util::hwlock_acquire(); // Begin critical section

			process_t* proc = proc_table[this->id];

			vfs::fd_ptr_t val = ID_INVALID;
			if (proc != nullptr && lfd != ID_INVALID)
			{
				vfs::fd_ptr_t* ptr = proc->fds[lfd];
				if (ptr != nullptr)
					val = *ptr;
			}

			util::hwlock_release(); // End critical section
			return val;
		}

		short proc_ptr_t::get_priority()
		{
			util::hwlock_acquire(); // Begin critical section

			process_t* proc = proc_table[this->id];

			short result = -1;
			if (proc != nullptr)
				result = proc->priority;

			util::hwlock_release(); // End critical section
			return result;
		}

		thrd_ptr_t proc_ptr_t::spawn_thread(void (*entry)(int argc, char* argv[]))
		{
			util::hwlock_acquire(); // Begin critical section

			process_t* proc = proc_table[this->id];

			thrd_ptr_t val = ID_INVALID;
			if (proc != nullptr)
			{
				id_t nid = proc->thread_counter ++;

				thrd_ptr_t nthread = create_thread(this->id, entry);
				nthread.set_lid(nid);
				proc->threads.add(nid, nthread);

				scheduler_schedule(nthread);

				val = nid;
			}

			util::hwlock_release(); // End critical section
			return val;
		}

		thrd_ptr_t create_thread(proc_ptr_t parent_proc, void (*entry)(int argc, char* argv[]))
		{
			util::hwlock_acquire(); // Begin critical section

			id_t nid = thread_counter ++;
			thread_t nthread;
			nthread.id = nid;
			nthread.lid = ID_INVALID;
			nthread.proc = parent_proc;
			nthread.state = thread_state::NEW;
			nthread.entry = (size_t)entry;

			size_t nstack = (size_t)new uint8_t[THREAD_STACK_SIZE];
			nthread.stack = (size_t)((size_t)nstack + THREAD_STACK_SIZE);
			nthread.stack_block = nstack;

			thread_table.add(nid, nthread);

			util::hwlock_release(); // End critical section
			return nid;
		}

		int proc_ptr_t::destroy_thread(thrd_ptr_t thread)
		{
			util::hwlock_acquire(); // Begin critical section

			process_t* proc = proc_table[this->id];

			int err = 1;
			if (proc != nullptr)
			{
				proc->threads.remove(thread.get_lid());
				thread_table.remove(thread);

				if (proc->threads.size() <= 0)
					this->kill();

				err = 0;
			}

			util::hwlock_release(); // End critical section
			return err;
		}

		id_t proc_ptr_t::create_fd(vfs::inode_ptr_t inode, id_t lid)
		{
			util::hwlock_acquire(); // Begin critical section

			process_t* proc = proc_table[this->id];

			vfs::fd_ptr_t val = ID_INVALID;
			if (proc != nullptr)
			{
				id_t nid = lid; // TODO : Overwrite previous!
				if (nid == ID_INVALID)
					nid = proc->lfd_counter++;
				else
					proc->lfd_counter = util::max(proc->lfd_counter, nid + 1); // HACK : Make sure subsequent fd ids come AFTER this lid to avoid conflicts!

				vfs::fd_ptr_t nfd = vfs::create_fd(inode);
				proc->fds.add(nid, nfd);

				val = nid;
			}

			util::hwlock_release(); // End critical section
			return val;
		}

		int proc_ptr_t::delete_fd(id_t lfd)
		{
			util::hwlock_acquire(); // Begin critical section

			process_t* proc = proc_table[this->id];

			int err = 1;
			if (proc != nullptr)
			{
				bool removed = proc->fds.remove(lfd);

				if (removed)
					err = 0;
			}

			util::hwlock_release(); // End critical section
			return err;
		}

		int proc_ptr_t::kill()
		{
			util::hwlock_acquire(); // Begin critical section

			int err = 1;
			if (this->id == kproc) // Don't kill the kernel!
				err = 0;
			else if (proc_table[this->id] != nullptr)
			{
				proc_table.remove(this->id);
				err = 0;
			}

			util::hwlock_release(); // End critical section
			return err;
		}
	}
}
