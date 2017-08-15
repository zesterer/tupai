//
// file : clock.hpp
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

#ifndef TUPAI_DEV_CLOCK_HPP
#define TUPAI_DEV_CLOCK_HPP

namespace tupai
{
	struct datetime_t
	{
		// Date
		short         year  = 1970;
		unsigned char month = 1;
		unsigned char day   = 1;

		// Time
		unsigned char hour  = 0;
		unsigned char min   = 0;
		unsigned char sec   = 0;

		bool operator==(const datetime_t& other)
		{
			return
				this->year == other.year &&
				this->month == other.month &&
				this->day == other.day &&
				this->hour == other.hour &&
				this->min == other.min &&
				this->sec == other.sec;
		}

		bool operator!=(const datetime_t& other)
		{
			return !(*this == other);
		}
	};

	namespace dev
	{
		void       clock_init();
		datetime_t clock_read();
	}
}

#endif
