//
// file : main.cpp
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
#include <tupai/main.hpp>

// Virtual devices
#include <tupai/dev/serial.hpp>
#include <tupai/dev/ps2.hpp>
#include <tupai/dev/tty.hpp>

// Concurrency
#include <tupai/sys/thread.hpp>

// Initial software
#include <tupai/shell.hpp>

// I/O
#include <tupai/util/out.hpp>
#include <tupai/util/in.hpp>

namespace tupai
{
	void test_thread_a()
	{
		while (true)
		{
			volatile int a = 0; while (a < 37587) a ++;
			util::print('A');
		}
	}

	void test_thread_b()
	{
		while (true)
		{
			volatile int a = 0; while (a < 38775) a ++;
			util::print('B');
		}
	}

	void test_thread_c()
	{
		while (true)
		{
			volatile int a = 0; while (a < 27825) a ++;
			util::print('C');
		}
	}


	int main()
	{
		// At this point, we should have a stable environment with memory
		// protection, a heap, a page frame allocator, etc. all configured.
		// The methods through which this is done are platform-dependent.
		// Now, however, it's relatively safe to run most code.

		// Initiate virtual devices
		dev::serial_init();
		//dev::ps2_init();
		dev::tty_init();

		// List PS/2 devices
		for (unsigned int i = 0; i < dev::ps2_count_ports(); i ++)
			util::println("PS/2 port: ", dev::ps2_list_ports()[i], "");

		sys::threading_init();
		util::println("Current thread ID is ", sys::thread_get_id());

		// Create a test thread
		sys::thread_create(test_thread_a);
		sys::thread_create(test_thread_b);
		sys::thread_create(test_thread_c);

		while (true);

		// Run the kernel shell
		shell_main();

		return 0;
	}
}
