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
#include <tupai/proc/proc.hpp>
#include <tupai/proc/thread.hpp>
#include <tupai/proc/process.hpp>
#include <tupai/proc/scheduler.hpp>
#include <tupai/util/str.hpp>
#include <tupai/util/hwlock.hpp>

#include <tupai/util/log.hpp>

namespace tupai
{
	namespace proc
	{
		static util::hashtable_t<proc_t> proc_table;
		static id_t proc_counter = 0;

		static util::hashtable_t<thread_t> thread_table;
		static id_t thread_counter = 0;

		static util::hwlock_t hwlock;

		static proc_ptr_t   cproc = ID_INVALID;   // The current running process
		static proc_ptr_t   kproc = ID_INVALID;   // The kernel process
		static thread_ptr_t cthread = ID_INVALID; // The current running thread

		// TODO : Do something more with this
		static const size_t THREAD_STACK_SIZE = 2048;

		thread_ptr_t create_thread(proc_ptr_t proc, void (*entry)(int argc, char* argv[]));

		/* Process control functions */

		proc_ptr_t get_kernel()
		{
			hwlock.lock(); // Begin critical section

			proc_ptr_t val = kproc;

			hwlock.unlock(); // End critical section

			return val;
		}

		proc_ptr_t get_current()
		{
			hwlock.lock(); // Begin critical section

			proc_ptr_t val = cproc;

			hwlock.unlock(); // End critical section

			return val;
		}

		thread_ptr_t get_current_thread()
		{
			hwlock.lock(); // Begin critical section

			thread_ptr_t val = cthread;

			hwlock.unlock(); // End critical section

			return val;
		}

		void set_current(proc_ptr_t proc)
		{
			hwlock.lock(); // Begin critical section

			cproc = proc;

			hwlock.unlock(); // End critical section
		}

		void set_current_thread(thread_ptr_t thread)
		{
			hwlock.lock(); // Begin critical section

			cthread = thread;
			cproc = thread.get_process();

			hwlock.unlock(); // End critical section
		}

		void init()
		{
			hwlock.lock(); // Begin critical section

			// Create the initial kernel process
			kproc = create("kernel", vfs::get_root());
			cproc = kproc;

			hwlock.unlock(); // End critical section
		}

		proc_ptr_t create(const char* name, vfs::inode_ptr_t dir)
		{
			hwlock.lock(); // Begin critical section

			proc_t nproc;
			nproc.id = ++proc_counter;
			util::str_cpy_n(name, nproc.name);
			nproc.dir = dir;

			proc_table.add(nproc.id, nproc);

			hwlock.unlock(); // End critical section
			return nproc.id;
		}

		void display()
		{
			for (size_t i = 0; i < proc_table.size(); i ++)
			{
				proc_t* proc = proc_table.nth(i);
				util::logln((long)i, " -> ", proc->name, " (", (long)proc->id, ")");

				for (size_t j = 0; j < proc->threads.size(); j ++)
					util::logln("    ", (long)j, " -> Thread ", (long)(*proc->threads.nth(j)).get_lid());
			}
		}

		/* Thread functions */

		id_t thread_ptr_t::get_lid()
		{
			hwlock.lock(); // Begin critical section

			thread_t* thread = thread_table[this->id];

			id_t val = ID_INVALID;
			if (thread != nullptr)
				val = thread->lid;

			hwlock.unlock(); // End critical section
			return val;
		}

		thread_state thread_ptr_t::get_state()
		{
			hwlock.lock(); // Begin critical section

			thread_t* thread = thread_table[this->id];

			thread_state val = thread_state::DEAD;
			if (thread != nullptr)
				val = thread->state;

			hwlock.unlock(); // End critical section
			return val;
		}

		proc_ptr_t thread_ptr_t::get_process()
		{
			hwlock.lock(); // Begin critical section

			thread_t* thread = thread_table[this->id];

			proc_ptr_t val = ID_INVALID;
			if (thread != nullptr)
				val = thread->proc;

			hwlock.unlock(); // End critical section
			return val;
		}

		size_t thread_ptr_t::get_entry()
		{
			hwlock.lock(); // Begin critical section

			thread_t* thread = thread_table[this->id];

			size_t val = 0;
			if (thread != nullptr)
				val = thread->entry;

			hwlock.unlock(); // End critical section
			return val;
		}

		size_t thread_ptr_t::get_stack()
		{
			hwlock.lock(); // Begin critical section

			thread_t* thread = thread_table[this->id];

			size_t val = 0;
			if (thread != nullptr)
				val = thread->stack;

			hwlock.unlock(); // End critical section
			return val;
		}

		void thread_ptr_t::set_lid(id_t lid)
		{
			hwlock.lock(); // Begin critical section

			thread_t* thread = thread_table[this->id];

			if (thread != nullptr)
				thread->lid = lid;

			hwlock.unlock(); // End critical section
		}

		void thread_ptr_t::set_state(thread_state state)
		{
			hwlock.lock(); // Begin critical section

			thread_t* thread = thread_table[this->id];

			if (thread != nullptr)
				thread->state = state;

			hwlock.unlock(); // End critical section
		}

		void thread_ptr_t::set_stack(size_t stack)
		{
			hwlock.lock(); // Begin critical section

			thread_t* thread = thread_table[this->id];

			if (thread != nullptr)
				thread->stack = stack;

			hwlock.unlock(); // End critical section
		}

		int thread_ptr_t::kill()
		{
			this->set_state(thread_state::DEAD);

			return 0;
		}

		/* Process functions */

		int proc_ptr_t::get_name(char* rbuff, size_t n)
		{
			hwlock.lock(); // Begin critical section

			proc_t* proc = proc_table[this->id];

			int err = 1;
			if (proc != nullptr)
			{
				util::str_cpy_n(proc->name, rbuff, n);
				err = 0;
			}

			hwlock.unlock(); // End critical section
			return err;
		}

		vfs::fd_ptr_t proc_ptr_t::get_fd(id_t lfd)
		{
			hwlock.lock(); // Begin critical section

			proc_t* proc = proc_table[this->id];

			vfs::fd_ptr_t val = ID_INVALID;
			if (proc != nullptr && lfd != ID_INVALID)
			{
				vfs::fd_ptr_t* ptr = proc->fds[lfd];
				if (ptr != nullptr)
					val = *ptr;
			}

			hwlock.unlock(); // End critical section
			return val;
		}

		thread_ptr_t proc_ptr_t::spawn_thread(void (*entry)(int argc, char* argv[]))
		{
			hwlock.lock(); // Begin critical section

			proc_t* proc = proc_table[this->id];

			thread_ptr_t val = ID_INVALID;
			if (proc != nullptr)
			{
				id_t nid = proc->thread_counter ++;

				thread_ptr_t nthread = create_thread(this->id, entry);
				nthread.set_lid(nid);
				proc->threads.add(nid, nthread);

				scheduler_schedule(nthread);

				val = nid;
			}

			hwlock.unlock(); // End critical section
			return val;
		}

		int proc_ptr_t::delete_thread(thread_ptr_t thread)
		{
			hwlock.lock(); // Begin critical section

			proc_t* proc = proc_table[this->id];

			int err = 1;
			if (proc != nullptr)
			{
				proc->threads.remove(thread.get_lid());
				thread_table.remove(thread);
				err = 0;
			}

			hwlock.unlock(); // End critical section
			return err;
		}

		thread_ptr_t create_thread(proc_ptr_t parent_proc, void (*entry)(int argc, char* argv[]))
		{
			hwlock.lock(); // Begin critical section

			id_t nid = thread_counter ++;
			thread_t nthread;
			nthread.id = nid;
			nthread.lid = ID_INVALID;
			nthread.proc = parent_proc;
			nthread.state = thread_state::NEW;
			nthread.entry = (size_t)entry;

			// TODO : Allocate thread stack!
			size_t nstack = (size_t)new uint8_t[THREAD_STACK_SIZE];
			nthread.stack = (size_t)((size_t)nstack + THREAD_STACK_SIZE);
			nthread.stack_block = nstack;

			thread_table.add(nid, nthread);

			hwlock.unlock(); // End critical section
			return nid;
		}

		id_t proc_ptr_t::create_fd(vfs::inode_ptr_t inode)
		{
			hwlock.lock(); // Begin critical section

			proc_t* proc = proc_table[this->id];

			vfs::fd_ptr_t val = ID_INVALID;
			if (proc != nullptr)
			{
				id_t nid = proc->lfd_counter++;

				vfs::fd_ptr_t nfd = vfs::create_fd(inode);
				proc->fds.add(nid, nfd);

				val = nid;
			}

			hwlock.unlock(); // End critical section
			return val;
		}

		int proc_ptr_t::delete_fd(id_t lfd)
		{
			hwlock.lock(); // Begin critical section

			proc_t* proc = proc_table[this->id];

			int err = 1;
			if (proc != nullptr)
			{
				bool removed = proc->fds.remove(lfd);

				if (removed)
					err = 0;
			}

			hwlock.unlock(); // End critical section
			return err;
		}
	}
}
