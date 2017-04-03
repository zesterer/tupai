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
#include <tupai/tupai.hpp>
#include <tupai/arch.hpp>
#include <tupai/tty.hpp>

#include <tupai/util/fmt.hpp>
#include <tupai/util/out.hpp>

namespace tupai
{
	void motd()
	{
		tty_print(tupai_get_name_decorative());
		tty_print(" ");
		tty_print(tupai_get_version());
		tty_print(" on ");
		tty_print(arch_get_family());
		tty_print("/");
		tty_print(arch_get_target());
		tty_print("\nCopyright 2017, Joshua Barretto\n");
		tty_print("\n");
	}

	int main()
	{
		// At this point, we should have a stable environment with memory
		// protection, a heap, a page frame allocator, etc. all configured.
		// The methods through which this is done are platform-dependent.
		// Now, however, it's relatively safe to run generic code on the
		// assumption that everything 'works'.

		// Initiate the TTY
		tty_init();

		motd();
		util::print("Welcome to the kernel. It doesn't do much yet.\n");

		// Test formatting!
		char mytest[50];
		util::fmt(mytest, "Hello, ", "there", "!", '\n');
		util::print(mytest, "And ", "another test!", '\n', "Haha!\n");

		return 0;
	}
}
