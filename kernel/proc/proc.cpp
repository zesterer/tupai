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
#include <tupai/util/str.hpp>
#include <tupai/util/hwlock.hpp>

namespace tupai
{
	namespace proc
	{
		static util::hashtable_t<proc_t> proc_table;
		static id_t proc_counter = 0;

		static util::hashtable_t<thread_t> thread_table;
		static id_t thread_counter = 0;

		static util::hwlock_t hwlock;

		static proc_ptr_t   cproc = ID_INVALID;
		static thread_ptr_t cthreads = ID_INVALID;

		thread_ptr_t proc_create_thread(proc_ptr_t proc, void (*entry)(int argc, char* argv[]));

		/* Process control functions */

		proc_ptr_t proc_get_current()
		{
			hwlock.lock(); // Begin critical section

			proc_ptr_t val = cproc;

			hwlock.unlock(); // End critical section

			return val;
		}

		void proc_init()
		{
			hwlock.lock(); // Begin critical section

			// Create the initial kernel process
			cproc = proc_create("kernel", vfs::vfs_get_root());

			hwlock.unlock(); // End critical section
		}

		proc_ptr_t proc_create(const char* name, vfs::inode_ptr_t dir)
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

				thread_ptr_t nthread = proc_create_thread(this->id, entry);
				proc->threads.add(nid, nthread);

				val = nid;
			}

			hwlock.unlock(); // End critical section
			return val;
		}

		thread_ptr_t proc_create_thread(proc_ptr_t parent_proc, void (*entry)(int argc, char* argv[]))
		{
			hwlock.lock(); // Begin critical section

			id_t nid = thread_counter ++;
			thread_t nthread;
			nthread.id = nid;
			nthread.proc = parent_proc;
			nthread.state = thread_state::NEW;
			nthread.entry = (void*)entry;

			// TODO : Allocate thread stack!
			nthread.stack = nullptr;
			nthread.stack_block = nullptr;

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

				vfs::fd_ptr_t nfd = vfs::vfs_create_fd(inode);
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

		/*
		vfs::fd_ptr_t proc_get_fd(proc_ptr_t proc_ptr, id_t lfd)
		{
			hwlock.lock(); // Begin critical section

			proc_t* proc = proc_table[proc_ptr];

			vfs::fd_ptr_t val = ID_INVALID;
			if (proc != nullptr)
			{
				vfs::fd_ptr_t* fd = proc->fds[lfd];

				if (fd != nullptr)
					val = *fd;
			}

			hwlock.unlock(); // End critical section
			return val;
		}

		id_t proc_create_fd(proc_ptr_t proc_ptr, vfs::inode_ptr_t inode)
		{
			hwlock.lock(); // Begin critical section

			proc_t* proc = proc_table[proc_ptr];

			id_t val = -1;
			if (proc != nullptr)
			{
				id_t nid = proc->lfd_counter++;

				vfs::fd_ptr_t nfd = vfs::vfs_create_fd(inode);
				proc->fds.add(nid, nfd);

				val = nid;
			}

			hwlock.unlock(); // Endcritical section
			return val;
		}

		int proc_remove_fd(proc_ptr_t proc_ptr, id_t lfd)
		{
			hwlock.lock(); // Begin critical section

			proc_t* proc = proc_table[proc_ptr];

			int val = 1;
			if (proc != nullptr)
			{
				bool removed = proc->fds.remove(lfd);

				if (removed)
					val = 0;
			}

			hwlock.unlock(); // End critical section
			return val;
		}
		*/
	}
}
