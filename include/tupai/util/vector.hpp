/*
* 	file : vector.hpp
*
* 	This file is part of Tupai.
*
* 	Tupai is free software: you can redistribute it and/or modify
* 	it under the terms of the GNU General Public License as published by
* 	the Free Software Foundation, either version 3 of the License, or
* 	(at your option) any later version.
*
* 	Tupai is distributed in the hope that it will be useful,
* 	but WITHOUT ANY WARRANTY; without even the implied warranty of
* 	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
* 	GNU General Public License for more details.
*
* 	You should have received a copy of the GNU General Public License
* 	along with Tupai.  If not, see <http://www.gnu.org/licenses/>.
*/

#ifndef TUPAI_UTIL_VECTOR_HPP
#define TUPAI_UTIL_VECTOR_HPP

// Tupai
#include <tupai/type.hpp>

#include <tupai/util/mem.hpp>
#include <tupai/util/math.hpp>

// --- TESTING ---
#include <tupai/util/out.hpp>

namespace tupai
{
	namespace util
	{
		template <typename T>
		struct vector
		{
		private:
			T* data = nullptr;
			umem len = 0;
			umem siz = 0;

			void free()
			{
				if (this->data != nullptr)
				{
					dealloc(this->data);
				}
			}

			void destruct()
			{
				if (this->data != nullptr)
				{
					for (umem i = 0; i < this->len; i ++)
						this->data[i].~T();

					this->free();
				}
			}

			void realloc(umem n)
			{
				// Allocate new space
				T* ndata = alloc<T>(n).val();

				// Copy the data across
				umem nlen = min(this->len, n);

				mem_copy((byte*)this->data, (byte*)ndata, sizeof(T) * nlen);

				//for (umem i = 0; i < nlen; i ++)
				//	ndata[i] = this->data[i];

				// Free the old data
				this->free();

				// Replace with new data
				this->data = ndata;
				this->len = nlen;
				this->siz = n;
			}

			void set()
			{
				// Free existing data
				this->destruct();

				// Allocate new data
				this->data = alloc<T>(1).val();
				this->siz = 1;
				this->len = 0;
			}

			void readjust()
			{
				if (this->len >= this->siz)
					realloc(this->siz * 2);
				else if (this->len <= this->siz / 2 - 1 && this->siz >= 2)
					realloc(this->siz / 2);
			}

		public:
			vector()
			{
				this->set();
			}

			~vector()
			{
				this->destruct();
			}

			void push(T item)
			{
				this->data[this->len] = item;
				this->len ++;

				this->readjust();
			}

			T pop()
			{
				T item = this->data[this->len - 1];
				this->len --;

				this->readjust();

				return item;
			}

			const T& getAt(umem i) const
			{
				return this->data[i];
			}

			T& getAtMut(umem i)
			{
				return this->data[i];
			}

			T& operator[](umem i)
			{
				return this->getAtMut(i);
			}

			umem length() const
			{
				return this->len;
			}

			const T* raw() const
			{
				return this->data;
			}

			T* raw_mut()
			{
				return this->data;
			}

			void empty()
			{
				this->set();
			}

			void operator=(const vector<T>& other)
			{
				this->empty();

				for (umem i = 0; i < other.length(); i ++)
					this->push(other.raw()[i]);
			}
		};
	}
}

#endif
