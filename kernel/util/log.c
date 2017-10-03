//
// file : log.c
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

#include <tupai/util/log.h>
#include <tupai/util/conv.h>
#include <tupai/dev/console.h>

#include <stdarg.h>

#define LOG_PUTS console_puts
#define LOG_PUTC console_putc

void logf(const char* fmt, ...)
{
	va_list args;
	int state = 0;

	va_start(args, fmt);
	for (; *fmt != '\0'; fmt ++)
	{
		switch (state)
		{
			case 1:
			{
				if (*fmt == '%')
					LOG_PUTC('%');
				else if (*fmt == 's')
					LOG_PUTS(va_arg(args, const char*));
				else if (*fmt == 'c')
					LOG_PUTC((char)va_arg(args, int));
				else if (*fmt == 'i' || *fmt == 'd')
				{
					char buff[I32_STR_MAX];
					if (i32_to_str((int32_t)va_arg(args, int), 10, 0, buff))
						LOG_PUTS(buff);
					else
						LOG_PUTC('!');
				}
				else if (*fmt == 'u')
				{
					char buff[U32_STR_MAX];
					if (u32_to_str((uint32_t)va_arg(args, uint), 10, 0, buff))
						LOG_PUTS(buff);
					else
						LOG_PUTC('!');
				}
				else if (*fmt == 'o')
				{
					char buff[U32_STR_MAX];
					if (u32_to_str((uint32_t)va_arg(args, uint), 8, 0, buff))
						LOG_PUTS(buff);
					else
						LOG_PUTC('!');
				}
				else if (*fmt == 'b')
				{
					char buff[U8_STR_MAX];
					if (u8_to_str((uint8_t)va_arg(args, uint), 2, 8, buff))
						LOG_PUTS(buff);
					else
						LOG_PUTC('!');
				}
				else if (*fmt == 'X')
				{
					char buff[U32_STR_MAX];
					if (u32_to_str((uint32_t)va_arg(args, uint), 16, 0, buff))
						LOG_PUTS(buff);
					else
						LOG_PUTC('!');
				}
				else if (*fmt == 'p')
				{
					LOG_PUTS("0x");
					char buff[PTR_STR_MAX];
					if (ptr_to_str(va_arg(args, size_t), 16, sizeof(size_t) * 2, buff))
						LOG_PUTS(buff);
					else
						LOG_PUTC('!');
				}
				else if (*fmt == 'x')
				{
					LOG_PUTS("<Unrecognised format specifier>");
				}
				else if (*fmt == 'x')
				{
					LOG_PUTS("<Unrecognised format specifier>");
				}
				else
				{
					LOG_PUTC('%');
					LOG_PUTC(*fmt);
				}
				state = 0;
				break;
			}

			default:
			{
				if (*fmt == '%')
					state = 1;
				else
					LOG_PUTC(*fmt);
				break;
			}
		}
	}
	va_end(args);
}

void log(const char* str)
{
	LOG_PUTS(str);
}

void logc(char c)
{
	LOG_PUTC(c);
}
