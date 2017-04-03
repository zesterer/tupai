//
// file : fmt.hpp
//
// This file is part of Tupai.
//
// Tupai is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
//
// Tupai is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY// without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with Tupai.  If not, see <http://www.gnu.org/licenses/>.
//

#ifndef TUPAI_UTIL_FMT_HPP
#define TUPAI_UTIL_FMT_HPP

// Tupai
#include <tupai/tty.hpp>

// Standard
#include <stddef.h>
#include <stdint.h>

namespace tupai
{
	namespace util
	{
		struct __fmt_funct
		{
			char* buff;
			size_t pos;
			size_t size;

			__fmt_funct(char* buff, size_t size)
			{
				this->pos = 0;
				this->buff = buff;
				this->size = size;
			}

			void operator()(char c)
			{
				if (this->pos + 1 >= this->size)
					return;

				this->buff[this->pos] = c;
				this->pos ++;
			}
		};

		template <typename F>
		void __fmt_arg(F& functor, const char* str)
		{
			for (size_t i = 0; str[i] != '\0'; i ++)
				functor(str[i]);
		}

		template <typename F>
		void __fmt_arg(F& functor, char c)
		{
			functor(c);
		}

		struct __pass_funct
		{
			template <typename ...T>
			__pass_funct(T...) {}
		};

		template <size_t SIZE, typename... Args>
		void fmt(char(&buff)[SIZE], Args&&... args)
		{
			// Clear the buffer
			for (size_t i = 0; i < SIZE; i ++)
				buff[i] = '\0';

			__fmt_funct funct(buff, SIZE);
			__pass_funct{(__fmt_arg(funct, args), 1)...};
		}
	}
}

#endif
