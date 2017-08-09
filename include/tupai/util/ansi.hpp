//
// file : ansi.hpp
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

#ifndef TUPAI_UTIL_ANSI_HPP
#define TUPAI_UTIL_ANSI_HPP

// Standard
#include <stddef.h>
#include <stdint.h>

namespace tupai
{
	namespace util
	{
		// Command types

		enum class ansi_cmd
		{
			NONE, // Nothing
			PASS, // Pass the character through

			CURSOR_UP,     // Move cursor up
			CURSOR_DOWN,   // Move cursor down
			CURSOR_FWD,    // Move cursor forward
			CURSOR_BACK,   // Move cursor back
			CURSOR_NEXTLN, // Move cursor to next line
			CURSOR_PREVLN, // Move cursor to previous line
			CURSOR_HORABS, // Move cursor to column
			CURSOR_POS,    // Set cursor position
			CURSOR_REPORT, // Report the cursor position as if typed
			CURSOR_SAVE,   // Save the cursor position
			CURSOR_RESTOR, // Restore cursor position
			CURSOR_HIDE,   // Hides the cursor
			CURSOR_SHOW,   // Shows the cursor

			ERASE_INDISP,  // Erase in display
			ERASE_INLINE,  // Erase in line

			SCROLL_UP,     // Scroll whole page up
			SCROLL_DOWN,   // Scroll whole page down

			SGR_PARAMETER, // Set SGR parameter

			AUX_ON,        // Turn AUX port on
			AUX_OFF,       // Turn AUX port off
		};

		// Command data

		struct ansi_cursor_up_t     { int n; };
		struct ansi_cursor_down_t   { int n; };
		struct ansi_cursor_fwd_t    { int n; };
		struct ansi_cursor_back_t   { int n; };
		struct ansi_cursor_nextln_t { int n; };
		struct ansi_cursor_prevln_t { int n; };
		struct ansi_cursor_horabs_t { int n; };
		struct ansi_cursor_pos_t    { int n, m; };

		struct ansi_erase_indisp_t { int n; };
		struct ansi_erase_inline_t { int n; };

		struct ansi_scroll_up_t   { int n; };
		struct ansi_scroll_down_t { int n; };

		struct ansi_sgr_parameter_t { int n; };

		// Command struct

		struct ansi_cmd_t
		{
			ansi_cmd type;

			union
			{
				char c;

				ansi_cursor_up_t     cursor_up;
				ansi_cursor_down_t   cursor_down;
				ansi_cursor_fwd_t    cursor_fwd;
				ansi_cursor_back_t   cursor_back;
				ansi_cursor_nextln_t cursor_nextln;
				ansi_cursor_prevln_t cursor_prevln;
				ansi_cursor_horabs_t cursor_horabs;
				ansi_cursor_pos_t    cursor_pos;

				ansi_erase_indisp_t erase_indisp;
				ansi_erase_inline_t erase_inline;

				ansi_scroll_up_t   scroll_up;
				ansi_scroll_down_t scroll_down;

				ansi_sgr_parameter_t sgr_parameter;
			};

			ansi_cmd_t()       { this->type = ansi_cmd::NONE; }
			ansi_cmd_t(char c) { this->type = ansi_cmd::PASS; this->c = c; }

			ansi_cmd_t(ansi_cursor_up_t cursor_up)         { this->type = ansi_cmd::CURSOR_UP; this->cursor_up = cursor_up; }
			ansi_cmd_t(ansi_cursor_down_t cursor_down)     { this->type = ansi_cmd::CURSOR_DOWN; this->cursor_down = cursor_down; }
			ansi_cmd_t(ansi_cursor_fwd_t cursor_fwd)       { this->type = ansi_cmd::CURSOR_FWD; this->cursor_fwd = cursor_fwd; }
			ansi_cmd_t(ansi_cursor_back_t cursor_back)     { this->type = ansi_cmd::CURSOR_BACK; this->cursor_back = cursor_back; }
			ansi_cmd_t(ansi_cursor_nextln_t cursor_nextln) { this->type = ansi_cmd::CURSOR_NEXTLN; this->cursor_nextln = cursor_nextln; }
			ansi_cmd_t(ansi_cursor_prevln_t cursor_prevln) { this->type = ansi_cmd::CURSOR_PREVLN; this->cursor_prevln = cursor_prevln; }
			ansi_cmd_t(ansi_cursor_horabs_t cursor_horabs) { this->type = ansi_cmd::CURSOR_HORABS; this->cursor_horabs = cursor_horabs; }
			ansi_cmd_t(ansi_cursor_pos_t cursor_pos)       { this->type = ansi_cmd::CURSOR_POS; this->cursor_pos = cursor_pos; }

			ansi_cmd_t(ansi_erase_indisp_t erase_indisp) { this->type = ansi_cmd::ERASE_INDISP; this->erase_indisp = erase_indisp; }
			ansi_cmd_t(ansi_erase_inline_t erase_inline) { this->type = ansi_cmd::ERASE_INLINE; this->erase_inline = erase_inline; }

			ansi_cmd_t(ansi_scroll_up_t scroll_up)     { this->type = ansi_cmd::SCROLL_UP; this->scroll_up = scroll_up; }
			ansi_cmd_t(ansi_scroll_down_t scroll_down) { this->type = ansi_cmd::SCROLL_DOWN; this->scroll_down = scroll_down; }

			ansi_cmd_t(ansi_sgr_parameter_t sgr_parameter) { this->type = ansi_cmd::SGR_PARAMETER; this->sgr_parameter = sgr_parameter; }
		};

		// State machine

		struct ansi_t
		{
			int state = 0;
			int num[2] = { 1, 1 };

			ansi_cmd_t consume(char c);
		};
	}
}

#endif
