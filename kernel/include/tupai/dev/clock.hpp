//
// file : clock.hpp
//
// This file is part of Tupai.
//
// Tupai is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
//
// Tupai is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with Tupai.  If not, see <http://www.gnu.org/licenses/>.
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
