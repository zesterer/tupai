//
// file : mutex.cpp
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
#include <tupai/util/in.hpp>
#include <tupai/dev/tty.hpp>

namespace tupai
{
	namespace util
	{
		void __readline(char* buff, size_t n)
		{
			size_t i = 0;
			while (i + 1 < n)
			{
				char c = dev::tty_read();

				if (c == '\b')
				{
					if (i > 0)
					{
						i --;
						dev::tty_write('\b');
						dev::tty_write(' ');
						dev::tty_write('\b');
					}
					continue;
				}
				else
					dev::tty_write(c);

				if (c == '\r' || c == '\n')
					break;
				else if (c != '\0')
				{
					buff[i] = c;
					i ++;
				}
			}
			buff[i] = '\0';
		}

	}
}
