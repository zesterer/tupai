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

namespace tupai
{
	struct console_config
	{
		uint columns;
		uint rows;
	};

	struct ConsoleCmd
	{
		enum class Type
		{
			WRITECHAR,
			CLEAR,
			MOVE,
			SETFORECOLOR,
			SETBACKCOLOR,
			RESET,

			NONE,
		};

		struct WriteChar { char c; WriteChar(char c) { this->c = c; } };
		struct Clear { int mode; Clear(int mode){ this->mode = mode; } };
		struct Move { uint col; uint row; Move(uint col, uint row){ this->col = col; this->row = row; } };
		struct SetForeColor { uint8 color; SetForeColor(uint8 color){ this->color = color; } };
		struct SetBackColor { uint8 color; SetBackColor(uint8 color){ this->color = color; } };
		struct Reset {  };

		Type type;

		union
		{
			WriteChar write_char;
			Clear clear;
			Move move;
			SetForeColor set_fore_color;
			SetBackColor set_back_color;
			Reset reset;
		};

		ConsoleCmd();
		ConsoleCmd(WriteChar data);
		ConsoleCmd(Clear data);
		ConsoleCmd(Move data);
		ConsoleCmd(SetForeColor data);
		ConsoleCmd(SetBackColor data);
		ConsoleCmd(Reset data);
	};

	struct Console
	{
	private:
		int escape_state;
		int escape_num[2];

		ConsoleCmd handle_escaped(char c);
		ConsoleCmd handle_sgr(int code);

	public:
		Console();
		ConsoleCmd write(char c);
	};
}

#endif
