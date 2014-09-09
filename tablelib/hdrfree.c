/* tabel_done.c
 */
/* Starbase Data Tables - An Ascii Database for UNIX
 
   copyright 1991, 1993, 1995, 1999 John B. Roll jr.
 */


#include "table.h"

/*1 table_hdrfree - Free a TableHead structure when done.
#*/
void	 table_hdrfree(table)
			TableHead	 table;
{
	int	i;

  /* Free up the table record that was allocated with table_header
   */
  if ( table == NULL ) return;

  if ( table->name ) Free(table->name);

  if ( table->values ) {
      for ( i = 0; i < table->nvalue; i++ )
       		table_rowfree(table->values[i]);
      Free(table->values);
  }

  if ( table->format ) {
      if ( table->ftype  ) Free(table->ftype);
      if ( table->fwidth ) Free(table->fwidth);
      if ( table->fprec  ) Free(table->fprec);
      if ( table->fflag  ) Free(table->fflag);

	for ( i = 0; i < table->header->ncol; i++ )
	    if ( table->format[i] ) Free(table->format[i]);
  }

  if ( table->header ) table_rowfree(table->header);
  if ( table->dashes ) table_rowfree(table->dashes);
  if ( table->hwidth ) Free(table->hwidth);
  if ( table->mwidth ) Free(table->mwidth);
  if ( table->owidth ) Free(table->owidth);
  if ( table->mprec  ) Free(table->mprec);

  Free(table);
}
