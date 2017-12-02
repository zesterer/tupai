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

#ifndef TUPAI_UTIL_DEF_LOG_HPP
#define TUPAI_UTIL_DEF_LOG_HPP

#include <tupai/util/type.hpp>

namespace tupai::util
{
	//! logc(c)
	//! Write a single character to the kernel log

	void logc(char c);

	//! log(format, args...)
	//! Write a string formatted by the given arguments to the kernel log

	template <typename ... Args>
	void log(const char* format, Args ... args);

	//! log(format, args...)
	//! Write a string formatted by the given arguments, followed by a newline, to the kernel log

	template <typename ... Args>
	void logln(const char* format, Args ... args);
}

#endif
