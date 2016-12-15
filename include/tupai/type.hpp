/*
* 	file : type.hpp
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

#ifndef TUPAI_TYPE_HPP
#define TUPAI_TYPE_HPP

// GCC
#include <stddef.h>
#include <stdint.h>

typedef uint8_t ubyte;
typedef int8_t  sbyte;
typedef uint8_t byte;

typedef uint8_t  uint8;
typedef int8_t   sint8;
typedef int8_t   int8;
typedef uint16_t uint16;
typedef int16_t  sint16;
typedef int16_t  int16;
typedef uint32_t uint32;
typedef int32_t  sint32;
typedef int32_t  int32;
typedef uint64_t uint64;
typedef int64_t  sint64;
typedef int64_t  int64;

typedef unsigned short     ushort;
typedef signed short       sshort;
typedef unsigned int       uint;
typedef signed int         sint;
typedef unsigned long      ulong;
typedef signed long        slong;
typedef unsigned long long uquad;
typedef signed long long   squad;

typedef uintptr_t umem;
typedef intptr_t  smem;

typedef int32_t counter_t;

#endif
