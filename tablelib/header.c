/* table_header.c
 */
/* Starbase Data Tables - An Ascii Database for UNIX
 
   copyright 1991, 1993, 1995, 1999 John B. Roll jr.
 */


#include "table.h"

char *SlurpLine();

int table_isdash(row)
	TableRow	row;
{
	int	i;

	for ( i = 1; i <= row->ncol; i++ ) {
		int	dash = 0;
		int	j;

		for ( j = 0; row->column[i][j]; j++ ) {
			/* The dashline may have only '-' or ' '.
			 */
			if ( row->column[i][j] != ' '
			  && row->column[i][j] != '-' ) {
				return 0;
			}

			if ( row->column[i][j] == '-' ) dash++;
		}

		/* The dashline must have at least 1 dash in each col.
		 */

		if ( !dash ) {
		    return 0;
		}
	}
	return 1;
}

TableHead table_header(file, mode)
    File	 file;	/* file descriptor of ascii table */
    int		 mode;
{
  TableHead	 T = NULL;
  TableHead	 table;
  TableRow	 header;
  int		j;

  int		nmax = 80;

  char		*check;
  int		 n;

  char *getenv();

  char	*parse = getenv("STARBASE_TAB");

  /* A TableHead structure is allocated from the heap, initialized
   * with the ascii table header information read in from the file
   * given in the #file argument and returned.
   */
  Malloc(table, sizeof(struct _TableHead));
  table->name   = NULL;
  table->hwidth = NULL;
  table->mwidth = NULL;
  table->owidth = NULL;
  table->mprec	= NULL;
  table->header = NULL;
  table->dashes = NULL;
  table->values = NULL;
  table->nvalue = 0;
  table->avalue = 0;
  table->mode   = mode | TABLE_STRICTEOF;
  table->format = NULL;

  table->ofs = '\t';
  table->ors = '\n';

  if ( file == NULL ) return table;

  if ( parse && !strcmp(parse, "NICE") ) {
	table->mode |= TABLE_NICETAB;
  }

  /* Handle starbase ASCII table.
   */
  if ( (       header = table_rowget(file, table, NULL, NULL, NULL, 0)) == NULL ) goto done;
  if ( (table->dashes = table_rowget(file, table, NULL, NULL, NULL, 0)) == NULL ) goto done;

  if ( !table_isdash(table->dashes) ) {
	do {
		if ( table->nvalue >= table->avalue ) {
			table->avalue *= 2;
			table->avalue += 10;
			ReAlloc(table->values, table->avalue * sizeof(TableRow));
		}

		/* Get the table name and polish it
		 */
		if ( table->name == NULL
		   && header->ncol == 1 
		   && header->column[1][0] != '\0' ) {
			char *name, *here, *ptr;

			NewString(name, header->column[1]);
			ptr = name;

			while ( *ptr ==  ' ' ) ptr++;
			here = ptr;
			while ( *ptr != '\0' ) ptr++;
			ptr--;
			while ( *ptr ==  ' ' ) ptr--;
			ptr++;
			*ptr = '\0';

			NewString(table->name, here);
			Free(name);
		}
		
		if ( !strcmp(header->column[1], "TAB")
		  && header->ncol == 2
		  && !strcmp(header->column[2], "NICE") ) {
		    table->mode = mode |= TABLE_NICETAB;
		}
		if ( !strcmp(header->column[1], "TAB")
		  && header->ncol == 2
		  && !strcmp(header->column[2], "STRICT") ) {
		    table->mode = mode = table->mode & ~TABLE_NICETAB;
		}

		table->values[table->nvalue++] = header;
		       header                  = table->dashes;

	} while ( (table->dashes = table_rowget(file, table, NULL, NULL, NULL, 0))
	       	&& !table_isdash(table->dashes) );

	if ( !table->dashes )					goto done;
  }

  table->header = header;

  if ( table->header->ncol != table->dashes->ncol )
    FPrint(Stderr, "table : different number of columns in header and dashline\n");

  T = table;
  T->mode = mode;

  if ( T->name == NULL ) NewString(table->name, "None");

 done:
  if ( T == NULL ) {
    table_hdrfree(table);
  } else {
      if ( T->mode & TABLE_NICETAB ) {
	table->ofs = ' ';
      }
      Calloc(table->hwidth, sizeof(int) * (table->header->ncol + 1));
      Calloc(table->mwidth, sizeof(int) * (table->header->ncol + 1));
      Calloc(table->mprec , sizeof(int) * (table->header->ncol + 1));
      Calloc(table->owidth, sizeof(int) * (table->header->ncol + 1));
  }

  return T;
}
