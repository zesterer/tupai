//
// file : lock.hpp
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

#ifndef TUPAI_UTIL_LOCK_HPP
#define TUPAI_UTIL_LOCK_HPP

// Tupai
#include <tupai/task/task.hpp>

// Standard
#include <stddef.h>
#include <stdint.h>

namespace tupai
{
	namespace util
	{
		class lock_t;

		class key_t
		{
			friend class lock_t;
		private:
			volatile lock_t* _lock;

			key_t(volatile lock_t* lock) { this->_lock = lock; }

		public:
			void release() volatile;
			~key_t() { this->release(); }
		};

		class lock_t
		{
			friend class key_t;
		private:
			volatile size_t           _ref = 0;
			volatile task::thrd_ptr_t _owner = ID_INVALID;
			volatile size_t           _val = 0;

			void lock() volatile;
			void unlock() volatile;
			void force_unref() volatile;

		public:
			key_t acquire() volatile;
		};
	}
}

#endif
