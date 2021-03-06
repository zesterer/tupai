//
// file : ref.hpp
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

#ifndef TUPAI_UTIL_REF_HPP
#define TUPAI_UTIL_REF_HPP

#include <tupai/util/type.hpp>

namespace tupai::util
{
	//! UniqueRef<T>
	//! Provides automatic construction / destruction of a singly-owned heap-allocated object

	template <typename T>
	struct UniqueRef
	{
	private:
		T* _item = nullptr;

		UniqueRef(T* item) : _item(item) {}
		UniqueRef(const UniqueRef<T>& other) = delete; // Non-copyable
		UniqueRef<T>& operator=(const UniqueRef<T>&) = delete; // Non-copyable

	public:
		template <typename... Args>
		static UniqueRef<T> create(Args... args)
		{
			return UniqueRef<T>(new T(args...));
		}

		~UniqueRef()
		{
			if (this->_item != nullptr)
				delete this->_item;
		}

		bool valid() { return this->_item != nullptr; }

		T& raw()
		{
			return *this->_item;
		}

		T& operator*()
		{
			if (this->valid())
				return *this->_item;
			else
				panic("Attempted to access invalid UniqueRef<{0}> item", type_name<T>());
		}
	};
}

#endif
