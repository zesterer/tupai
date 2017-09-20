//
// file : file.hpp
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

#ifndef TUPAI_CALL_FILE_HPP
#define TUPAI_CALL_FILE_HPP

// Standard
#include <stddef.h>
#include <stdint.h>

namespace tupai
{
	namespace call
	{
		size_t open (size_t arg0, size_t arg1, size_t arg2, size_t arg3);
		size_t read (size_t arg0, size_t arg1, size_t arg2, size_t arg3);
		size_t write(size_t arg0, size_t arg1, size_t arg2, size_t arg3);
		size_t close(size_t arg0, size_t arg1, size_t arg2, size_t arg3);
		size_t seek (size_t arg0, size_t arg1, size_t arg2, size_t arg3);
	}
}

#endif