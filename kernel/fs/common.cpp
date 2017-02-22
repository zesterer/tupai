/*
* 	file : fs.cpp
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
#include <tupai/fs/common.hpp>
#include <tupai/util/cstr.hpp>

// Libk
#include <libk/stdio.hpp>

namespace tupai
{
	namespace fs
	{
		bool path_is_relative(const char* path)
		{
			return path[0] == '/';
		}

		status_t path_concat(const char* path0, const char* path1, char* buffer)
		{
			if (path_is_relative(path1))
			{
				util::cstr_copy(path1, buffer);
				return 1;
			}

			umem path0_len = util::cstr_length(path0);

			util::cstr_copy(path0, buffer);
			if (path0[path0_len - 1] != '/')
				util::cstr_append("/", buffer);
			util::cstr_append(path1, buffer);

			return 0;
		}
	}
}
