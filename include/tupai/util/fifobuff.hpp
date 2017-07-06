//
// file : fifobuff.hpp
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

#ifndef TUPAI_UTIL_FIFOBUFF_HPP
#define TUPAI_UTIL_FIFOBUFF_HPP

// Tupai
#include <tupai/util/mutex.hpp>
#include <tupai/util/spinlock.hpp>

// Standard
#include <stddef.h>
#include <stdint.h>

namespace tupai
{
	namespace util
	{
		template <typename T, size_t SIZE>
		struct fifobuff_t;

		template <typename T, size_t SIZE> void   __fifo_write(volatile fifobuff_t<T, SIZE>& buff, T c);
		template <typename T, size_t SIZE> T      __fifo_read (volatile fifobuff_t<T, SIZE>& buff);
		template <typename T, size_t SIZE> size_t __fifo_len  (volatile fifobuff_t<T, SIZE>& buff);

		template <typename T, size_t SIZE>
		struct fifobuff_t
		{
		public:
			volatile T      arr[SIZE] = { 0, };
			volatile size_t head   = 0;
			volatile size_t tail   = 0;
			volatile size_t length = 0;

			util::mutex_t spinlock;

		public:
			void    write(T c) volatile { return __fifo_write(*this, c); }
			uint8_t read()     volatile { return __fifo_read(*this); }
			size_t  len()      volatile { return __fifo_len(*this); }

			void    write_unsafe(T c) volatile { return __fifo_write_unsafe(*this, c); }
			uint8_t read_unsafe()     volatile { return __fifo_read_unsafe(*this); }
			size_t  len_unsafe()      volatile { return __fifo_len_unsafe(*this); }
		};

		template <typename T, size_t SIZE>
		void __fifo_write_unsafe(volatile fifobuff_t<T, SIZE>& buff, T c)
		{
			if (buff.length != 0)
			{
					buff.head = (buff.head + 1) % SIZE;
					if (buff.head == buff.tail)
						buff.tail = (buff.tail + 1) % SIZE;
			}

			buff.arr[buff.head] = c;

			if (buff.length < SIZE)
				buff.length ++;
		}

		template <typename T, size_t SIZE>
		void __fifo_write(volatile fifobuff_t<T, SIZE>& buff, T c)
		{
			buff.spinlock.lock(); // Begin critical section

			__fifo_write_unsafe(buff, c);

			buff.spinlock.unlock(); // End critical section
		}

		template <typename T, size_t SIZE>
		T __fifo_read_unsafe(volatile fifobuff_t<T, SIZE>& buff)
		{
			// Wait loop
			while (true)
			{
				// Wait until data appears
				while (__fifo_len_unsafe(buff) <= 0);

				// Double-check to make sure there's STILL data
				if (buff.length > 0)
					break; // Exit to start reading data
			}

			buff.length --;

			T val = buff.arr[buff.tail];

			if (buff.length != 0)
					buff.tail = (buff.tail + 1) % SIZE;

			return val;
		}

		template <typename T, size_t SIZE>
		T __fifo_read(volatile fifobuff_t<T, SIZE>& buff)
		{
			// Wait loop
			while (true)
			{
				// Wait until data appears
				while (__fifo_len(buff) <= 0);

				// Lock the buffer while we check
				buff.spinlock.lock(); // Begin critical section

				// Double-check to make sure there's STILL data
				if (buff.length > 0)
					break; // Exit to start reading data
				else
					buff.spinlock.unlock(); // Somebody jumped the queue. Damn!
			}

			buff.length --;

			T val = buff.arr[buff.tail];

			if (buff.length != 0)
					buff.tail = (buff.tail + 1) % SIZE;

			buff.spinlock.unlock(); // End critical section

			return val;
		}

		template <typename T, size_t SIZE>
		size_t __fifo_len_unsafe(volatile fifobuff_t<T, SIZE>& buff)
		{
			size_t len = buff.length;

			return len;
		}
		template <typename T, size_t SIZE>
		size_t __fifo_len(volatile fifobuff_t<T, SIZE>& buff)
		{
			buff.spinlock.lock(); // Begin critical section

			size_t len = __fifo_len_unsafe(buff);

			buff.spinlock.unlock(); // End critical section

			return len;
		}
	}
}

#endif
