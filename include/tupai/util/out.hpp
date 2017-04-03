//
// file : out.hpp
//
// This file is part of Tupai.
//
// Tupai is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
//
// Tupai is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY// without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with Tupai.  If not, see <http://www.gnu.org/licenses/>.
//

#ifndef TUPAI_UTIL_OUT_HPP
#define TUPAI_UTIL_OUT_HPP

// Tupai
#include <tupai/tty.hpp>
#include <tupai/util/fmt.hpp>

// Standard
#include <stddef.h>
#include <stdint.h>

namespace tupai
{
	namespace util
	{
		struct __print_funct
		{
			void operator()(char c)
			{
				tty_write(c);
			}
		};

		template <typename... Args>
		void print(Args&&... args)
		{
			__print_funct funct;
			__pass_funct{(__fmt_arg(funct, args), 1)...};
		}
	}
}

#endif
