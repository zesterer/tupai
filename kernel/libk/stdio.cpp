//
// file : stdio.cpp : Input/Output operations
//
// This file is part of Tupai.
//
// Tupai is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
//
// Tupai is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with Tupai.  If not, see <http://www.gnu.org/licenses/>.
//
// This file is part of the Tupai C standard library implementation.
//

// Tupai
#include <tupai/type.hpp>
#include <tupai/sys/call.hpp>

// LibC
#include <stdio.h>

// Standard
#include <stdint.h>

// Standard streams
FILE* stdin  = (FILE*)0;
FILE* stdout = (FILE*)1;
FILE* stderr = (FILE*)2;

// File access
FILE* fopen(const char* filename, const char* mode)
{
	(void)mode;

	tupai::id_t fd;
	tupai::sys::call(tupai::sys::CALL::OPEN, (size_t)filename, (size_t)&fd);

	if (fd == -1)
		return NULL;
	else
		return (FILE*)(fd + 1);
}

int fclose(FILE* stream)
{
	tupai::id_t fd = (tupai::id_t)stream;
	tupai::sys::call(tupai::sys::CALL::CLOSE, (size_t)fd);

	return 0;
}

// File I/O
size_t fread (void* ptr, size_t size, size_t count, FILE* stream)
{
	size_t n = size * count;
	tupai::id_t fd = (tupai::id_t)stream;
	tupai::sys::call(tupai::sys::CALL::READ, (size_t)fd, (size_t)&n, (size_t)ptr);
	return n;
}

size_t fwrite(const void* ptr, size_t size, size_t count, FILE* stream)
{
	size_t n = size * count;
	tupai::id_t fd = (tupai::id_t)stream;
	tupai::sys::call(tupai::sys::CALL::WRITE, (size_t)fd, (size_t)&n, (size_t)ptr);
	return n;
}

// TODO : Complete this
