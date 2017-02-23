/*
* 	file : stdio.cpp
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

// Libk
#include <libk/stdio.hpp>
#include <libk/stdlib.hpp>
#include <libk/string.hpp>
#include <libk/ctype.hpp>

// Tupai
#include <tupai/util/out.hpp>
#include <tupai/mempool.hpp>
#include <tupai/util/conv.hpp>

#if defined(SYSTEM_ARCH_i686)
	#include <tupai/i686/kbd.hpp>
#endif

namespace libk
{
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

	sint printf(const char* format, ...)
	{
		umem len = 0;
		umem format_len = strlen(format);

		__builtin_va_list args;
		__builtin_va_start(args, format);

		umem last = 0;
		umem i;
		for (i = 0; i < format_len; i ++)
		{
			switch (format[i])
			{
				case '%':
				{
					if (i >= format_len - 1) // Make sure we're not at the end of the string
						break;

					switch (format[i + 1])
					{
						case 's': // We're writing a string
						{
							putsn((char*)&(format[last]), i - last); // Write the string so far
							// Write the string we found in the gap
							char* str = __builtin_va_arg(args, char*);
							putsn((char*)str, strlen(str)); // Write the inserted string
							len += strlen(str);
							last = i + 2; // The string format sequence was 2 characters long
							break;
						}
						case 'd': // We're writing a signed int
						case 'i':
						case 'x': // We're writing a hexadecimal int
						case 'X':
						case 'u': // We're writing an unsigned int
						case 'o': // We're writing an octal int
						{
							putsn((char*)&(format[last]), i - last); // Write the string so far

							// Write the string we found in the gap
							//char str[sizeof(int) * 8 + 1]; // This string is the maximum size an int could possibly be

							sint v = __builtin_va_arg(args, int);

							if (format[i + 1] == 'u') // Is it unsigned?
								v = abs(v);

							sint base = (format[i + 1] == 'x' || format[i + 1] == 'X') ? 16 : ((format[i + 1] == 'o') ? 8 : 10); // Which base are we using?

							//itoa(v, str, base); // Convert it to a string
							auto result = tupai::util::compose<int32>(v, base);
							char* str = result.val().raw_mut();

							if (format[i + 1] == 'x') // Convert it to uppercase if we have to
							{
								for (int i = 0; str[i] != '\0'; i ++)
									str[i] = tolower(str[i]);
							}

							putsn((char*)str, strlen(str)); // Write the inserted integer
							len += strlen(str);
							last = i + 2; // The integer format sequence was 2 characters long
							break;
						}
						case 'c':
						{
							putsn((char*)&(format[last]), i - last); // Write the string so far

							// Write the character we found in the gap
							char character = (char)__builtin_va_arg(args, int);
							putchar((char)character); // Write the inserted character
							len ++;
							last = i + 2; // The character format sequence was 2 characters long
							break;
						}
						default:
							break;
					}
				}
				default:
					break;
			}

			len ++;
		}
		putsn((char*)&(format[last]), i - last); // Write any trailing string

		__builtin_va_end(args);

		return len;
	}

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

	bool getisempty()
	{
		#if defined(SYSTEM_ARCH_i686)
			return tupai::kbd_ringbuffer.length() <= 0;
		#else
			true;
		#endif
	}

	sint getchar()
	{
		#if defined(SYSTEM_ARCH_i686)
			while (true)
			{
				if (tupai::kbd_ringbuffer.length() > 0)
					return tupai::kbd_ringbuffer.pop();
				else
					asm volatile ("int $0x80");
			}
		#else
			return '\0';
		#endif
	}

	char* gets(char* str)
	{
		if (str == nullptr)
			str = (char*)malloc(sizeof(char) * BUFSIZ);

		for (umem i = 0; i < BUFSIZ; i ++)
		{
			char c = getchar();
			if (c == '\n' || c == '\0')
			{
				str[i] = '\0';
				break;
			}
			else
				str[i] = c;
		}

		return str;
	}

	sint putc(sint c, FILE* stream);

	sint putchar(sint c)
	{
		tupai::util::printc(c);
		return 0;
	}

	sint puts(const char* str)
	{
		tupai::util::print(str);
		return 0;
	}

	sint putsn(const char* str, umem n)
	{
		for (umem i = 0; i < n; i ++)
			tupai::util::printc(str[i]);

		return 0;
	}

	sint ungetc(sint c, FILE* stream);

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
