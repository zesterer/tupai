//
// file : process.hpp
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
			process_state state;      // Process state
			vfs::inode_ptr_t dir;     // Current directory

			short priority = 0;

			id_t thread_counter = 0;
			util::hashtable_t<thrd_ptr_t> threads; // Process threads

			id_t lfd_counter = 0;
			util::hashtable_t<vfs::fd_ptr_t> fds; // File descriptors

			process_t& _move(process_t& other)
			{
				this->id = other.id;
				util::str_cpy_n(other.name, this->name, PROC_NAME_MAX);
				this->state = other.state;
				this->dir = other.dir;
				this->thread_counter = other.thread_counter;
				this->threads = util::move(other.threads);
				this->lfd_counter = other.lfd_counter;
				this->fds = util::move(other.fds);
				
				other.id = ID_INVALID;
				other.state = process_state::DEAD;
				other.dir = ID_INVALID;
				other.thread_counter = 0;
				other.lfd_counter = 0;
				
				return *this;
			}

			process_t& _copy(const process_t& other)
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

			// Default constructor
			process_t() {}

			// Copy constructor
			process_t(const process_t& other)
			{
				this->_copy(other);
			}

			// Copy assignment operator
			process_t& operator=(const process_t& other)
			{
				return this->_copy(other);
			}

			// Move constructor
			process_t(process_t&& other)
			{
				this->_move(other);
			}

			// Move assignment operator
			process_t& operator=(process_t&& other)
			{
				return this->_move(other);
			}

			~process_t();
		};
	}
}

#endif
