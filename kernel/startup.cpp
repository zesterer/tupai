/*
* 	file : startup.cpp
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
#include <tupai/startup.hpp>

#include <tupai/util/out.hpp>
#include <tupai/util/ansi.hpp>

namespace tupai
{
	void startup_print_unit_init(const char* name, int status)
	{
		util::ansi_reset();
		util::printc('[');

		if (status == 0)
		{
			util::ansi_set_fg_color(2);
			util::print(" OK ");
		}
		else
		{
			util::ansi_set_fg_color(4);
			util::print("FAIL");
		}

		util::ansi_reset();
		util::printf("] Initiated %s\n", name);
	}

	void startup_welcome()
	{
		#if defined(CFG_SHOW_LICENSE_AT_BOOT)
			util::ansi_clear();
		#else
			util::print("\n");
		#endif

		util::ansi_set_fg_color(util::ansi::WHITE);
		util::print("Welcome to ");
		util::print(SYSTEM_NAME_DECORATIVE);
		util::print(" ");
		util::print(SYSTEM_VERSION_MAJOR);
		util::print(".");
		util::print(SYSTEM_VERSION_MINOR);
		util::print(".");
		util::print(SYSTEM_VERSION_RELEASE);
		util::print("\n");

		#if defined(CFG_SHOW_LICENSE_AT_BOOT)
			const char GPL3_LICENCE_SHORT[] = "Copyright (C) 2016 Joshua Barretto\n\n" \
			"This program comes with ABSOLUTELY NO WARRANTY.\nThis is free software, and you are " \
			"welcome to\nredistribute it under certain conditions.\n\nYou should have received a copy" \
			" of the GNU\nGeneral Public License along with this program.\nIf not, see <http://www.gn" \
			"u.org/licenses/>.\n";

			util::printc('\n');
			util::print(GPL3_LICENCE_SHORT);
		#endif

		util::printc('\n');
		util::ansi_reset();
	}
}
