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

		umem align_floor(umem n, umem align = 1024);
		umem align_ceiling(umem n, umem align = 1024);
	}
}

inline void* operator new(size_t, void *p)     throw() { return p; }
inline void* operator new[](size_t, void *p)   throw() { return p; }
inline void  operator delete  (void *, void *) throw() {}
inline void  operator delete[](void *, void *) throw() {}

#endif
