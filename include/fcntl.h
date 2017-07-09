//
// file : string.h : String and memory operations
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
// This file is part of the Tupai POSIX implementation.
//

// Standard
#include <sys/types.h>

#define NULL 0

// File access functions
int creat(const char* fpath, mode_t mode);
int fcntl(int fd, int cmd, ...);
int open (const char* fpath, int flags, ...);
