/*
* 	file : string.hpp
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

#ifndef TUPAI_UTIL_STRING_HPP
#define TUPAI_UTIL_STRING_HPP

// Tupai
#include <tupai/type.hpp>

#include <tupai/util/mem.hpp>
#include <tupai/util/math.hpp>
#include <tupai/util/vector.hpp>

namespace tupai
{
	namespace util
	{
		template <typename T>
		struct gen_string
		{
		private:
			T* data = nullptr;
			umem len = 0;

			void free()
			{
				if (this->data != nullptr)
					dealloc(this->data);
			}

			void set(const gen_string<T>& other)
			{
				// Free anything we have now
				this->free();

				// Allocate new space
				this->data = alloc<T>(other.length() + 1).val();

				// Copy string
				for (umem i = 0; i < other.length() + 1; i ++)
					this->data[i] = other.raw()[i];

				// Set length
				this->len = other.length();
			}

			void set(const T* dat)
			{
				// Free anything we have now
				this->free();

				// Get length
				for (this->len = 0; dat[this->len] != '\0'; this->len ++);

				// Allocate new space
				this->data = alloc<T>(this->len + 1).val();

				// Copy string
				for (umem i = 0; i < this->len; i ++)
					this->data[i] = dat[i];
				this->data[this->len] = '\0';
			}

			void set(const T* dat, umem start, umem n = ~(umem)0)
			{
				// Free anything we have now
				this->free();

				// Get length
				for (this->len = 0; dat[this->len] != '\0'; this->len ++);
				this->len -= start;
				this->len = min(this->len, n);

				// Allocate new space
				this->data = alloc<T>(this->len + 1).val();

				// Copy string
				for (umem i = 0; i < this->len; i ++)
					this->data[i] = dat[start + i];
				this->data[this->len] = '\0';
			}

			void set(const T* dat0, const T* dat1)
			{
				// Free anything we have now
				this->free();

				// Get length
				this->len = 0;
				for (umem i = 0; dat0[i] != '\0'; i ++)
					this->len ++;
				for (umem i = 0; dat1[i] != '\0'; i ++)
					this->len ++;

				// Allocate new space
				this->data = alloc<T>(this->len + 1).val();

				// Copy string
				umem j = 0;
				for (umem i = 0; dat0[i] != '\0'; i ++)
				{
					this->data[j] = dat0[i];
					j ++;
				}
				for (umem i = 0; dat1[i] != '\0'; i ++)
				{
					this->data[j] = dat1[i];
					j ++;
				}
				this->data[j] = '\0';
			}

		public:
			gen_string()
			{
				this->set("\0");
			}

			gen_string(const gen_string<T>& other)
			{
				this->set(other);
			}

			gen_string(const T* dat)
			{
				this->set(dat);
			}

			gen_string(const T* dat0, const T* dat1)
			{
				this->set(dat0, dat1);
			}

			gen_string(const T* dat, umem start, umem n = ~(umem)0)
			{
				this->set(dat, start, n);
			}

			~gen_string()
			{
				this->free();
			}

			void append(const gen_string<T>& other)
			{
				// Allocate new space
				umem nlen = this->len + other.length();
				char* ndata = alloc<T>(nlen + 1).val();

				// Copy string
				for (umem i = 0; i < this->len; i ++)
					ndata[i] = this->data[i];
				for (umem i = 0; i < other.length(); i ++)
					ndata[i + this->len] = other.raw()[i];
				ndata[nlen] = '\0';

				// Free original string
				this->free();

				// Replace string
				this->data = ndata;
				this->len = nlen;

			}

			void operator=(const gen_string<T>& other)
			{
				this->set(other);
			}

			void operator=(const T* dat)
			{
				this->set(dat);
			}

			void operator+=(const gen_string<T>& other)
			{
				return this->append(other);
			}

			gen_string<T> add(const gen_string<T>& other) const
			{
				return gen_string<T>(this->data, other.raw());
			}

			gen_string<T> operator+(const gen_string<T>& other) const
			{
				return this->add(other);
			}

			T& operator[](umem i)
			{
				return this->data[i];
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

			bool equals(const gen_string<T>& other) const
			{
				// TODO : Rewrite this properly
				//if (other.length() != this->len)
				//	return false;

				for (umem i = 0; i < util::max(this->len, other.length()) + 1; i ++)
				{
					if (this->data[i] != other.raw()[i])
						return false;
				}

				return true;
			}

			bool operator==(const gen_string<T>& other) const
			{
				return this->equals(other);
			}

			gen_string<T> substr(umem start, umem n = ~(umem)0)
			{
				return gen_string<T>(this->raw(), start, n);
			}

			operator const char*()
			{
				return this->data;
			}

			vector<gen_string<T>> split(T c = ' ')
			{
				vector<gen_string<T>> vec;

				umem start = 0;
				for (umem i = start; i < this->len + 1; i ++)
				{
					if (this->data[i] == c || this->data[i] == '\0')
					{
						vec.push(gen_string<T>(this->data, start, i - start));
						start = i + 1;

						if (this->data[i] == '\0')
							break;
					}
				}

				return vec;
			}
		};

		template <typename T>
		gen_string<T> operator+(const T* arr, gen_string<T> str)
		{
			return gen_string<T>(arr) + str;
		}

		typedef gen_string<char> string;
	}
}

#endif
