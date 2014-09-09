/* table_add.c
 */
/* Starbase Data Tables - An Ascii Database for UNIX
 
   copyright 1991, 1993, 1995, 1999 John B. Roll jr.
 */

#include "table.h"


int table_coladd(table, text)
			TableHead	 table;
			char		*text;
{
  int i;
  int leng = strlen(text) + 1;

  /* Insert a new column in the header
   */
  table_rowloc(table->header, table->header->nbuf + leng
			    , table->header->acol + 1);
  table->header->ncol++;

  table->header->nbuf += leng;
  table->header->column[table->header->ncol + 1]
    = &table->header->buffer[table->header->nbuf];

  strcpy(table->header->column[table->header->ncol], text);


  /* Insert a new column in the dashes
   */
  table_rowloc(table->dashes, table->dashes->nbuf + leng
                            , table->dashes->acol + 1);
  table->dashes->ncol++;

  table->dashes->nbuf += leng;
  table->dashes->column[table->dashes->ncol + 1]
    = &table->dashes->buffer[table->dashes->nbuf];

  for ( i = 0; i < leng - 1; i++ ) 
	table->dashes->column[table->dashes->ncol][i] = '-';
  table->dashes->column[table->dashes->ncol][i] = '\0';


  if ( table->format ) {
    ReAlloc(table->format, (table->header->ncol + 2) * sizeof(char *));
    table->format[table->header->ncol] = NULL;

    ReAlloc(table->ftype , (table->header->ncol + 2) * sizeof(int *));
    table->ftype [table->header->ncol] = 0;
    ReAlloc(table->fwidth, (table->header->ncol + 2) * sizeof(int *));
    table->fwidth[table->header->ncol] = 0;
    ReAlloc(table->fprec , (table->header->ncol + 2) * sizeof(int *));
    table->fprec [table->header->ncol] = 0;
    ReAlloc(table->fflag , (table->header->ncol + 2) * sizeof(int *));
    table->fflag [table->header->ncol] = 0;
  }

  if ( table->hwidth ) {
    ReAlloc(table->hwidth, (table->header->ncol + 2) * sizeof(int));
    table->hwidth[table->header->ncol] = leng - 1;
  }
  ReAlloc(table->mprec , (table->header->ncol + 2) * sizeof(int));
  table->mprec [table->header->ncol] = 0;
  ReAlloc(table->mwidth, (table->header->ncol + 2) * sizeof(int));
  table->mwidth[table->header->ncol] = 0;
  ReAlloc(table->owidth, (table->header->ncol + 2) * sizeof(int));
  table->owidth[table->header->ncol] = 0;

  return table->header->ncol;
}
