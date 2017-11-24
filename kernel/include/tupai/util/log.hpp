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

#include <tupai/util/fmt.hpp>
#ifdef ARCH_FAMILY_x86
	#include <tupai/x86/vga/textmode.hpp>
#endif

namespace tupai::util
{
	struct OStream
	{
		void operator<<(char c)
		{
			#ifdef ARCH_FAMILY_x86
				x86::vga::write_char(c);
			#endif
		}
	};

	template <typename ... Args>
	void log(const char* format, Args ... args)
	{
		OStream out;
		fmt(out, format, args ...);
	}

	template <typename ... Args>
	void logln(const char* format, Args ... args)
	{
		log(format, args ...);
		log("\n");
	}
}

#endif
