//
// file : framebuffer.hpp
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

#ifndef TUPAI_ARM_RPI2_FRAMEBUFFER_HPP
#define TUPAI_ARM_RPI2_FRAMEBUFFER_HPP

// Standard
#include <stddef.h>
#include <stdint.h>

namespace tupai
{
	namespace arm
	{
		namespace rpi2
		{

			struct framebuffer_t
			{
				int       valid;
				size_t    width;
				size_t    height;
				uint32_t* data;
			};

			extern framebuffer_t g_framebuffer;

			void framebuffer_init(size_t width, size_t height);
		}
	}
}

#endif
