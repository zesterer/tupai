//
// file : process.hpp
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

#ifndef TUPAI_TASK_PROCESS_HPP
#define TUPAI_TASK_PROCESS_HPP

// Tupai
#include <tupai/task/task.hpp>
#include <tupai/vfs/vfs.hpp>

// Standard
#include <stddef.h>
#include <stdint.h>

namespace tupai
{
	namespace task
	{
		struct process_t
		{
			id_t id;                  // Process ID
			char name[PROC_NAME_MAX]; // Process name
			process_state state;         // Process state
			vfs::inode_ptr_t dir;     // Current directory

			id_t thread_counter = 0;
			util::hashtable_t<thrd_ptr_t> threads; // Process threads

			id_t lfd_counter = 0;
			util::hashtable_t<vfs::fd_ptr_t> fds; // File descriptors

			~process_t();
		};
	}
}

#endif
