//
// file : conv.hpp
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

#ifndef TUPAI_UTIL_CONV_HPP
#define TUPAI_UTIL_CONV_HPP

// Standard
#include <stddef.h>
#include <stdint.h>

namespace tupai
{
	namespace util
	{
		bool compose(int8_t val, char* buff, size_t n, int base = 10, int pad = 0);
		bool compose(uint8_t val, char* buff, size_t n, int base = 10, int pad = 0);

		bool compose(int16_t val, char* buff, size_t n, int base = 10, int pad = 0);
		bool compose(uint16_t val, char* buff, size_t n, int base = 10, int pad = 0);

		bool compose(int32_t val, char* buff, size_t n, int base = 10, int pad = 0);
		bool compose(uint32_t val, char* buff, size_t n, int base = 10, int pad = 0);

		bool compose(int64_t val, char* buff, size_t n, int base = 10, int pad = 0);
		bool compose(uint64_t val, char* buff, size_t n, int base = 10, int pad = 0);

		bool compose(void* val, char* buff, size_t n);
		bool compose(bool val, char* buff, size_t n);
	}
}

#endif
