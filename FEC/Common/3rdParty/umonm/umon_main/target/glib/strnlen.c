#include "config.h"
#include "ctype.h"
#include "genlib.h"
#include "stddefs.h"

/* strlen():
 * Returns the number of
 * non-NULL bytes in string argument.
 */
int
strnlen(char *s, int maxlen)
{
	int i;
	char *sp = s;

	for(i=0;i<maxlen;sp++,i++) {
		if (*sp == 0)
			return(i);
	}
	return(maxlen);
}
