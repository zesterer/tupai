//
// file : debug.hpp
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

#ifndef TUPAI_DEBUG_HPP
#define TUPAI_DEBUG_HPP

// Tupai
#include <tupai/util/fmt.hpp>

namespace tupai
{
	void debug_init();
	void debug_write(char c);
	void debug_print(const char* str);

	struct __debug_ostream
	{
		inline void write(char c)
		{
			debug_write(c);
		}
	};

	template <typename... Args>
	inline void debug_print(Args&&... args __attribute__((unused)))
	{
		#if defined(DEBUG_ENABLED)
		{
			debug_print(""); // For the debug prefix
			__debug_ostream ostream;
			util::__pass_funct{(util::__fmt_arg(ostream, args), 1)...};
		}
		#endif
	}

	template <typename... Args>
	inline void debug_println(Args&&... args __attribute__((unused)))
	{
		#if defined(DEBUG_ENABLED)
		{
			debug_print(args...);
			debug_write('\n');
		}
		#endif
	}
}

#endif
