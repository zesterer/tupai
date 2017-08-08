//
// file : task.hpp
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

#ifndef TUPAI_TASK_PROC_HPP
#define TUPAI_TASK_PROC_HPP

// Tupai
#include <tupai/vfs/vfs.hpp>
#include <tupai/util/hashtable.hpp>

// Standard
#include <stddef.h>
#include <stdint.h>

namespace tupai
{
	namespace task
	{
		const id_t INVALID_PROC_ID = -2;
		const id_t NO_PROC_ID      = -1;
		const id_t KERNEL_PROC_ID  = 0;

		const size_t PROC_NAME_MAX = 256;

		enum class process_state
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

		struct proc_ptr_t; // Forward declaration

		struct thrd_ptr_t
		{
			id_t id;

			thrd_ptr_t() {}
			thrd_ptr_t(id_t id) { this->id = id; }
			operator id_t() { return this->id; }

			id_t get_lid();
			thread_state get_state();
			proc_ptr_t get_process();
			size_t get_entry();
			size_t get_stack();
			void set_lid(id_t lid);
			void set_state(thread_state state);
			void set_stack(size_t stack);
			int kill();
		};

		struct proc_ptr_t
		{
			id_t id;

			proc_ptr_t() {}
			proc_ptr_t(id_t id) { this->id = id; }
			operator id_t() { return this->id; }

			template <size_t SIZE> int get_name(char(&buff)[SIZE]) { return this->get_name(buff, SIZE); }

			int           get_name(char* rbuff, size_t n);
			vfs::fd_ptr_t get_fd(id_t lfd);

			thrd_ptr_t spawn_thread(void (*entry)(int argc, char* argv[]));
			int          destroy_thread(thrd_ptr_t);
			id_t         create_fd(vfs::inode_ptr_t inode);
			int          delete_fd(id_t lfd);

			int kill();
		};

		proc_ptr_t   get_kernel();
		proc_ptr_t   get_current();
		thrd_ptr_t get_current_thread();
		void         set_current(proc_ptr_t proc);
		void         set_current_thread(thrd_ptr_t thread);

		void       init();
		proc_ptr_t create_process(const char* name, vfs::inode_ptr_t dir);
		void       display();
	}
}

#endif
