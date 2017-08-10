//
// file : thread.cpp
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

// Tupai
#include <tupai/task/thread.hpp>
#include <tupai/util/log.hpp>

namespace tupai
{
	namespace task
	{
		thread_t::~thread_t()
		{
			if (this->stack_block != 0)
			{
				delete (uint8_t*)this->stack_block;
				util::logln("Deleted ", this->stack_block);
			}
		}
	}
}
