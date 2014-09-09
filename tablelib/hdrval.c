/* hdrval.c
 */
/* Starbase Data Tables - An Ascii Database for UNIX
 
   copyright 1991, 1993, 1995, 1999 John B. Roll jr.
 */

/*
	Lookup, Add Delete and Manipulate

	table_hdrfind()
	table_hdrnvalues()
	table_hdrnth()

	table_hdradd()
	table_hdraddrow()
	table_hdrdel()
	table_hdrdelval()


	Get/ Set values

	table_valn()

	table_hdrget()
	table_hdrgetb()
	table_hdrgeti()
	table_hdrgetd()
	table_hdrgets()

	table_hdrset()
	table_hdrsetb()
	table_hdrseti()
	table_hdrsetd()
	table_hdrsets()

	
	Low level

	table_valnew()
 */


#include "table.h"


TableRow table_hdrfind(t, name, n)
	TableHead	 t;
	char		*name;
	int		 n;
{

	int	i;
	int	l;

  	if ( t == NULL ) return NULL;
	if ( n == 0    ) n = 1;

	l = strlen(name);

	while ( *name == ' ' ) name++;

	for ( i = 0; i < t->nvalue; i++ ) {
		char	*p = t->values[i]->column[1];

		if (    *p == '#' ) p++;
		while ( *p == ' ' ) p++;

		if ( !strncmp(p, name, l) && p[l] == '\0' ) {
		    n--;
		    if ( !n ) return t->values[i];
		}
	}

	return NULL;
}


TableRow table_hdrnth(table, nth)
	TableHead	table;
	int		nth;
{
	if ( nth > table->nvalue ) return NULL;
	else			   return table->values[nth-1];
}

int table_hdrnvalues(table)
	TableHead	table;
{
	return table->nvalue;
}

int table_hdrgetncol(table, name, n)
        TableHead        table;
        char            *name;
	int		 n;
{
		TableRow val;

	if ( (val = table_hdrfind(table, name, n)) ) {

	    return val->ncol - 1;
	}
	
	return 0;
}


char *table_hdrget(t, name, row, col)
        TableHead   t;
        char    *name;
	int	 row;
	int	 col;
{
		TableRow val;

	if ( (val = table_hdrfind(t, name, row)) ) {
	    if ( val->ncol >= col )
		    return val->column[col + 1];
	    else    return NULL;
	}
	    
	return NULL;
}

int table_hdrgetb(t, name, row, col)
	TableHead	t;
	char	*name;
	int	 row;
	int	 col;
{
	char	*v;

	if ( (v = table_hdrget(t, name, row, col)) ) {
	    while ( *v == ' ' ) v++;
	    return 	\
		       (!strncmp(v, "T", 1)	&& v[1] == '\0')
		    || (!strncmp(v, "True", 4)	&& v[4] == '\0')
		    || (!strncmp(v, "true", 4)	&& v[4] == '\0')
		    || (!strncmp(v, "TRUE", 4)	&& v[4] == '\0')
		    || (!strncmp(v, "yes", 3)	&& v[3] == '\0')
		    || (!strncmp(v, "Yes", 3)	&& v[3] == '\0')
		    || (!strncmp(v, "YES", 3)	&& v[3] == '\0')
		    || (!strncmp(v, "on", 2)	&& v[2] == '\0')
		    || (!strncmp(v, "On", 2)	&& v[2] == '\0')
		    || (!strncmp(v, "ON", 2)	&& v[2] == '\0')
		    || (!strncmp(v, "1", 1)	&& v[1] == '\0');
	}

	return 0;
}

int table_hdrgeti(t, name, row, col)
	TableHead	t;
	char	*name;
	int	 row;
	int	 col;
{
	char	*v;

	if ( (v = table_hdrget(t, name, row, col)) ) {
		return atoi(v);
	}

	return 0;
}


double table_hdrgetd(t, name, row, col)
	TableHead	t;
	char	*name;
	int	 row;
	int	 col;
{
	char	*v;

	if ( (v = table_hdrget(t, name, row, col)) ) {
		return SAOstrtod(v, NULL);
	}

	return 0.0;
}

char *table_hdrgets(t, name, row, col)
	TableHead	t;
	char	*name;
	int	 row;
	int	 col;
{
	char	*v1, *v2;

	if ( (v1 = table_hdrget(t, name, row, col)) ) {
		NewString(v2, v1);
	} else
		NewString(v2, "");

	return v2;
}

TableRow table_valnew()
{
	TableRow val;

	return Calloc(val, sizeof(struct _TableRow));
}

TableRow table_hdraddrow(table, row)
	TableHead	table;
	TableRow	row;
{
	if ( table->nvalue >= table->avalue ) {
		table->avalue *= 2;
		table->avalue += 10;
		ReAlloc(table->values, table->avalue * sizeof(TableRow));
	}

	table->values[table->nvalue] = row;

	table->nvalue++;
	return table->values[table->nvalue-1];
}

TableRow table_hdradd(table, name)
	TableHead	table;
	char		*name;
{
	return table_hdraddrow(table
		, table_colset(table_rowloc(NULL, 0, 0), 1, name, strlen(name)));
}


void table_hdrset(t, name, row, col, value, flag)
	TableHead	 t;
	char		*name;
	int		 row;
	int		 col;
	char		*value;
	int		 flag;
{
		TableRow	val;

	if ( ((val = table_hdrfind(t, name, row)) == NULL) && flag ) {
	    val = table_hdradd(t, name);
	}

	if ( val == NULL ) {
	    return;
	}

	table_colset(val, col + 1, value, strlen(value));
}

void table_hdrsetb(t, name, row, col, value, flag)
	TableHead	 t;
	char		*name;
	int		 row;
	int		 col;
	int		 value;
	int		 flag;
{
		char	buffer[64];

	sprintf(buffer, "%s", value == 0 ? "F" : "T");
	table_hdrset(t, name, row, col, buffer, flag);
}

void table_hdrseti(t, name, row, col, value, format, flag)
	TableHead	 t;
	char		*name;
	int		 row;
	int		 col;
	int		 value;
	char		*format;
	int		 flag;
{
		char	buffer[64];

	sprintf(buffer, format == NULL ? "%d" : format, value);
	table_hdrset(t, name, row, col, buffer, flag);
}

void table_hdrsetd(t, name, row, col, value, format, flag)
	TableHead	 t;
	char		*name;
	double		 value;
	int		 row;
	int		 col;
	char		*format;
	int		 flag;
{
		char	buffer[64];

	sprintf(buffer, format == NULL ? "%6g" : format, value);
	table_hdrset(t, name, row, col, buffer, flag);
}

void table_hdrsets(t, name, row, col, value, flag)
	TableHead	 t;
	char		*name;
	int		 row;
	int		 col;
	char		*value;
	int		 flag;
{
	table_hdrset(t, name, row, col, value, flag);
}


TableRow table_hdrdelval(table, val)
	TableHead	table;
	TableRow	val;
{
		int	 i;

	for ( i = 0; i < table->nvalue; i++ ) {
		if ( val == table->values[i] ) break;
	}

	if ( i == table->nvalue ) return NULL;

	for ( ; i < table->nvalue - 1; i++ ) {
		table->values[i] = table->values[i+1];
	}

	return val;
}

void table_hdrdel(table, name, row)
	TableHead	table;
	char		*name;
	int		 row;
{
		TableRow	val = table_hdrfind(table, name, row);

	if ( val != NULL ) table_rowfree(table_hdrdelval(table, val));
}


int table_hdrtablencol(table, name) 
	TableHead	table;
	char		*name;
{
	return table_hdrgetncol(table, name, 1);
}
int table_hdrtablecolnum(table, name, colname)
	TableHead	table;
	char		*name;
	char		*colname;
{
  if ( table == NULL ) return 0;

  return table_colnummatch(table_hdrfind(table, name, 1), name, 1, table->mode) - 1;
}

char *table_hdrtablecolname(table, name, n)
	TableHead	table;
	char		*name;
	int		 n;
{
	TableRow	row;

  if ( table == NULL ) return NULL;
  row = table_hdrfind(table, name, 1);
  return row->column[n+1];
}

void table_hdrtableset(table, name, row, col, val, flag)
	TableHead	table;
	char		*name;
	int		 row;
	int		 col;
	char		*val;
	int		 flag;
{
	table_hdrset(table, name, row+2, col, val, flag);
}
void table_hdrtablesetb(table, name, row, col, val, flag)
	TableHead	table;
	char		*name;
	int		 row;
	int		 col;
	int		 val;
	int		 flag;
{
	table_hdrsetb(table, name, row+2, col, val, flag);
}
void table_hdrtableseti(table, name, row, col, val, format, flag)
	TableHead	table;
	char		*name;
	int		 row;
	int		 col;
	int		 val;
	char		*format;
	int		 flag;
{
	table_hdrseti(table, name, row+2, col, val, format, flag);
}
void table_hdrtablesetd(table, name, row, col, val, format, flag)
	TableHead	table;
	char		*name;
	int		 row;
	int		 col;
	double		 val;
	char		*format;
	int		 flag;
{
	table_hdrsetd(table, name, row+2, col, val, format, flag);
}
void table_hdrtablesets(table, name, row, col, val, flag)
	TableHead	table;
	char		*name;
	int		 row;
	int		 col;
	char		*val;
	int		 flag;
{
	table_hdrset(table, name, row+2, col, val, flag);
}

char *table_hdrtableget(table, name, row, col)
	TableHead	table;
	char		*name;
	int		 row;
	int		 col;
{
	return table_hdrget(table, name, row+2, col);
}
int table_hdrtablegetb(table, name, row, col)
	TableHead	table;
	char		*name;
	int		 row;
	int		 col;
{
	return table_hdrgetb(table, name, row+2, col);
}
int table_hdrtablegeti(table, name, row, col)
	TableHead	table;
	char		*name;
	int		 row;
	int		 col;
{
	return table_hdrgeti(table, name, row+2, col);
}
double table_hdrtablegetd(table, name, row, col)
	TableHead	table;
	char		*name;
	int		 row;
	int		 col;
{
	return table_hdrgetd(table, name, row+2, col);
}
char *table_hdrtablegets(table, name, row, col)
	TableHead	table;
	char		*name;
	int		 row;
	int		 col;
{
	return table_hdrgets(table, name, row+2, col);
}

