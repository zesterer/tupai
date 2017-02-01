/*
* 	file : console.cpp
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
#include <tupai/console.hpp>
#include <tupai/util/char.hpp>
#include <tupai/util/math.hpp>

namespace tupai
{
	ConsoleCmd::ConsoleCmd() { this->type = ConsoleCmd::Type::NONE; }
	ConsoleCmd::ConsoleCmd(ConsoleCmd::WriteChar data) { this->type = ConsoleCmd::Type::WRITECHAR; this->write_char = data; }
	ConsoleCmd::ConsoleCmd(ConsoleCmd::Clear data) { this->type = ConsoleCmd::Type::CLEAR; this->clear = data; }
	ConsoleCmd::ConsoleCmd(ConsoleCmd::Move data) { this->type = ConsoleCmd::Type::MOVE; this->move = data; }
	ConsoleCmd::ConsoleCmd(ConsoleCmd::SetForeColor data) { this->type = ConsoleCmd::Type::SETFORECOLOR; this->set_fore_color = data; }
	ConsoleCmd::ConsoleCmd(ConsoleCmd::SetBackColor data) { this->type = ConsoleCmd::Type::SETBACKCOLOR; this->set_back_color = data; }
	ConsoleCmd::ConsoleCmd(ConsoleCmd::Reset data) { this->type = ConsoleCmd::Type::RESET; this->reset = data; }

	Console::Console()
	{
		this->escape_state = 0;
		this->escape_num[0] = 1;
		this->escape_num[1] = 1;
	}

	ConsoleCmd Console::write(char c)
	{
		if (this->escape_state != 0) // Escaped character
			return this->handle_escaped(c);
		else if (util::is_printable(c) || (ubyte)c >= 128) // Printable + Extended ASCII
			return ConsoleCmd(ConsoleCmd::WriteChar(c));
		else if (util::is_newline(c)) // Newlines - \n and \r
			return ConsoleCmd(ConsoleCmd::WriteChar(c));
		else if (util::is_backspace(c)) // Backspace - \r
			return ConsoleCmd(ConsoleCmd::WriteChar(c));
		else if (util::is_escape(c)) // Escape - 0x1B
			this->escape_state = 1;

		return ConsoleCmd();
	}

	ConsoleCmd Console::handle_escaped(char c)
	{
		bool rehandle = false; // Whether the character should be rehandled (on a state boundary, for example)
		switch (this->escape_state)
		{
		case 1:
			{
				if (c == '[') // Check for CSI code
					this->escape_state = 2;
				else
					{
						this->escape_state = 0;
						rehandle = true;
					}
			}
			break;

		case 2:
			{
				if (util::is_digit(c)) // Starts an escape sequence
				{
					this->escape_num[0] = 1 * util::digit_to_num(c).val();
					this->escape_state = 3;
				}
				else // Return to normal - TODO: add CSI s and CSI u implementation here
				{
					this->escape_state = 0;
					rehandle = true;
				}
			}
			break;

		case 3:
			{
				if (util::is_digit(c))
				{
					this->escape_num[0] *= 10;
					this->escape_num[0] += util::digit_to_num(c).val();
					this->escape_state = 3;
				}
				else if (c == 'J') // Clear screen
				{
					this->escape_state = 0;
					if (this->escape_num[0] == 2) // TODO : Add 0 and 1 after this!
						return ConsoleCmd(ConsoleCmd::Clear(2));
				}
				else if (c == 'm') // SGR code
				{
					this->escape_state = 0;
					return this->handle_sgr(this->escape_num[0]);
				}
				else if (c == ';') // Second parameter
				{
					this->escape_num[1] = 0;
					this->escape_state = 4;
				}
				else // Reset
				{
					this->escape_state = 0;
					rehandle = true;
				}
			}
			break;

		case 4:
			{
				if (util::is_digit(c))
				{
					this->escape_num[1] = 1 * util::digit_to_num(c).val();
					this->escape_state = 5;
				}
				else // Reset
				{
					this->escape_state = 0;
					rehandle = true;
				}
			}
			break;

		case 5:
			{
				if (util::is_digit(c))
				{
					this->escape_num[1] *= 10;
					this->escape_num[1] += util::digit_to_num(c).val();
					this->escape_state = 5;
				}
				else if (c == 'f') // Move cursor H/V position
				{
					this->escape_state = 0;
					return ConsoleCmd(ConsoleCmd::Move(this->escape_num[0], this->escape_num[1]));
				}
				else
				{
					this->escape_state = 0;
					rehandle = true;
				}
			}
			break;

		case 0:
		default: // Erroneous state
			this->escape_state = 0;
			break;
		}

		if (rehandle)
			return this->write(c);
		else
			return ConsoleCmd();
	}

	ConsoleCmd Console::handle_sgr(int code)
	{
		if (code == 0)
			return ConsoleCmd(ConsoleCmd::Reset());
		else if (code >= 30 && code <= 37)
			return ConsoleCmd(ConsoleCmd::SetForeColor(code - 30));
		else if (code >= 90 && code <= 97)
			return ConsoleCmd(ConsoleCmd::SetForeColor(8 + code - 90));
		else if (code >= 40 && code <= 47)
			return ConsoleCmd(ConsoleCmd::SetBackColor(code - 40));
		else if (code >= 100 && code <= 107)
			return ConsoleCmd(ConsoleCmd::SetBackColor(8 + code - 100));

		return ConsoleCmd();
	}
}
