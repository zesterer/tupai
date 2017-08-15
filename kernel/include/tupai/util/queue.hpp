//
// file : queue.hpp
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

#ifndef TUPAI_UTIL_QUEUE_HPP
#define TUPAI_UTIL_QUEUE_HPP

// Tupai
#include <tupai/util/mutex.hpp>

// Standard
#include <stddef.h>
#include <stdint.h>

namespace tupai
{
	namespace util
	{
		template <typename T, typename S, S N>
		struct _queue_t
		{
		public:
			T items[N];
			S head = 0;
			S tail = 0;
			S length = 0;

		public:
			_queue_t()
			{
				this->head = 0;
				this->tail = 0;
				this->length = 0;
			}

			void push(T item)
			{
				if (this->length != 0)
				{
					this->head = (this->head + 1) % N;
					if (this->head == this->tail)
						this->tail = (this->tail + 1) % N;
				}

				this->items[this->head] = item;

				if (this->length < N)
					this->length ++;
			}

			void push(T item) volatile
			{
				if (this->length != 0)
				{
					this->head = (this->head + 1) % N;
					if (this->head == this->tail)
						this->tail = (this->tail + 1) % N;
				}

				this->items[this->head] = item;

				if (this->length < N)
					this->length ++;
			}

			T pop()
			{
				this->length --;

				T item = this->items[this->tail];

				if (this->length != 0)
					this->tail = (this->tail + 1) % N;

				return item;
			}

			T pop() volatile
			{
				this->length --;

				T item = this->items[this->tail];

				if (this->length != 0)
					this->tail = (this->tail + 1) % N;

				return item;
			}

			S len() { return this->length; }
			S len() volatile { return this->length; }
		};

		template <typename T, size_t N>
		using queue_t = _queue_t<T, size_t, N>;

		template <typename T, size_t N>
		using unsafe_queue_t = _queue_t<volatile T, volatile size_t, N>;
	}
}

#endif
