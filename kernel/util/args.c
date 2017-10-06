//
// file : args.c
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

#include <tupai/util/args.h>

static const char* read_var(const char* str, const char* var, const char** ret);
static const char* read_var_all(const char* str);
static const char* read_word(const char* str, const char* word);
static const char* read_word_all(const char* str);
static const char* read_char(const char* str, char c);
static void copy_word(const char* word, char* buff, size_t buff_size);

int find_arg(const char* args, const char* arg, char* buff, size_t buff_size)
{
	const char* nargs = args;
	const char* oargs = nargs;

	while (true)
	{
		const char* ret;

		oargs = nargs;
		if ((nargs = read_var(nargs, arg, &ret)) != oargs)
		{
			copy_word(ret, buff, buff_size);
			return 0;
		}

		oargs = nargs;
		if ((nargs = read_var_all(nargs)) == oargs)
			return 1;
	}
}

const char* read_var(const char* str, const char* var, const char** ret)
{
	const char* nstr = str;
	const char* ostr = nstr;

	ostr = nstr;
	if ((nstr = read_word(nstr, var)) == ostr)
		return str;

	ostr = nstr;
	if ((nstr = read_char(nstr, '=')) == ostr)
		return str;

	*ret = nstr;

	ostr = nstr;
	if ((nstr = read_word_all(nstr)) == ostr)
		return str;

	return nstr;
}

const char* read_var_all(const char* str)
{
	const char* nstr = str;
	const char* ostr = nstr;

	ostr = nstr;
	if ((nstr = read_word_all(nstr)) == ostr)
		return str;

	ostr = nstr;
	if ((nstr = read_char(nstr, '=')) == ostr)
		return str;

	ostr = nstr;
	if ((nstr = read_word_all(nstr)) == ostr)
		return str;

	return nstr;
}

const char* read_word(const char* str, const char* word)
{
	const char* cstr = str;
	while (true)
	{
		switch (*cstr)
		{
			case ' ':
			case '=':
			case '\t':
			case '\0':
			{
				if (*word == '\0')
					return cstr;
				else
					return str;
			}
			break;

			default:
			{
				if (*cstr == *word)
				{
					cstr ++;
					word ++;
				}
				else
					return str;
			}
			break;
		}
	}
}

const char* read_word_all(const char* str)
{
	while (true)
	{
		switch (*str)
		{
			case ' ':
			case '\t':
			case '=':
			case '\0':
				return str;

			default:
				str ++;
				break;
		}
	}
}

const char* read_char(const char* str, char c)
{
	if (*str == c)
		return ++str;
	else
		return str;
}

void copy_word(const char* word, char* buff, size_t buff_size)
{
	for (size_t i = 0; i < buff_size - 1; i ++)
	{
		switch (word[i])
		{
			case ' ':
			case '=':
			case '\t':
			case '\0':
				buff[i] = '\0';
				break;

			default:
				buff[i] = word[i];
				break;
		}
	}

	buff[buff_size - 1] = '\0';
}
