//
// file : path.cpp
//
// Copyright (c) 2017 Joshua Barretto <joshua.s.barretto@gmail.com>
//
// This file is part of Tupai.
//
// Permission to use, copy, modify, and distribute this software for any
// purpose with or without fee is hereby granted, provided that the above
// copyright notice and this permission notice appear in all copies.
//
// THE SOFTWARE IS PROVIDED "AS IS" AND THE AUTHOR DISCLAIMS ALL WARRANTIES
// WITH REGARD TO THIS SOFTWARE INCLUDING ALL IMPLIED WARRANTIES OF
// MERCHANTABILITY AND FITNESS. IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR
// ANY SPECIAL, DIRECT, INDIRECT, OR CONSEQUENTIAL DAMAGES OR ANY DAMAGES
// WHATSOEVER RESULTING FROM LOSS OF USE, DATA OR PROFITS, WHETHER IN AN
// ACTION OF CONTRACT, NEGLIGENCE OR OTHER TORTIOUS ACTION, ARISING OUT OF
// OR IN CONNECTION WITH THE USE OR PERFORMANCE OF THIS SOFTWARE.
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
