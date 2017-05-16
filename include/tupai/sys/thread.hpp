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

			id_t id;
			state cstate = state::DEAD;

			void* entry = 0x0;
			void* stack = 0x0;
		};

		void threading_init();
		bool threading_enabled();

		id_t  thread_create(void(*addr)(), bool create_stack = true);
		id_t  thread_get_id();
		void  thread_assign(id_t id);
		void  thread_update(id_t id, void* stack);
		void* thread_next_stack();
	}
}

#endif
