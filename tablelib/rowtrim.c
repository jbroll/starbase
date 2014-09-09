/* table_rowtrim.c
 */
/* Starbase Data Tables - An Ascii Database for UNIX
 
   copyright 1991, 1993, 1995, 1999 John B. Roll jr.
 */


#include <ctype.h>
#include "table.h"
		

void table_rowtrim(row, width, prec, ncol, justify, select, nsel)
			TableRow	 row;
			int		*width;
			int		*prec;
			int		 ncol;
			int		*justify;
			int		*select;
			int		 nsel;
{
  int   x;

  if ( ncol == -1 ) {
      ncol = row->ncol;
  } else {
      ncol = Min(ncol, row->ncol);
  }
  nsel = select == NULL ? ncol : nsel;

  for ( x = 1; x <= nsel; x++ ) {

    int   	  k	/* The current character    	*/
      	, dot = -1	/* decimal point of floating	*/
	, chr =  0	/* first non space character	*/
	, pad =  0	/* first tail pad character 	*/
	, dig =  0;	/* number of digits		*/

    int	wid;
    int	j = select ? select[x] : x;

    if ( j == 0 || j > ncol ) continue;

    if ( justify && justify[j] == TAB_NONE )
    	continue;
    if ( prec && prec[j] < -2 ) 
	continue;

    for ( k = 0;   row->column[j][k] != '\0' && row->column[j][k] == ' '; k++ );
    chr = k;
    pad = k;

    for ( ; row->column[j][k] != '\0'; k++ ) {
      if ( row->column[j][k] != ' ' ) pad = k + 1;
      if ( prec && prec[j] >= -1 && dot != -2 ) {
	if ( row->column[j][k] == '.' )
	  if ( dot > 0 )  dot = -2; /* Two dots */
	  else	   	  dot = k;
	if ( isdigit(row->column[j][k]) ) {
	    dig++;
	    continue;
	}

	switch ( row->column[j][k] ) {
	 case ' ' :
	 case '.' : continue;
	 case 'h' :
	 case ':' :
	 case 'm' :
	 case 'E' :
	 case 'e' : if ( k != chr ) {
		 	switch ( row->column[j][k+1] ) {
		         case '0' :
		         case '1' :
		         case '2' :
		         case '3' :
		         case '4' :
		         case '5' :
		         case '6' :
		         case '7' :
		         case '8' :
		         case '9' :
		         case '+' :
		         case '-' : {
			    continue;
			 }
			}
		    }
	 case '-' :
	 case '+' : if (   k == chr 
			|| row->column[j][k-1] == 'E' 
			|| row->column[j][k-1] == 'e' )
		        continue;
	}

	dot = -2;
      }
    }

    /* Trim the string to only the characters that count
     */
    row->column[j][pad]  ='\0';
    row->column[j] 	+= chr;

    if ( !width 		 ) continue;
    if ( *row->column[j] == '\0' ) continue;

    /* The string has been parsed.  Now figure out how much widther
     * the column might have to be to accomidate the width and prec
     * that exist in previous entries and this guy.
     */
    wid = pad - chr;

    if ( prec ) {
	if ( dig > 0 && wid > 0 && dot != -2 && prec[j] >= 0 ) {
	  int pre;
	  int l0, l1;

	  if ( dot == -1 ) pre = 0;
	  else 		   pre = pad - dot - 1;

	  /* The width of the string to the left of a "."
	   */
	  l0 = width[j] - prec[j] - ( prec[j] != 0 );
	  l1 = wid      - pre     - ( pre     != 0 );

	  /* The width of the field = left + right + possible "."
	   */
	  if ( width[j] >= 0 ) {
	     width[j] = Max(l0, l1)
		      + Max(prec[j], pre)
		      +    (prec[j] || pre);
	  }
	  prec[j]  = Max(prec[j], pre);
	} else {
	  prec[j] = -1;
	  if ( width[j] >= 0 ) width[j] = Max(width[j], wid);
	}
    } else {
	if ( width[j] >= 0 ) width[j] = Max(width[j], wid);
    }
  }
}
