//
// file : hashtable.hpp
//
// Copyright (c) 2017 Joshua Barretto <joshua.s.barretto@gmail.com>
//
// This file is part of Tupai.
//
// Permission to use, copy, modify, and distribute this software for any
// purpose with or without fee is hereby granted, provided that the above
// copyright notice and this permission notice appear in all copies.
//
// THE SOFTWARE IS PROVIDED "AS IS" AND THE AUTHOR DISCLAIMS ALL WARRANTIES
// WITH REGARD TO THIS SOFTWARE INCLUDING ALL IMPLIED WARRANTIES OF
// MERCHANTABILITY AND FITNESS. IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR
// ANY SPECIAL, DIRECT, INDIRECT, OR CONSEQUENTIAL DAMAGES OR ANY DAMAGES
// WHATSOEVER RESULTING FROM LOSS OF USE, DATA OR PROFITS, WHETHER IN AN
// ACTION OF CONTRACT, NEGLIGENCE OR OTHER TORTIOUS ACTION, ARISING OUT OF
// OR IN CONNECTION WITH THE USE OR PERFORMANCE OF THIS SOFTWARE.
//

#ifndef TUPAI_UTIL_HASHTABLE_HPP
#define TUPAI_UTIL_HASHTABLE_HPP

// Tupai
#include <tupai/type.hpp>
#include <tupai/util/cpp.hpp>
#include <tupai/util/mem.hpp>
#include <tupai/panic.hpp>
#include <tupai/util/log.hpp>

// Standard
#include <stddef.h>
#include <stdint.h>

namespace tupai
{
	namespace util
	{
		static __attribute__((unused)) size_t hash(id_t x) { return x; } //^ 0xF37E2A92F37E2A92; }

		const size_t HASHTABLE_CAPACITY = 512;

		template <typename T>
		struct hashtable_t
		{
			uint8_t data[HASHTABLE_CAPACITY * sizeof(T)];
			//T*      data = nullptr;
			bool    used[HASHTABLE_CAPACITY];
			id_t    keys[HASHTABLE_CAPACITY];
			size_t  item_count = 0;

			hashtable_t()
			{
				for (size_t i = 0; i < HASHTABLE_CAPACITY; i ++)
					this->used[i] = false;

				//this->data = (T*)(new uint8_t[HASHTABLE_CAPACITY * sizeof(T)]);
			}

			/*
			hashtable_t(const hashtable_t& other)
			{
				this->data = (T*)(new uint8_t[HASHTABLE_CAPACITY * sizeof(T)]);

				util::mem_copy(other.used, this->used, sizeof(this->used));
				util::mem_copy(other.keys, this->keys, sizeof(this->keys));
				this->item_count = other.item_count;

				for (size_t i = 0; i < HASHTABLE_CAPACITY; i ++)
				{
					if (this->used[i])
						this->data[i] = other.data[i];
				}
			}

			hashtable_t& operator=(const hashtable_t& other)
			{
				this->data = (T*)(new uint8_t[HASHTABLE_CAPACITY * sizeof(T)]);

				util::mem_copy(other.used, this->used, sizeof(this->used));
				util::mem_copy(other.keys, this->keys, sizeof(this->keys));
				this->item_count = other.item_count;

				for (size_t i = 0; i < HASHTABLE_CAPACITY; i ++)
				{
					if (this->used[i])
						this->data[i] = other.data[i];
				}

				return *this;
			}

			hashtable_t(hashtable_t&& other)
			{
				this->data = other.data;
				util::mem_copy(other.used, this->used, sizeof(this->used));
				util::mem_copy(other.keys, this->keys, sizeof(this->keys));
				this->item_count = other.item_count;

				other.data = nullptr;
			}

			hashtable_t& operator=(hashtable_t&& other)
			{
				this->data = other.data;
				util::mem_copy(other.used, this->used, sizeof(this->used));
				util::mem_copy(other.keys, this->keys, sizeof(this->keys));
				this->item_count = other.item_count;

				other.data = nullptr;

				return *this;
			}

			~hashtable_t()
			{
				if (this->data != nullptr)
					delete[] this->data;
			}
			*/

			size_t size()
			{
				return this->item_count;
			}

			bool add(id_t key, T& item)
			{
				size_t offset = hash(key) % HASHTABLE_CAPACITY;
				for (size_t i = 0; i < HASHTABLE_CAPACITY; i ++)
				{
					size_t index = (offset + i) % HASHTABLE_CAPACITY;

					if (!this->used[index])
					{
						this->used[index]  = true;
						this->keys[index]  = key;
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
						((T*)this->data)[index].~T();
						this->keys[index] = ID_INVALID;
						this->used[index] = false;
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

			id_t nth_key(size_t index)
			{
				size_t count = 0;
				for (size_t i = 0; i < HASHTABLE_CAPACITY; i ++)
				{
					if (this->used[i])
					{
						if (count == index)
							return this->keys[i];
						else
							count ++;
					}
				}

				return ID_INVALID;
			}
		};
	}
}

#endif
