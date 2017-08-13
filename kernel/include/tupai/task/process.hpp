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
#include <tupai/util/str.hpp>
#include <tupai/util/mem.hpp>

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

			/*
			process_t() {}

			process_t(const process_t& other)
			{
				this->id = other.id;
				util::str_cpy_n(other.name, this->name, PROC_NAME_MAX);
				this->state = other.state;
				this->dir = other.dir;
				this->thread_counter = other.thread_counter;
				this->threads = other.threads;
				this->lfd_counter = other.lfd_counter;
				this->fds = other.fds;
			}

			process_t& operator=(const process_t& other)
			{
				this->id = other.id;
				util::str_cpy_n(other.name, this->name, PROC_NAME_MAX);
				this->state = other.state;
				this->dir = other.dir;
				this->thread_counter = other.thread_counter;
				this->threads = other.threads;
				this->lfd_counter = other.lfd_counter;
				this->fds = other.fds;

				return *this;
			}

			process_t(process_t&& other)
			{
				this->id = other.id;
				util::str_cpy_n(other.name, this->name, PROC_NAME_MAX);
				this->state = other.state;
				this->dir = other.dir;
				this->thread_counter = other.thread_counter;
				this->threads = util::move(other.threads);
				this->lfd_counter = other.lfd_counter;
				this->fds = util::move(other.fds);
			}

			process_t& operator=(process_t&& other)
			{
				this->id = other.id;
				util::str_cpy_n(other.name, this->name, PROC_NAME_MAX);
				this->state = other.state;
				this->dir = other.dir;
				this->thread_counter = other.thread_counter;
				this->threads = util::move(other.threads);
				this->lfd_counter = other.lfd_counter;
				this->fds = util::move(other.fds);

				return *this;
			}
			*/

			~process_t();
		};
	}
}

#endif
