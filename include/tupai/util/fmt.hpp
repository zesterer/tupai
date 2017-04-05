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
#include <tupai/util/conv.hpp>

// Standard
#include <stddef.h>
#include <stdint.h>

namespace tupai
{
	namespace util
	{
		// Default format ostream, for writing to character buffers
		struct __fmt_ostream
		{
			char* buff;
			size_t pos;
			size_t size;

			__fmt_ostream(char* buff, size_t size)
			{
				this->buff = buff;
				this->pos = 0;
				this->size = size;
			}

			inline void write(char c)
			{
				if (this->pos + 1 >= this->size)
					return;

				this->buff[this->pos] = c;
				this->pos ++;
			}
		};

		// String formatter
		template <typename T>
		void __fmt_arg(T& ostream, const char* str)
		{
			for (size_t i = 0; str[i] != '\0'; i ++)
				ostream.write(str[i]);
		}

		// Character formatter
		template <typename T>
		void __fmt_arg(T& ostream, char c)
		{
			ostream.write(c);
		}

		/*
		// Generic integer composition formatter
		template <typename T, typename U>
		void __fmt_arg(T& ostream, U i)
		{
			char buff[sizeof(U) * 8 + 4];
			compose(i, buff, sizeof(buff) / sizeof(char));
			__fmt_arg(ostream, buff);
		}
		*/

		// Int formatter
		template <typename T>
		void __fmt_arg(T& ostream, int i, int base = 10, int pad = 0)
		{
			char buff[sizeof(int) * 8 + 4];
			compose(i, buff, sizeof(buff) / sizeof(char), base, pad);
			__fmt_arg(ostream, buff);
		}

		// Unsigned int formatter
		template <typename T>
		void __fmt_arg(T& ostream, unsigned int i, int base = 10, int pad = 0)
		{
			char buff[sizeof(unsigned int) * 8 + 4];
			compose(i, buff, sizeof(buff) / sizeof(char), base, pad);
			__fmt_arg(ostream, buff);
		}

		// Long formatter
		template <typename T>
		void __fmt_arg(T& ostream, long i, int base = 10, int pad = 0)
		{
			char buff[sizeof(long) * 8 + 4];
			compose(i, buff, sizeof(buff) / sizeof(char), base, pad);
			__fmt_arg(ostream, buff);
		}

		// Unsigned long formatter
		template <typename T>
		void __fmt_arg(T& ostream, unsigned long i, int base = 10, int pad = 0)
		{
			char buff[sizeof(unsigned long) * 8 + 4];
			compose(i, buff, sizeof(buff) / sizeof(char), base, pad);
			__fmt_arg(ostream, buff);
		}

		// Long long formatter
		template <typename T>
		void __fmt_arg(T& ostream, long long i, int base = 10, int pad = 0)
		{
			char buff[sizeof(long long) * 8 + 4];
			compose(i, buff, sizeof(buff) / sizeof(char), base, pad);
			__fmt_arg(ostream, buff);
		}

		// Unsigned long long formatter
		template <typename T>
		void __fmt_arg(T& ostream, unsigned long long i, int base = 10, int pad = 0)
		{
			char buff[sizeof(unsigned long long) * 8 + 4];
			compose(i, buff, sizeof(buff) / sizeof(char), base, pad);
			__fmt_arg(ostream, buff);
		}

		// Int format wrapper
		template <typename T>
		struct fmt_int
		{
			T val;
			int base;
			int pad;

			fmt_int(T val, int base = 10, int pad = 0)
			{
				this->val = val;
				this->base = base;
				this->pad = pad;
			}
		};

		// Int format formatter
		template <typename T, typename U>
		void __fmt_arg(T& ostream, fmt_int<U> fmt)
		{
			__fmt_arg(ostream, fmt.val, fmt.base, fmt.pad);
		}

		// Boolean formatter
		template <typename T>
		void __fmt_arg(T& ostream, bool val)
		{
			char buff[6];
			compose(val, buff, sizeof(buff) / sizeof(char));
			__fmt_arg(ostream, buff);
		}

		// Pointer formatter
		template <typename T>
		void __fmt_arg(T& ostream, void* val)
		{
			char buff[sizeof(void*) * 8 + 4];
			compose(val, buff, sizeof(buff) / sizeof(char));
			__fmt_arg(ostream, buff);
		}

		struct __pass_funct
		{
			template <typename ...T>
			inline __pass_funct(T...) {}
		};

		template <size_t SIZE, typename... Args>
		inline void fmt(char(&buff)[SIZE], Args&&... args)
		{
			// Clear the buffer
			for (size_t i = 0; i < SIZE; i ++)
				buff[i] = '\0';

			__fmt_ostream ostream(buff, SIZE);
			__pass_funct{(__fmt_arg(ostream, args), 1)...};
		}
	}
}

#endif
