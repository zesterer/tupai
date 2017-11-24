//
// file : box.hpp
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

#ifndef TUPAI_UTIL_BOX_HPP
#define TUPAI_UTIL_BOX_HPP

#include <tupai/util/panic.hpp>
#include <tupai/util/type.hpp>
#include <tupai/util/cpp.hpp>

namespace tupai::util
{
	template <typename T>
	struct Box
	{
	private:
		bool _constructed;
		union { T _item; };

	public:
		Box() {}
		~Box() { if (this->_constructed) this->_item.~T(); }

		template <typename... Args>
		void create(Args... args)
		{
			new (&this->_item) T(args...);
			this->_constructed = true;
		}

		T* operator->()
		{
			if (this->_constructed)
				return &this->_item;
			else
				panic("Attempted to access unconstructed Box<{0}> item", type_name<T>());
		}

		T& item()
		{
			if (this->_constructed)
				return this->_item;
			else
				panic("Attempted to access unconstructed Box<{0}> item", type_name<T>());
		}
	};
}

#endif
