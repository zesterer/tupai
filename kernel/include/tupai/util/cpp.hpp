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

// Default placement versions of operator new
inline void* operator new(size_t, void* __p) throw();
inline void* operator new[](size_t, void* __p) throw();

// Default placement versions of operator delete
inline void operator delete  (void*, void*) throw();
inline void operator delete[](void*, void*) throw();

// Default operator new
inline void* operator new(size_t sz) throw();
inline void* operator new[](size_t sz) throw();

// Default operator delete
inline void operator delete  (void* ptr) throw();
inline void operator delete[](void* arr) throw();

// Default operator sized delete
inline void operator delete  (void* ptr, size_t sz) throw();
inline void operator delete[](void* arr, size_t sz) throw();

// Delayed implementation to prevent circular references
#include <tupai/util/panic.hpp>
#include <tupai/mem/kheap.hpp>
#include <tupai/util/box.hpp>

// Default placement versions of operator new
inline void* operator new(size_t, void* __p) throw() { return __p; }
inline void* operator new[](size_t, void* __p) throw() { return __p; }

// Default placement versions of operator delete
inline void operator delete  (void*, void*) throw() {}
inline void operator delete[](void*, void*) throw() {}

// Default operator new
inline void* operator new(size_t sz) throw() { return reinterpret_cast<void*>(tupai::mem::kheap::heap->alloc(sz).except("Kernel heap error when attempting to allocate block of size {}", sz)); }
inline void* operator new[](size_t sz) throw() { return reinterpret_cast<void*>(tupai::mem::kheap::heap->alloc(sz).except("Kernel heap error when attempting to allocate block of size {}", sz)); }

// Default operator delete
inline void operator delete  (void* ptr) throw() { tupai::mem::kheap::heap->dealloc(reinterpret_cast<uintptr_t>(ptr)).except("Kernel heap error when attempting to deallocate block at {}", ptr); }
inline void operator delete[](void* arr) throw() { tupai::mem::kheap::heap->dealloc(reinterpret_cast<uintptr_t>(arr)).except("Kernel heap error when attempting to deallocate array at {}", arr); }

// Default operator sized delete
inline void operator delete  (void* ptr, size_t sz) throw() { tupai::mem::kheap::heap->dealloc(reinterpret_cast<uintptr_t>(ptr)).except("Kernel heap error when attempting to deallocate block at {} of size {}", ptr, sz); }
inline void operator delete[](void* arr, size_t sz) throw() { tupai::mem::kheap::heap->dealloc(reinterpret_cast<uintptr_t>(arr)).except("Kernel heap error when attempting to deallocate array at {} of size {}", arr, sz); }

#endif
