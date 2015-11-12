#include "config.h"
#include "ctype.h"
#include "genlib.h"
#include "stddefs.h"

#ifndef MEMCPY_CHUNK
#define MEMCPY_CHUNK (256*1024)
#endif

/* memcpy():
 *  Copy n bytes from 'from' to 'to'; return 'to'.
 *	This version of memcpy() tries to take advantage of address alignment.
 *  The goal is to do as many of the copies on 4-byte aligned addresses,
 *  falling back to 2-byte alignment, and finally, if there is no other
 *  way, simple byte-by-byte copy.
 *  Note that there is some point where the amount of overhead may exceed
 *  the byte count; hence, this will take longer for small byte counts.
 *  The assumption here is that small byte count memcpy() calls don't really
 *  care.
 */
char *
memcpy(char *to,char *from,int count)
{
	char	*to_copy, *end;

	to_copy = to;
	if (count) {
		end = to + count;
		while(to < end)
			*to++ = *from++;
	}
	return(to_copy);
}

void
bcopy(char *from, char *to, int size)
{
	memcpy(to,from,size);
}
