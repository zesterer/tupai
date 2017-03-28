//
// file : kmain.cpp
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
#include <tupai/common/kmain.hpp>
#include <tupai/common/init.hpp>
#include <tupai/common/tty.hpp>
#include <tupai/common/sys.hpp>

namespace tupai
{
	void kwelcome()
	{
		tty_print(sys_get_name_decorative());
		tty_print(" ");
		tty_print(sys_get_version());
		tty_print(" on ");
		tty_print(sys_get_family());
		tty_print("/");
		tty_print(sys_get_arch());
		tty_print("\nCopyright 2017, Joshua Barretto\n");
		tty_print("\n");
	}

	int kmain()
	{
		// Init a stable environment
		init();

		// TTY
		tty_init();
		kwelcome();

		tty_print("Welcome to the kernel. It doesn't do much yet.\n");

		return 0;
	}
}
