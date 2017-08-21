//
// file : stdio.cpp : Input/Output operations
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
#include <tupai/type.hpp>
#include <tupai/sys/call.hpp>

// LibC
#include <stdio.h>

// Standard
#include <stdint.h>

// Standard streams
FILE* stdin  = (FILE*)0;
FILE* stdout = (FILE*)1;
FILE* stderr = (FILE*)2;

// File access
FILE* fopen(const char* filename, const char* mode)
{
	(void)mode;

	tupai::id_t fd;
	tupai::sys::call(tupai::sys::CALL::OPEN, (size_t)filename, (size_t)&fd);

	if (fd == -1)
		return NULL;
	else
		return (FILE*)(fd);
}

int fclose(FILE* stream)
{
	tupai::id_t fd = (tupai::id_t)stream;
	tupai::sys::call(tupai::sys::CALL::CLOSE, (size_t)fd);

	return 0;
}

// File I/O
size_t fread (void* ptr, size_t size, size_t count, FILE* stream)
{
	size_t n = size * count;
	tupai::id_t fd = (tupai::id_t)stream;
	tupai::sys::call(tupai::sys::CALL::READ, (size_t)fd, (size_t)&n, (size_t)ptr);
	return n;
}

size_t fwrite(const void* ptr, size_t size, size_t count, FILE* stream)
{
	size_t n = size * count;
	tupai::id_t fd = (tupai::id_t)stream;
	tupai::sys::call(tupai::sys::CALL::WRITE, (size_t)fd, (size_t)&n, (size_t)ptr);
	return n;
}

// TODO : Complete this
