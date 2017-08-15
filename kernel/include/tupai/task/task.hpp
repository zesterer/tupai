//
// file : task.hpp
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

#ifndef TUPAI_TASK_PROC_HPP
#define TUPAI_TASK_PROC_HPP

// Tupai
#include <tupai/vfs/vfs.hpp>
#include <tupai/util/hashtable.hpp>
#include <tupai/util/result.hpp>

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
			util::result<short> get_priority();
			util::result<short> get_effective_priority();
			
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

			int get_name(char* rbuff, size_t n);
			vfs::fd_ptr_t get_fd(id_t lfd);
			util::result<short> get_priority();

			thrd_ptr_t spawn_thread(void (*entry)(int argc, char* argv[]));
			int destroy_thread(thrd_ptr_t);
			id_t create_fd(vfs::inode_ptr_t inode, id_t lid = ID_INVALID);
			int delete_fd(id_t lfd);

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
