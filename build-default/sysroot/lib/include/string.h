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
// This file is part of the Tupai C standard library implementation.
//

// Standard
#include <stdint.h>

#define NULL 0

// Memory manipulation
void* memcpy (void* dest, const void* src, size_t n); // Copies n bytes from src to dest
void* memmove(void* dest, const void* src, size_t n); // Copies n bytes from src to dest
void* memset (void* str, int c, size_t n);            // Copies the byte c to the first n bytes of str

// String manipulation
char*  strcar  (char* dest, const char* src);           // Appends src to the end of dest
char*  strncat (char* dest, const char* src, size_t n); // Appends at most n characters of src to the end of dest
char*  strcpy  (char* dest, const char* src);           // Copies the string src to dest
char*  strncpy (char* dest, const char* src, size_t n); // Copies at most the first n characters of src to dest
size_t strxfrm (char* dest, const char* src, size_t n); // Transforms the first n characters of src into current locale and places them in dest

// Comparasion
int memcmp (const void* str1, const void* str2, size_t n) const; // Compares the first n bytes of str1 and str2
int strcmp (const char* str1, const char* str2) const;           // Compares str1 to str2
int strncmp(const char* str1, const char* str2, size_t n) const; // Compares at most the first n characters of str1 and str2
int strcoll(const char* str1, const char* str2) const;           // Compares str1 to str2. Dependent on LC_COLLATE

// Searching
void*  memchr (const void* str, int c, size_t n) const;             // Searches for the first occurence of the byte c in the first n bytes of the string pointed to by str
char*  strchr (const char* str, int c) const;                       // Searches for the first occurence of the character c in str
size_t strcspn(const char* str1, const char* str2) const;           // Calculates the length of the initial segment of str1 which consists entirely of characters not in str2
char*  strpbrk(const char* str1, const char* str2) const;           // Finds the first character in str1 that matches any character in str2
char*  strrchr(const char* str, int c) const;                       // Finds the last occurence of the character c in str
size_t strspn (const char* str1, const char* str2) const;           // Calculates the length of the initial segment of str1 which consists entirely of characters in str2
char*  strstr (const char* haystack, const char* needle) const;     // Finds the first occurence of the string needle in the string haystack
char*  strtok (char* str, const char* delim);                       // Breaks the string str into tokens separated by delim

// Other
char*  strerror(int errnum);            // Returns an error message for the given error code
size_t strlen  (const char* str) const; // Computes the length of the string str
