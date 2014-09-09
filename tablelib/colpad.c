/* table_colpad.c
 */
/* Starbase Data Tables - An Ascii Database for UNIX
 
   copyright 1991, 1993, 1995, 1999 John B. Roll jr.
 */


#include "table.h"


void table_colpad(file, str, mwidth, owidth, prec, type)
    File	 file;
    char	*str;
    int		mwidth;
    int		owidth;
    int		prec;
    int		type;
{
		int	 len = strlen(str);
		int	i;
		int	cwidth;

    if ( owidth < 0 && type == TAB_NONE )
	type = TAB_DEFAULT;

    if ( type == TAB_DEFAULT )
	if ( prec == -1 )   type = TAB_LEFT;
	else 	 	    type = TAB_RIGHT;

    if ( type == TAB_NONE ) {
	FPuts(file, str);
	return;
    }

    if ( owidth ) cwidth = owidth;
    else          cwidth = mwidth;

    /* Cramp output to max width chars
     */
    if ( cwidth < 0 ) {
	cwidth = -cwidth;
	if ( cwidth < len ) {
	    if ( type == TAB_RIGHT ) str += len - cwidth;
	    len = cwidth;
	}
    }

    if ( mwidth > cwidth ) mwidth = cwidth;

    if ( prec > 0 ) {
		char 	*pnt = str;
		int   dot, pre, pst;
		int   i;

	    switch ( type ) {
	     case TAB_RIGHT:
	        for ( i = 0; i < cwidth - mwidth; i++ ) PutC(file, ' ');
		break;
	     case TAB_CENTER:
		for ( i = 0; i < (cwidth - mwidth + 1) / 2; i++ )
		    PutC(file, ' ');
		break;
	    }

	    if ( (pnt = strchr(str, '.')) == NULL ) pnt = str + len;
	    dot = len - (pnt - str);

	    if ( (pst = prec - (dot - 1)) < 0 ) pst = 0;
	    pre = mwidth - (len + pst);

	    for ( i = 0; i < pre; i++ ) PutC(file, ' ');
	    for ( i = 0; i < len; i++ ) PutC(file, str[i]);
	    for ( i = 0; i < pst; i++ ) PutC(file, ' ');

	    switch ( type ) {
	     case TAB_LEFT:
	        for ( i = 0; i < cwidth - mwidth; i++ ) PutC(file, ' ');
		break;
	     case TAB_CENTER:
		for ( i = 0; i < (cwidth - mwidth) / 2; i++ )
		    PutC(file, ' ');
		break;
	    }
    } else {
	switch ( type ) {
	 case TAB_RIGHT:
	    for ( i = 0; i < cwidth - len; i++ ) PutC(file, ' ');
	    for ( i = 0; i < len; i++ ) 	 PutC(file, str[i]);
	    break;
	 case TAB_LEFT:
	    for ( i = 0; i < len; i++ ) 	 PutC(file, str[i]);
	    for ( i = 0; i < cwidth - len; i++ ) PutC(file, ' ');
	    break;
	 case TAB_CENTER:
	    for ( i = 0; i < (cwidth - len + 1) / 2; i++ ) PutC(file, ' ');
	    for ( i = 0; i < len; i++ ) 	           PutC(file, str[i]);
	    for ( i = 0; i < (cwidth - len    ) / 2; i++ ) PutC(file, ' ');
	    break;
	}
    }
}
