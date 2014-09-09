/* table_colnum.c
 */
/* Starbase Data Tables - An Ascii Database for UNIX
 
   copyright 1991, 1993, 1995, 1999 John B. Roll jr.
 */


#include "table.h"
#include <ctype.h>

int table_colnummatch(row, text, index, mode)
     TableRow	 row;
     char	*text;
     int	 index;
     int	 mode;
{
  int	i, j, k;
  int leng = strlen(text);

  /* Compare input text with column names ignoring 
   * leading and trailing space.
   */
  for ( i = index+1; i <= row->ncol; i++ ) {
    for ( j = 0; row->column[i][j] == ' '
	 && row->column[i][j] != '\0'  ; j++ );

    for ( k = 0; 
	( row->column[i][j] == text[k] )
	 && row->column[i][j] != '\0'
	 && text[k] 	  != '\0'
	; j++, k++ );

    if ( k == leng ) {
      for ( ; row->column[i][j] == ' '
	   && row->column[i][j] != '\0'; j++);

      if ( row->column[i][j] == '\0' ) {
	return i;
      }
    }
  }


  return index;
}

/* Return the column number in the table #table named by the 
 * #text string.
 */
int table_colnum(table, text) 
     TableHead	 table;
     char	*text;
{
  if ( table == NULL ) return 0;

  return table_colnummatch(table->header, text, 0, table->mode);
}

char *table_colnam(table, col)
     TableHead   table;
     int	 col;
{
	return table->header->column[col];
}
