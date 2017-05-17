//
// file : thread.hpp
//
// This file is part of Tupai.
//
// Tupai is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
//
// Tupai is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY// without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with Tupai.  If not, see <http://www.gnu.org/licenses/>.
//

#ifndef TUPAI_SYS_THREAD_HPP
#define TUPAI_SYS_THREAD_HPP

// Standard
#include <stddef.h>
#include <stdint.h>

namespace tupai
{
	namespace sys
	{
		typedef long id_t;

		struct thread_t
		{
			enum class state
			{
				UNSPAWNED,
				ACTIVE,
				WAITING,
				DEAD,
			};

			volatile id_t id = -1;
			volatile state cstate = state::DEAD;

			volatile size_t entry = 0x0;
			volatile size_t stack = 0x0;
		};

		void threading_init();
		bool threading_enabled();

		id_t   thread_create(void(*entry)(), bool create_stack = true);
		id_t   thread_get_id();
		void   thread_kill(id_t id);
		size_t thread_next_stack(size_t ostack);

		template <typename R=void, R(*F)()>
		void thread_spawner()
		{
			return F();
			//thread_kill(thread_get_id());
		}
	}
}

#endif
