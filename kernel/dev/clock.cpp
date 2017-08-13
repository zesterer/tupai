//
// file : clock.cpp
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

// Tupai
#include <tupai/dev/clock.hpp>

#if defined(ARCH_FAMILY_X86)
	#include <tupai/x86/cmos.hpp>
#else
	#warning "Architecture provides no clock device!"
#endif

namespace tupai
{
	namespace dev
	{
		static bool clock_initiated = false;

		void clock_init()
		{
			if (clock_initiated)
				return;

			#if defined(ARCH_FAMILY_X86)
				x86::cmos_init();
			#endif

			clock_initiated = true;
		}

		datetime_t clock_read()
		{
			#if defined(ARCH_FAMILY_X86)
				return x86::cmos_read();
			#else
			{
				datetime_t time;

				time.sec  = 0;
				time.min  = 0;
				time.hour = 0;

				time.day   = 1;
				time.month = 1;
				time.year  = 2017;

				return time;
			}
			#endif
		}
	}
}
