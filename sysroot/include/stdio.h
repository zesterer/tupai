//
// file : stdio.h : Input/Output operations
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
// This file is part of the Tupai C standard library implementation.
//

// Standard
#include <stdint.h>
#include <stddef.h>

// Types
typedef int64_t FILE;
typedef int64_t fpos_t;

// Macros
//#define NULL 0
#define EOF -1
#define FILENAME_MAX 512

// Standard streams
extern FILE* stdin;
extern FILE* stdout;
extern FILE* stderr;

// File access
FILE* fopen(const char* filename, const char* mode);
int   fclose(FILE* stream);

// File I/O
size_t fread (void* ptr, size_t size, size_t count, FILE* stream);
size_t fwrite(const void* ptr, size_t size, size_t count, FILE* stream);

// File positioning
int fseek(FILE* stream, long offset, int origin);

// Output
int puts(const char* str);

// TODO : Complete this
