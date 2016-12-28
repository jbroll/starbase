/* table_setcol.c
 */
/* Starbase Data Tables - An Ascii Database for UNIX
 
   copyright 1991, 1993, 1995, 1999 John B. Roll jr.
 */

#include "table.h"


void prrow(TableRow row) {
    int i;

    fprintf(stderr, "prrow ncol %d nbuf %d abuf %d\n", row->ncol, row->nbuf, row->abuf);
    for ( i = 1; i <= row->ncol; i++ ) {
	fprintf(stderr, "  col %p size %ld\n", row->column[i], row->column[i+1] - row->column[i]);
    }
}

void table_colseti(row, col, value, format)
	TableRow	 row	/* The row in which to change the column
				   value.				*/;
	int		 col	/* The column to set.			*/;
	int	         value	/* A character pointer to the new value */;
	char		*format;
{
	    char	valuestr[64];

    sprintf(valuestr, format == NULL ? "%d" : format, value);
    table_colset(row, col, valuestr, strlen(valuestr));
}

void table_colsetd(row, col, value, format)
	TableRow	 row	/* The row in which to change the column
				   value.				*/;
	int		 col	/* The column to set.			*/;
	double	         value	/* A character pointer to the new value */;
	char		*format;
{
	    char	valuestr[64];

    sprintf(valuestr, format == NULL ? "%g" : format, value);
    table_colset(row, col, valuestr, strlen(valuestr));
}

void table_colsets(row, col, value)
	TableRow	 row	/* The row in which to change the column
				   value.				*/;
	int		 col	/* The column to set.			*/;
	char	        *value	/* A character pointer to the new value */;
{
    table_colset(row, col, value, strlen(value));
}

TableRow table_colset(row, col, value, length)
	TableRow	row	/* The row in which to change the column
				   value.				*/;
	int		col	/* The column to set.			*/;
	char	       *value	/* A character pointer to the new value */;
	int		length	/* The length of the new value		*/;
{
	int diff, i;

	/* Add some new column if necessary
	 */
	if ( row->ncol < col ) {
			int i = row->ncol;
		        int nbuf = row->abuf;
			int ncol = row->acol;

		if ( row->abuf < row->nbuf + (col - row->ncol) )
		    nbuf = row->nbuf + (col - row->ncol);

		if ( row->acol < col )
		    ncol = col;

		if ( row->abuf != nbuf || row->acol != ncol )
		    table_rowloc(row, nbuf, col+1);

		for ( i++; i <= col + 1; i++ ) {
		    row->column[i] = &row->buffer[row->nbuf++];
		}

		row->ncol = col;
	}

	//fprintf(stderr, "colset %d of %d %p %d\n", col, row->ncol, value, length);
	//prrow(row);

	/* Compute the difference in length between the current value at 
	 * column col and the length of the new value.
	 */
	diff = (length + 1) - (row->column[col+1] - row->column[col]);

	if ( diff + row->nbuf > row->abuf ) {
	    /* Make the buffer larger	
	     */
	    table_rowloc(row, diff + row->nbuf, row->ncol);
	}

	row->nbuf += diff;

	//prrow(row);

	if ( diff ) {
		    char	*p;


	    /* Shift the data in buffer by diff.  Taking care
	     * not to copy over ourselves.
	     */
	    if ( diff > 0 ) {
		for ( p  = row->column[row->ncol + 1]
		    ; p >= row->column[      col + 1]
		    ; p-- ) {
			*(p + diff) = *p;
		}
	    } else {
		for ( p  = row->column[      col + 1]
		    ; p <= row->column[row->ncol + 1]
		    ; p++ ) {
			*(p + diff) = *p;
		}
	    }

	    /* Move the column pointers to see the data.
	     */
	    for ( i = col + 1; i <= row->ncol + 1; i++ )
		row->column[i] += diff;
	}

	//prrow(row);

	//fprintf(stderr, "strncpy %p %p %d\n", row->column[col], value, length);
	strncpy(row->column[col], value, length);
	row->column[col][length] = '\0';

	return row;
}
