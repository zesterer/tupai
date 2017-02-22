/*
* 	file : prompt.cpp
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

// Tupai
#include <tupai/prompt.hpp>
#include <tupai/tty.hpp>
#include <tupai/timer.hpp>

#include <tupai/util/conv.hpp>
#include <tupai/util/mem.hpp>
#include <tupai/util/cstr.hpp>

#include <tupai/fs/fs.hpp>

#include <tupai/prog/snake.hpp>
#include <tupai/prog/adventure.hpp>
#include <tupai/prog/sys.hpp>
#include <tupai/prog/timer.hpp>

#include <tupai/i686/pit.hpp>

// Libk
#include <libk/stdlib.hpp>
#include <libk/stdio.hpp>
#include <libk/string.hpp>

namespace tupai
{
	void readline(char* buffer, umem buffer_size)
	{
		umem buffer_pos = 0;
		while (true)
		{
			char input_char = libk::getchar();

			if (input_char != '\0')
			{
				if (input_char == '\n' || buffer_pos == buffer_size - 1) // Newline or end of buffer
				{
					buffer[buffer_pos] = '\0';
					return;
				}
				else if (input_char == '\b') // Backspace
				{
					if (buffer_pos > 0)
					{
						buffer_pos --;
						libk::putchar('\b');
					}
				}
				else
				{
					tty_write(input_char);

					buffer[buffer_pos] = input_char;
					buffer_pos ++;
				}
			}
		}
	}

	struct command_pair
	{
		int (*main_func)(int argc, char* argv[]);
		const char* cmd_name;

		command_pair(int (*main_func)(int argc, char* argv[]), const char* cmd_name)
		{
			this->main_func = main_func;
			this->cmd_name = cmd_name;
		}
	};

	int prompt()
	{
		command_pair commands[] =
		{
			command_pair(prog::sys_main, "sys"),
			command_pair(prog::snake_main, "snake"),
			command_pair(prog::adventure_main, "adventure"),
			command_pair(prog::timer_main, "timer"),
		};

		tty_write_str("Type 'help' for more information\n");

		char initial_dir[] = "/";
		char* current_dir = util::alloc<char>(sizeof(initial_dir) / sizeof(char)).val();
		util::cstr_copy(initial_dir, current_dir);

		while (true)
		{
			tty_set_fg_color(tty_color::CYAN);
			tty_write_str("kernel");
			tty_reset();
			tty_write('@');
			tty_set_fg_color(tty_color::RED);
			tty_write_str("tupai");
			tty_reset();
			tty_write_str(":");
			tty_set_fg_color(tty_color::YELLOW);
			tty_write_str(current_dir);
			tty_reset();
			tty_write_str("> ");

			const umem INPUT_BUFFER_SIZE = 1024;
			char buffer[INPUT_BUFFER_SIZE];
			readline(buffer, INPUT_BUFFER_SIZE);

			tty_write('\n');

			if (libk::strcmp(buffer, "help") == 0)
			{
				tty_write_str("--- Help ---\n");
				tty_write_str("help        - Show this help screen\n");
				tty_write_str("sys         - Show system information\n");
				tty_write_str("snake       - Play a demo snake game\n");
				tty_write_str("adventure   - Play an adventure game\n");
				tty_write_str("timer       - Timer test program\n");
				tty_write_str("uptime      - Find the system uptime\n");
				tty_write_str("chars       - Display printable characters\n");
				tty_write_str("color       - Display printable colors\n");
				tty_write_str("cd <dir>    - Change directory\n");
				tty_write_str("pwd         - View current directory\n");
				tty_write_str("ls          - List files in directory\n");
				tty_write_str("mkdir <dir> - Create a new directory\n");
				tty_write_str("exit        - Close the prompt session\n");
				tty_write_str("clear       - Clear the screen\n");
				tty_write_str("abort       - Abort the system\n");
			}
			else if (libk::strcmp(buffer, "uptime") == 0)
			{
				uint64 ctime = timer_get_nanoseconds();
				uint64 milliseconds = ctime / (1000);
				uint64 seconds = milliseconds / (1000);
				uint64 minutes = seconds / 60;
				uint64 hours = minutes / 60;
				uint64 days = hours / 24;

				libk::printf("Uptime: ");

				if (days > 0)
					libk::printf("%i days, ", days);
				if (hours > 0)
					libk::printf("%i hours, ", hours % 24);
				if (minutes > 0)
					libk::printf("%i mins, ", minutes % 60);
				if (seconds > 0)
					libk::printf("%i sec, ", seconds % 60);
				if (milliseconds > 0)
					libk::printf("%i ms", milliseconds % 1000);

				libk::putchar('\n');
			}
			else if (libk::strcmp(buffer, "exit") == 0)
			{
				return 0;
			}
			else if (libk::strcmp(buffer, "abort") == 0)
			{
				tty_write_str("Aborting...\n");
				libk::abort();
			}
			else if (libk::strcmp(buffer, "clear") == 0)
			{
				tty_clear();
			}
			else if (libk::strcmp(buffer, "chars") == 0)
			{
				for (int i = 32; i < 256; i ++)
				{
					tty_write(i);
					tty_write(' ');
					if ((i + 1) % 32 == 0)
						tty_write('\n');
				}
			}
			else if (libk::strcmp(buffer, "color") == 0)
			{
				const char* names[] = {"  Black  ", "   Red   ", "  Green  ", " Yellow  ", "  Blue   ", " Magenta ", "  Cyan   ", "  White  "};

				for (uint i = 0; i < (sizeof(names) / sizeof(char*)); i ++)
				{
					// Normal
					tty_set_bg_color(i);
					tty_set_fg_color(tty_color::WHITE);
					tty_write(' ');
					tty_write_str(util::compose((uint32)i, 16).val().raw());
					tty_write_str(names[i]);
					tty_reset();

					// Bright
					tty_set_bg_color(i + 8);
					tty_set_fg_color(tty_color::BLACK);
					tty_write(' ');
					tty_write_str(util::compose((uint32)i + 8, 16).val().raw());
					tty_write_str(names[i]);
					tty_reset();

					tty_write('\n');
				}
			}
			else if (buffer[0] == 'c' && buffer[1] == 'd')
			{
				if (buffer[2] == ' ')
				{
					//umem npath_len = util::cstr_length(buffer + 3);
					char* ndir = util::alloc<char>(fs::PATH_MAX_LENGTH + 1).val();

					fs::path_concat(current_dir, buffer + 3, ndir);

					// Check new dir exists
					fs::node* nnode = fs::fs_find(ndir);
					if (nnode != nullptr && nnode->is_directory())
					{
						nnode->get_path(ndir);
						delete current_dir;
						current_dir = ndir;
					}
					else
					{
						delete ndir;
						libk::printf("No such directory '%s'\n", buffer + 3);
					}
				}
				else
				{
					delete current_dir;
					current_dir = util::alloc<char>(sizeof(initial_dir) / sizeof(char)).val();
					util::cstr_copy(initial_dir, current_dir);
				}
			}
			else if (libk::strcmp(buffer, "pwd") == 0)
			{
				libk::printf("%s\n", current_dir);
			}
			else if (libk::strcmp(buffer, "ls") == 0)
			{
				fs::node* current_node = fs::fs_find(current_dir);
				if (current_node != nullptr)
				{
					fs::node* nodebuff[32];
					umem count;
					status_t status = fs::fs_list_children(current_node, nodebuff, &count, 0);

					if (status == 0)
					{
						for (umem i = 0; i < count; i ++)
						{
							const char* nodename = nodebuff[i]->name.str();
							tty_set_fg_color((byte)nodebuff[i]->type + 9);
							libk::printf("%s  ", nodename);
							tty_reset();
						}
						libk::putchar('\n');
					}
					else
						libk::printf("Unknown error\n");
				}
				else
					libk::printf("Invalid path '%s'\n", current_dir);
			}
			else if (buffer[0] == 'm' && buffer[1] == 'k' && buffer[2] == 'd' && buffer[3] == 'i' && buffer[4] == 'r')
			{
				if (buffer[5] == ' ')
				{
					fs::node* nnode = fs::fs_find(current_dir);
					if (nnode != nullptr && nnode->is_directory())
					{
						nnode->add_child(new fs::node(buffer + 6, fs::node_type::DIR));
					}
				}
				else
					libk::printf("Error: directory name not specified\n");
			}
			else
			{
				// Search for a command
				bool found = false;
				for (umem i = 0; i < sizeof(commands) / sizeof(command_pair); i ++)
				{
					if (libk::strcmp(commands[i].cmd_name, buffer) == 0)
					{
						commands[i].main_func(0, nullptr);
						found = true;
					}
				}

				// It's not been found
				if (!found && libk::strlen(buffer) > 0)
				{
					tty_write_str(buffer);
					tty_write_str(": command not found\n");
				}
			}
		}

		return 1;
	}
}
