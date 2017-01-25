/*
* 	file : kbd.hpp
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

#ifndef TUPAI_I686_KBD_HPP
#define TUPAI_I686_KBD_HPP

// Tupai
#include <tupai/type.hpp>
#include <tupai/generic/ringbuff.hpp>

namespace tupai
{
	struct KeyEvent
	{
		enum class Type
		{
			PRESSED,
			RELEASED,
		};

		uint8 row;
		uint8 col;
		Type type;
		uint8 flags; // 0 = Shift pressed, 1 = Alt pressed, 2 = Ctrl pressed
	};

	extern generic::ringbuff<char> kbd_ringbuffer;
	void kbd_init();
	void kbd_enable();
}

#endif
