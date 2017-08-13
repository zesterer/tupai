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
// but WITHOUT ANY WARRANTY without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with Tupai.  If not, see <http://www.gnu.org/licenses/>.
//

#ifndef TUPAI_TASK_THREAD_HPP
#define TUPAI_TASK_THREAD_HPP

// Tupai
#include <tupai/task/task.hpp>

// Standard
#include <stddef.h>
#include <stdint.h>

namespace tupai
{
	namespace task
	{
		// TODO : Do something more with this
		static const size_t THREAD_STACK_SIZE = 8192;

		struct thread_t
		{
			id_t id = ID_INVALID;
			id_t lid = ID_INVALID;
			proc_ptr_t proc;
			thread_state state;

			size_t entry = 0x0;
			size_t stack = 0x0;
			size_t stack_block = 0x0;

			thread_t& _copy(const thread_t& other)
			{
				this->id = other.id;
				this->lid = other.lid;
				this->proc = other.proc;
				this->state = other.state;

				this->entry = other.entry;

				this->stack_block = (size_t)(new uint8_t[THREAD_STACK_SIZE]);
				this->stack = this->stack_block + THREAD_STACK_SIZE;

				for (size_t i = 0; i < THREAD_STACK_SIZE; i ++)
					((uint8_t*)this->stack_block)[i] = ((uint8_t*)other.stack_block)[i];

				return *this;
			}

			thread_t& _move(thread_t& other)
			{
				this->id = other.id;
				this->lid = other.lid;
				this->proc = other.proc;
				this->state = other.state;

				this->entry = other.entry;

				this->stack = other.stack;
				this->stack_block = other.stack_block;

				other.id = ID_INVALID;
				other.lid = ID_INVALID;
				other.proc = ID_INVALID;
				other.state = thread_state::DEAD;

				other.entry = 0;
				other.stack = 0;
				other.stack_block = 0;

				return *this;
			}

			// Default constructor
			thread_t()
			{
				this->id = ID_INVALID;
				this->lid = ID_INVALID;
				this->proc = ID_INVALID;
				this->state = thread_state::DEAD;

				this->entry = 0x0;
				this->stack = 0x0;
				this->stack_block = 0x0;
			}

			// Copy constructor
			thread_t(const thread_t& other)
			{
				this->_copy(other);
			}

			// Copy assignment operator
			thread_t& operator=(const thread_t& other)
			{
				return this->_copy(other);
			}

			// Move constructor
			thread_t(thread_t&& other)
			{
				this->_move(other);
			}

			// Move assignment operator
			thread_t& operator=(thread_t&& other) noexcept
			{
				return this->_move(other);
			}

			// Destructor
			~thread_t();
		};
	}
}

#endif
