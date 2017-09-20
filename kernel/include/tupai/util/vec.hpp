//
// file : vec.hpp
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

#ifndef TUPAI_UTIL_VEC_HPP
#define TUPAI_UTIL_VEC_HPP

// Tupai
#include <tupai/util/mem.hpp>
#include <tupai/util/cpp.hpp>
#include <tupai/util/math.hpp>
#include <tupai/util/mem.hpp>
#include <tupai/util/spinlock.hpp>
#include <tupai/panic.hpp>

// Standard
#include <stddef.h>
#include <stdint.h>

namespace tupai
{
	namespace util
	{
		template <typename T>
		class Vec
		{
		private:

			T* _array;
			size_t _capacity;
			size_t _size;

			void _resize(size_t size)
			{
				T* old = this->_array;
				this->_capacity = size;
				this->_array = static_cast<T*>(mem_alloc(sizeof(T) * this->_capacity));

				for (size_t i = 0; i < this->_size; i ++)
					this->_array[i] = old[i];

				if (old != nullptr)
					mem_dealloc(old);
			}

			Vec<T>& _copy(Vec<T>& other)
			{
				this->_size = other._size;
				this->_capacity = other._capacity;

				if (this->_size > 0)
					this->_array = static_cast<T*>(mem_alloc(sizeof(T) * this->_capacity));
				else
					this->_array = nullptr;

				for (size_t i = 0; i < this->_size; i ++)
					this->_array[i] = other._array[i];

				return *this;
			}

			Vec<T>& _move(Vec<T>& other)
			{
				this->_array    = other._array;
				this->_capacity = other._capacity;
				this->_size     = other._size;

				other._array    = nullptr;
				other._capacity = 0;
				other._size     = 0;

				return *this;
			}

		public:

			// Default constructor
			Vec()
			{
				this->_array    = nullptr;
				this->_capacity = 0;
				this->_size     = 0;
			}

			// Copy constructor
			Vec(Vec<T>& other)
			{
				this->_copy(other);
			}

			// Copy assignment operator
			Vec<T>& operator=(Vec<T>& other)
			{
				return this->_copy(other);
			}

			// Move constructor
			Vec(Vec<T>&& other)
			{
				this->_move(other);
			}

			// Move assignment operator
			Vec<T>& operator=(Vec<T>&& other) noexcept
			{
				return this->_move(other);
			}

			// Destructor
			~Vec() noexcept
			{
				for (size_t i = 0; i < this->_size; i ++)
					this->_array[i].~T();

				if (this->_array != nullptr)
					mem_dealloc(this->_array);
			}

			size_t size() const
			{
				return this->_size;
			}

			size_t capacity() const
			{
				return this->_capacity;
			}

			T& get(size_t i)
			{
				return this->_array[i];
			}

			T& operator[](size_t i)
			{
				return this->_array[i];
			}

			void push(T item)
			{
				if (this->_size >= this->_capacity)
					this->_resize(util::max(this->_capacity * 2, (size_t)1));

				new ((void*)&this->_array[this->_size]) T(item); // Placement new
				this->_size ++;
			}

			T pop()
			{
				if (this->_size <= 0)
					panic("Attempted to pop from empty vector");

				T item = this->_array[this->_size - 1];
				this->_array[this->_size - 1].~T(); // Manually invoke deconstructor
				this->_size --;

				if (this->_size <= this->_capacity / 2)
					this->_resize(this->_capacity / 2);

				return item;
			}
		};
	}
}

#endif