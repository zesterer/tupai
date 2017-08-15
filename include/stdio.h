//
// file : stdio.h : Input/Output operations
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

// Standard
#include <stdint.h>

// Types
typedef int64_t FILE;
typedef int64_t fpos_t;

// Macros
//#define NULL 0
#define EOF -1
#define FILENAME_MAX 512

// Standard streams
extern FILE* stdin;
extern FILE* stdout;
extern FILE* stderr;

// File access
FILE* fopen(const char* filename, const char* mode);
int   fclose(FILE* stream);

// File I/O
size_t fread (void* ptr, size_t size, size_t count, FILE* stream);
size_t fwrite(const void* ptr, size_t size, size_t count, FILE* stream);

// TODO : Complete this
