/* rowfmt.c
 */
/* Starbase Data Tables - An Ascii Database for UNIX
 
   copyright 1991, 1993, 1995, 1999 John B. Roll jr.
 */


#include <xos.h>
#include <xfile.h>
#include <table.h>

#define selcolumn(select, i)            ( (select) ? (select)[i] : i )

void table_rowfmt(table, row, select, nsel)
	TableHead	table;
	TableRow	row;
	int		*select;
	int		nsel;
{
	char	buffer[1024];	/* Well, there you go again! */

	int	ncol = row->ncol;
	char	*type;
	int	i;

    if ( !table ) return;
    if ( !table->format ) return;

    if ( select == NULL ) nsel = row->ncol;

    for ( i = 1; i <= nsel; i++ ) {
	    int col = selcolumn(select, i);

	if ( !col ) { continue; }

	if ( table->format[col] ) {
		int	 j;
		char	*str;

	    if ( col > ncol || ((str = table_colval(row, col)) && !*str) ) {
		for ( j = 0; j < Min(sizeof(buffer), table->fwidth[col]); j++ ) {
		    buffer[j] = ' ';
		}
		buffer[j] = '\0';
	    } else {
		switch ( table->ftype[col] ) {
		 case 0:	
		    snprintf(buffer, sizeof(buffer), table->format[col]
			    , table_colvali(row, col));
		    break;
		 case 'f':
		 case 'g':
		    snprintf(buffer, sizeof(buffer), table->format[col]
			    , table_colvald(row, col));
		    break;
		 case 's': 
		    snprintf(buffer, sizeof(buffer)
			   , table->format[col], table_colval(row, col));
		    break;
		 case ':':
		 case ' ':
		 case '@':
		 case 'b':
		 case 'x':
		 case 'o':
		    SAOconvert(buffer, table_colvald(row, col)
			    , table->ftype[col]
			    , table->fwidth[col]
			    , table->fprec[col]
			    , table->fflag[col]);
		    break;
		}
	    }

	    table_colset(row, col, buffer, strlen(buffer));
	}
    }
}
