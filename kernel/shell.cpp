//
// file : shell.cpp
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

// Tuapi
#include <tupai/shell.hpp>
#include <tupai/tupai.hpp>
#include <tupai/arch.hpp>
#include <tupai/panic.hpp>
#include <tupai/util/in.hpp>
#include <tupai/util/out.hpp>
#include <tupai/util/str.hpp>

namespace tupai
{
	void shell_motd()
	{
		util::print('\n', tupai_get_name_decorative(), " ",
			tupai_get_version(), " on ",
			arch_get_family(), "/", arch_get_target(), '\n',
			"Copyright 2017, Joshua Barretto\n", '\n'
		);
	}

	void shell_main()
	{
		// Display the MOTD
		shell_motd();

		bool halted = false;
		util::println("Type 'help' for more info.");
		while (!halted)
		{
			util::print("$ ");

			char buff[64];
			util::readline(buff);
			tty_write('\n');

			if (util::str_equal(buff, "help"))
			{
				util::print(
					"Available commands:\n",
					"  help  -> Show this help text\n",
					"  motd  -> Show the MOTD\n",
					"  panic -> Trigger a kernel panic\n",
					"  info  -> Show system info\n",
					"  halt  -> Halt the kernel\n"
				);
			}
			else if (util::str_equal(buff, "motd"))
			{
				shell_motd();
			}
			else if (util::str_equal(buff, "panic"))
			{
				util::println("Panicing...");
				panic("Panic triggered artificially");
			}
			else if (util::str_equal(buff, "info"))
			{
				util::print(
					"System Info:\n",
					"  address_size -> ", sizeof(void*) * 8, " bits\n"
				);
			}
			else if (util::str_equal(buff, "halt"))
				halted = true;
			else
				util::println("Command '", buff, "' not found!");
		}
		util::println("Halting...");
	}
}
