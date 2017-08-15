//
// file : tar.hpp
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

#ifndef TUPAI_UTIL_TAR_HPP
#define TUPAI_UTIL_TAR_HPP

// Standard
#include <stddef.h>
#include <stdint.h>

namespace tupai
{
	namespace util
	{
		struct tar_header_t
		{
			char    filename[100];
			char    mode[8];
			char    uid[8];
			char    gid[8];
			char    size[12];
			char    mtime[12];
			uint8_t checksum[8];
			char    typeflag;
		} __attribute__((packed));

		void          tar_print_all(tar_header_t* header);
		void          tar_print(tar_header_t* header);
		tar_header_t* tar_next(tar_header_t* header);
		size_t        tar_size(tar_header_t* header);
		void*         tar_data(tar_header_t* header);
	}
}

#endif
