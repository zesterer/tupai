/*
* 	file : pipemgr.cpp
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

// Tupai
#include <tupai/pipemgr.hpp>
#include <tupai/generic/ringbuff.hpp>
#include <tupai/util/mem.hpp>
#include <tupai/util/cstr.hpp>

namespace tupai
{
	struct pipe
	{
		id_t id;
		bool used = false;
		generic::ringbuff<byte> buffer;
		char* path;
	};

	const umem PIPE_CACHE_COUNT = 32;
	pipe pipecache[PIPE_CACHE_COUNT];

	id_t pipe_id_counter = 1;
	id_t pipe_new_id()
	{
		pipe_id_counter ++;
		return pipe_id_counter - 1;
	}

	void pipemgr_init()
	{
		// Do nothing for now
	}

	id_t pipemgr_create(const char* path, pipe_flags flags)
	{
		// Ignore path
		for (umem i = 0; i < PIPE_CACHE_COUNT; i ++)
		{
			if (!pipecache[i].used)
			{
				pipecache[i].used = true;
				pipecache[i].id = pipe_new_id();
				pipecache[i].buffer.init(32);

				char* path_str = util::alloc<char>(util::cstr_length(path)).val();
				pipecache[i].path = path_str;

				return pipecache[i].id;
			}
		}

		return 0;
	}

	id_t pipemgr_open(const char* path)
	{
		// Ignore path
		for (umem i = 0; i < PIPE_CACHE_COUNT; i ++)
		{
			if (pipecache[i].used)
			{
				if (util::cstr_equal(path, pipecache[i].path))
					return pipecache[i].id;
			}
		}

		return 0;
	}

	status_t pipemgr_write(id_t id, const byte* data, umem n)
	{
		for (umem i = 0; i < PIPE_CACHE_COUNT; i ++)
		{
			if (pipecache[i].id == id)
			{
				for (umem j = 0; j < n; j ++)
				{
					byte b = data[j];

					pipecache[i].buffer.push(b);
				}

				return 0;
			}
		}

		return 1;
	}

	status_t pipemgr_read(id_t id, byte* buffer, umem* n, umem max)
	{
		for (umem i = 0; i < PIPE_CACHE_COUNT; i ++)
		{
			if (pipecache[i].id == id)
			{
				for (umem j = 0; max == 0 || j < max; j ++)
				{
					byte b;

					//pipecache[i].buffer.lock();
					if (pipecache[i].buffer.length() > 0)
					{
						//pipecache[i].buffer.unlock();
						b = pipecache[i].buffer.pop();
					}
					else
					{
						//pipecache[i].buffer.unlock();
						*n = j;
						return 0;
					}

					buffer[j] = b;
				}

				*n = max;
				return 0;
			}
		}

		*n = 0;
		return 1;
	}
}
