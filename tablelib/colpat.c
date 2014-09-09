/* table_colarg.c
 */
/* Starbase Data Tables - An Ascii Database for UNIX
 
   copyright 1991, 1993, 1995, 1999 John B. Roll jr.
 */

#include <xos.h>

#include "table.h"
#include "string.h"

int table_colpat(tabl, patt, list, maxx)
			TableHead	 tabl;
			char		*patt;
			int		*list;
			int		 maxx;
{
	int	n = 0;
	int	i;

	for ( i = 1; i <= tabl->header->ncol; i++ ) {
	    if ( tmatch(tabl->header->column[i], patt) ) {
	        list[n++] = i;

		if ( n >= maxx ) return n;
	    }
	}

	return n;
}


int table_colargpat(tabl, col, name, leng, flag, form, value, check, list, maxx)
			TableHead	 tabl;
                        char            *col;
			char		*name;
			int		 leng;
                        char            *flag;
                        char            *form[];
                        TableColArg     *value;
                        int             *check;
			int		*list;
			int		 maxx;
{
	char	*here = NULL;
	int	 n;
	int	 chr;

	if ( list[0] = table_colarg(tabl, col, flag, form, value, check) ) {
	    	strncpy(name, table_colnam(tabl, list[0]), maxx);
		return 1;
	} else {
	    if ( here = strpbrk(col, flag) ) {
		 chr = *here;
		 *here = '\0';
	    }

	    n = table_colpat(tabl, col, list, maxx);

	    if ( name ) strncpy(name, col, leng);
	    if ( here ) *here = chr;

	    return n;
	}
}
