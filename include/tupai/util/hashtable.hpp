//
// file : hashtable.hpp
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

#ifndef TUPAI_UTIL_HASHTABLE_HPP
#define TUPAI_UTIL_HASHTABLE_HPP

// Tupai
#include <tupai/type.hpp>
#include <tupai/util/mem.hpp>

// Standard
#include <stddef.h>
#include <stdint.h>

namespace tupai
{
	namespace util
	{
		static size_t hash(id_t x) { return x ^ 0xF37E2A92F37E2A92; }

		const size_t HASHTABLE_CAPACITY = 256;

		template <typename T>
		struct hashtable_t
		{
			uint8_t data[HASHTABLE_CAPACITY * sizeof(T)];
			bool    used[HASHTABLE_CAPACITY];
			id_t    keys[HASHTABLE_CAPACITY];
			size_t  item_count = 0;

			size_t size()
			{
				return this->item_count;
			}

			bool add(id_t key, T item)
			{
				size_t offset = hash(key) % HASHTABLE_CAPACITY;
				for (size_t i = 0; i < HASHTABLE_CAPACITY; i ++)
				{
					size_t index = (offset + i) % HASHTABLE_CAPACITY;

					if (!this->used[index])
					{
						this->used[index]        = true;
						this->keys[index]        = key;
						((T*)this->data)[index]  = item;
						this->item_count ++;
						return true;
					}
				}
				return false;
			}

			bool remove(id_t key)
			{
				size_t offset = hash(key) % HASHTABLE_CAPACITY;
				for (size_t i = 0; i < HASHTABLE_CAPACITY; i ++)
				{
					size_t index = (offset + i) % HASHTABLE_CAPACITY;

					if (this->keys[index] == key && this->used[index] == true)
					{
						this->used[index] = false;
						((T*)this->data)[index].~T();
						this->item_count --;
						return true;
					}
				}
				return false;
			}

			T* get(id_t key)
			{
				size_t offset = hash(key) % HASHTABLE_CAPACITY;
				for (size_t i = 0; i < HASHTABLE_CAPACITY; i ++)
				{
					size_t index = (offset + i) % HASHTABLE_CAPACITY;

					if (this->keys[index] == key && this->used[index] == true)
						return &((T*)this->data)[index];
				}
				return nullptr;
			}

			T* operator[](id_t key)
			{
				return this->get(key);
			}

			T* nth(size_t index)
			{
				size_t count = 0;
				for (size_t i = 0; i < HASHTABLE_CAPACITY; i ++)
				{
					if (this->used[i])
					{
						if (count == index)
							return &((T*)this->data)[i];
						else
							count ++;
					}
				}
				return nullptr;
			}
		};
	}
}

#endif
