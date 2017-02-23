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

#include <tupai/sys/timer.hpp>

#include <tupai/fs/fs.hpp>

#include <tupai/util/out.hpp>
#include <tupai/util/ansi.hpp>
#include <tupai/util/cstr.hpp>
#include <tupai/util/conv.hpp>
#include <tupai/util/mem.hpp>

// Programs
#include <tupai/prog/snake.hpp>
#include <tupai/prog/sys.hpp>
#include <tupai/prog/adventure.hpp>
#include <tupai/prog/timer.hpp>
#include <tupai/prog/mmap.hpp>

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
					util::printc(c);
				}
			}
			else
			{
				buffer[index] = c;
				index ++;
				util::printc(c);
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
		util::ansi_set_fg_color(14);
		util::print("Usage: kshell [options]\n");
		util::ansi_reset();
		util::print("Tupai kernel shell\n\n");
		util::ansi_set_fg_color(11);
		util::print("Options:\n");
		util::ansi_reset();
		util::print("  -h      Display this help screen\n");
		util::print("  -v      Display version\n");
		util::ansi_set_fg_color(11);
		util::print("Special commands:\n");
		util::ansi_reset();
		util::print("  help    Display this help screen\n");
		util::print("  list    List additional commands\n");
		util::print("  clear   Clear the screen\n");
		util::print("  exit    Exit the session\n");
		util::ansi_set_fg_color(11);
		util::print("Utility commands:\n");
		util::ansi_reset();
		util::print("  repeat <n> <cmd> [args] Repeat a command n times\n");
		util::print("  echo <string>           Output a string\n");
		util::print("  color                   Display color information\n");
		util::print("  chars                   Display character set\n");
		util::print("  uptime                  Display system uptime\n");
		util::ansi_set_fg_color(11);
		util::print("Filesystem commands:\n");
		util::ansi_reset();
		util::print("  ls [dir]     List files in the current directory\n");
		util::print("  tree [dir]   List files in all sub-directories\n");
		util::print("  pwd          Display the current directory\n");
		util::print("  cd [dir]     Change directory\n");
		util::print("  mkdir <name> Create a new directory\n");
	}

	void kshell_display_version()
	{
		util::print("kshell 0.1\n");
	}

	void kshell_display_list()
	{
		util::print("Additional commands:\n");
		util::print("  snake     Run command-line snake\n");
		util::print("  sys       Display system details\n");
		util::print("  adventure Play an adventure game\n");
		util::print("  timer     Run a test timer\n");
		util::print("  mmap      View the memory map\n");
	}

	void kshell_display_color()
	{
		const char* names[] = {"  Black  ", "   Red   ", "  Green  ", " Yellow  ", "  Blue   ", " Magenta ", "  Cyan   ", "  White  "};

		for (uint i = 0; i < (sizeof(names) / sizeof(char*)); i ++)
		{
			// Normal
			util::ansi_set_bg_color(i);
			util::ansi_set_fg_color(util::ansi::WHITE);
			util::printc(' ');
			util::print(util::compose((uint32)i, 16).val().raw());
			util::print(names[i]);
			util::ansi_reset();

			// Bright
			util::ansi_set_bg_color(i + 8);
			util::ansi_set_fg_color(util::ansi::BLACK);
			util::printc(' ');
			util::print(util::compose((uint32)i + 8, 16).val().raw());
			util::print(names[i]);
			util::ansi_reset();

			util::printc('\n');
		}
	}

	void kshell_display_chars()
	{
		for (int i = 32; i < 256; i ++)
		{
			util::printc(i);
			util::printc(' ');
			if ((i + 1) % 32 == 0)
				util::printc('\n');
		}
	}

	void kshell_display_uptime()
	{
		uint64 ctime = sys::timer_get_nanoseconds();
		uint64 milliseconds = ctime / 1000;
		uint64 seconds = ctime / 1000000;
		uint64 minutes = (ctime / 1000000) / 60;
		uint64 hours = (ctime / 1000000) / 3600;
		uint64 days = (ctime / 1000000) / (3600 * 24);

		util::print("Up for ");

		if (days > 0)
		{
			util::print(util::compose(days).val().raw());
			util::print(" days, ");
		}
		if (hours > 0)
		{
			util::print(util::compose(hours % 24).val().raw());
			util::print(" hours, ");
		}
		if (minutes > 0)
		{
			util::print(util::compose(minutes % 60).val().raw());
			util::print(" mins, ");
		}
		if (seconds > 0)
		{
			util::print(util::compose(seconds % 60).val().raw());
			util::print(" sec, ");
		}
		if (milliseconds > 0)
		{
			util::print(util::compose(milliseconds % 1000).val().raw());
			util::print(" ms");
		}

		util::printc('\n');
	}

	void kshell_display_node_tree(fs::node* node, int depth = 0)
	{
		if (node->is_root())
		{
			util::print("/\n");
			if (node->child != nullptr)
				kshell_display_node_tree(node->child, depth + 1);
		}
		else
		{
			util::ansi_set_fg_color(8);
			for (int i = 0; i < depth - 1; i ++)
				util::print("  ");
			if (depth > 0)
				util::print("|-");
			util::ansi_reset();
			util::ansi_set_fg_color((byte)node->type + 9);
			util::print(node->name.str());
			util::ansi_reset();

			if (node->is_directory())
				util::print("/\n");
			else
				util::printc('\n');

			if (node->child != nullptr)
				kshell_display_node_tree(node->child, depth + 1);

			if (node->next != nullptr)
				kshell_display_node_tree(node->next, depth);
		}
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
				util::ansi_clear();
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
			else if (util::cstr_equal(argv[0], "mmap") && argc == 1)
			{
				return prog::mmap_main(argc - 1, &argv[1]);
			}
			else if (util::cstr_equal(argv[0], "repeat") && argc >= 2)
			{
				safeval<uint32> n = util::parse<uint32>(argv[1]);

				if (!n.is_valid())
				{
					util::printc('"');
					util::print(argv[1]);
					util::print("\" is not a valid integer\n");
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
			else if (util::cstr_equal(argv[0], "echo") && argc == 2)
			{
				util::println(argv[1]);
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
							util::ansi_set_fg_color((byte)nodebuff[i]->type + 9);
							util::print(nodename);
							util::print("  ");
							util::ansi_reset();
						}
						libk::putchar('\n');
					}
					else
						util::print("Unknown error\n");
				}
				else
				{
					util::print("Invalid path '");
					util::print(cdir);
					util::print("'\n");
				}

				delete ndir;
			}
			else if (util::cstr_equal(argv[0], "tree") && argc <= 2)
			{
				char* ndir = util::alloc<char>(fs::PATH_MAX_LENGTH + 1).val();
				if (argc == 1)
					util::cstr_copy(cdir, ndir);
				else
					fs::path_concat(cdir, argv[1], ndir);

				fs::node* current_node = fs::fs_find(ndir);
				if (current_node != nullptr)
				{
					kshell_display_node_tree(current_node, 0);
				}
				else
				{
					util::print("Invalid path '");
					util::print(cdir);
					util::print("'\n");
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
						util::print("No such directory '");
						util::print(argv[1]);
						util::print("'\n");
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
				util::println(cdir);
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
					util::print("Error: directory name not specified\n");
			}
			else if (util::cstr_length(argv[0]) > 0)
			{
				util::print(argv[0]);
				if (argc > 1)
				{
					util::print(" (+ ");
					util::print(util::compose((uint32)argc - 1).val().raw());
					util::print(" args) ");
				}
				util::print(": command not found\n");
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

		if (!do_exit)
			util::print("Type 'help' for more information\n");

		while (!do_exit)
		{
			// Prompt
			//util::print("[");
			util::ansi_set_fg_color(util::ansi::LIGHT_RED);
			util::print("kernel");
			util::ansi_reset();
			//util::printc('@');
			//util::ansi_set_fg_color(util::ansi::CYAN);
			//util::print("tupai");
			//util::ansi_reset();
			util::printc(':');
			util::ansi_set_fg_color(util::ansi::LIGHT_YELLOW);
			util::print(cdir);
			util::ansi_reset();
			util::print(" $ ");

			// Get the line
			char* linebuff = util::alloc<char>(512).val();
			readline(linebuff);
			util::printc('\n');

			// Get arguments
			char* strbuff = util::alloc<char>(512).val();
			char** argbuff = util::alloc<char*>(64).val();
			int arg_count = 0;
			parse_args(linebuff, argbuff, strbuff, &arg_count);

			// Execute
			{
				int result = kshell_execute(arg_count, argbuff, cdir); // 0 = OK, 1 = ERROR, 2 = exit

				if (result == 2)
					do_exit = true;
			}

			// Free unneeded memory
			delete linebuff;
			delete strbuff;
			delete argbuff;
		}

		delete cdir;

		return 0;
	}
}
