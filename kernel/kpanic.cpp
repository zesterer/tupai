/*
* 	file : kpanic.cpp
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
#include <tupai/kpanic.hpp>
#include <tupai/tty.hpp>

namespace tupai
{
	extern "C" void _kpanic() __attribute__((noreturn));
	extern "C" long _kpanic_errormsg;
	extern "C" long _kpanic_errorcode;

	extern "C" void _khalt() __attribute__((noreturn));

	void kpanic(const char* msg, long code)
	{
		_kpanic_errormsg = (long)msg;
		_kpanic_errorcode = code;
		_kpanic();
	}

	void khalt()
	{
		_khalt();
	}

	void kbreak()
	{
		// Magic bochs breakpoint
		asm volatile ("xchgw %bx, %bx");
	}

	void kfault() // To be called by ISRs only!
	{
		uint32 isr_id = 0;
		tty_write_str("Exception ");
		tty_write('0' + ((isr_id / 100) % 10));
		tty_write('0' + ((isr_id / 10) % 10));
		tty_write('0' + (isr_id % 10));
		tty_write_str(" occured!\n");
		kbreak();
	}
}
