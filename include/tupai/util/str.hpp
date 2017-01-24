/*
* 	file : str.hpp
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

#ifndef TUPAI_UTIL_STR_HPP
#define TUPAI_UTIL_STR_HPP

// Tupai
#include <tupai/type.hpp>
#include <tupai/util/safetype.hpp>
#include <tupai/kdebug.hpp>

#include <libk/string.hpp>

namespace tupai
{
	namespace util
	{
		template <typename T, umem N>
		struct str
		{
		private:
			T internal[N];
			umem length;

		public:
			str()
			{
				this->length = N - 1;
				for (umem i = 0; i < this->len(); i ++)
					this->raw_mut()[i] = ' ';
				this->raw_mut()[this->len()] = '\0';
			}

			str(T c)
			{
				this->length = 1;
				this->raw_mut()[0] = c;
				this->raw_mut()[this->len()] = '\0';
			}

			str(const T(&data) [N])
			{
				umem i;
				for (i = 0; data[i] != '\0'; i ++)
					this->raw_mut()[i] = data[i];
				this->length = i;
				this->raw_mut()[this->len()] = '\0';
			}

			str(const str<T, N>& other)
			{
				for (umem i = 0; i < other.len(); i ++)
					this->raw_mut()[i] = other.at(i);
				this->length = other.len();
				this->raw_mut()[this->len()] = '\0';
			}

			T& operator[](umem index)
			{
				return this->internal[index % this->len()];
			}

			T at(umem index) const
			{
				return this->internal[index % this->len()];
			}

			umem len() const
			{
				return this->length;
			}

			umem size() const
			{
				return N;
			}

			const T* raw() const
			{
				return this->internal;
			}

			T* raw_mut()
			{
				return this->internal;
			}

			template<umem Nother>
			str<T, N + Nother - 1> add(const str<T, Nother>& other) const
			{
				T buffer[N + Nother - 1];
				umem i = 0;
				for (umem j = 0; j < this->len(); j ++)
				{
					buffer[i] = this->raw()[j];
					i ++;
				}
				for (umem j = 0; j < other.len(); j ++)
				{
					buffer[i] = other.at(j);
					i ++;
				}
				buffer[i] = '\0';

				return str<T, N + Nother - 1>(buffer);
			}

			template<umem Nother>
			str<T, N + Nother - 1> operator+(const str<T, Nother>& other) const
			{
				return this->add(other);
			}

			str<T, N + 1> operator+(T c) const
			{
				return this->add(str<T, 2>(c));
			}

			template<umem Nother>
			bool equals(const str<T, Nother>& other) const
			{
				if (other.len() != this->len())
					return false;

				for (umem i = 0; i < this->len(); i ++)
				{
					if (this.at(i) != other[i])
						return false;
				}

				return true;
			}

			template<umem Nother>
			bool operator==(const str<T, Nother>& other) const
			{
				return this->equals(other);
			}

			bool equals(const T* data) const
			{
				umem i;
				for (i = 0; data[i] != '\0' && i < this->len(); i ++)
				{
					if (this.at(i) != data[i])
						return false;
				}

				return (i == this->len());
			}

			bool operator==(const T* data) const
			{
				return this->equals(data);
			}
		};

		#define makestr(_str) (tupai::util::str<char, sizeof(_str) / sizeof(char)>(_str))
		#define makewstr(_str) (tupai::util::str<wchar, sizeof(_str) / sizeof(wchar)>(_str))
	}
}

#endif
