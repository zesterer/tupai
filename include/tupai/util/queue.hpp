//
// file : queue.hpp
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
