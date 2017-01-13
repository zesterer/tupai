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
			this->internal[N - 1] = '\0';
		}

		str(const T(&data) [N])
		{
			this->length = N - 1;
			libk::memcpy(this->internal, data, (N - 1) * sizeof(T));
			this->internal[N - 1] = '\0';
		}

		str(const str<T, N>& other)
		{
			this->length = N - 1;
			libk::memcpy(this->internal, other.raw(), (N - 1) * sizeof(T));
			this->internal[N - 1] = '\0';
		}

		T& operator[](umem index)
		{
			return this->internal[index % this->length];
		}

		umem len() const
		{
			return this->length;
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
		str<T, N + Nother - 1> add(const str<T, Nother>& other)
		{
			str<T, N + Nother - 1> nstr = str<T, N + Nother - 1>();
			libk::memcpy(nstr.raw_mut(), this->raw(), this->len() * sizeof(T));
			libk::memcpy(nstr.raw_mut() + this->len() * sizeof(T), other.raw(), Nother * sizeof(T));
			nstr.raw_mut()[this->len() + other.len()] = '\0';
			return nstr;
		}

		template<umem Nother>
		str<T, N + Nother - 1> operator+(const str<T, Nother>& other)
		{
			return this->add(other);
		}
	};

	#define makestr(_str) str<char, sizeof(_str) / sizeof(char)>(_str)
	#define makewstr(_str) str<wchar, sizeof(_str) / sizeof(wchar)>(_str)
}

#endif
