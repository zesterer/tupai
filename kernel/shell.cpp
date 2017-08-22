//
// file : shell.cpp
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

// Tuapi
#include <tupai/shell.hpp>
#include <tupai/tupai.hpp>
#include <tupai/arch.hpp>
#include <tupai/panic.hpp>

// Utility access
#include <tupai/mem/kmem.hpp>
#include <tupai/mem/mmap.hpp>
#include <tupai/dev/clock.hpp>
#include <tupai/sys/call.hpp>
#include <tupai/task/task.hpp>

#include <tupai/util/fmt.hpp>
#include <tupai/util/str.hpp>
#include <tupai/util/vector.hpp>
#include <tupai/util/term.hpp>

#include <tupai/vfs/vfs.hpp>

// LibC
#include <stdio.h>

namespace tupai
{
	char pbuff[2048];
	template <typename... Args>
	void print(Args&&... args)
	{
		util::fmt(pbuff, args ...);
		fwrite(pbuff, 1, util::str_len(pbuff), stdout);
	}

	template <size_t SIZE>
	void readline(char(&buff)[SIZE])
	{
		size_t i = 0;
		while (i + 1 < SIZE)
		{
			char c;
			fread(&c, sizeof(char), 1, stdin);

			if (c == '\b')
			{
				if (i > 0)
				{
					i --;
					print("\b \b");
				}
				continue;
			}
			//else if (c == '\0')
			//	return;
			else
				print(c);

			if (c == '\r' || c == '\n')
				break;
			else if (c != '\0')
			{
				buff[i] = c;
				i ++;
			}
		}
		buff[i] = '\0';
	}

	void shell_motd()
	{
		print(util::TERM_FG_COLOR_CYAN, tupai_get_name_decorative(), " ",
			tupai_get_version(), util::TERM_DEFAULT, " on ",
			util::TERM_FG_COLOR_GREEN, arch_get_family(), "/", arch_get_target(), util::TERM_DEFAULT, '\n',
			util::TERM_FG_COLOR_LIGHT_GRAY, "Copyright 2017, ", KMAINTAINER_NAME, util::TERM_DEFAULT, '\n');
	}

	size_t split_args(char* cmd, char* buff[], size_t size)
	{
		bool in_arg = false;
		int j = 0;

		// Read characters from the command until the null terminator
		for (size_t i = 0; cmd[i] != '\0'; i ++)
		{
			// If it's whitespace, zero and skip it
			if (cmd[i] == ' ' || cmd[i] == '\t')
			{
				cmd[i] = '\0';
				in_arg = false;
			}
			else
			{
				// If it's not whitespace and we just entered a new region, assign it's location to the buffer list
				if (!in_arg)
				{
					if ((size_t)j >= size) // We've reached the buffer limit
						break;

					buff[j] = &cmd[i];
					j ++;
				}
				in_arg = true;
			}
		}

		return j;
	}

	void shell_main(int argc, char* argv[])
	{
		(void)argc;
		(void)argv;

		//stdout = fopen("/dev/stdout", "w");

		// Display information
		shell_motd();
		print("\nType 'help' for more info.\n\n");

		bool halted = false;
		while (!halted)
		{
			print("[", util::TERM_FG_COLOR_RED, "kernel", util::TERM_DEFAULT, "] ");

			char buff[64];
			readline(buff);

			char* argv[64];
			size_t argc = split_args(buff, argv, 64);

			if (argc == 0)
			{
				// Nothing
			}
			else
			{
				switch (util::str_hash(argv[0]))
				{
				case util::str_hash("help"):
					{
						print(
							"Available commands:\n",
							"  help    -> Show this help text\n",
							"  fs      -> Show filesystem tree\n",
							"  proc    -> Show running processes\n",
							"  mmap    -> Show the memory map\n",
							"  cat     -> Display the contents of a file\n",
							"  echo    -> Echo the typed text to stdout\n",
							"  pool    -> Show kernel memory pool\n",
							"  motd    -> Show the MOTD\n",
							"  div     -> Trigger a divided-by-zero exception\n",
							"  panic   -> Trigger a kernel panic\n",
							"  info    -> Show system info\n",
							"  time    -> Show the system time\n"
						);
						break;
					}

				case util::str_hash("fs"):
					{
						vfs::display();
						break;
					}

				case util::str_hash("proc"):
					{
						task::display();
						break;
					}

				case util::str_hash("mmap"):
					{
						mem::mmap::display();
						break;
					}

				case util::str_hash("cat"):
					{
						if (argc > 1)
						{
							//const size_t BUFF_SIZE = 8192;
							//char* rbuff = new char[BUFF_SIZE];

							//util::logln("Opening...");

							FILE* f = fopen(argv[1], "r");
							//size_t n = fread(rbuff, 1, BUFF_SIZE - 1, f);

							//util::logln("Reading...");

							size_t n = 1;
							char buff[2];
							while (n > 0)
							{
								n = fread(buff, sizeof(char), 2, f);
								if (n > 0)
									print(buff[0]);
							}

							fclose(f);
							//delete rbuff;
						}
						else
							print("Usage: cat <file>\n");
						break;
					}

				case util::str_hash("echo"):
					{
						if (argc > 1)
						{
							FILE* f = fopen("/dev/stdout", "w");
							for (size_t i = 1; i < argc; i ++)
							{
								if (i != 1)
									fwrite(" ", 1, 1, f);

								fwrite(argv[i], 1, util::str_len(argv[i]), f);
							}
							fclose(f);

							print('\n');
						}
						break;
					}

				case util::str_hash("pool"):
					{
						mem::kmem::log();
						break;
					}

				case util::str_hash("motd"):
					{
						shell_motd();
						break;
					}

				case util::str_hash("div"):
					{
						int volatile a = 5 / 0; //
						break;
					}

				case util::str_hash("panic"):
					{
						panic("Panic triggered artificially");
						break;
					}

				case util::str_hash("info"):
					{
						print(
							"System Info:\n",
							"  address_size -> ", (long)sizeof(void*) * 8, " bits\n",
							"  kernel_start -> ", (void*)arch_get_kernel_start(), '\n',
							"  kernel_end   -> ", (void*)arch_get_kernel_end(), '\n',
							'\n'
						);
						mem::kmem::info();
						//sys::mmap_display();
						break;
					}

				case util::str_hash("time"):
					{
						datetime_t time = dev::clock_read();

						print(
							"The system time is ",
							util::fmt_int<short        >(time.year, 10, 4), '-', util::fmt_int<unsigned char>(time.month, 10, 2), '-', util::fmt_int<unsigned char>(time.day, 10, 2), ' ',
							util::fmt_int<unsigned char>(time.hour, 10, 2), ':', util::fmt_int<unsigned char>(time.min  , 10, 2), ':', util::fmt_int<unsigned char>(time.sec, 10, 2),
							'\n'
						);
						break;
					}

				default:
					{
						if (util::str_len(argv[0]) > 0)
							print("Command '", argv[0], "' not found!\n");
						break;
					}
				}
			}
		}

		fclose(stdout);
	}
}
