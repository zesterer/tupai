/*
* 	file : random.cpp
*
* 	This file is part of Tupai.
*
* 	Tupai is free software: you can redistribute it and/or modify
* 	it under the terms of the GNU General Public License as published by
* 	the Free Software Foundation, either version 3 of the License, or
* 	(at your option) any later version.
*
* 	Tupai is distributed in the hope that it will be useful,
* 	but WITHOUT ANY WARRANTY; without even the implied warranty of
* 	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
* 	GNU General Public License for more details.
*
* 	You should have received a copy of the GNU General Public License
* 	along with Tupai.  If not, see <http://www.gnu.org/licenses/>.
*/

// Tupai
#include <tupai/sys/random.hpp>
#include <tupai/sys/timer.hpp>

namespace tupai
{
	namespace sys
	{
		uint64 value;

		static void random_churn();

		void random_init()
		{
			value = 0x7CA451C378291479;
		}

		void random_seed(uint32 data)
		{
			value += data;
			random_churn();
		}

		uint32 random_get()
		{
			random_churn();
			return (uint32)value;
		}

		void random_churn()
		{
			value += timer_get_nanoseconds();
			value = (((value ^ 0x97A6213321275773) ^ (value * 0x133337)) << (value % 8)) + ((value >> 16) % 256);
		}
	}
}
