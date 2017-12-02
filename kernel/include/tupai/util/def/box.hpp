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

#ifndef TUPAI_UTIL_DEF_BOX_HPP
#define TUPAI_UTIL_DEF_BOX_HPP

#include <tupai/util/type.hpp>

namespace tupai::util
{
	//! Box<T>
	//! Used to provide a safe late / optional construction abstraction around an object

	template <typename T>
	struct Box
	{
	private:
		union { T _item; };
		bool _constructed;

	public:
		Box() {}

		Box(const Box<T>& other) // Copy construction
		{
			this->_constructed = other._constructed;
			if (this->_constructed)
				this->_item = other._item;
		}

		Box(Box<T>&& other) // Move construction
		{
			this->_constructed = other._constructed;
			if (this->_constructed)
				this->_item = move(other._item);

			// Invalid other
			other._constructed = false;
		}

		~Box() { if (this->_constructed) this->_item.~T(); }

		template <typename ... Args> void create(Args ... args);

		void destroy()
		{
			this->_constructed = false;
			this->_item.~T();
		}

		T& item();

		T* operator->()
		{
			return &this->item();
		}

		T& operator*()
		{
			return this->item();
		}
	};

	//! StaticBox<T>
	//! Used to provide a safe late / optional construction abstraction around a static object

	template <typename T>
	struct StaticBox
	{
	private:
		T* _item;
		bool _constructed;

	public:
		StaticBox() {}

		StaticBox(const StaticBox<T>& other) // Copy construction
		{
			this->_constructed = other._constructed;
			if (this->_constructed)
				this->_item = other._item;
		}

		StaticBox(StaticBox<T>&& other) // Move construction
		{
			this->_constructed = other._constructed;
			if (this->_constructed)
				this->_item = move(other._item);

			// Invalid other
			other._constructed = false;
		}

		~StaticBox() { if (this->_constructed) this->_item->~T(); }

		void place(uintptr_t loc)
		{
			this->_item = reinterpret_cast<T*>(loc);
		}

		template <typename ... Args> void create(Args ... args);

		void destroy()
		{
			this->_constructed = false;
			this->_item->~T();
		}

		T& item();

		T* operator->()
		{
			return &this->item();
		}

		T& operator*()
		{
			return this->item();
		}
	};

	//! UnsafeBox<T>
	//! Used to provide a late / optional construction abstraction around an object
	//! UnsafeBox assumes that the construction state of the object is externally known

	template <typename T>
	struct _UnsafeBox
	{
	private:
		union { T _item; };

		_UnsafeBox(const _UnsafeBox<T>& other); // Copy construction
		_UnsafeBox(_UnsafeBox<T>&& other); // Move construction

	public:
		_UnsafeBox() {}

		void copy_from_unsafe(const _UnsafeBox<T>& other)
		{
			this->_item = other._item;
		}

		void move_from_unsafe(_UnsafeBox<T>&& other)
		{
			this->_item = move(other._item);
		}

		template <typename... Args> void create(Args... args);

		void destroy()
		{
			this->_item.~T();
		}

		T& item()
		{
			return this->_item;
		}

		T* operator->()
		{
			return &this->item();
		}

		T& operator*()
		{
			return this->item();
		}
	};
}

#endif
