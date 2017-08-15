//
// file : table.hpp
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

#ifndef TUPAI_UTIL_TABLE_HPP
#define TUPAI_UTIL_TABLE_HPP

// Tupai
#include <tupai/util/hashtable.hpp>
#include <tupai/util/spinlock.hpp>

namespace tupai
{
	namespace util
	{
		template <typename T, typename Lock>
		struct table_t
		{
			hashtable_t<T> hashtable;
			Lock lock;

			size_t size()
			{
				this->lock.lock(); // Begin critical section
				size_t val = this->hashtable.size();
				this->lock.unlock(); // End critical section
				return val;
			}

			bool contains(id_t key)
			{
				this->lock.lock(); // Begin critical section
				bool val = this->hashtable.contains(key);
				this->lock.unlock(); // End critical section
				return val;
			}

			bool add(id_t key, T item)
			{
				this->lock.lock(); // Begin critical section
				bool val = this->hashtable.add(key, item);
				this->lock.unlock(); // End critical section
				return val;
			}

			bool remove(id_t key)
			{
				this->lock.lock(); // Begin critical section
				bool val = this->hashtable.remove(key);
				this->lock.unlock(); // End critical section
				return val;
			}

			T& operator[](id_t key)
			{
				this->lock.lock(); // Begin critical section
				T& val = *this->hashtable.get(key);
				this->lock.unlock(); // End critical section
				return val;
			}

			T& nth(size_t i)
			{
				this->lock.lock(); // Begin critical section
				T& val = *this->hashtable.nth(i);
				this->lock.unlock(); // End critical section
				return val;
			}

			id_t nth_key(size_t i)
			{
				this->lock.lock(); // Begin critical section
				id_t val = this->hashtable.nth_key(i);
				this->lock.unlock(); // End critical section
				return val;
			}
		};
	}
}

#endif
