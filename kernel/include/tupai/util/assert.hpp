//
// file : assert.hpp
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

#ifndef TUPAI_UTIL_ASSERT_HPP
#define TUPAI_UTIL_ASSERT_HPP

#include <tupai/util/type.hpp>

namespace tupai::util
{
	#define assert(EXPR, STR) tupai::util::_assert(EXPR, #EXPR, STR)

	static inline void _assert(bool expr, const char* expr_str, const char* msg);
}

// Delayed implementation to prevent circular references
#include <tupai/util/panic.hpp>

namespace tupai::util
{
	static inline void _assert(bool expr, const char* expr_str, const char* msg)
	{
		if (!expr)
			panic("Assertion '{}' failed: {}", expr_str, msg);
	}
}

#endif
