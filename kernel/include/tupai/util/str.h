//
// file : str.h
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

#ifndef TUPAI_UTIL_STR_H
#define TUPAI_UTIL_STR_H

#include <tupai/util/mem.h>
#include <tupai/type.h>

inline size_t str_copy(const char* src, char* tgt)
{
	size_t i = 0;
	for (; src[i] != '\0'; i ++)
		tgt[i] = src[i];
	tgt[i] = '\0';
	return i;
}

inline size_t str_cat(char* tgt, const char* src)
{
	size_t i, j;
	for (i = 0; tgt[i] != '\0'; i ++);
	for (j = 0; src[j] != '\0'; j ++)
		tgt[i + j] = src[j];
	tgt[i + j] = '\0';
	return i + j;
}

inline bool str_equal(const char* str1, const char* str2)
{
	for (size_t i = 0; str1[i] != '\0'; i ++)
		if (str1[i] != str2[i])
			return false;
	return true;
}

inline size_t str_len(const char* str)
{
	size_t i = 0;
	for (; str[i] != '\0'; i ++);
	return i;
}

inline char* str_new(const char* str)
{
	char* nstr = alloc((str_len(str) + 1) * sizeof(char));
	str_copy(str, nstr);
	return nstr;
}

#endif
