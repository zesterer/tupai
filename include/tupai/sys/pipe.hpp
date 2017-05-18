//
// file : pipe.hpp
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

#ifndef TUPAI_SYS_PIPE_HPP
#define TUPAI_SYS_PIPE_HPP

// Tupai
#include <tupai/sys/fifobuff.hpp>

// Standard
#include <stddef.h>
#include <stdint.h>

namespace tupai
{
	namespace sys
	{
		template <size_t SIZE>
		struct pipe_t
		{
		public:
			volatile fifobuff_t<SIZE> in;
			volatile fifobuff_t<SIZE> out;

		public:
			void    write(uint8_t c) volatile { return this->out.write(c); }
			uint8_t read()           volatile { return this->in.read(); }

			void    write_in(uint8_t c) volatile { return this->in.write(c); }
			uint8_t read_out()          volatile { return this->out.read(); }
		};
	}
}

#endif
