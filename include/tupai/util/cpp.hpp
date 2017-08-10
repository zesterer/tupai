//
// file : cpp.hpp
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

#ifndef TUPAI_UTIL_CPP_HPP
#define TUPAI_UTIL_CPP_HPP

// Standard
#include <stddef.h>
#include <stdint.h>

namespace tupai
{
	namespace util
	{
		/*
		template <typename T>
		T&& move(T&& arg)
		{
			return static_cast<T&&>(arg);
		}
		*/

		template< class T > struct remove_reference      {typedef T type;};
		template< class T > struct remove_reference<T&>  {typedef T type;};
		template< class T > struct remove_reference<T&&> {typedef T type;};

		template <typename T>
		typename remove_reference<T>::type&& move(T&& arg) noexcept { return static_cast<typename remove_reference<decltype(arg)>::type&&>(arg); }
	}
}

#endif
