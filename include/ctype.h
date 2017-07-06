//
// file : ctype.h : Character handling functions
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
