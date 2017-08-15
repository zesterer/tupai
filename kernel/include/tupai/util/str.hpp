//
// file : str.hpp
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

#ifndef TUPAI_UTIL_STR_HPP
#define TUPAI_UTIL_STR_HPP

// Standard
#include <stddef.h>
#include <stdint.h>

namespace tupai
{
	namespace util
	{
		static inline size_t str_len(const char* str)
		{
			size_t i;
			for (i = 0; str[i] != '\0'; i ++);
			return i;
		}

		static inline void str_cpy(const char* src, char* dest)
		{
			size_t i;
			for (i = 0; src[i] != '\0'; i ++)
				dest[i] = src[i];
			dest[i] = '\0';
		}

		static inline void str_cpy_n(const char* src, char* dest, size_t n)
		{
			size_t i;
			for (i = 0; src[i] != '\0' && i + 1 < n; i ++)
				dest[i] = src[i];
			dest[i] = '\0';
		}

		template <size_t N>
		static inline void str_cpy_n(const char* src, char(&dest)[N])
		{
			size_t i;
			for (i = 0; src[i] != '\0' && i + 1 < N; i ++)
				dest[i] = src[i];
			dest[i] = '\0';
		}

		static inline bool str_equal(const char* str0, const char* str1)
		{
			size_t i = 0;
			do
			{
				if (str0[i] != str1[i])
					return false;
				i ++;
			} while (str0[i] != '\0' || str1[i] != '\0');

			return true;
		}
	}
}

#endif
