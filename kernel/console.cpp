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

#if defined(SYSTEM_ARCH_i686)
	#include <tupai/x86_family/vga.hpp>
#endif

namespace tupai
{
	void console::set_virtualtty(virtualtty* vtty)
	{
		this->vtty = vtty;
	}

	void console::write_char(char c)
	{
		if (this->ansi_esc_state != 0)
			this->handle_escaped(c);
		else if (util::is_escape(c))
			this->ansi_esc_state = 1;
		else
		{
			if (this->vtty != nullptr)
				this->vtty->write_entry(c);
		}
	}

	void console::clear()
	{
		if (this->vtty != nullptr)
			this->vtty->clear();
	}

	void console::set_sgr(int code)
	{
		if (this->vtty != nullptr)
		{
			if (code == 0)
				this->vtty->reset_color();
			else if (code >= 30 && code <= 37)
				this->vtty->set_fg_color(code - 30);
			else if (code >= 90 && code <= 97)
				this->vtty->set_fg_color(8 + code - 90);
			else if (code >= 40 && code <= 47)
				this->vtty->set_bg_color(code - 40);
			else if (code >= 100 && code <= 107)
				this->vtty->set_bg_color(8 + code - 100);
		}
	}

	void console::move(int col, int row)
	{
		if (this->vtty != nullptr)
			this->vtty->move(col, row);
	}

	void console::enable_cursor(bool enable)
	{
		if (this->vtty != nullptr)
			this->vtty->enable_cursor(enable);
	}

	void console::handle_escaped(char c)
	{
		bool rehandle = false; // Whether the character should be rehandled (on a state boundary, for example)
		switch (this->ansi_esc_state)
		{
		case 1:
			{
				if (util::is_digit(c)) // Starts most CSI escape sequences
				{
					this->ansi_esc_num[0] = 1 * util::digit_to_num(c).val();
					this->ansi_esc_state = 2;
				}
				else if (c == '?') // Cursor status escape
				{
					this->ansi_esc_state = 5;
				}
				else // Return to normal - TODO: add CSI s and CSI u implementation here
				{
					this->ansi_esc_state = 0;
					rehandle = true;
				}
			}
			break;

		case 2:
			{
				if (util::is_digit(c))
				{
					this->ansi_esc_num[0] *= 10;
					this->ansi_esc_num[0] += util::digit_to_num(c).val();
					this->ansi_esc_state = 2;
				}
				else if (c == 'J') // Clear screen
				{
					if (this->ansi_esc_num[0] == 2) // TODO : Add 0 and 1 after this!
						this->clear();
					this->ansi_esc_state = 0;
				}
				else if (c == 'm') // SGR code
				{
					this->set_sgr(this->ansi_esc_num[0]);
					this->ansi_esc_state = 0;
				}
				else if (c == ';') // Second parameter
				{
					this->ansi_esc_num[1] = 0;
					this->ansi_esc_state = 3;
				}
				else // Reset
				{
					this->ansi_esc_state = 0;
					rehandle = true;
				}
			}
			break;

		case 3:
			{
				if (util::is_digit(c))
				{
					this->ansi_esc_num[1] = 1 * util::digit_to_num(c).val();
					this->ansi_esc_state = 4;
				}
				else // Reset
				{
					this->ansi_esc_state = 0;
					rehandle = true;
				}
			}
			break;

		case 4:
			{
				if (util::is_digit(c))
				{
					this->ansi_esc_num[1] *= 10;
					this->ansi_esc_num[1] += util::digit_to_num(c).val();
					this->ansi_esc_state = 4;
				}
				else if (c == 'f') // Move cursor H/V position
				{
					this->move(ansi_esc_num[0], ansi_esc_num[1]);
					this->ansi_esc_state = 0;
				}
				else
				{
					this->ansi_esc_state = 0;
					rehandle = true;
				}
			}
			break;

		case 5: // Cursor status escape
			{
				if (util::is_digit(c))
				{
					this->ansi_esc_num[0] = 1 * util::digit_to_num(c).val();
					this->ansi_esc_state = 6;
				}
				else // Reset
				{
					this->ansi_esc_state = 0;
					rehandle = true;
				}
			}
			break;

		case 6: // Cursor status escape
			{
				if (util::is_digit(c))
				{
					this->ansi_esc_num[0] *= 10;
					this->ansi_esc_num[0] += util::digit_to_num(c).val();
					this->ansi_esc_state = 6;
				}
				else if (c == 'h' && this->ansi_esc_num[0] == 25) // Cursor show
				{
					this->enable_cursor(true);
					this->ansi_esc_state = 0;
				}
				else if (c == 'l' && this->ansi_esc_num[0] == 25) // Cursor hide
				{
					this->enable_cursor(false);
					this->ansi_esc_state = 0;
				}
				else
				{
					this->ansi_esc_state = 0;
					rehandle = true;
				}
			}
			break;

		case 0:
		default: // Erroneous state
			this->ansi_esc_state = 0;
			break;
		}

		if (rehandle)
			this->write_char(c);
	}
}
