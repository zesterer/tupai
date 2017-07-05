//
// file : call.hpp
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

#ifndef TUPAI_SYS_CALL_HPP
#define TUPAI_SYS_CALL_HPP

// Standard
#include <stddef.h>
#include <stdint.h>

namespace tupai
{
	namespace sys
	{
		const uint8_t CALL_IRQ = 0x80;

		enum class CALL
		{
			// Process control
			YIELD  = 0x100,
			LMUTEX = 0x101, // temporary
			UMUTEX = 0x102, // temporary
			// and more...

			// File descriptor creation
			OPEN   = 0x200,
			PIPE   = 0x201,
			SOCKET = 0x202,
			// and more...

			// File descriptor destruction
			CLOSE = 0x300,
			// and more...

			// File manipulation
			READ  = 0x400,
			WRITE = 0x401,
			SEEK  = 0x402,
			// and more...
		};

		void call_bind();
		void call_init();

		void call(CALL call, size_t arg0 = 0, size_t arg1 = 0, size_t arg2 = 0);
	}
}

#endif
