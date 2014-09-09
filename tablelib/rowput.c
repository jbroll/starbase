/* table_rowput.c
 */
/* Starbase Data Tables - An Ascii Database for UNIX
 
   copyright 1991, 1993, 1995, 1999 John B. Roll jr.
 */


#include "table.h"

void table_dashes(file, n, ch)
	File	file;
	int 	n;
	int	ch;
{
	if ( n < 0 ) n = -n;

	while ( n-- ) PutC(file, '-');
	PutC(file, ch);
}

#define selcolumn(select, i)		( (select) ? (select)[i] : i )

#define putcolumn(file, select, i, last)	{			\
	int j = selcolumn(select, i);					\
									\
	if ( table && table->mode & TABLE_JUSTIFY ) {			\
		int 	owidth = table->owidth[j]; 			\
		int 	mwidth = table->mwidth[j]; 			\
		int	prec   = table->mprec ? table->mprec[j] : -1;	\
		int	jus;						\
									\
	    if ( justify == NULL ) jus = TAB_DEFAULT;			\
	    else	           jus = justify[j];			\
									\
	    table_colpad( file, j <= row->ncol ? row->column[j] : ""	\
		      	, mwidth, owidth				\
			, prec, jus);					\
	    PutC(file, last);						\
	} else {							\
	    if ( j <= row->ncol ) FPuts(file, row->column[j]);		\
	    PutC(file, last);						\
	}								\
    }

void table_rowput(file, table, row, justify, select, nsel)
			File		 file;
			TableHead	 table;
			TableRow	 row;
			int		*justify;
			int		*select;
			int		 nsel;
{
		int	i;
		char	ofs = table ? table->ofs : '\t';
		char	ors = table ? table->ors : '\n';

    if ( file == NULL || row == NULL ) return;

    if ( select == NULL ) nsel = row->ncol;

    for ( i = 1; i < nsel; i++ ) {
      if ( table && table->mode & TABLE_NICETAB ) {
	putcolumn(file, select, i, row->fields[j]);
      } else {
	putcolumn(file, select, i, ofs);
      }
    }

    putcolumn(file, select, i, ors);
}


/* table_hdrput
 */
void table_hdrput(file, table, flags, justify, select, nsel)
     File	 file;
     TableHead	 table;
     int	 flags;
     int	*justify;
     int	*select;
     int	 nsel;
{
  int	 i;
  int	*prec;
  char	ofs = table ? table->ofs : '\t';
  char	ors = table ? table->ors : '\n';

  /* Size up the header and table column widths.
   */
  if ( table->hwidth ) {
    if ( table->mode & TABLE_PARSE ) {
	int j;

     table_rowtrim(table->header, table->hwidth, NULL, table->header->ncol
			, justify, select, nsel);
     table_rowtrim(table->dashes, table->hwidth, NULL, table->dashes->ncol
			, justify, select, nsel);
    }
    for ( i = 1; i <= table->header->ncol; i++ )
      if ( table->owidth[i] >= 0 )
	table->owidth[i] = Max(table->hwidth[i], table->mwidth[i]);
    Free(table->hwidth);
  }

  if ( !flags ) flags = TBLHDR_ALL;

  /* knock the precision from the table so that the header
   * isn't padded like a number.
   */
  prec = table->mprec;	table->mprec = NULL;

  if ( flags & TBLHDR_VALUES && flags & TBLHDR_TEXT )
      for ( i = 0; i < table->nvalue; i++ )
	      table_rowput(file, table, table->values[i], NULL, NULL, 0);
  else
  if ( flags & TBLHDR_VALUES )
      /* Print out the values
       */
      for ( i = 0; i < table->nvalue; i++ )
	  if ( table->values[i]->ncol >= 2 && table_isid(table->values[i]->column[1]) )
	      table_rowput(file, table, table->values[i], NULL, NULL, 0);

  if ( flags & TBLHDR_HEADLINE )
      table_rowput(file, table, table->header, justify, select, nsel);

  /* Output the dashes line
   */
  if ( flags & TBLHDR_DASHLINE ) {
	int i;
	int j;

	if ( select == NULL ) nsel = table->dashes->ncol;

	for ( i = 1; i < nsel; i++ ) {
	    j = selcolumn(select, i);

	    if (  justify && justify[j] == TAB_NONE && table->owidth[j] >= 0
	      || !justify && table->owidth[j] == 0 ) {
		FPuts(file, table->dashes->column[j]);
		if ( table->mode & TABLE_NICETAB )
		    PutC(file, table->dashes->fields[j]);
		else
		    PutC(file, ofs);
		
	    } else
		table_dashes(file, Abs(table->owidth[j]), ofs);
	}
	j = selcolumn(select, i);
	if (  justify && justify[j] == TAB_NONE && table->owidth[j] >= 0
	  || !justify && table->owidth[j] == 0 ) {
	    FPuts(file, table->dashes->column[j]);
	    PutC(file, ors);
	} else
	    table_dashes(file, Abs(table->owidth[j]), ors);
  }

  table->mprec = prec;
}

