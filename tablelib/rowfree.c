/* table_rowfree.c
 */
/* Starbase Data Tables - An Ascii Database for UNIX
 
   copyright 1991, 1993, 1995, 1999 John B. Roll jr.
 */


#include "table.h"


void table_rowfree(row)
     TableRow	row;
{
  /*- Free the memory allocated by #table_row		*/

  if ( row->abuf != -1 )
    Free(row->buffer);
  Free(row->column);
  Free(row->fields);
  Free(row);
}
