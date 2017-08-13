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
// but WITHOUT ANY WARRANTY without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with Tupai.  If not, see <http://www.gnu.org/licenses/>.
//

#ifndef TUPAI_SYS_PIPE_HPP
#define TUPAI_SYS_PIPE_HPP

// Tupai
#include <tupai/util/fifo.hpp>
#include <tupai/vfs/vfs.hpp>

// Standard
#include <stddef.h>
#include <stdint.h>

namespace tupai
{
	namespace sys
	{
		const size_t PIPE_SIZE = 4096;
		using pipe_t = util::fifo_t<uint8_t, PIPE_SIZE>;

		void             pipe_init();
		vfs::inode_ptr_t mount_pipe(pipe_t* pipe, const char* path);
	}
}

#endif
