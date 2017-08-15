//
// file : pipe.hpp
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

#ifndef TUPAI_SYS_PIPE_HPP
#define TUPAI_SYS_PIPE_HPP

// Tupai
#include <tupai/util/fifo.hpp>
#include <tupai/vfs/vfs.hpp>

// Standard
#include <stddef.h>
#include <stdint.h>

namespace tupai
{
	namespace sys
	{
		const size_t PIPE_SIZE = 4096;
		using pipe_t = util::fifo_t<uint8_t, PIPE_SIZE>;

		void             pipe_init();
		vfs::inode_ptr_t mount_pipe(pipe_t* pipe, const char* path);
	}
}

#endif
