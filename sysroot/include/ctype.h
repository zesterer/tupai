//
// file : ctype.h : Character handling functions
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
// This file is part of the Tupai C standard library implementation.
//

// Character classification functions
int isalnum(int c);  // Checks whether the passed character is alphanumeric
int isalpha(int c);  // Checks whether the passed character is alphabetic
int iscntrl(int c);  // Checks whether the passed character is a control character
int isdigit(int c);  // Checks whether the passed character is a decimal digit
int isgraph(int c);  // Checks whether the passed character has graphical representation
int islower(int c);  // Checks whether the passed character is a lowercase letter
int isprint(int c);  // Checks whether the passed character is printable
int ispunct(int c);  // Checks whether the passed character is punctuation
int isspace(int c);  // Checks whether the passed character is whitespace
int isupper(int c);  // Checks whether the passed character is an uppercase letter
int isxdigit(int c); // Checks whether the passed character is a hexadecimal digit

// Character conversion functions
int tolower(int c); // Converts the passed character to lowercase
int toupper(int c); // Converts the passed character to uppercase
