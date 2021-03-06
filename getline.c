﻿#include<stdio.h>
#include<string.h>
// source : https://gist.github.com/ingramj/1105106


/* Implementations of the getdelim() and getline() functions from POSIX 2008,
   just in case your libc doesn't have them.

   getdelim() reads from a stream until a specified delimiter is encountered.
   getline() reads from a stream until a newline is encountered.

   See: http://pubs.opengroup.org/onlinepubs/9699919799/functions/getdelim.html

   NOTE: It is always the caller's responsibility to free the line buffer, even
   when an error occurs.

   Copyright (c) 2011 James E. Ingram

   Permission is hereby granted, free of charge, to any person obtaining a copy
   of this software and associated documentation files (the "Software"), to deal
   in the Software without restriction, including without limitation the rights
   to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
   copies of the Software, and to permit persons to whom the Software is
   furnished to do so, subject to the following conditions:

   The above copyright notice and this permission notice shall be included in
   all copies or substantial portions of the Software.

   THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
   IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
   FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
   AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
   LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
   OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
   THE SOFTWARE.
*/
#pragma once
#include <assert.h>

#include <limits.h>
#include <stdbool.h>
#include <errno.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>

#if __STDC_VERSION__ >= 199901L
/* restrict is a keyword */
#else
# define restrict
#endif


#ifndef _POSIX_SOURCE
typedef long ssize_t;
#define SSIZE_MAX LONG_MAX
#endif

bool getdelim_include_delimiter = false;
bool getline_include_delimiter = false;

#define __getdelim_default_grow_by_allocation 1;
#define __getline_default_grow_by_allocation 1;
#define __getdelim_default_minimum_allocation 1;
#define __getline_default_minimum_allocation 1;

size_t getdelim_grow_by_allocation = __getdelim_default_grow_by_allocation;
size_t getline_grow_by_allocation = __getline_default_grow_by_allocation;
size_t getdelim_minimum_allocation = __getdelim_default_minimum_allocation;
size_t getline_minimum_allocation = __getline_default_minimum_allocation;

ssize_t getdelim(char **restrict lineptr, size_t *restrict n, int delimiter,
                 FILE *restrict stream);
ssize_t getline(char **restrict lineptr, size_t *restrict n,
                FILE *restrict stream);



#define _GETDELIM_GROWBY getdelim_grow_by_allocation    /* amount to grow line buffer by */
#define _GETDELIM_MINLEN getdelim_minimum_allocation      /* minimum line buffer size */

unsigned long long line = 0;
ssize_t getdelim(char **restrict lineptr, size_t *restrict n, int delimiter,
                 FILE *restrict stream)
{
	char *buf, *pos;
	int c;
	ssize_t bytes;

	if (lineptr == NULL || n == NULL) {
		errno = EINVAL;
		return -1;
	}
	if (stream == NULL) {
		errno = EBADF;
		return -1;
	}

	/* resize (or allocate) the line buffer if necessary */
	buf = *lineptr;
	if (buf == NULL || *n < _GETDELIM_MINLEN) {
		buf = realloc(*lineptr, _GETDELIM_GROWBY);
		if (buf == NULL) {
			errno = ENOMEM;
			return -1;
		}
		*n = _GETDELIM_GROWBY;
		*lineptr = buf;
	}

	/* read characters until delimiter is found, end of file is reached, or an
	   error occurs. */
	bytes = 0;
	pos = buf;
	while ((c = getc(stream)) != EOF) {
		if (bytes + 1 >= SSIZE_MAX) {
			errno = EOVERFLOW;
			return -1;
		}
		bytes++;
		if (bytes >= *n - 1) {
			buf = realloc(*lineptr, *n + _GETDELIM_GROWBY); // seg faults here
			if (buf == NULL) {
				errno = ENOMEM;
				return -1;
			}
			*n += _GETDELIM_GROWBY;
			pos = buf + bytes - 1;
			*lineptr = buf;
		}

		if (c == delimiter) {
			if (getdelim_include_delimiter == true) *pos++ = (char) c;
			line++;
			break;
		}
		else *pos++ = (char) c;
	}
	if (ferror(stream) || (feof(stream) && (bytes == 0))) {
		/* EOF, or an error from getc(). */
		return -1;
	}
	*pos = '\0';
	if (line == 1) {
		assert(line == 1);
		*lineptr += 3;
		bytes -= 3;
	}
	return bytes;
}


ssize_t getline(char **restrict lineptr, size_t *restrict n,
                FILE *restrict stream)
{
	if (getline_include_delimiter == true) getdelim_include_delimiter = true;
	// prioritize getdelim, use getline as a fallback
	if (getline_grow_by_allocation != 1 && getdelim_grow_by_allocation == 1) getdelim_grow_by_allocation = getline_grow_by_allocation;
	if (getline_minimum_allocation != 1 && getdelim_minimum_allocation == 1) getdelim_minimum_allocation = getline_minimum_allocation;
		
	return getdelim(lineptr, n, '\n', stream);
}
#define ps(x)      printf("%s = %s\n", #x, x);

struct lines{
    char stack_lines1[100];
    char stack_lines2[100];
 };

int main(int argc,char *argv[])
{
    struct lines st[20];
    FILE *file1,*file2;
    char * line1;
    int line_count1=0,flag2[10],flag1[10],line_count2=0;
    int i=0,j=0,k=0,n=0,m=0,o=0;
    if (argc < 2) {
    	puts("usage: diff file1 file2");
    	return -1;
    }
    file1 = fopen(argv[1],"r");
    size_t l;
    while (getline(&line1, &l, file1) != -1) {
    	ps(line1);
    }
    free(line1);
    fclose(file1);
    return 0;
}

