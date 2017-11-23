//
// file : kshell.c
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

#include <tupai/kshell.h>
#include <tupai/util/log.h>
#include <tupai/util/str.h>
#include <tupai/dev/console.h>
#include <tupai/cpu.h>

void readline(char* buff)
{
	while (true)
	{
		char c = console_read_in();
		switch (c)
		{
			case '\n':
				*(buff++) = '\0';
				logc(c);
				return;

			case '\0':
				*(buff++) = '\0';
				return;

			default:
				*(buff++) = c;
				logc(c);
				break;
		}
	}
}

void kshell_main(int argc, char* argv[])
{
	(void)argc;
	(void)argv;

	log("Type 'help' for more info\n");
	while (true)
	{
		log("kshell> ");

		char buff[256];
		readline(buff);

		if (str_equal(buff, "help"))
		{
			log("Available commands:\n");
			log(" - help : Show this screen\n");
		}
		else
			logf("Unknown command '%s'\n", buff);
	}
}
