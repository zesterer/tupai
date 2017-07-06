//
// file : proc.hpp
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

#ifndef TUPAI_SYS_PROC_HPP
#define TUPAI_SYS_PROC_HPP

// Tupai
#include <tupai/fs/inode.hpp>
#include <tupai/fs/desc.hpp>
#include <tupai/util/hashtable.hpp>

// Standard
#include <stddef.h>
#include <stdint.h>

namespace tupai
{
	namespace sys
	{
		const id_t INVALID_PROC_ID = -2;
		const id_t NO_PROC_ID      = -1;
		const id_t KERNEL_PROC_ID  = 0;

		enum class proc_state
		{
			ACTIVE,
			SUSPENDED,
			DEAD,
		};

		enum class thread_state
		{
			NEW,
			ACTIVE,
			WAITING,
			DEAD,
		};

		struct proc_t
		{
			const static size_t NAME_MAX = 256;

			struct thread_t
			{
				const static size_t NAME_MAX = 256;

				id_t id;
				char name[NAME_MAX];
				thread_state state;

				void* entry;
				void* stack;
			};

			id_t id;             // Process ID
			char name[NAME_MAX]; // Process name
			proc_state state;    // Process state
			id_t dir;            // Current directory

			id_t thread_counter = 0;
			util::hashtable_t<id_t, thread_t*> threads; // Process threads

			id_t desc_counter = 0;
			util::hashtable_t<id_t, fs::desc_t*> descs; // File descriptors

			~proc_t()
			{
				for (size_t i = 0; i < this->threads.size(); i ++)
					delete this->threads.nth(i);

				for (size_t i = 0; i < this->descs.size(); i ++)
					delete this->descs.nth(i);
			}
		};

		void proc_init();

		id_t        proc_get_current();
		const char* proc_get_name(id_t pid);
		fs::desc_t* proc_get_desc(id_t pid, id_t desc);

		id_t proc_create(const char* name, id_t dir);
		id_t proc_create_desc(id_t pid, fs::inode_t* inode);
		int  proc_close_desc(id_t pid, id_t desc);
	}
}

#endif
