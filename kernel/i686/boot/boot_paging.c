/*
* 	file : paging.c
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

#include <stdint.h>
#include <stddef.h>

static const size_t page_size = 0x1000; // 4K
static const size_t page_table_size = page_size * 1024; // 4M

static const size_t kernel_lower_phys_start = 0x00000000; // 1M
static const size_t kernel_upper_phys_start = 0xC0000000; // 3G
static const size_t kernel_size = 0x1000000; // 16M
static const size_t kernel_table_num = kernel_size / page_table_size; // 16M

uint32_t page_directory[1024] __attribute__((aligned(4096)));
uint32_t page_tables_kernel_lower[kernel_table_num][1024] __attribute__((aligned(4096)));
uint32_t page_tables_kernel_upper[kernel_table_num][1024] __attribute__((aligned(4096)));

extern "C" void _boot_load_page_directory(uint32_t* page_directory);
extern "C" void _boot_enable_paging();

// Kernel setup
extern "C" void boot_setup_paging()
{
	// Zero page directory
	for (size_t i = 0; i < 1024; i ++)
		page_directory[i] = 0x00000000 | 0x002;

	// Setup kernel page tables
	for (size_t table = 0; table < kernel_table_num; table ++)
	{
		// Lower
		for (size_t i = 0; i < 1024; i ++)
			page_tables_kernel_lower[table][i] = (i * 0x1000) | 0x003;

		// Upper
		for (size_t i = 0; i < 1024; i ++)
			page_tables_kernel_upper[table][i] = (i * 0x1000) | 0x003;
	}

	// Place page tables within page directory
	for (size_t table = 0; table < kernel_table_num; table ++)
	{
		// Lower
		page_directory[kernel_lower_phys_start / page_table_size + table] = ((uint32_t)page_tables_kernel_lower[table] & 0xFFFFF000) | 0x003;

		// Upper
		page_directory[kernel_upper_phys_start / page_table_size + table] = (uint32_t)page_tables_kernel_upper[table];
	}

	_boot_load_page_directory(page_directory);
	_boot_enable_paging();
}
