//
// file : term.hpp
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

#ifndef TUPAI_UTIL_TERM_HPP
#define TUPAI_UTIL_TERM_HPP

// Standard
#include <stddef.h>
#include <stdint.h>

namespace tupai
{
	namespace util
	{
		static const char TERM_DEFAULT[]             = "\x1B[0m";

		static const char TERM_FG_COLOR_BLACK[]      = "\x1B[30m";
		static const char TERM_FG_COLOR_RED[]        = "\x1B[31m";
		static const char TERM_FG_COLOR_GREEN[]      = "\x1B[32m";
		static const char TERM_FG_COLOR_BROWN[]      = "\x1B[33m";
		static const char TERM_FG_COLOR_BLUE[]       = "\x1B[34m";
		static const char TERM_FG_COLOR_PURPLE[]     = "\x1B[35m";
		static const char TERM_FG_COLOR_CYAN[]       = "\x1B[36m";
		static const char TERM_FG_COLOR_LIGHT_GRAY[] = "\x1B[37m";

		static const char TERM_BG_COLOR_BLACK[]      = "\x1B[40m";
		static const char TERM_BG_COLOR_RED[]        = "\x1B[41m";
		static const char TERM_BG_COLOR_GREEN[]      = "\x1B[42m";
		static const char TERM_BG_COLOR_BROWN[]      = "\x1B[43m";
		static const char TERM_BG_COLOR_BLUE[]       = "\x1B[44m";
		static const char TERM_BG_COLOR_PURPLE[]     = "\x1B[45m";
		static const char TERM_BG_COLOR_CYAN[]       = "\x1B[46m";
		static const char TERM_BG_COLOR_LIGHT_GRAY[] = "\x1B[47m";

		static const char TERM_FG_COLOR_DARK_GRAY[]  = "\x1B[90m";
		static const char TERM_FG_COLOR_PINK[]       = "\x1B[91m";
		static const char TERM_FG_COLOR_LIME_GREEN[] = "\x1B[92m";
		static const char TERM_FG_COLOR_YELLOW[]     = "\x1B[93m";
		static const char TERM_FG_COLOR_LIGHT_BLUE[] = "\x1B[94m";
		static const char TERM_FG_COLOR_MAGENTA[]    = "\x1B[95m";
		static const char TERM_FG_COLOR_AQUA[]       = "\x1B[96m";
		static const char TERM_FG_COLOR_WHITE[]      = "\x1B[97m";

		static const char TERM_BG_COLOR_DARK_GRAY[]  = "\x1B[100m";
		static const char TERM_BG_COLOR_PINK[]       = "\x1B[101m";
		static const char TERM_BG_COLOR_LIME_GREEN[] = "\x1B[102m";
		static const char TERM_BG_COLOR_YELLOW[]     = "\x1B[103m";
		static const char TERM_BG_COLOR_LIGHT_BLUE[] = "\x1B[104m";
		static const char TERM_BG_COLOR_MAGENTA[]    = "\x1B[105m";
		static const char TERM_BG_COLOR_AQUA[]       = "\x1B[106m";
		static const char TERM_BG_COLOR_WHITE[]      = "\x1B[107m";
	}
}

#endif
