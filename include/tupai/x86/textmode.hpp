//
// file : textmode.hpp
//
// This file is part of Tupai.
//
// Tupai is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
//
// Tupai is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with Tupai.  If not, see <http://www.gnu.org/licenses/>.
//

#ifndef TUPAI_X86_TEXTMODE_HPP
#define TUPAI_X86_TEXTMODE_HPP

// Tupai
#include <tupai/util/ansi.hpp>

namespace tupai
{
	namespace x86
	{
		void textmode_init();
		void textmode_cursor_enable();
		void textmode_cursor_disable();
		void textmode_cursor_move(int col, int row);
		void textmode_move(int ncol, int nrow);
		void textmode_write(char c);
		void textmode_scroll(int n);
		void textmode_clear();

		void textmode_apply(util::ansi_cmd_t cmd);
		void textmode_apply_sgr(int n);
	}
}

#endif
