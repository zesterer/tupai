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
#include <tupai/util/math.hpp>
#include <tupai/panic.hpp>

// Standard
#include <stddef.h>
#include <stdint.h>

namespace tupai
{
	namespace util
	{
		static size_t hash(id_t x) { return x; }

		template <typename Key, typename T>
		struct hashtable_t
		{
			size_t item_count = 0;
			size_t capacity;
			bool* filled = nullptr;
			Key*  keys   = nullptr;
			T*    items  = nullptr;

			void clear_filled()
			{
				for (size_t i = 0; i < this->capacity; i ++)
					this->filled[i] = false;
			}

			bool _add(Key key, T item)
			{
				size_t offset = hash(key) % this->capacity;
				bool added = false;
				for (size_t i = 0; i < this->capacity; i ++)
				{
					size_t index = (offset + i) % this->capacity;
					if (!this->filled[index])
					{
						this->filled[index] = true;
						this->keys[index]   = key;
						this->items[index]  = item;
						added = true;
						break;
					}
				}

				return added;
			}

			void resize(size_t n)
			{
				bool* ofilled    = this->filled;
				Key*  okeys      = this->keys;
				T*    oitems     = this->items;
				size_t ocapacity = this->capacity;

				this->filled   = new bool[n];
				this->keys     = new Key[n];
				this->items    = new T[n];
				this->capacity = n;

				this->clear_filled();

				for (size_t i = 0; i < ocapacity; i ++)
				{
					if (ofilled[i] == true)
						this->_add(okeys[i], oitems[i]);
				}

				delete ofilled;
				delete okeys;
				delete oitems;
			}

			hashtable_t()
			{
				this->filled   = new bool[1];
				this->keys     = new Key[1];
				this->items    = new T[1];
				this->capacity = 1;

				this->clear_filled();
			}

			size_t size() const
			{
				return this->item_count;
			}

			bool add(Key key, T item)
			{
				if (this->item_count >= this->capacity)
					this->resize(this->capacity * 2);

				bool added = this->_add(key, item);

				if (added)
					this->item_count ++;

				return added;
			}

			bool remove(Key key)
			{
				size_t offset = hash(key) % this->capacity;
				bool removed = false;
				for (size_t i = 0; i < this->capacity; i ++)
				{
					size_t index = (offset + i) % this->capacity;
					if (this->keys[index] == key)
					{
						this->filled[index] = false;
						this->keys[index].~Key();
						this->items[index].~T();
						removed = true;
						break;
					}
				}

				if (removed)
					this->item_count --;

				if (this->item_count <= this->capacity / 2 && this->capacity / 2 > 0)
					this->resize(this->capacity / 2);

				return removed;
			}

			T get(Key key) const
			{
				size_t offset = hash(key) % this->capacity;
				for (size_t i = 0; i < this->capacity; i ++)
				{
					size_t index = (offset + i) % this->capacity;
					if (this->filled[index] && this->keys[index] == key)
						return this->items[index];
				}
				return nullptr;
			}

			T operator[](size_t i) const
			{
				return this->get(i);
			}

			T nth(size_t n) const
			{
				size_t count = 0;
				for (size_t i = 0; i < this->capacity; i ++)
				{
					if (this->filled[i])
					{
						if (count >= n)
							return this->items[i];
						else
							count ++;
					}
				}
				return nullptr;
			}

			Key nth_key(size_t n) const
			{
				size_t count = 0;
				for (size_t i = 0; i < this->capacity; i ++)
				{
					if (this->filled[i])
					{
						if (count >= n)
							return this->keys[i];
						else
							count ++;
					}
				}
				return Key();
			}

			bool contains(Key key) const
			{
				for (size_t i = 0; i < this->capacity; i ++)
				{
					if (this->filled[i] && this->keys[i] == key)
						return true;
				}
				return false;
			}
		};
	}
}

#endif
