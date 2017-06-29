//
// file : debug.hpp
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

#ifndef TUPAI_DEBUG_HPP
#define TUPAI_DEBUG_HPP

// Tupai
#include <tupai/util/fmt.hpp>

namespace tupai
{
	void debug_init();
	void debug_write(char c);
	void debug_print(const char* str);

	struct __debug_ostream
	{
		inline void write(char c)
		{
			debug_write(c);
		}
	};

	template <typename... Args>
	inline void debug_print(Args&&... args __attribute__((unused)))
	{
		#if defined(DEBUG_ENABLED)
		{
			debug_print(""); // For the debug prefix
			__debug_ostream ostream;
			util::__pass_funct{(util::__fmt_arg(ostream, args), 1)...};
		}
		#endif
	}

	template <typename... Args>
	inline void debug_println(Args&&... args __attribute__((unused)))
	{
		#if defined(DEBUG_ENABLED)
		{
			debug_print(args...);
			debug_write('\n');
		}
		#endif
	}
}

#endif
