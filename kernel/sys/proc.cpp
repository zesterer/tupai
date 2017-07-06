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
#include <tupai/fs/vfs.hpp>
#include <tupai/util/str.hpp>

#include <tupai/util/out.hpp>

namespace tupai
{
	namespace sys
	{
		static util::hashtable_t<id_t, proc_t*> procs;
		static id_t proc_counter = 0;

		static id_t cproc;

		void proc_init()
		{
			procs = util::hashtable_t<id_t, proc_t*>();
			cproc = proc_create("kernel", fs::vfs_get_root());
		}

		id_t proc_get_current()
		{
			return cproc;
		}

		const char* proc_get_name(id_t pid)
		{
			proc_t* proc = procs[pid];

			if (proc == nullptr)
				return "null";
			else
				return proc->name;
		}

		fs::desc_t* proc_get_desc(id_t pid, id_t desc)
		{
			proc_t* proc = procs[pid];

			if (proc == nullptr)
				return nullptr;
			else
				return proc->descs[desc];
		}

		id_t proc_create(const char* name, id_t dir)
		{
			proc_t* nproc = new proc_t();
			nproc->id = ++proc_counter;
			util::str_cpy_n(name, nproc->name);
			nproc->dir = dir;

			return nproc->id;
		}

		id_t proc_create_desc(id_t pid, fs::inode_t* inode)
		{
			proc_t* proc = procs[pid];

			fs::desc_t* ndesc = new fs::desc_t();
			ndesc->id = ++proc->desc_counter;
			ndesc->inode = inode->gid;
			proc->descs.add(ndesc->id, ndesc);

			return ndesc->id;
		}
	}
}
