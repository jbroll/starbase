/* table_colarg.c
 */
/* Starbase Data Tables - An Ascii Database for UNIX
 
   copyright 1991, 1993, 1995, 1999 John B. Roll jr.
 */

#include <xos.h>

#include "table.h"
#include "string.h"

int table_colarg(table, column, flag, form, value, check)
                        TableHead       table;
                        char            *column;
                        char            *flag;
                        char            *form[];
                        TableColArg     *value;
                        int             *check;
{
		char	*col;
		char	*here, *_ends;
		int	 modify;
		int	 i;

                double atof();
		int n;

        if ( column == NULL ) return 0;

	if ( table && (n = table_colnum(table, column)) )
	    return n;

	if ( !strchr(flag, '\001') ) {
	    NewString(col, column);
	    modify = 0;
	} else {
	    col = column;
	    modify = 1;
	}

	for ( i = 0; flag[i] && flag[i] != '\001'; i++ ) {
	    check[i] = 0;
	}
 
	here = col;
	_ends = &here[strlen(here)];

        if ( flag ) {
            for ( here = strpbrk(col, flag); here; here = strpbrk(here, flag) ) {
                int idx = strchr(flag, *here) - flag;
		char	*ends;

                if ( check[idx] )
                   FPrint(Stderr, "column flag: \"%c\": specified more than once\n", flag[idx]);

                check[idx] = 1;

                if ( *form[idx] != 'S' ) {
		    *here++ = '\0';
		    ends = strpbrk(here, flag);
		} else {
		    ends = strpbrk(here+1, flag);
		}

		if ( ends == NULL ) ends = _ends;
		 
 
 
                /**/ if ( *form[idx] == 'i' )
                              value [idx].i = strtol(here, &here, 0);
                else if ( *form[idx] == 's' || *form[idx] == 'S' ) {

                   value[idx].s = malloc((ends - here) + 1);
		   strncpy(value[idx].s, here, (ends - here));
		   *(value[idx].s + (ends - here)) = '\0';

                   if ( *form[idx] == 'S' ) *here++ = '\0';
		}
                else if ( *form[idx] == 'r' )
                   value[idx].f = strtod(here, &here);
                else if ( *form[idx] == 'f' ) {
                    value[idx].i = 0;
                    for ( ; *here && here < ends; here++ ) {
			char *k;

                        if ( (k = strchr(&form[idx][2], *here)) )
                            value[idx].i |= 1 << (k - &form[idx][2]);
                        else
                            FPrint(Stderr, "column flag: \"%c\" not in set of allowed flags: %s\n", *here, form[idx]);
		    }	
		}
	   }
	}

	n = table_colnum(table, col);
	if ( !modify ) Free(col);
	return n;
}
