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
#include <tupai/kdebug.hpp>
#include <tupai/util/mutex.hpp>

namespace tupai
{
	namespace generic
	{
		template<typename T>
		struct ringbuff
		{
		private:

			T* data = nullptr;
			volatile umem size = 0;

			volatile umem len = 0;
			volatile umem front = 0;
			volatile umem back = 0;

			volatile bool allow_overflow = true;

			util::mutex mutex;
			bool push_locked = true;

		public:

			void init(umem size)
			{
				this->mutex.lock();

				this->size = size;
				this->data = new T[this->size];

				this->len = 0;
				this->front = 0;
				this->back = 0;

				allow_overflow = true;

				this->mutex.unlock();
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
				this->mutex.lock();

				delete this->data;

				this->mutex.unlock();
			}

			void set_push_locked(bool locked)
			{
				this->push_locked = locked;
			}

			T& operator[](const int index)
			{
				this->mutex.lock();

				T& val = this->data[(this->back + index) % this->len];

				this->mutex.unlock();
				return val;
			}

			void push(T& item)
			{
				if (push_locked)
					this->mutex.lock();

				if (this->len >= this->size && !this->allow_overflow)
					kpanic("Attempted to push to full ring buffer");

				this->data[this->front] = item;

				this->front = (this->front + 1) % this->size;
				this->len ++;

				if (this->len >= this->size) // Overflow onto last entry
				{
					this->back = (this->back + 1) % this->size;
					this->len  = this->size;
				}

				if (push_locked)
					this->mutex.unlock();
			}

			T& pop()
			{
				this->mutex.lock();

				if (this->len <= 0)
					kpanic("Attempted to pop from empty ring buffer");

				umem tmp_back = this->back;

				this->back = (this->back + 1) % this->size;
				this->len --;

				T& val = this->data[tmp_back];

				this->mutex.unlock();

				return val;
			}

			bool is_locked()
			{
				return this->mutex.is_locked();
			}

			void lock()
			{
				this->mutex.lock();
			}

			void unlock()
			{
				this->mutex.unlock();
			}

			umem length()
			{
				this->mutex.lock();

				umem len = this->len;

				this->mutex.unlock();
				return len;
			}
		};
	}
}

#endif
