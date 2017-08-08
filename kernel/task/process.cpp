//
// file : process.cpp
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
#include <tupai/task/process.hpp>

namespace tupai
{
	namespace task
	{
		process_t::~process_t()
		{
			for (size_t i = 0; i < this->threads.size(); i ++)
				this->threads.nth(i)->kill();

			for (size_t i = 0; i < this->fds.size(); i ++)
				this->fds.nth(i)->destroy();
		}
	}
}
