/*
* 	file : common.hpp
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

#ifndef TUPAI_FS_COMMON_HPP
#define TUPAI_FS_COMMON_HPP

// Tupai
#include <tupai/type.hpp>
#include <tupai/util/mem.hpp>
#include <tupai/util/cstr.hpp>
#include <tupai/util/math.hpp>

namespace tupai
{
	namespace fs
	{
		static const umem PATH_MAX_LENGTH = 512;

		struct node_name
		{
			static const umem MAX_LENGTH = 256;

			char* name = nullptr;

			node_name() {}
			node_name(const char* name)
			{
				umem len = util::min(node_name::MAX_LENGTH, util::cstr_length(name));
				this->name = util::alloc<char>(len + 1).val();
				util::cstr_copy(name, this->name, len);
			}

			~node_name()
			{
				// TODO : Fix this
				//if (this->name != nullptr)
				//	util::dealloc(this->name);
			}

			const char* str() const
			{
				if (this->name == nullptr)
					return "\0";
				else
					return this->name;
			}

			bool operator==(const node_name& other)
			{
				return util::cstr_equal(this->str(), other.str());
			}
		};

		bool     path_is_relative(const char* path);
		status_t path_concat(const char* path0, const char* path1, char* buffer);
	}
}

#endif
