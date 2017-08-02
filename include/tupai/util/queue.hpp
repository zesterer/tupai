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

// Standard
#include <stddef.h>
#include <stdint.h>

namespace tupai
{
	namespace util
	{
		template <typename T, size_t N>
		struct queue_t
		{
		public:
			T items[N];
			size_t head = 0;
			size_t tail = 0;
			size_t length = 0;

		public:
			void push(T& item)
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

			size_t len()
			{
				return this->length;
			}
		};
	}
}

#endif
