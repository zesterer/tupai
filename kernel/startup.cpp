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
#include <tupai/tty.hpp>

// --- TMP ---
#include <tupai/tty.hpp>

namespace tupai
{
	void startup_print_unit_init(const char* name, int status)
	{
		tty_reset();
		tty_write('[');

		if (status == 0)
		{
			tty_set_fg_color(0x0A);
			tty_write_str(" OK ");
		}
		else
		{
			tty_set_fg_color(0x0C);
			tty_write_str("FAIL");
		}

		tty_reset();
		tty_write_str("] ");
		tty_write_str("Initiated '");
		tty_write_str(name);
		tty_write_str("'\n");
	}

	void startup_welcome()
	{
		#if defined(CFG_SHOW_LICENSE_AT_BOOT)
			tty_clear();
		#else
			tty_write_str("\n");
		#endif

		tty_set_fg_color(tty_color::WHITE);
		tty_write_str("Welcome to ");
		tty_write_str(SYSTEM_NAME_DECORATIVE);
		tty_write_str(" ");
		tty_write_str(SYSTEM_VERSION_MAJOR);
		tty_write_str(".");
		tty_write_str(SYSTEM_VERSION_MINOR);
		tty_write_str(".");
		tty_write_str(SYSTEM_VERSION_RELEASE);
		tty_write_str("\n");

		#if defined(CFG_SHOW_LICENSE_AT_BOOT)
			const char GPL3_LICENCE_SHORT[] = "Copyright (C) 2016 Joshua Barretto\n\n" \
			"This program comes with ABSOLUTELY NO WARRANTY.\nThis is free software, and you are " \
			"welcome to\nredistribute it under certain conditions.\n\nYou should have received a copy" \
			" of the GNU\nGeneral Public License along with this program.\nIf not, see <http://www.gn" \
			"u.org/licenses/>.\n";

			tty_write('\n');
			tty_write_str(GPL3_LICENCE_SHORT);
		#endif

		tty_write('\n');
		tty_reset();
	}
}
