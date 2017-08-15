//
// file : tar.cpp
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

// Tupai
#include <tupai/util/tar.hpp>
#include <tupai/util/log.hpp>
#include <tupai/util/mem.hpp>

namespace tupai
{
	namespace util
	{
		void tar_print_all(tar_header_t* header)
		{
			logln("--- Tar ---");
			while (header != nullptr)
			{
				tar_print(header);
				header = tar_next(header);
			}
		}

		void tar_print(tar_header_t* header)
		{
			logln("File: filename = ", header->filename, " size = ", (long)tar_size(header), " type = ", (char)header->typeflag);
		}

		tar_header_t* tar_next(tar_header_t* header)
		{
			size_t offset = util::align_ceiling(512 + tar_size(header), 512);
			tar_header_t* nheader = (tar_header_t*)((size_t)header + offset);

			if (nheader->filename[0] == '\0')
				return nullptr;
			else
				return nheader;
		}

		size_t tar_size(tar_header_t* header)
		{
			size_t csize = 0;
			for (int i = 0; i < 12 && header->size[i] != '\0'; i ++)
			{
				csize *= 8;
				csize += (size_t)(header->size[i] - '0');
			}

			return csize;
		}

		void* tar_data(tar_header_t* header)
		{
			return (void*)((size_t)header + 512);
		}
	}
}
