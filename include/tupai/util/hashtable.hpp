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

#include <tupai/util/out.hpp>

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

		/*
		template <typename T>
		struct hashtable_t
		{
			size_t item_count = 0;
			size_t capacity;
			bool  inited = false;

			bool* filled = nullptr;
			id_t* keys   = nullptr;
			T*    items  = nullptr;

			void clear_filled()
			{
				for (size_t i = 0; i < this->capacity; i ++)
					this->filled[i] = false;
			}

			bool _add(id_t key, T item)
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
				id_t* okeys      = this->keys;
				T*    oitems     = this->items;
				size_t ocapacity = this->capacity;

				this->filled   = (bool*)new uint8_t[n * sizeof(bool)];
				this->keys     = (id_t*)new uint8_t[n * sizeof(id_t)];
				this->items    = (T*)   new uint8_t[n * sizeof(T)];
				this->capacity = n;

				this->clear_filled();

				for (size_t i = 0; i < ocapacity; i ++)
				{
					if (ofilled[i] == true)
					{
						size_t offset = hash(okeys[i]) % this->capacity;
						for (size_t j = 0; j < this->capacity; j ++)
						{
							size_t index = (offset + j) % this->capacity;
							if (!this->filled[index])
							{
								this->filled[index] = true;
								this->keys[index]   = okeys[i];
								mem_copy(&oitems[i], &this->items[index], sizeof(T));
								break;
							}
						}
					}
				}

				if (this->filled != nullptr) delete (uint8_t*)this->filled;
				if (this->keys != nullptr)   delete (uint8_t*)this->keys;
				if (this->items != nullptr)  delete (uint8_t*)this->items;
			}

			void _init()
			{
				this->filled    = new bool[1];
				this->keys      = new id_t[1];
				this->items     = new T[1];
				this->capacity  = 1;

				this->clear_filled();

				this->inited = true;
			}

			hashtable_t(const hashtable_t<T>& other)
			{
				this->filled = (bool*)new uint8_t[other.capacity * sizeof(bool)];
				this->keys   = (id_t*)new uint8_t[other.capacity * sizeof(id_t)];
				this->items  = (T*)   new uint8_t[other.capacity * sizeof(T)];

				this->capacity   = other.capacity;
				this->item_count = other.item_count;

				this->clear_filled();
				for (size_t i = 0; i < other.capacity; i ++)
				{
					if (other.filled[i] == true)
					{
						this->filled[i] = true;
						this->keys[i]   = other.keys[i];
						this->items[i]  = other.items[i];
					}
				}

				this->inited = true;
			}

			hashtable_t()
			{
				this->_init();
			}

			~hashtable_t()
			{
				if (this->inited)
				{
					for (size_t i = 0; i < this->capacity; i ++)
					{
						if (this->filled[i] == true)
							this->items[i].~T();
					}

					if (this->filled != nullptr) delete (uint8_t*)this->filled;
					if (this->keys != nullptr)   delete (uint8_t*)this->keys;
					if (this->items != nullptr)  delete (uint8_t*)this->items;
				}
			}

			size_t size() const
			{
				return this->item_count;
			}

			bool add(id_t key, T item)
			{
				if (!this->inited)
					this->_init();

				if (this->item_count >= this->capacity)
					this->resize(this->capacity * 2);

				bool added = this->_add(key, item);

				if (added)
					this->item_count ++;

				return added;
			}

			bool remove(id_t key)
			{
				if (this->item_count == 0)
					return false;

				size_t offset = hash(key) % this->capacity;
				bool removed = false;
				for (size_t i = 0; i < this->capacity; i ++)
				{
					size_t index = (offset + i) % this->capacity;
					if (this->keys[index] == key)
					{
						this->filled[index] = false;
						this->keys[index].~id_t();
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

			T* get(id_t key)
			{
				if (this->item_count == 0)
					return nullptr;

				size_t offset = hash(key) % this->capacity;
				for (size_t i = 0; i < this->capacity; i ++)
				{
					size_t index = (offset + i) % this->capacity;
					if (this->filled[index] && this->keys[index] == key)
						return &this->items[index];
				}
				return nullptr;
			}

			T* operator[](size_t i)
			{
				return this->get(i);
			}

			T* nth(size_t n)
			{
				if (this->item_count == 0)
					return nullptr;

				size_t count = 0;
				for (size_t i = 0; i < this->capacity; i ++)
				{
					if (this->filled[i])
					{
						if (count >= n)
							return &this->items[i];
						else
							count ++;
					}
				}
				return nullptr;
			}

			id_t nth_key(size_t n) const
			{
				if (this->item_count == 0)
					return ID_INVALID;

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
				return ID_INVALID;
			}

			bool contains(id_t key) const
			{
				if (this->item_count == 0)
					return false;

				for (size_t i = 0; i < this->capacity; i ++)
				{
					if (this->filled[i] && this->keys[i] == key)
						return true;
				}
				return false;
			}
		};
		*/
	}
}

#endif
