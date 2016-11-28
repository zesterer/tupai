/*
* 	file : ringbuff.hpp
*
* 	This file is part of Tupai.
*
* 	Tupai is free software: you can redistribute it and/or modify
* 	it under the terms of the GNU General Public License as published by
* 	the Free Software Foundation, either version 3 of the License, or
* 	(at your option) any later version.
*
* 	Tupai is distributed in the hope that it will be useful,
* 	but WITHOUT ANY WARRANTY; without even the implied warranty of
* 	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
* 	GNU General Public License for more details.
*
* 	You should have received a copy of the GNU General Public License
* 	along with Tupai.  If not, see <http://www.gnu.org/licenses/>.
*/

#ifndef TUPAI_GENERIC_RINGBUFF_HPP
#define TUPAI_GENERIC_RINGBUFF_HPP

// Tupai
#include <tupai/type.hpp>
#include <tupai/kpanic.hpp>
#include <tupai/tty.hpp>

namespace tupai
{
	namespace generic
	{
		extern "C" umem tmp_var;
		umem tmp_var;

		template<typename T>
		struct ringbuff
		{
			T* data = nullptr;
			umem size = 0;

			umem len = 0;
			umem front = 0;
			umem back = 0;

			void init(umem size)
			{
				this->size = size;
				this->len = 0;
				this->front = 0;
				this->back = 0;
				this->data = new T[this->size];
			}

			ringbuff()
			{
			}

			ringbuff(umem size)
			{
				this->init(size);
			}

			~ringbuff()
			{
				delete this->data;
			}

			T& operator[](const int index)
			{
				return this->data[(this->back + index) % this->len];
			}

			void push(T& item)
			{
				if (this->len >= this->size)
					kpanic("Attempted to push to full ring buffer");

				tty_write_str("Data location is ");
				tty_write('0' + ((umem)this->data / 1000000000) % 10);
				tty_write('0' + ((umem)this->data / 100000000) % 10);
				tty_write('0' + ((umem)this->data / 10000000) % 10);
				tty_write('0' + ((umem)this->data / 1000000) % 10);
				tty_write('0' + ((umem)this->data / 100000) % 10);
				tty_write('0' + ((umem)this->data / 10000) % 10);
				tty_write('0' + ((umem)this->data / 1000) % 10);
				tty_write('0' + ((umem)this->data / 100) % 10);
				tty_write('0' + ((umem)this->data / 10) % 10);
				tty_write('0' + ((umem)this->data / 1) % 10);
				tty_write_str("!\n");

				this->data[this->front] = item;

				tty_write_str("Push 1!\n");
				this->front = (this->front + 1) % this->len;
				tty_write_str("Push 2!\n");
				this->len ++;
			}

			T& pop()
			{
				if (this->len <= 0)
					kpanic("Attempted to pop from empty ring buffer");

				umem tmp_back = this->back;

				this->back = (this->back + 1) % this->len;
				this->len --;

				return this->data[tmp_back];
			}

			umem length()
			{
				return this->len;
			}
		};
	}
}

#endif
