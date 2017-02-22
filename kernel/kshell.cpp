/*
* 	file : kshell.cpp
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
#include <tupai/kshell.hpp>
#include <tupai/tty.hpp>
#include <tupai/timer.hpp>

#include <tupai/fs/fs.hpp>

#include <tupai/util/cstr.hpp>
#include <tupai/util/conv.hpp>
#include <tupai/util/mem.hpp>

// Programs
#include <tupai/prog/snake.hpp>
#include <tupai/prog/sys.hpp>
#include <tupai/prog/adventure.hpp>
#include <tupai/prog/timer.hpp>

// Libk
#include <libk/stdio.hpp>

namespace tupai
{
	status_t readline(char* buffer)
	{
		umem index = 0;

		while (true)
		{
			char c = libk::getchar();

			if (c == '\n' || c == '\0')
			{
				break;
			}
			else if (c == '\b')
			{
				if (index > 0)
				{
					index --;
					tty_write(c);
				}
			}
			else
			{
				buffer[index] = c;
				index ++;
				tty_write(c);
			}
		}

		buffer[index] = '\0';

		return 0;
	}

	status_t parse_args(const char* line, char** argbuff, char* strbuff, int* arg_count)
	{
		umem index = 0;
		argbuff[index] = &strbuff[0];

		bool ws = false;
		bool quotes = false;
		umem i;
		for (i = 0; line[i] != '\0'; i ++)
		{
			if (ws && line[i] != ' ')
			{
				index ++;
				argbuff[index] = &strbuff[i];
				ws = false;
			}

			if (!ws && line[i] == ' ' && !quotes)
			{
				strbuff[i] = '\0';
				ws = true;
			}

			if (!ws)
			{
				if (line[i] == '\'')
				{
					quotes = !quotes;
					if (quotes)
						argbuff[index] += sizeof(char);
					else
						strbuff[i] = '\0';
				}
				else
					strbuff[i] = line[i];
			}
		}

		strbuff[i] = '\0';
		*arg_count = index + 1;

		return 0;
	}

	void kshell_display_help()
	{
		tty_set_fg_color(14);
		tty_write_str("Usage: kshell [options]\n");
		tty_reset();
		tty_write_str("Tupai kernel shell\n\n");
		tty_set_fg_color(11);
		tty_write_str("Options:\n");
		tty_reset();
		tty_write_str("  -h      Display this help screen\n");
		tty_write_str("  -v      Display version\n");
		tty_set_fg_color(11);
		tty_write_str("Special commands:\n");
		tty_reset();
		tty_write_str("  help    Display this help screen\n");
		tty_write_str("  list    List additional commands\n");
		tty_write_str("  clear   Clear the screen\n");
		tty_write_str("  exit    Exit the session\n");
		tty_set_fg_color(11);
		tty_write_str("Utility commands:\n");
		tty_reset();
		tty_write_str("  repeat <n> <cmd> [args] Repeat a command n times\n");
		tty_write_str("  color                   Display color information\n");
		tty_write_str("  chars                   Display character set\n");
		tty_write_str("  uptime                  Display system uptime\n");
		tty_set_fg_color(11);
		tty_write_str("Filesystem commands:\n");
		tty_reset();
		tty_write_str("  ls [dir]     List files in the current directory\n");
		tty_write_str("  pwd          Display the current directory\n");
		tty_write_str("  cd [dir]     Change directory\n");
		tty_write_str("  mkdir <name> Create a new directory\n");
	}

	void kshell_display_version()
	{
		tty_write_str("kshell 0.1\n");
	}

	void kshell_display_list()
	{
		tty_write_str("Additional commands:\n");
		tty_write_str("  snake     Run command-line snake\n");
		tty_write_str("  sys       Display system details\n");
		tty_write_str("  adventure Play an adventure game\n");
		tty_write_str("  timer     Run a test timer\n");
	}

	void kshell_display_color()
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

	void kshell_display_chars()
	{
		for (int i = 32; i < 256; i ++)
		{
			tty_write(i);
			tty_write(' ');
			if ((i + 1) % 32 == 0)
				tty_write('\n');
		}
	}

	void kshell_display_uptime()
	{
		uint64 ctime = timer_get_nanoseconds();
		uint64 milliseconds = ctime / 1000;
		uint64 seconds = ctime / 1000000;
		uint64 minutes = (ctime / 1000000) / 60;
		uint64 hours = (ctime / 1000000) / 3600;
		uint64 days = (ctime / 1000000) / (3600 * 24);

		tty_write_str("Up for ");

		if (days > 0)
		{
			tty_write_str(util::compose(days).val().raw());
			tty_write_str(" days, ");
		}
		if (hours > 0)
		{
			tty_write_str(util::compose(hours % 24).val().raw());
			tty_write_str(" hours, ");
		}
		if (minutes > 0)
		{
			tty_write_str(util::compose(minutes % 60).val().raw());
			tty_write_str(" mins, ");
		}
		if (seconds > 0)
		{
			tty_write_str(util::compose(seconds % 60).val().raw());
			tty_write_str(" sec, ");
		}
		if (milliseconds > 0)
		{
			tty_write_str(util::compose(milliseconds % 1000).val().raw());
			tty_write_str(" ms");
		}

		tty_write('\n');
	}

	int kshell_execute(int argc, char* argv[], char*& cdir)
	{
		if (argc > 0)
		{
			if (util::cstr_equal(argv[0], "exit") && argc == 1)
			{
				return 2;
			}
			else if (util::cstr_equal(argv[0], "clear") && argc == 1)
			{
				tty_clear();
			}
			else if (util::cstr_equal(argv[0], "list") && argc == 1)
			{
				kshell_display_list();
			}
			else if (util::cstr_equal(argv[0], "help") && argc == 1)
			{
				kshell_display_help();
			}
			else if (util::cstr_equal(argv[0], "kshell"))
			{
				return kshell(argc - 1, &argv[1], cdir);
			}
			else if (util::cstr_equal(argv[0], "snake") && argc == 1)
			{
				return prog::snake_main(argc - 1, &argv[1]);
			}
			else if (util::cstr_equal(argv[0], "sys") && argc == 1)
			{
				return prog::sys_main(argc - 1, &argv[1]);
			}
			else if (util::cstr_equal(argv[0], "adventure") && argc == 1)
			{
				return prog::adventure_main(argc - 1, &argv[1]);
			}
			else if (util::cstr_equal(argv[0], "timer") && argc == 1)
			{
				return prog::timer_main(argc - 1, &argv[1]);
			}
			else if (util::cstr_equal(argv[0], "repeat") && argc >= 2)
			{
				safeval<uint32> n = util::parse<uint32>(argv[1]);

				if (!n.is_valid())
				{
					tty_write('"');
					tty_write_str(argv[1]);
					tty_write_str("\" is not a valid integer\n");
				}
				else
				{
					for (uint32 i = 0; i < n.val(); i ++)
					{
						kshell_execute(argc - 2, &argv[2], cdir);

						// Preempt TODO : Remove this
						asm volatile ("int $0x80");
					}
				}
			}
			else if (util::cstr_equal(argv[0], "color") && argc == 1)
			{
				kshell_display_color();
			}
			else if (util::cstr_equal(argv[0], "chars") && argc == 1)
			{
				kshell_display_chars();
			}
			else if (util::cstr_equal(argv[0], "uptime") && argc == 1)
			{
				kshell_display_uptime();
			}
			else if (util::cstr_equal(argv[0], "ls") && argc <= 2)
			{
				char* ndir = util::alloc<char>(fs::PATH_MAX_LENGTH + 1).val();
				if (argc == 1)
					util::cstr_copy(cdir, ndir);
				else
					fs::path_concat(cdir, argv[1], ndir);

				fs::node* current_node = fs::fs_find(ndir);
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
							tty_write_str(nodename);
							tty_write_str("  ");
							tty_reset();
						}
						libk::putchar('\n');
					}
					else
						tty_write_str("Unknown error\n");
				}
				else
				{
					tty_write_str("Invalid path '");
					tty_write_str(cdir);
					tty_write_str("'\n");
				}

				delete ndir;
			}
			else if (util::cstr_equal(argv[0], "cd") && argc <= 2)
			{
				if (argc == 2)
				{
					char* ndir = util::alloc<char>(fs::PATH_MAX_LENGTH + 1).val();
					fs::path_concat(cdir, argv[1], ndir);

					// Check new dir exists
					fs::node* nnode = fs::fs_find(ndir);
					if (nnode != nullptr && nnode->is_directory())
					{
						nnode->get_path(ndir);
						delete cdir;
						cdir = ndir;
					}
					else
					{
						delete ndir;
						tty_write_str("No such directory '");
						tty_write_str(argv[1]);
						tty_write_str("'\n");
					}
				}
				else
				{
					delete cdir;
					cdir = util::alloc<char>(fs::PATH_MAX_LENGTH + 1).val();
					util::cstr_copy("/", cdir);
				}
			}
			else if (util::cstr_equal(argv[0], "pwd") && argc == 1)
			{
				tty_write_str(cdir);
				tty_write('\n');
			}
			else if (util::cstr_equal(argv[0], "mkdir") && argc <= 2)
			{
				if (argc == 2)
				{
					fs::node* nnode = fs::fs_find(cdir);
					if (nnode != nullptr && nnode->is_directory())
					{
						nnode->add_child(new fs::node(argv[1], fs::node_type::DIR));
					}
				}
				else
					tty_write_str("Error: directory name not specified\n");
			}
			else
			{
				tty_write_str(argv[0]);
				if (argc > 1)
				{
					tty_write_str(" (+ ");
					tty_write_str(util::compose((uint32)argc - 1).val().raw());
					tty_write_str(" args) ");
				}
				tty_write_str(": command not found\n");
			}
		}

		return 0;
	}

	int kshell(int argc, char* argv[], const char* wd)
	{
		char* cdir = util::alloc<char>(fs::PATH_MAX_LENGTH + 1).val();
		util::cstr_copy(wd, cdir);

		bool do_exit = false;

		for (int i = 0; i < argc; i ++)
		{
			if (util::cstr_equal(argv[0], "-h"))
			{
				kshell_display_help();
				do_exit = true;
			}
			else if (util::cstr_equal(argv[0], "-v"))
			{
				kshell_display_version();
				do_exit = true;
			}
		}

		while (true && !do_exit)
		{
			// Prompt
			tty_write_str("[");
			tty_set_fg_color(tty_color::LIGHT_RED);
			tty_write_str("kernel");
			tty_reset();
			tty_write('@');
			tty_set_fg_color(tty_color::CYAN);
			tty_write_str("tupai");
			tty_reset();
			tty_write(':');
			tty_set_fg_color(tty_color::LIGHT_YELLOW);
			tty_write_str(cdir);
			tty_reset();
			tty_write_str("] ");

			// Get the line
			char linebuff[512];
			readline(linebuff);
			tty_write('\n');

			// Get arguments
			char strbuff[512];
			char* argbuff[32];
			int arg_count = 0;
			parse_args(linebuff, argbuff, strbuff, &arg_count);

			// Execute
			{
				int result = kshell_execute(arg_count, argbuff, cdir); // 0 = OK, 1 = ERROR, 2 = exit

				if (result == 2)
					do_exit = true;
			}
		}

		return 0;
	}
}
