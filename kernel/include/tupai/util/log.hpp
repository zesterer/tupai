//
// file : log.hpp
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

#ifndef TUPAI_UTIL_LOG_HPP
#define TUPAI_UTIL_LOG_HPP

// Tupai
#include <tupai/dev/tty.hpp>
#include <tupai/util/fmt.hpp>

// Standard
#include <stddef.h>
#include <stdint.h>

namespace tupai
{
	namespace util
	{
		struct __log_ostream
		{
			static void end();
			static void write(char c);
		};

		inline void log(const char* str)
		{
			__log_ostream ostream;
			for (size_t i = 0; str[i] != '\0'; i ++)
				ostream.write(str[i]);
			ostream.end();
		}

		template <typename... Args>
		inline void log(Args&&... args)
		{
			__log_ostream ostream;
			__pass_funct{(__fmt_arg(ostream, args), 1)...};
			ostream.end();
		}

		template <typename... Args>
		inline void logln(Args&&... args)
		{
			log(args..., '\n');
		}
	}
}

#endif
