/*
* 	file : mem.hpp
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

#ifndef TUPAI_UTIL_MEM_HPP
#define TUPAI_UTIL_MEM_HPP

// Tupai
#include <tupai/type.hpp>
#include <tupai/util/safetype.hpp>
#include <tupai/mempool.hpp>

namespace tupai
{
	namespace util
	{
		template<typename T>
		safeptr<T> alloc(umem n = 1)
		{
			T* ptr = (T*)mempool_alloc(sizeof(T) * n);
			return safeptr<T>(ptr, ptr != nullptr);
		}

		template<typename T>
		void dealloc(T* ptr)
		{
			mempool_dealloc(ptr);
		}

		const umem MEM_ALIGN_BYTE = 1;
		const umem MEM_ALIGN_WORD = 2;
		const umem MEM_ALIGN_LONG = 4;
		const umem MEM_ALIGN_QUAD = 8;
		const umem MEM_ALIGN_KB = 1024;
		const umem MEM_ALIGN_MB = 1024 * 1024;
		const umem MEM_ALIGN_GB = 1024 * 1024 * 1024;
		extern const umem MEM_ALIGN_PAGE;
		umem align_floor(umem n, umem align);
		umem align_ceiling(umem n, umem align);
	}
}

inline void* operator new(size_t, void *p)     throw() { return p; }
inline void* operator new[](size_t, void *p)   throw() { return p; }
inline void  operator delete  (void *, void *) throw() {}
inline void  operator delete[](void *, void *) throw() {}

#endif
