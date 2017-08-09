//
// file : ansi.cpp
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

// Tupai
#include <tupai/util/ansi.hpp>
#include <tupai/util/char.hpp>

namespace tupai
{
	namespace util
	{
		ansi_cmd_t ansi_t::consume(char c)
		{
			ansi_cmd_t cmd;

			switch (this->state)
			{
			case 0:
				{
					switch (c)
					{
					case '\x1B':
						this->state = 1;
						break;

					default:
						cmd = ansi_cmd_t(c);
						break;
					}
				}
				break;

			case 1:
				{
					this->num[0] = 0;
					this->num[1] = 0;

					switch (c)
					{
					case '[':
						this->state = 2;
						break;

					default:
						cmd = ansi_cmd_t(c);
						break;
					}
				}
				break;

			case 2:
				{
					if (is_digit(c))
					{
						this->num[0] *= 10;
						this->num[0] += digit_to_num(c);
					}
					else
					{
						if (c == 'm')
							cmd = ansi_cmd_t(ansi_sgr_parameter_t(num[0]));

						this->state = 0;
					}
				}
				break;

			default:
				{
					this->state = 0;
					this->num[0] = 0;
					this->num[1] = 0;
				}
				break;
			}

			return cmd;
		}
	}
}
