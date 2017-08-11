//
// file : vector.hpp
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

#ifndef TUPAI_UTIL_VECTOR_HPP
#define TUPAI_UTIL_VECTOR_HPP

// Tupai
#include <tupai/util/math.hpp>
#include <tupai/util/mem.hpp>
#include <tupai/util/cpp.hpp>
#include <tupai/panic.hpp>

// Standard
#include <stddef.h>
#include <stdint.h>

namespace tupai
{
	namespace util
	{
		template <typename T>
		struct vector_t
		{
			T* _array;
			size_t _capacity;
			size_t _size;

			vector_t<T>& _copy(const vector_t<T>& other)
			{
				this->_size = other._size;
				this->_capacity = other._capacity;

				if (this->_size > 0)
					this->_array = (T*)(new uint8_t[this->_capacity]);
				else
					this->_array = nullptr;

				for (size_t i = 0; i < this->_size; i ++)
					this->_array[i] = other._array[i];

				return *this;
			}

			vector_t<T>& _move(vector_t<T>& other)
			{
				this->_array    = other._array;
				this->_capacity = other._capacity;
				this->_size     = other._size;

				other._array    = nullptr;
				other._capacity = 0;
				other._size     = 0;

				return *this;
			}

			// Default constructor
			vector_t()
			{
				this->_array    = nullptr;
				this->_capacity = 0;
				this->_size     = 0;
			}

			// Copy constructor
			vector_t(const vector_t<T>& other)
			{
				this->_copy(other);
			}

			// Copy assignment operator
			vector_t<T>& operator=(const vector_t<T>& other)
			{
				return this->_copy(other);
			}

			// Move constructor
			vector_t(vector_t<T>&& other)
			{
				this->_move(other);
			}

			// Move assignment operator
			vector_t<T>& operator=(vector_t<T>&& other) noexcept
			{
				return this->_move(other);
			}

			// Destructor
			~vector_t() noexcept
			{
				for (size_t i = 0; i < this->_size; i ++)
					this->_array[i].~T();

				if (_array != nullptr)
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

			void resize(size_t size)
			{
				T* old = this->_array;
				this->_capacity = size;
				this->_array = (T*)(new uint8_t[this->_capacity * sizeof(T)]);

				for (size_t i = 0; i < this->_size; i ++)
					this->_array[i] = old[i];

				if (old != nullptr)
					delete old;
			}

			T& get(size_t i)
			{
				return this->_array[i];
			}

			T& operator[](size_t i)
			{
				return this->_array[i];
			}

			void push(const T& item)
			{
				if (this->_size >= this->_capacity)
					this->resize(util::max(this->_capacity * 2, (size_t)1));

				this->_array[this->_size] = item;
				this->_size ++;
			}

			T pop()
			{
				if (this->_size <= 0)
					panic("Attempted to pop from empty vector");

				T item = this->_array[this->_size - 1];
				this->_size --;

				if (this->_size <= this->_capacity / 2)
					this->resize(this->_capacity / 2);

				return item;
			}
		};
	}
}

#endif
