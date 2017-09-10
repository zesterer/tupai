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

		// template <typename K, typename T>
		// class _hashtable_t
		// {
		// private:
		// 	struct _node_t
		// 	{
		// 		K    _key;
		// 		T    _item;
		// 		bool _valid = false;
		//
		// 		_node_t() {}
		// 		_node_t(K key, const T& item) : _key(key), _item(item) { this->valid = true; }
		// 	};
		//
		// 	_node_t* _items = nullptr;
		// 	size_t   _cap   = 0;
		// 	size_t   _size  = 0;
		//
		// 	void _resize(size_t ncap)
		// 	{
		// 		// Preserve the old table
		// 		_node_t* oitems = this->_items;
		// 		size_t   ocap   = this->_cap;
		//
		// 		// Create new table
		// 		this->_items = (_node_t*)(new uint8_t[sizeof(_node_t) * ncap]);
		// 		this->_cap   = ncap;
		//
		// 		// Invalidate new table
		// 		for (size_t i = 0; i < this->_cap; i ++)
		// 			this->_items[i]._valid = false;
		//
		// 		// Place old items in new table
		// 		for (size_t i = 0; i < ocap; i ++)
		// 		{
		// 			if (oitems[i]._valid)
		// 			{
		// 				size_t h = hash(oitems[i]._key);
		// 				for (size_t offset = 0; offset < this->_cap; offset ++)
		// 				{
		// 					size_t j = (h + offset) % this->_cap;
		// 					if (!this->_items[j]._valid)
		// 					{
		// 						mem_copy(&oitems[i], &this->_items[j], sizeof(_node_t));
		// 						break;
		// 					}
		// 				}
		// 			}
		// 		}
		//
		// 		// Free old table (but do not deconstruct!)
		// 		if (oitems != nullptr)
		// 			delete (uint8_t*)oitems;
		// 	}
		//
		// 	_hashtable_t& _copy(const _hashtable_t& other)
		// 	{
		// 		this->_cap   = other._cap;
		// 		this->_size  = other._size;
		//
		// 		this->_items = (_node_t*)(new uint8_t[sizeof(_node_t) * this->_cap]);
		// 		for (size_t i = 0; i < this->_cap; i ++)
		// 			this->_items[i] = other._items[i];
		//
		// 		return *this;
		// 	}
		//
		// 	_hashtable_t& _move(_hashtable_t& other)
		// 	{
		// 		this->_items = other._items;
		// 		this->_cap   = other._cap;
		// 		this->_size  = other._size;
		//
		// 		other._items = nullptr;
		// 		other._cap   = 0;
		// 		other._size  = 0;
		//
		// 		return *this;
		// 	}
		//
		// public:
		// 	_hashtable_t() {}
		//
		// 	_hashtable_t(const _hashtable_t& other)
		// 	{
		// 		this->_copy(other);
		// 	}
		//
		// 	_hashtable_t& operator=(const _hashtable_t& other)
		// 	{
		// 		return this->_copy(other);
		// 	}
		//
		// 	_hashtable_t(_hashtable_t&& other)
		// 	{
		// 		this->_move(other);
		// 	}
		//
		// 	_hashtable_t& operator=(_hashtable_t&& other)
		// 	{
		// 		return this->_move(other);
		// 	}
		//
		// 	~_hashtable_t()
		// 	{
		// 		if (this->_items != nullptr)
		// 		{
		// 			// Deconstruct items
		// 			for (size_t i = 0; i < this->_cap; i ++)
		// 			{
		// 				if (this->_items[i]._valid)
		// 					this->_items[i]._item.~T();
		// 			}
		//
		// 			delete (uint8_t*)this->_items;
		// 		}
		// 	}
		//
		// 	bool add(K key, T item)
		// 	{
		// 		bool val = false;
		//
		// 		if (this->_size >= this->_cap)
		// 			this->_resize(this->_cap == 0 ? 1 : (this->_cap * 2));
		//
		// 		size_t h = hash(key);
		// 		for (size_t offset = 0; offset < this->_cap; offset ++)
		// 		{
		// 			size_t i = (h + offset) % this->_cap;
		// 			if (!this->_items[i]._valid)
		// 			{
		// 				this->_items[i]._key   = key;
		// 				this->_items[i]._item  = item;
		// 				this->_items[i]._valid = true;
		// 				this->_size ++;
		// 				val = true;
		// 				break;
		// 			}
		// 		}
		//
		// 		return val;
		// 	}
		//
		// 	bool remove(K key)
		// 	{
		// 		bool val = false;
		//
		// 		size_t h = hash(key);
		// 		for (size_t offset = 0; offset < this->_cap; offset ++)
		// 		{
		// 			size_t i = (h + offset) % this->_cap;
		// 			if (this->_items[i]._valid && this->_items[i]._key == key)
		// 			{
		// 				this->_items[i]._item.~T();
		// 				this->_items[i]._valid = false;
		// 				this->_size --;
		// 				val = true;
		// 				break;
		// 			}
		// 		}
		//
		// 		if (this->_size < this->_cap / 2 && this->_cap > 1)
		// 			this->_resize(this->_cap / 2);
		//
		// 		return val;
		// 	}
		//
		// 	T* get(K key)
		// 	{
		// 		size_t h = hash(key);
		// 		for (size_t offset = 0; offset < this->_cap; offset ++)
		// 		{
		// 			size_t i = (h + offset) % this->_cap;
		// 			if (this->_items[i]._valid && this->_items[i]._key == key)
		// 				return &this->_items[i]._item;
		// 		}
		//
		// 		return nullptr;
		// 	}
		//
		// 	T* operator[](id_t key)
		// 	{
		// 		return this->get(key);
		// 	}
		//
		// 	size_t size() { return this->_size; }
		//
		// 	T* nth(size_t n)
		// 	{
		// 		size_t j = 0;
		// 		for (size_t i = 0; i < this->_cap; i ++)
		// 		{
		// 			if (this->_items[i]._valid)
		// 			{
		// 				if (j == n)
		// 					return &this->_items[i]._item;
		// 				j ++;
		// 			}
		// 		}
		//
		// 		return nullptr;
		// 	}
		// };
		//
		//template <typename T>
		//using hashtable_t = _hashtable_t<id_t, T>;

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

			bool contains(id_t key)
			{
				size_t offset = hash(key) % HASHTABLE_CAPACITY;
				for (size_t i = 0; i < HASHTABLE_CAPACITY; i ++)
				{
					size_t index = (offset + i) % HASHTABLE_CAPACITY;

					if (this->keys[index] == key && this->used[index] == true)
						return true;
				}
				return false;
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
