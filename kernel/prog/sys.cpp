/*
* 	file : sys.cpp
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
#include <tupai/prog/sys.hpp>

#include <tupai/util/in.hpp>
#include <tupai/util/out.hpp>
#include <tupai/util/conv.hpp>

#include <tupai/x86_family/multiboot.hpp>
#include <tupai/memory.hpp>

namespace tupai
{
	namespace prog
	{
		void sys_wait()
		{
			util::println("\nPress any key to continue.");
			util::getc();
			util::printc('\n');
		}

		int sys_main(int argc __attribute__ ((unused)), char* argv[] __attribute__ ((unused)))
		{
			util::println("--- Brief ---");
			util::printf("%s %s.%s.%s on %s\n", SYSTEM_NAME_DECORATIVE, SYSTEM_VERSION_MAJOR, SYSTEM_VERSION_MINOR, SYSTEM_VERSION_RELEASE, SYSTEM_ARCH);

			sys_wait();

			util::println("--- System Information ---");
			util::printf("SYSTEM_NAME            : %s\n", SYSTEM_NAME);
			util::printf("SYSTEM_NAME_DECORATIVE : %s\n", SYSTEM_NAME_DECORATIVE);
			util::printf("SYSTEM_VERSION_MAJOR   : %s\n", SYSTEM_VERSION_MAJOR);
			util::printf("SYSTEM_VERSION_MINOR   : %s\n", SYSTEM_VERSION_MINOR);
			util::printf("SYSTEM_VERSION_RELEASE : %s\n", SYSTEM_VERSION_RELEASE);
			util::printf("SYSTEM_ARCH            : %s\n", SYSTEM_ARCH);

			sys_wait();

			util::println("--- Type Information ---");
			util::printf("sizeof(void*)       : %i\n", sizeof(void*));
			util::printf("sizeof(size_t)      : %i\n", sizeof(size_t));
			util::printf("sizeof(char)        : %i\n", sizeof(char));
			util::printf("sizeof(short)       : %i\n", sizeof(short));
			util::printf("sizeof(int)         : %i\n", sizeof(int));
			util::printf("sizeof(long)        : %i\n", sizeof(long));
			util::printf("sizeof(long long)   : %i\n", sizeof(long long));
			util::printf("sizeof(float)       : %i\n", sizeof(float));
			util::printf("sizeof(double)      : %i\n", sizeof(double));
			util::printf("sizeof(long double) : %i\n", sizeof(long double));

			sys_wait();

			util::println("--- Multiboot Information ---");
			util::printf("Available Lower : 0x%s\n", util::compose(x86_family::multiboot_get_header().mem_lower * 1024, 16));
			util::printf("Available Upper : 0x%s\n", util::compose(x86_family::multiboot_get_header().mem_upper * 1024, 16));
			util::printf("Available Total : 0x%s\n", util::compose((x86_family::multiboot_get_header().mem_upper - x86_family::multiboot_get_header().mem_lower) * 1024, 16));

			sys_wait();

			util::println("--- Memory Information ---");
			util::printf("Used  : 0x%s\n", util::compose(memory_get_info().used, 16));
			util::printf("Total : 0x%s\n", util::compose(memory_get_info().size, 16));

			sys_wait();

			return 0;
		}
	}
}
