/*
* 	file : console.hpp
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

#ifndef TUPAI_CONSOLE_HPP
#define TUPAI_CONSOLE_HPP

// Tupai
#include <tupai/type.hpp>
#include <tupai/util/safetype.hpp>
#include <tupai/virtualtty.hpp>

namespace tupai
{
	struct console
	{
	private:
		int ansi_esc_state = 0;
		int ansi_esc_num[2] = { 1, 1 };

		virtualtty* vtty = nullptr;

		void handle_escaped(char c);
		void clear();
		void set_sgr(int code);
		void move(int col, int row);

	public:
		void set_virtualtty(virtualtty* vtty);
		void write_char(char c);
	};

	void console_init_global();
	safeptr<console> console_get_global();
}

#endif
