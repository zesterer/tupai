//
// file : cmos.cpp
//
// This file is part of Tupai.
//
// Tupai is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
//
// Tupai is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY// without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with Tupai.  If not, see <http://www.gnu.org/licenses/>.
//

// Tupai
#include <tupai/x86/cmos.hpp>
#include <tupai/x86/port.hpp>
#include <tupai/debug.hpp>

namespace tupai
{
	namespace x86
	{
		static const uint16_t CMOS_CMD  = 0x70;
		static const uint16_t CMOS_DATA = 0x71;

		static const uint16_t CURRENT_YEAR = 2017;

		static int cent_reg = 0x00;

		static bool    cmos_updating();
		static uint8_t cmos_get_reg(uint8_t reg);

		void cmos_init()
		{
			// Do nothing yet
		}

		datetime_t cmos_read()
		{
			// Read from CMOS RTC
			datetime_t time;
			uint8_t cent = 0;

			// Previously read values
			datetime_t l_time;
			uint8_t l_cent = 0;

			// Repeatedly read until consistent values are read
			int i = 0;
			do
			{
				l_time = time;

				// Wait for no update to be in progress
				while (cmos_updating());

				time.sec   = cmos_get_reg(0x00);
				time.min   = cmos_get_reg(0x02);
				time.hour  = cmos_get_reg(0x04);
				time.day   = cmos_get_reg(0x07);
				time.month = cmos_get_reg(0x08);
				time.year  = cmos_get_reg(0x09);
				if (cent_reg != 0) cent = cmos_get_reg(cent_reg);

				i ++;
			} while (i <= 1 || time != l_time || cent != l_cent);

			// Register B is the RTC format register
			uint8_t regb = cmos_get_reg(0x0B);

			// BCD to binary conversion if needed
			if (!(regb & 0x04))
			{
				time.sec = (time.sec & 0x0F) + (time.sec / 16) * 10;
				time.min = (time.min & 0x0F) + (time.min / 16) * 10;
				time.hour = (time.hour & 0x0F) + ((((time.hour & 0x70) / 16) * 10) | (time.hour & 0x80)); // Deal with pos / neg value in sign bit

				time.day = (time.day & 0x0F) + (time.day / 16) * 10;
				time.month = (time.month & 0x0F) + (time.month / 16) * 10;
				time.year = (time.year & 0x0F) + (time.year / 16) * 10;
			}

			// 12-hour to 24-hour clock
			if (!(regb & 0x02) && (time.hour & 0x80))
				time.hour = ((time.hour & 0x7F) + 12) % 24;

			// Calculate year from known data
			if (cent_reg != 0x00) // We have century information
				time.year += cent * 100;
			else
			{
				time.year += (CURRENT_YEAR / 100) * 100;
				if (time.year < CURRENT_YEAR) time.year += 100;
			}

			return time;
		}

		bool cmos_updating()
		{
			outb(CMOS_CMD, 0x0A);
			return (inb(CMOS_DATA) & 0x80) > 0;
		}

		uint8_t cmos_get_reg(uint8_t reg)
		{
			outb(CMOS_CMD, reg);
			return inb(CMOS_DATA);
		}
	}
}
