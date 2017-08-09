//
// file : cpu.hpp
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

#ifndef TUPAI_CPU_HPP
#define TUPAI_CPU_HPP

// Standard
#include <stddef.h>
#include <stdint.h>

namespace tupai
{
	namespace cpu
	{
		void wait();
		void halt() __attribute__((__noreturn__));
		void hang() __attribute__((__noreturn__));

		const char* get_exception_name(size_t code);
		bool        get_exception_critical(size_t code);

		bool is_irq();
		void end_irq();
	}
}

#endif
