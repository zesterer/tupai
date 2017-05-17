//
// file : tty.hpp
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

#ifndef TUPAI_TTY_HPP
#define TUPAI_TTY_HPP

// Standard
#include <stddef.h>
#include <stdint.h>

namespace tupai
{
	namespace dev
	{
		void tty_init();

		void tty_write(char c);
		void tty_print(const char* str);

		char tty_read();
		void tty_readline(char* buff, size_t n);

		template <size_t SIZE>
		void tty_readline(char(&buff)[SIZE])
		{
			tty_readline(buff, SIZE);
		}
	}
}

#endif