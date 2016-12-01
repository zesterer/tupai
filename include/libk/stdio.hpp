/*
* 	file : stdio.hpp
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

#ifndef _STDIO_HPP
#define _STDIO_HPP

// Tupai
#include <tupai/type.hpp>

// GCC
#include <stdarg.h>

namespace libk
{
	/* Types */

	struct _file_handle
	{
		ulong id;
	};

	typedef _file_handle FILE;
	typedef umem fpos_t;

	/* Macros */

	const uint BUFSIZ = 512;
	const uint EOF = -1;
	const uint FILENAME_MAX = 255;
	const uint FOPEN_MAX = 8;
	const uint L_tmpnam = 8;
	const uint TMP_MAX = 8;

	/* Operations On Files */

	sint  remove(const char* filename);
	sint  rename(const char* oldname, const char* newname);
	FILE* tmpfile();
	char* tmpnam(char* str);

	// File Access */

	sint  fclose(FILE* stream);
	sint  fflush(FILE* stream);
	FILE* fopen(const char* filename, const char* mode);
	FILE* freopen(const char* filename, const char* mode, FILE* stream);
	void  setbuf(FILE* stream, char* buffer);
	sint  setvbuf(FILE* stream, char* buffer, sint mode, umem size);

	/* Formatted Input/Output */

	sint fprintf(FILE* stream, const char* format, ...);
	sint fscanf(FILE* stream, const char* format, ...);
	sint printf(const char* format, ...);
	sint scanf(const char* format, ...);
	sint snprintf(char* s, umem n, const char* format, ...);
	sint sprintf(char* str, const char* format, ...);
	sint sscanf(const char* s, const char* format, ...);
	sint vfprintf(FILE* stream, const char* format, va_list arg);
	sint vfscanf(FILE* stream, const char* format, va_list arg);
	sint vprintf(const char* format, va_list arg);
	sint vscanf(const char* format, va_list arg);
	sint vsnprintf(char* s, umem n, const char* format, va_list arg);
	sint vsprintf(char* s, const char* format, va_list arg);
	sint vsscanf(const char* s, const char* format, va_list arg);

	/* Character Input/Output */

	bool  fgetisempty(FILE* stream);
	sint  fgetc(FILE* stream);
	char* fgets(char* str, sint n, FILE* stream);
	sint  fputc(sint c, FILE* stream);
	sint  fputs(const char* str, FILE* stream);
	sint  getc(FILE* stream);
	bool  getisempty();
	sint  getchar();
	char* gets(char* str = nullptr);
	sint  putc(sint c, FILE* stream);
	sint  putchar(sint c);
	sint  puts(const char* str);
	sint  putsn(const char* str, umem n);
	sint  ungetc(sint c, FILE* stream);

	/* Direct Input/Output */

	umem fread(void* ptr, umem size, umem nmemb, FILE* stream);
	umem fwrite(const void* ptr, umem size, umem nmemb, FILE* stream);

	/* File Positioning */

	sint  fgetpos(FILE* stream, fpos_t* pos);
	sint  fseek(FILE* stream, slong offset, sint origin);
	sint  fsetpos(FILE* stream, const fpos_t* pos);
	slong tell(FILE* stream);
	void  ewind(FILE* stream);

	/* Error Handling */

	void clearerr(FILE* stream);
	sint feof(FILE* stream);
	sint ferror(FILE* stream);
	void perror(const char* str);
}

#endif
