//
// file : vector.hpp
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

#ifndef TUPAI_UTIL_VECTOR_HPP
#define TUPAI_UTIL_VECTOR_HPP

// Tupai
#include <tupai/util/math.hpp>
#include <tupai/util/mem.hpp>
#include <tupai/util/cpp.hpp>
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
		struct Vector
		{
		private:

			T* _array;
			size_t _capacity;
			size_t _size;
			spinlock_t _mutex;

			void _resize(size_t size)
			{
				T* old = this->_array;
				this->_capacity = size;
				this->_array = (T*)(new uint8_t[this->_capacity * sizeof(T)]);

				for (size_t i = 0; i < this->_size; i ++)
					this->_array[i] = old[i];

				if (old != nullptr)
					delete (uint8_t*)old;
			}

			Vector<T>& _copy(Vector<T>& other)
			{
				this->_mutex.lock(); // Begin critical section
				other._mutex.lock(); // Begin critical section

				this->_size = other._size;
				this->_capacity = other._capacity;

				if (this->_size > 0)
					this->_array = (T*)(new uint8_t[this->_capacity * sizeof(T)]);
				else
					this->_array = nullptr;

				for (size_t i = 0; i < this->_size; i ++)
					this->_array[i] = other._array[i];

				other._mutex.unlock(); // End critical section
				this->_mutex.unlock(); // End critical section

				return *this;
			}

			Vector<T>& _move(Vector<T>& other)
			{
				this->_mutex.lock(); // Begin critical section
				other._mutex.lock(); // Begin critical section

				this->_array    = other._array;
				this->_capacity = other._capacity;
				this->_size     = other._size;

				other._array    = nullptr;
				other._capacity = 0;
				other._size     = 0;

				other._mutex.unlock(); // End critical section
				this->_mutex.unlock(); // End critical section

				return *this;
			}

		public:

			// Default constructor
			Vector()
			{
				this->_array    = nullptr;
				this->_capacity = 0;
				this->_size     = 0;
			}

			// Copy constructor
			Vector(Vector<T>& other)
			{
				this->_copy(other);
			}

			// Copy assignment operator
			Vector<T>& operator=(Vector<T>& other)
			{
				return this->_copy(other);
			}

			// Move constructor
			Vector(Vector<T>&& other)
			{
				this->_move(other);
			}

			// Move assignment operator
			Vector<T>& operator=(Vector<T>&& other) noexcept
			{
				return this->_move(other);
			}

			// Destructor
			~Vector() noexcept
			{
				for (size_t i = 0; i < this->_size; i ++)
					this->_array[i].~T();

				if (this->_array != nullptr)
					delete (uint8_t*)this->_array;
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

			void push(T& item)
			{
				this->_mutex.lock(); // Begin critical section

				if (this->_size >= this->_capacity)
					this->_resize(util::max(this->_capacity * 2, (size_t)1));

				this->_array[this->_size] = item;
				this->_size ++;

				this->_mutex.unlock(); // End critical section
			}

			T pop()
			{
				this->_mutex.lock(); // Begin critical section

				if (this->_size <= 0)
					panic("Attempted to pop from empty vector");

				T item = this->_array[this->_size - 1];
				this->_size --;

				if (this->_size <= this->_capacity / 2)
					this->_resize(this->_capacity / 2);

				this->_mutex.unlock(); // End critical section

				return item;
			}
		};

		template <typename T>
		using vector_t = Vector<T>;
	}
}

#endif
