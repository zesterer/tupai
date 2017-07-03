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
#include <tupai/util/vector.hpp>

// Standard
#include <stddef.h>
#include <stdint.h>

namespace tupai
{
	namespace sys
	{
		typedef long pid_t;
		typedef long tid_t;

		const pid_t INVALID_PROC_ID = -2;
		const pid_t NO_PROC_ID      = -1;
		const pid_t KERNEL_PROC_ID  = 0;

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

				tid_t id;
				char name[NAME_MAX];
				thread_state state;

				void* entry;
				void* stack;
			};

			pid_t id;            // Process ID
			char name[NAME_MAX]; // Process name
			proc_state state;    // Process state
			fs::id_t dir;        // Current directory

			tid_t thread_counter = 0;
			util::vector_t<thread_t> threads; // Process threads

			fs::id_t desc_counter = 0;
			util::vector_t<fs::desc_t> desc; // File descriptors
		};

		void proc_init();
		const char* proc_get_name(pid_t id);
	}
}

#endif
