/* table_row.c
 */
/* Starbase Data Tables - An Ascii Database for UNIX
 
   copyright 1991, 1993, 1995, 1999 John B. Roll jr.
 */


#include "table.h"

TableRow table_rowget(file, table, row, justify, select, nsel)
     File	 file;	/* file descriptor to read from  */
     TableHead	 table; /* optional #TableHead	*/
     TableRow	 row;	/* optional #TableRow	*/
     int	*justify;
     int	*select;
     int	 nsel;
{
	int mode, n;

    if ( table && table->header ) {
	mode = table->mode;
	n    = table->header->ncol;
    } else {
	mode = 0;
	n    = -1;
    }

    if ( (row = table_parsline(file, row, mode, n)) == NULL )
	return NULL;

    if ( table && table->format )
	table_rowfmt(table, row, select, nsel);

    if ( table && table->mode & TABLE_PARSE )
        table_rowtrim(row, table->mwidth, table->mprec, n
			, justify, select, nsel);

   return row;
}
