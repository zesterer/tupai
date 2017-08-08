//
// file : shell.cpp
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

// Tuapi
#include <tupai/shell.hpp>
#include <tupai/tupai.hpp>
#include <tupai/arch.hpp>
#include <tupai/panic.hpp>

#include <tupai/mem/kmem.hpp>
#include <tupai/mem/mmap.hpp>
#include <tupai/dev/clock.hpp>
#include <tupai/sys/call.hpp>
#include <tupai/task/task.hpp>

#include <tupai/util/in.hpp>
#include <tupai/util/fmt.hpp>
#include <tupai/util/log.hpp>
#include <tupai/util/str.hpp>
#include <tupai/util/vector.hpp>

#include <tupai/vfs/vfs.hpp>

// LibC
#include <stdio.h>

namespace tupai
{
	FILE* stdout;

	char pbuff[2048];
	template <typename... Args>
	void print(Args&&... args)
	{
		util::log(args ...);
		//util::fmt(pbuff, args ...);
		//fwrite(pbuff, 1, util::str_len(pbuff), stdout);
	}

	void shell_motd()
	{
		print(tupai_get_name_decorative(), " ",
			tupai_get_version(), " on ",
			arch_get_family(), "/", arch_get_target(), '\n',
			"Copyright 2017, ", P_MAINTAINER_NAME, '\n');
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

		stdout = fopen("/dev/stdout", "w");

		// Display information
		shell_motd();
		print("Type 'help' for more info.\n\n");

		bool halted = false;
		while (!halted)
		{
			print("$ ");

			char buff[64];
			util::readline(buff);

			char* argv[64];
			size_t argc = split_args(buff, argv, 64);

			if (argc == 0)
			{
				// Nothing
			}
			else if (util::str_equal(argv[0], "help"))
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
			}
			else if (util::str_equal(argv[0], "fs"))
			{
				vfs::display();
			}
			else if (util::str_equal(argv[0], "proc"))
			{
				task::display();
			}
			else if (util::str_equal(argv[0], "mmap"))
			{
				mem::mmap::display();
			}
			else if (util::str_equal(argv[0], "cat"))
			{
				if (argc > 1)
				{
					//const size_t BUFF_SIZE = 8192;
					//char* rbuff = new char[BUFF_SIZE];

					FILE* f = fopen(argv[1], "r");
					//size_t n = fread(rbuff, 1, BUFF_SIZE - 1, f);

					size_t n = 1;
					char buff[2];
					while (n > 0)
					{
						n = fread(&buff, 1, 2, f);
						if (n > 0)
							print(buff[0]);
					}

					fclose(f);
					//delete rbuff;
				}
				else
					print("Usage: cat <file>\n");
			}
			else if (util::str_equal(argv[0], "echo"))
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
			}
			else if (util::str_equal(argv[0], "pool"))
			{
				mem::kmem::log();
			}
			else if (util::str_equal(argv[0], "motd"))
			{
				shell_motd();
			}
			else if (util::str_equal(argv[0], "div"))
			{
				int volatile a = 5 / 0; //
			}
			else if (util::str_equal(argv[0], "panic"))
			{
				panic("Panic triggered artificially");
			}
			else if (util::str_equal(argv[0], "info"))
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
			}
			else if (util::str_equal(argv[0], "time"))
			{
				datetime_t time = dev::clock_read();

				print(
					"The system time is ",
					util::fmt_int<short        >(time.year, 10, 4), '-', util::fmt_int<unsigned char>(time.month, 10, 2), '-', util::fmt_int<unsigned char>(time.day, 10, 2), ' ',
					util::fmt_int<unsigned char>(time.hour, 10, 2), ':', util::fmt_int<unsigned char>(time.min  , 10, 2), ':', util::fmt_int<unsigned char>(time.sec, 10, 2),
					'\n'
				);
			}
			else if (util::str_len(argv[0]) > 0)
				print("Command '", argv[0], "' not found!\n");
		}

		fclose(stdout);
	}
}
