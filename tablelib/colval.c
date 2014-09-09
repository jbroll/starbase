/* colval.c
 */
/* Starbase Data Tables - An Ascii Database for UNIX
 
   copyright 1991, 1993, 1995, 1999 John B. Roll jr.
 */


#include "table.h"

int table_colvali(row, col)
	TableRow	row;
	int		col;
{
	int		i = 1;

	if ( row->ncol < col ) return 0;

	return strtol(row->column[col], NULL, 0);
}

double table_colvalr(row, col)
	TableRow	row;
	int		col;
{
	int		i = 1;
	char		*str;

	if ( row->ncol < col ) return 0;

	str = row->column[col];
	while ( *str == ' ' ) str++;

	if ( !strcmp(str, "INDEF") ) return INDEFR;

	return SAOstrtod(row->column[col], NULL);
}

double table_colvald(row, col)
	TableRow	row;
	int		col;
{
	int		i = 1;
	char		*str;

	if ( row->ncol < col ) return 0;

	str = row->column[col];
	while ( *str == ' ' ) str++;

	if ( !strcmp(str, "INDEF") ) return INDEFD;

	return SAOstrtod(row->column[col], NULL);
}

char *table_colvals(row, col)
        TableRow        row;
        int             col;
{
	return strdup(table_colval(row, col));
}

char *table_colval(row, col)
	TableRow	row;
	int		col;
{
	int		i = 1;

	if ( row->ncol < col ) return "";

	return row->column[col];
}


