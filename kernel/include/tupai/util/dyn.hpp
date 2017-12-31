//
// file : dyn.hpp
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

#ifndef TUPAI_UTIL_DYN_HPP
#define TUPAI_UTIL_DYN_HPP

#include <tupai/util/def/dyn.hpp>
#include <tupai/util/mem.hpp>
#include <tupai/mem/kheap.hpp>
#include <tupai/util/panic.hpp>
#include <tupai/util/typedata.hpp>

namespace tupai::util
{
	inline Box<mem::Pool> mem_heap() { return mem::kheap::heap; }

	template <typename T>
	Dyn<T>& Dyn<T>::copy(const Dyn<T>& other)
	{
		this->_len = other._len;
		this->_items = mem_heap()->alloc<T>(this->_len).expect("Could not allocate dynamic object(s) of type {}", type_name<T>());

		for (size_t i = 0; i < this->_len; i ++)
			this->_items[i] = other._items[i];

		return *this;
	}

	template <typename T>
	Dyn<T>::~Dyn()
	{
		for (size_t i = 0; i < this->_len; i ++)
			this->_items[i].~T();

		mem_heap()->dealloc(this->_items);
	}

	template <typename T>
	T& Dyn<T>::item()
	{
		panic_if(this->_len == 0, "Tried to access invalid dynamic object of type {}", type_name<T>());
		return this->_items[0];
	}

	template <typename T>
	T& Dyn<T>::item(size_t i)
	{
		panic_if(i >= this->_len, "Tried to access invalid dynamic object at index {}", i);
		return this->_items[i];
	}

	template <typename T> template <typename ... Args> Dyn<T> Dyn<T>::create(Args ... args)
	{
		T* ptr = mem_heap()->alloc<T>(1).expect("Could not allocate object of type {}", type_name<T>());
		new (ptr) T; // Construct object
		return Dyn<T>(ptr, 1);
	}

	template <typename T> template <typename ... Args> Dyn<T> Dyn<T>::create_arr(size_t len, Args ... args)
	{
		T* ptr = mem_heap()->alloc<T>(len).expect("Could not allocate array of size {} and type {}", len, type_name<T>());
		new (ptr) T[len]; // Construct objects
		return Dyn<T>(ptr, len);
	}
}

#endif
