//
// file : scheduler.hpp
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
