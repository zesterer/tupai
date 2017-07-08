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
#include <tupai/sys/proc.hpp>
#include <tupai/util/str.hpp>
#include <tupai/util/hwlock.hpp>

#include <tupai/util/out.hpp>

namespace tupai
{
	namespace sys
	{
		static util::hashtable_t<proc_t*> procs;
		static id_t proc_counter = 0;

		static util::hwlock_t hwlock;
		static volatile id_t cproc;

		void proc_init()
		{
			hwlock.lock(); // Begin critical section

			cproc = proc_create("kernel", vfs::vfs_get_root());

			hwlock.unlock(); // End critical section
		}

		id_t proc_get_current()
		{
			hwlock.lock(); // Begin critical section

			id_t val = cproc;

			hwlock.unlock(); // End critical section

			return val;
		}

		const char* proc_get_name(id_t pid)
		{
			hwlock.lock(); // Begin critical section

			proc_t* proc = *procs[pid];

			const char* val = "null";
			if (proc != nullptr)
				val = proc->name;

			hwlock.unlock(); // End critical section
			return val;
		}

		vfs::fd_ptr_t proc_get_fd(id_t pid, id_t lfd)
		{
			hwlock.lock(); // Begin critical section

			proc_t* proc = *procs[pid];

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

		id_t proc_create(const char* name, vfs::inode_ptr_t dir)
		{
			hwlock.lock(); // Begin critical section

			proc_t* nproc = new proc_t();
			nproc->id = ++proc_counter;
			util::str_cpy_n(name, nproc->name);
			nproc->dir = dir;

			procs.add(nproc->id, nproc);

			hwlock.unlock(); // End critical section
			return nproc->id;
		}

		id_t proc_create_fd(id_t pid, vfs::inode_ptr_t inode)
		{
			hwlock.lock(); // Begin critical section

			proc_t* proc = *procs[pid];

			id_t val = -1;
			if (proc != nullptr)
			{
				id_t nid = proc->desc_counter++;

				vfs::fd_ptr_t nfd = vfs::vfs_create_fd(inode);
				proc->fds.add(nid, nfd);

				val = nid;
			}

			hwlock.unlock(); // Endcritical section
			return val;
		}

		int proc_remove_fd(id_t pid, id_t lfd)
		{
			hwlock.lock(); // Begin critical section

			proc_t* proc = *procs[pid];

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
	}
}
