//
// file : fifo.h
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

#ifndef TUPAI_UTIL_FIFO_H
#define TUPAI_UTIL_FIFO_H

#include <tupai/type.h>

typedef struct fifo
{
	size_t front;
	size_t back;
	size_t len;

	uint8_t* data;
	size_t size;
} fifo_t;

void fifo_init(fifo_t* fifo, uint8_t* data, size_t size);
bool fifo_get(fifo_t* fifo, size_t index, uint8_t* ret);
bool fifo_push(fifo_t* fifo, uint8_t byte);
bool fifo_pop(fifo_t* fifo, uint8_t* ret);

#endif
