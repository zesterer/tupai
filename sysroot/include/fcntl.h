//
// file : fcntl.h : Character handling functions
//
// Copyright (c) 2017 Joshua Barretto <joshua.s.barretto@gmail.com>
//
// This file is part of Tupai.
//
// Permission to use, copy, modify, and distribute this software for any
// purpose with or without fee is hereby granted, provided that the above
// copyright notice and this permission notice appear in all copies.
//
// THE SOFTWARE IS PROVIDED "AS IS" AND THE AUTHOR DISCLAIMS ALL WARRANTIES
// WITH REGARD TO THIS SOFTWARE INCLUDING ALL IMPLIED WARRANTIES OF
// MERCHANTABILITY AND FITNESS. IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR
// ANY SPECIAL, DIRECT, INDIRECT, OR CONSEQUENTIAL DAMAGES OR ANY DAMAGES
// WHATSOEVER RESULTING FROM LOSS OF USE, DATA OR PROFITS, WHETHER IN AN
// ACTION OF CONTRACT, NEGLIGENCE OR OTHER TORTIOUS ACTION, ARISING OUT OF
// OR IN CONNECTION WITH THE USE OR PERFORMANCE OF THIS SOFTWARE.
//
// This file is part of the Tupai POSIX standard library implementation.
//

// Standard
#include <sys/types.h>

#define NULL 0

// File creation modes

#define O_CLOEXEC   (1 <<  0) // Close on exec() called
#define O_CREAT     (1 <<  1) // Create if does not exist
#define O_DIRECTORY (1 <<  2) // Fail if non-directory
#define O_EXCL      (1 <<  3) // Exclusive use
#define O_NOCTTY    (1 <<  4) // Do not assign controlling terminal
#define O_NOFOLLOW  (1 <<  5) // Do not follow symbolic links
#define O_TRUNC     (1 <<  6) // Truncate flag
#define O_TTY_INIT  (1 <<  7) // Set conforming termios structure parameters
#define O_APPEND    (1 <<  8) // Append mode
#define O_DSYNC     (1 <<  9) // Write according to synced I/O data integrity completion
#define O_NONBLOCK  (1 << 10) // Non-blocking mode
#define O_RSYNC     (1 << 11) // Syncronized read I/O operations
#define O_SYNC      (1 << 12) // Write according to synced I/O file integrity completion
#define O_ACCMODE   (1 << 13) // Make for file access modes

// File access modes

#define O_EXEC   (1 << 14) // Open for execute-only (non-directory files)
#define O_RDONLY (1 << 15) // Open read-only
#define O_RDWR   (1 << 16) // Open read-write
#define O_SEARCH (1 << 17) // Open directory for search only
#define O_WRONLY (1 << 18) // Open for write-only

// File access functions

int creat(const char* fpath, mode_t mode);
int fcntl(int fd, int cmd);
int open (const char* fpath, int flags);
