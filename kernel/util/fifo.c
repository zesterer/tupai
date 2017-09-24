//
// file : fifo.c
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

#include <tupai/util/fifo.h>

void fifo_init(fifo_t* fifo, uint8_t* data, size_t size)
{
	fifo->front = 0;
	fifo->back = 0;
	fifo->len = 0;

	fifo->data = data;
	fifo->size = size;
}

bool fifo_get(fifo_t* fifo, size_t index, uint8_t* ret)
{
	if (fifo->size > index)
	{
		size_t offset = (fifo->front + index) % fifo->size;
		*ret = fifo->data[offset];
		return true;
	}
	else
		return false;
}

bool fifo_push(fifo_t* fifo, uint8_t byte)
{
	if (fifo->len < fifo->size)
	{
		fifo->back = (fifo->back + 1) % fifo->size;
		fifo->data[fifo->back] = byte;
		fifo->len ++;
		return true;
	}
	else
		return false;
}

bool fifo_pop(fifo_t* fifo, uint8_t* ret)
{
	if (fifo->len > 0)
	{
		*ret = fifo->data[fifo->front];
		fifo->front = (fifo->front + 1) % fifo->size;
		return true;
	}
	else
		return false;
}
