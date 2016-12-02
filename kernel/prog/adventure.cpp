/*
* 	file : adventure.cpp
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
#include <tupai/prog/adventure.hpp>

#include <tupai/tty.hpp>

// Libk
#include <libk/stdio.hpp>
#include <libk/stdlib.hpp>
#include <libk/string.hpp>

namespace tupai
{
	namespace prog
	{
		char* get_line()
		{
			char* buffer = (char*)libk::malloc(sizeof(char) * libk::BUFSIZ);
			umem i = 0;

			while (i < libk::BUFSIZ - 1)
			{
				char c = libk::getchar();

				if (c == '\0' || c == '\n')
					break;
				else
				{
					buffer[i] = c;
					i ++;
					libk::putchar(c);
				}
			}

			buffer[i] = '\0';
			return buffer;
		}

		struct Item { uint type; uint count; Item(){} Item(uint t, uint c) { type = t; count = c; } };
		const sint item_cell_count = 16;
		struct Cell { uint ground; Item items[item_cell_count]; };
		const sint world_size = 32;
		struct World { Cell cells[world_size][world_size]; };
		World world;

		enum { W_VOID = 0, W_GRASS = 1, W_SAND = 2, };
		enum { I_APPLE = 1, I_SWORD = 2, I_TORCH = 3, };
		const char* itemnames[] = { "", "apple", "sword", "torch" };

		const sint inventory_size = 8;
		Item player_inv[8];
		sint player_x;
		sint player_y;

		void generate()
		{
			for (uint x = 0; x < world_size; x ++)
			{
				for (uint y = 0; y < world_size; y ++)
				{
					world.cells[x][y].ground = W_GRASS; // Grass

					if (libk::rand() % 3 == 0)
						world.cells[x][y].ground = W_SAND; // Sand

					for (sint i = 0; i < item_cell_count; i ++)
					{
						world.cells[x][y].items[i] = Item(0, 100);

						if (libk::rand() % 16 == 0) world.cells[x][y].items[i] = Item(libk::rand() % 2 + 1, 100);
					}
				}
			}
		}

		void show_help()
		{
			libk::printf("Accepted commands:\n");
			libk::printf("walk - Walk at a bearing\n");
			libk::printf("pick - Pick up an item\n");
			libk::printf("drop - Drop an item\n");
			libk::printf("use  - Use an item\n");
			libk::printf("look - Observe your surroundings\n");
			libk::printf("inv  - Check your inventory\n");
		}

		void show_look()
		{
			Cell& cell = world.cells[player_x][player_y];
			switch (cell.ground)
			{
			case W_VOID:
				libk::printf("You're standing on nothing.\n");
				break;
			case W_GRASS:
				libk::printf("You're standing on grass.\n");
				break;
			case W_SAND:
				libk::printf("You're standing on sand.\n");
				break;
			default:
				break;
			}

			sint j = 0;
			for (sint i = 0; i < item_cell_count; i ++)
			{
				if (cell.items[i].type != 0)
				{
					j ++;
					libk::printf("[%i] There is a %s on the floor.\n", j, itemnames[cell.items[i].type]);
				}
			}
		}

		void show_inv()
		{
			libk::printf("Your inventory:\n");

			bool has_item = false;
			for (sint i = 0; i < inventory_size; i ++)
			{
				if (player_inv[i].type != 0)
				{
					libk::printf("[%i] A %i / %i %s.\n", i + 1, player_inv[i].count, 100, itemnames[player_inv[i].type]);
					has_item = true;
				}
			}

			if (!has_item)
				libk::printf("There are no items in your inventory!\n");
		}

		bool do_move(sint x, sint y)
		{
			player_x += x;
			player_y += y;

			bool fixed = false;
			if (player_x < 0) { player_x = 0; fixed = true; }
			if (player_y < 0) { player_y = 0; fixed = true; }
			if (player_x >= world_size) { player_x = world_size - 1; fixed = true; }
			if (player_y >= world_size) { player_y = world_size - 1; fixed = true; }

			return !fixed;
		}

		bool inv_add(Item item)
		{
			for (int i = 0; i < inventory_size; i ++)
			{
				if (player_inv[i].type == 0)
				{
					player_inv[i] = item;
					return true;
				}
			}

			return false;
		}

		void do_pick()
		{
			libk::printf("Which item (enter number)?\n");
			libk::puts("> ");
			char* line = get_line();
			libk::putchar('\n');

			sint n = libk::atoi(line);
			libk::free(line);

			if (n == 0)
			{
				libk::printf("Invalid item!\n");
				return;
			}

			Cell& cell = world.cells[player_x][player_y];
			sint j = 0;
			for (sint i = 0; i < item_cell_count; i ++)
			{
				if (cell.items[i].type != 0)
				{
					j ++;
					if (n == j)
					{
						bool worked = inv_add(cell.items[i]);
						if (worked)
						{
							libk::printf("Picked up the %s.\n", itemnames[cell.items[i].type]);
							cell.items[i].type = 0;
							return;
						}
						else
							libk::printf("No space left in inventory!\n");
					}
				}
			}

			libk::printf("No item picked!\n");
		}

		void do_walk()
		{
			libk::printf("In which direction? [n/e/s/w]\n");
			libk::puts("> ");
			char* line = get_line();
			libk::putchar('\n');

			bool worked = true;
			if (libk::strcmp(line, "n") == 0)
			{
				worked = do_move(0, -1);
				libk::printf("Walking North...\n");
			}
			else if (libk::strcmp(line, "e") == 0)
			{
				worked = do_move(1, 0);
				libk::printf("Walking East...\n");
			}
			else if (libk::strcmp(line, "s") == 0)
			{
				worked = do_move(0, 1);
				libk::printf("Walking South...\n");
			}
			else if (libk::strcmp(line, "w") == 0)
			{
				worked = do_move(-1, 0);
				libk::printf("Walking West...\n");
			}
			else
				libk::printf("Invalid direction!\n");

			if (!worked)
				libk::printf("Could not walk there!\n");

			libk::free(line);
		}

		int adventure_main(int argc, char* argv[])
		{
			generate();

			// Clear inv
			for (sint i = 0; i < inventory_size; i ++)
				player_inv[i] = Item(0, 0);

			player_x = world_size / 2;
			player_y = world_size / 2;

			bool running = true;
			while (running)
			{
				libk::puts("> ");
				char* line = get_line();
				libk::putchar('\n');

				if (libk::strcmp(line, "exit") == 0)
					running = false;
				else if (libk::strcmp(line, "help") == 0)
					show_help();
				else if (libk::strcmp(line, "look") == 0)
					show_look();
				else if (libk::strcmp(line, "walk") == 0)
					do_walk();
				else if (libk::strcmp(line, "inv") == 0)
					show_inv();
				else if (libk::strcmp(line, "pick") == 0)
					do_pick();
				else
					libk::printf("I don't understand!\n");

				libk::free(line);
			}

			return 0;
		}
	}
}
