/*
* 	file : mempool.hpp
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

#ifndef TUPAI_MEMPOOL_HPP
#define TUPAI_MEMPOOL_HPP

// Tupai
#include <tupai/type.hpp>
#include <tupai/i686/i686.hpp>

namespace tupai
{
	const umem mempool_begin = 0x1000000 + KERNEL_VIRTUAL_OFFSET; // 16M offset + Kernel virtual offset
	const umem mempool_size   = 0x1000000; // 16M size

	void  mempool_init(void* ptr, umem size, umem blocksize);
	void* mempool_alloc(umem n);
	void* mempool_realloc(void* ptr, umem n);
	void  mempool_dealloc(void* ptr);
}

#endif
