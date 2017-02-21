/*
* 	file : pipemgr.hpp
*
* 	This file is part of Tupai.
*
* 	Tupai is free software: you can redistribute it and/or modify
* 	it under the terms of the GNU General Public License as published by
* 	the Free Software Foundation, either version 3 of the License, or
* 	(at your option) any later version.
*
* 	Tupai is distributed in the hope that it will be useful,
* 	but WITHOUT ANY WARRANTY; without even the implied warranty of
* 	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
* 	GNU General Public License for more details.
*
* 	You should have received a copy of the GNU General Public License
* 	along with Tupai.  If not, see <http://www.gnu.org/licenses/>.
*/

#ifndef TUPAI_PIPEMGR_HPP
#define TUPAI_PIPEMGR_HPP

// Tupai
#include <tupai/type.hpp>

namespace tupai
{
	typedef uint32 pipe_flags;

	void pipemgr_init();

	id_t    pipemgr_create(const char* path, pipe_flags flags = 0x0);
	id_t    pipemgr_open(const char* path);
	status_t pipemgr_write(id_t id, const byte* data, umem n = 1);
	status_t pipemgr_read(id_t id, byte* buffer, umem* n, umem max = 0);
}

#endif
