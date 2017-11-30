//
// file : cpp.hpp
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

#ifndef TUPAI_UTIL_CPP_HPP
#define TUPAI_UTIL_CPP_HPP

#include <tupai/util/type.hpp>
#include <tupai/util/panic.hpp>

// Default placement versions of operator new
inline void* operator new(size_t, void* __p) throw() { return __p; }
inline void* operator new[](size_t, void* __p) throw() { return __p; }

// Default placement versions of operator delete
inline void operator delete  (void*, void*) throw() {}
inline void operator delete[](void*, void*) throw() {}

// Default operator new
inline void* operator new(size_t sz) throw() { tupai::util::panic("Attempted to allocate object of size {}", sz); return nullptr; }
inline void* operator new[](size_t sz) throw() { tupai::util::panic("Attempted to allocate array of size {}", sz); return nullptr; }

// Default operator delete
inline void operator delete  (void* ptr) throw() { tupai::util::panic("Attempted to delete pointer at {}", (uintptr_t)ptr); }
inline void operator delete[](void* arr) throw() { tupai::util::panic("Attempted to delete array at {}", (uintptr_t)arr); }

// Default operator sized delete
inline void operator delete  (void* ptr, size_t sz) throw() { tupai::util::panic("Attempted to delete pointer at {} of size {}", (uintptr_t)ptr, sz); }
inline void operator delete[](void* arr, size_t sz) throw() { tupai::util::panic("Attempted to delete array at {} of size {}", (uintptr_t)arr, sz); }

namespace tupai::util
{
	template< class T > struct remove_ref      {typedef T type;};
	template< class T > struct remove_ref<T&>  {typedef T type;};
	template< class T > struct remove_ref<T&&> {typedef T type;};

	template <typename T>
	typename remove_ref<T>::type&& move(T&& arg) noexcept
	{
		return static_cast<typename remove_ref<decltype(arg)>::type&&>(arg);
	}
}

#endif
