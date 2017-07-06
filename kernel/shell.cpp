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

#include <tupai/dev/clock.hpp>

#include <tupai/sys/kmem.hpp>
#include <tupai/sys/thread.hpp>
#include <tupai/sys/mmap.hpp>
#include <tupai/sys/call.hpp>

#include <tupai/util/in.hpp>
#include <tupai/util/out.hpp>
#include <tupai/util/str.hpp>
#include <tupai/util/vector.hpp>

#include <tupai/fs/vfs.hpp>

namespace tupai
{
	void shell_motd()
	{
		util::println(tupai_get_name_decorative(), " ",
			tupai_get_version(), " on ",
			arch_get_family(), "/", arch_get_target(), '\n',
			"Copyright 2017, ", P_MAINTAINER_NAME);
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

		// Display information
		shell_motd();
		util::println("Type 'help' for more info.\n");

		bool halted = false;
		while (!halted)
		{
			util::print("$ ");

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
				util::print(
					"Available commands:\n",
					"  help    -> Show this help text\n",
					"  threads -> Show running threads\n",
					"  fs      -> Show filesystem tree\n",
					"  cat     -> Display the contents of a file\n",
					"  pool    -> Show kernel memory pool\n",
					"  motd    -> Show the MOTD\n",
					"  div     -> Trigger a divided-by-zero exception\n",
					"  panic   -> Trigger a kernel panic\n",
					"  info    -> Show system info\n",
					"  time    -> Show the system time\n"
				);
			}
			else if (util::str_equal(argv[0], "threads"))
			{
				size_t n = sys::threads_count();
				for (size_t i = 0; i < n; i ++)
				{
					char name[64];
					sys::id_t id = sys::thread_get_id(i);
					sys::thread_get_name(id, name);
					util::println(i, ' ', id, ' ', name);
				}
			}
			else if (util::str_equal(argv[0], "fs"))
			{
				fs::vfs_display();
			}
			else if (util::str_equal(argv[0], "cat"))
			{
				if (argc > 1)
				{
					id_t cfile;
					const char* path = argv[1];
					sys::call(sys::CALL::OPEN, (size_t)path, (size_t)&cfile);

					char rbuff[512];
					ssize_t n = 511;
					sys::call(sys::CALL::READ, (size_t)cfile, (size_t)&n, (size_t)rbuff);

					if (n > 0)
						util::print(rbuff);

					sys::call(sys::CALL::CLOSE, (size_t)cfile);
				}
				else
					util::println("Please specify a file!");
			}
			else if (util::str_equal(argv[0], "pool"))
			{
				sys::kmem_display();
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
				util::print(
					"System Info:\n",
					"  address_size -> ", sizeof(void*) * 8, " bits\n",
					"  kernel_start -> ", (void*)arch_get_kernel_start(), '\n',
					"  kernel_end   -> ", (void*)arch_get_kernel_end(), '\n'
				);
				sys::kmem_info();
				sys::mmap_display();
			}
			else if (util::str_equal(argv[0], "time"))
			{
				datetime_t time = dev::clock_read();

				util::println(
					"The system time is ",
					util::fmt_int<short        >(time.year, 10, 4), '-', util::fmt_int<unsigned char>(time.month, 10, 2), '-', util::fmt_int<unsigned char>(time.day, 10, 2), ' ',
					util::fmt_int<unsigned char>(time.hour, 10, 2), ':', util::fmt_int<unsigned char>(time.min  , 10, 2), ':', util::fmt_int<unsigned char>(time.sec, 10, 2)
				);
			}
			else if (util::str_len(argv[0]) > 0)
				util::println("Command '", argv[0], "' not found!");
		}
	}
}
