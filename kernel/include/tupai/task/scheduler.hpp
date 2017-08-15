//
// file : scheduler.hpp
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

#ifndef TUPAI_TASK_SCHEDULER_HPP
#define TUPAI_TASK_SCHEDULER_HPP

// Tupai
#include <tupai/task/task.hpp>

// Standard
#include <stddef.h>
#include <stdint.h>

namespace tupai
{
	namespace task
	{
		void       scheduler_init();
		void       scheduler_schedule(thrd_ptr_t thread, int priority = 0);
		void       scheduler_increment();
		thrd_ptr_t scheduler_current();
		size_t     scheduler_preempt(size_t old_stack);
	}
}

#endif
