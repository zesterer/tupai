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
#include <tupai/util/string.hpp>
#include <tupai/util/vector.hpp>

namespace tupai
{
	namespace fs
	{
		static const umem PATH_MAX_LENGTH = 512;
		static const umem NODENAME_MAX__LENGTH = 256;

		struct path
		{
		private:
			util::vector<util::string> pathvec;
			bool absolute = true;
			bool valid = true;

			void validate()
			{
				for (umem i = 0; i < this->pathvec.length(); i ++)
				{
					if (this->pathvec[i].length() <= 0)
					{
						this->valid = false;
						return;
					}
				}

				this->valid = true;
			}

			void set(util::string pathstr)
			{
				this->pathvec = pathstr.split('/');
				this->absolute = (pathstr[0] == '/');

				this->validate();
			}

		public:
			path()
			{
				this->set("/");
			}

			path(util::string pathstr)
			{
				this->set(pathstr);
			}

			util::string& operator[](umem i)
			{
				return this->pathvec[i];
			}

			umem length()
			{
				return this->pathvec.length();
			}

			bool is_valid()
			{
				return this->valid;
			}

			bool is_absolute()
			{
				return this->absolute;
			}

			bool append(util::string pathstr)
			{
				path npath(pathstr);

				if (npath.is_absolute() && npath.is_valid())
					return false;
				else
				{
					for (umem i = 0; i < npath.length(); i ++)
						this->pathvec.push(npath[i]);

					return true;
				}
			}

			util::string str()
			{
				util::string nstr;
				for (umem i = 0; i < this->pathvec.length(); i ++)
					nstr += "/" + this->pathvec[i];
				return nstr;
			}
		};

		bool     path_is_relative(const char* path);
		status_t path_concat(const char* path0, const char* path1, char* buffer);
	}
}

#endif
