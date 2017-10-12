//
// file : path.c
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

#include <tupai/util/path.h>
#include <tupai/util/str.h>

const char* path_element_start(const char* path, size_t n)
{
	size_t c = 0;
	bool inelem = false;
	for (size_t i = 0; path[i] != '\0'; i ++)
	{
		switch (path[i])
		{
			case '/':
				if (inelem)
				{
					c ++;
					inelem = false;
				}
				break;

			default:
				if (c == n)
					return &path[i];
				else
					inelem = true;
				break;
		}
	}

	return nullptr;
}

bool path_extract(const char* path, size_t n, char buff[ELEMENT_MAX_LEN + 1])
{
	const char* start = path_element_start(path, n);
	if (start == nullptr)
		return false;
	else
	{
		size_t i = 0;
		for (; *start != '\0' && *start != '/'; start ++)
		{
			if (i >= ELEMENT_MAX_LEN - 1)
				break;
			else
				buff[i] = *start;
			i ++;
		}
		buff[i] = '\0';
		return true;
	}
}

size_t path_elements(const char* path)
{
	size_t c = 0;
	bool inelem = false;
	for (size_t i = 0; path[i] != '\0'; i ++)
	{
		switch (path[i])
		{
			case '/':
				if (inelem)
				{
					c ++;
					inelem = false;
				}
				break;

			default:
				inelem = true;
				break;
		}
	}

	return c + (inelem ? 1 : 0);
}

bool path_isabs(const char* path)
{
	return path[0] == '/';
}

bool path_extract_base(const char* path, char buff[PATH_MAX_LEN + 1])
{
	size_t nelem = path_elements(path);

	if (nelem == 0 || (!path_isabs(path) && nelem <= 1))
		return false;
	else
	{
		buff[0] = '\0';

		if (path_isabs(path))
			str_cat(buff, "/");

		for (size_t i = 0; i < nelem - 1; i ++)
		{
			if (i > 0)
				str_cat(buff, "/");

			char tmp[ELEMENT_MAX_LEN + 1];
			path_extract(path, i, tmp);
			str_cat(buff, tmp);
		}

		return true;
	}
}
