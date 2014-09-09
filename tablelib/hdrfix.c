/* hdrfix.c
 */
/* Starbase Data Tables	- An Ascii Database for UNIX

   copyright 1991, 1993, 1995, 1999 John B. Roll jr.
 */

#include <table.h>

int table_hdrfix(ofile, str, n, d)
	FILE	*ofile;
	char	*str;
	int	 n;
	int	 d;
{
	char	buf[TABLE_TRIMBUF];

		char *tr = trim(str, buf);

	if ( n ) {
	    fprintf(ofile, "%s_%d%c", tr, n, d);
	    return strlen(tr) + 2;
	} else {
	    fprintf(ofile, "%s%c", str, d);
	    return strlen(str);
	}
}
