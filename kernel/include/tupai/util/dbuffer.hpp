//
// file : dbuffer.hpp
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

#ifndef TUPAI_UTIL_DBUFFER_HPP
#define TUPAI_UTIL_DBUFFER_HPP

// Tupai
#include <tupai/util/spinlock.hpp>
#include <tupai/util/mem.hpp>

// Standard
#include <stddef.h>
#include <stdint.h>

namespace tupai
{
	namespace util
	{
		template <typename T>
		struct dbuffer_t
		{
		private:

			T* _data = nullptr;
			size_t _size = 0;
			spinlock_t _mutex;

			void _resize(size_t n)
			{
				this->_data = (T*)(new uint8_t[n * sizeof(T)]);
				this->_size = n;
			}

			dbuffer_t<T>& _copy(dbuffer_t<T>& other)
			{
				this->_mutex.lock(); // Begin critical section
				other._mutex.lock(); // Begin critical section

				this->_size = other._size;

				if (this->_size > 0)
					this->_data = (T*)(new uint8_t[this->_size * sizeof(T)]);
				else
					this->_data = nullptr;

				for (size_t i = 0; i < this->_size; i ++)
					this->_data[i] = other._data[i];

				other._mutex.unlock(); // End critical section
				this->_mutex.unlock(); // End critical section

				return *this;
			}

			dbuffer_t<T>& _move(dbuffer_t<T>& other)
			{
				this->_mutex.lock(); // Begin critical section
				other._mutex.lock(); // Begin critical section

				this->_data = other._data;
				this->_size = other._size;

				other._data = nullptr;
				other._size = 0;

				other._mutex.unlock(); // End critical section
				this->_mutex.unlock(); // End critical section

				return *this;
			}

		public:

			// Default constructor
			dbuffer_t()
			{
				this->_data = nullptr;
				this->_size = 0;
			}

			// Copy constructor
			dbuffer_t(dbuffer_t<T>& other)
			{
				this->_copy(other);
			}

			// Copy assignment operator
			dbuffer_t<T>& operator=(dbuffer_t<T>& other)
			{
				return this->_copy(other);
			}

			// Move constructor
			dbuffer_t(dbuffer_t<T>&& other)
			{
				this->_move(other);
			}

			// Move assignment operator
			dbuffer_t<T>& operator=(dbuffer_t<T>&& other) noexcept
			{
				return this->_move(other);
			}

			// Destructor
			~dbuffer_t() noexcept
			{
				for (size_t i = 0; i < this->_size; i ++)
					this->_data[i].~T();

				if (this->_data != nullptr)
					delete (uint8_t*)this->_data;
			}

			void write(const T* add, size_t n, size_t pos)
			{
				T* odata = this->_data;
				size_t osize = this->_size;
				this->_resize(this->_size + n);

				if (odata == nullptr)
					util::mem_copy(add, this->_data, n);
				else
				{
					if (pos > 0 && osize > 0)
						util::mem_copy(odata, this->_data, pos);
					if (n > 0)
						util::mem_copy(add, &this->_data[pos], n);
					if (osize > pos)
						util::mem_copy(&odata[pos], &this->_data[pos + n], osize - pos);
					delete (uint8_t*)odata;
				}
			}

			const T* data()
			{
				return this->_data;
			}

			size_t size()
			{
				return this->_size;
			}
		};
	}
}

#endif
