//
// file : irq.hpp
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

#ifndef TUPAI_IRQ_HPP
#define TUPAI_IRQ_HPP

// Standard
#include <stdint.h>
#include <stddef.h>

namespace tupai
{
	namespace irq
	{
		bool are_enabled();

		void enable();
		void disable();

		void ack(uint8_t irq);
		void mask(uint8_t irq, bool enable);

		void bind(uint8_t irq, void* address, bool hardware = false);
	}
}

#endif
