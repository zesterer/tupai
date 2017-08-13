//
// file : table.hpp
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

#ifndef TUPAI_UTIL_TABLE_HPP
#define TUPAI_UTIL_TABLE_HPP

// Tupai
#include <tupai/util/hashtable.hpp>
#include <tupai/util/spinlock.hpp>

namespace tupai
{
	namespace util
	{
		template <typename T, typename Lock>
		struct table_t
		{
			hashtable_t<T> hashtable;
			Lock lock;

			size_t size()
			{
				this->lock.lock(); // Begin critical section
				size_t val = this->hashtable.size();
				this->lock.unlock(); // End critical section
				return val;
			}

			bool contains(id_t key)
			{
				this->lock.lock(); // Begin critical section
				bool val = this->hashtable.contains(key);
				this->lock.unlock(); // End critical section
				return val;
			}

			bool add(id_t key, T item)
			{
				this->lock.lock(); // Begin critical section
				bool val = this->hashtable.add(key, item);
				this->lock.unlock(); // End critical section
				return val;
			}

			bool remove(id_t key)
			{
				this->lock.lock(); // Begin critical section
				bool val = this->hashtable.remove(key);
				this->lock.unlock(); // End critical section
				return val;
			}

			T& operator[](id_t key)
			{
				this->lock.lock(); // Begin critical section
				T& val = *this->hashtable.get(key);
				this->lock.unlock(); // End critical section
				return val;
			}

			T& nth(size_t i)
			{
				this->lock.lock(); // Begin critical section
				T& val = *this->hashtable.nth(i);
				this->lock.unlock(); // End critical section
				return val;
			}

			id_t nth_key(size_t i)
			{
				this->lock.lock(); // Begin critical section
				id_t val = this->hashtable.nth_key(i);
				this->lock.unlock(); // End critical section
				return val;
			}
		};
	}
}

#endif
