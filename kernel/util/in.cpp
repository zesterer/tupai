//
// file : mutex.cpp
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

// Tupai
#include <tupai/util/in.hpp>
#include <tupai/dev/tty.hpp>

namespace tupai
{
	namespace util
	{
		void __readline(char* buff, size_t n)
		{
			size_t i = 0;
			while (i + 1 < n)
			{
				char c = dev::tty_read();
				dev::tty_write(c);

				if (c == '\r' || c == '\n')
					break;

				if (c != '\0')
				{
					buff[i] = c;
					i ++;
				}
			}
			buff[i] = '\0';
		}

	}
}
