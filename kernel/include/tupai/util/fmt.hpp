//
// file : fmt.hpp
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

		// int8_t formatter
		template <typename T>
		void __fmt_arg(T& ostream, int8_t i, int base = 10, int pad = 0)
		{
			char buff[sizeof(int8_t) * 8 + 4];
			compose(i, buff, sizeof(buff) / sizeof(char), base, pad);
			__fmt_arg(ostream, buff);
		}

		// uint8_t formatter
		template <typename T>
		void __fmt_arg(T& ostream, uint8_t i, int base = 10, int pad = 0)
		{
			char buff[sizeof(uint8_t) * 8 + 4];
			compose(i, buff, sizeof(buff) / sizeof(char), base, pad);
			__fmt_arg(ostream, buff);
		}

		// int16_t formatter
		template <typename T>
		void __fmt_arg(T& ostream, int16_t i, int base = 10, int pad = 0)
		{
			char buff[sizeof(int16_t) * 8 + 4];
			compose(i, buff, sizeof(buff) / sizeof(char), base, pad);
			__fmt_arg(ostream, buff);
		}

		// uint16_t formatter
		template <typename T>
		void __fmt_arg(T& ostream, uint16_t i, int base = 10, int pad = 0)
		{
			char buff[sizeof(uint16_t) * 8 + 4];
			compose(i, buff, sizeof(buff) / sizeof(char), base, pad);
			__fmt_arg(ostream, buff);
		}

		// int32_t formatter
		template <typename T>
		void __fmt_arg(T& ostream, int32_t i, int base = 10, int pad = 0)
		{
			char buff[sizeof(int32_t) * 8 + 4];
			compose(i, buff, sizeof(buff) / sizeof(char), base, pad);
			__fmt_arg(ostream, buff);
		}

		// uint32_t formatter
		template <typename T>
		void __fmt_arg(T& ostream, uint32_t i, int base = 10, int pad = 0)
		{
			char buff[sizeof(uint32_t) * 8 + 4];
			compose(i, buff, sizeof(buff) / sizeof(char), base, pad);
			__fmt_arg(ostream, buff);
		}

		// int64_t formatter
		template <typename T>
		void __fmt_arg(T& ostream, int64_t i, int base = 10, int pad = 0)
		{
			char buff[sizeof(int64_t) * 8 + 4];
			compose(i, buff, sizeof(buff) / sizeof(char), base, pad);
			__fmt_arg(ostream, buff);
		}

		// uint64_t formatter
		template <typename T>
		void __fmt_arg(T& ostream, uint64_t i, int base = 10, int pad = 0)
		{
			char buff[sizeof(uint64_t) * 8 + 4];
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
