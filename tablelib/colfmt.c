/* colfmt.c
 */
/* Starbase Data Tables - An Ascii Database for UNIX
 
   copyright 1991, 1993, 1995, 1999 John B. Roll jr.
 */


#include <xos.h>
#include <xfile.h>
#include <table.h>

int table_colfmt(table, col, format)
	TableHead	table;
	int		col;
	char		*format;
{
	int	i;
	int	type;
	int	prec;
	int	flag;
	int	width;

    if ( !table ) return 0;
    if ( col > table->header->ncol )
	return 0;

    if ( !SAOparsefmt(format, &type, &width, &prec, &flag) )
	return 0;

    if ( !table->format ) {
	Malloc(table->fwidth, (table->header->ncol+2) * sizeof(char *));
	Malloc(table->ftype , (table->header->ncol+2) * sizeof(char *));
	Malloc(table->fprec , (table->header->ncol+2) * sizeof(char *));
	Malloc(table->fflag , (table->header->ncol+2) * sizeof(char *));

	Malloc(table->format, (table->header->ncol+2) * sizeof(char *));
	for ( i = 0; i <= table->header->ncol; i++ )
	    table->format[i] = NULL;
    }

    if ( table->format[col] ) {
	Free(table->format[col]);
    }

    table->format[col] = strdup(format);

    table->fwidth[col] = width;
    table->fprec[col]  = prec;
    table->fflag[col]  = flag;
    table->ftype[col]  = type;

    table_setdash(table, col, width);

    return 1;
}

void table_setdash(table, col, n)
        TableHead       table;
        int             col;
        int             n;
{
	int	i;
	char	dashes[256];

    n = Clip(1, 255, n);

    for ( i = 0; i < n; i++ ) {
	    dashes[i] = '-';
    }
    dashes[n] = '\0';

    table_colset(table->dashes, col, dashes, n);
}    
