//
// file : path.cpp
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
#include <tupai/vfs/path.hpp>
#include <tupai/util/str.hpp>

namespace tupai
{
	namespace vfs
	{
		size_t path_element_count(const char* path)
		{
			size_t count = 0;

			bool separator = true;
			for (size_t i = 0; path[i] != '\0'; i ++)
			{
				switch (path[i])
				{
				case '/':
					separator = true;
					break;

				default:
					{
						if (separator)
							count ++;
						separator = false;
					}
					break;
				}
			}

			return count;
		}

		bool path_element_get(const char* path, char(&buff)[FILENAME_MAX], size_t index)
		{
			size_t count = 0;

			bool separator = true;
			for (size_t i = 0; path[i] != '\0'; i ++)
			{
				switch (path[i])
				{
				case '/':
					separator = true;
					break;

				default:
					{
						if (separator)
						{
							if (count == index)
							{
								size_t j;
								for (j = 0; j < FILENAME_MAX - 1; j ++)
								{
									if (path[i + j] == '/' || path[i + j] == '\0')
										break;
									buff[j] = path[i + j];
								}
								buff[j] = '\0';
								return true;
							}

							count ++;
						}
						separator = false;
					}
					break;
				}
			}

			return false;
		}
	}
}
