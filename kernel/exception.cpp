//
// file : exception.cpp
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
#include <tupai/exception.hpp>
#include <tupai/sys/call.hpp>
#include <tupai/util/log.hpp>
#include <tupai/task/task.hpp>
#include <tupai/task/scheduler.hpp>
#include <tupai/arch.hpp>
#include <tupai/cpu.hpp>
#include <tupai/irq.hpp>
#include <tupai/panic.hpp>

namespace tupai
{
	size_t exception_handle(size_t stack, size_t code, size_t error)
	{
		bool critical = cpu::get_exception_critical(code);
		const char* except_name = cpu::get_exception_name(code);

		task::thrd_ptr_t cthread = task::get_current_thread();
		char proc_name[task::PROC_NAME_MAX];
		cthread.get_process().get_name(proc_name);

		if (critical)
		{
			panic(
				"Exception in thread '", (long)cthread,   // Thread ID
				"' of process '", proc_name,              // Process name
				"' : ", except_name,                      // Exception name
				" [", util::fmt_int<long>(error, 16), ']' // Error code
			);
		}
		else
		{
			util::logln(
				"Exception in thread '", (long)cthread,   // Thread ID
				"' of process '", proc_name,              // Process name
				"' : ", except_name,                      // Exception name
				" [", util::fmt_int<long>(error, 16), ']' // Error code
			);

			#if defined(DEBUG_ENABLED)
				arch_display_reg_state((arch_reg_state*)stack);
			#endif

			cthread.kill();
		}

		return task::scheduler_preempt(stack);
	}
}
