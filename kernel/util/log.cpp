//
// file : log.cpp
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
#include <tupai/util/log.hpp>
#include <tupai/util/hwlock.hpp>
#include <tupai/util/vector.hpp>
#include <tupai/dev/tty.hpp>

namespace tupai
{
	namespace util
	{
		typedef vector_t<char> log_t;

		vector_t<log_t> logs;
		log_t           clog;

		void __log_ostream::end()
		{
			if (clog.size() > 0)
			{
				//logs.push(clog);
				clog = log_t();
			}
		}

		void __log_ostream::write(char c)
		{
			util::hwlock_acquire(); // Begin critical section

			// TODO : Uncomment this!
			//clog.push(c);
			dev::tty_write(c);

			util::hwlock_release(); // End critical section
		}
	}
}
