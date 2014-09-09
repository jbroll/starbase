/* fixtotable.c
 */
/* Starbase Data Tables	- An Ascii Database for UNIX

   copyright 1991, 1993, 1995, 1999 John B. Roll jr.
 */

#include <xos.h>
#include <xfile.h>
#include "../tablelib/table.h"

#define NCOLFLAGS	2
#define START		0
#define FINIS		1

char	*flag	=   ":-";
char	*form[] = { "i", "i" };

typedef struct _Column {
	char	*name;
	int	start;
	int	finis;
} *Column;
 
void unknown(arg)
	char	*arg;
{
    FPrint(Stderr, "fixtotable: unknown option %s\n", arg);
    exit(1);
}

void hdrout(column, ncols, ofile)
	Column	column;
	int	ncols;
	File	ofile;
{
	int	i, j;

    for ( i = 0; i < ncols; i++ ) {
	    FPrint(ofile, "%s", column[i].name);
	    PutC(ofile, i < ncols - 1 ? '\t' : '\n');
    }
    for ( i = 0; i < ncols; i++ ) {
	    for ( j = strlen(column[i].name); j-- ; )
		    PutC(ofile, '-');
	    PutC(ofile, i < ncols - 1 ? '\t' : '\n');
    }
}
	

int main(argc, argv)
     int         argc;
     char       *argv[];
{
	File	ifile = Stdin;
	File	ofile = Stdout;
	int	i, j;

	int		check[NCOLFLAGS];
	TableColArg	value[NCOLFLAGS];

	int	ncols = 0;
	int	acols = 0;

	Column	column = NULL;
	char	*colon;

	char	*line = NULL, *SlurpLine();
	int	 n = 0;
	int	 max = 1024;

	char	comment = 0;
	int	skip    = 0;
	int	copy    = 0;
	int	header	= 0;

    for ( i = 1; i < argc; i++ ) {
	if ( argv[i][0] == '-' ) {
	    switch ( argv[i][1] ) {
	     case 'i' : 
		    if ( (ifile = (!strcmp("-", argv[i+1]))
			    ? Stdin : Open(argv[i+1], "r")) == NULL ) {
			FPrint(Stderr, "fixtotable: can't open input file: %s\n", argv[i+1]);
			perror("fixtotable");
			exit(1);
		    }
		    i++;
		    break;
	     case 'o' : 
		    if ( (ofile = (!strcmp("-", argv[i+1]))
			    ? Stdout : Open(argv[i+1], "w")) == NULL ) {
			FPrint(Stderr, "fixtotable: can't open output file: %s\n", argv[i+1]);
			perror("fixtotable");
			exit(1);
		    }
		    i++;
		    break;
	     case 's' :
		if ( !strcmp(&argv[i][1], "skip") ) {
		    skip = atoi(argv[++i]);
		} else
		    unknown(argv[i]);
		break;

	     case 'c' :
		if ( !strcmp(&argv[i][1], "comment") ) {
		    comment = argv[++i][0];
		} else 
		if ( !strcmp(&argv[i][1], "copy") ) {
		    copy++;
		} else
		    unknown(argv[i]);
		break;

	     default: unknown(argv[i]);
	    }
	    continue;
	}

	for ( j = 0; j < NCOLFLAGS; j++ ) check[j] = 0;

	table_colarg(NULL, argv[i], flag, form, value, check);

	if ( acols <= ncols ) {
		acols += 10;
		ReAlloc(column, acols * sizeof(Column) *sizeof(int));
	}

	if ( colon = strchr(argv[i], ':') )
		*colon = '\0';

	column[ncols].name  = argv[i];

	if ( !check[START] ) 
		column[ncols].start = 0;
	else
		column[ncols].start = value[START].i < 1 ?  0 : value[START].i - 1;

	
	if ( !check[FINIS] ) 
		column[ncols].finis = column[ncols].start + 1;
	else
	    if ( check[FINIS] == 0 )
		column[ncols].finis = -1;
	    else
		column[ncols].finis = value[FINIS].i < 1 ? -1 : value[FINIS].i;

	ncols++;
    }

    if ( !ncols ) {
	    FPrint(Stderr, "fixtotable: no columns specified!\n");
	    exit(1);
    }

    /* Skip n lines
     */
    for ( i = 0; i < skip; i++ ) {
	line = SlurpLine(ifile, line, &n, &max);
	if ( !n ) break;

	if ( copy ) { Print("%s", line); }
    }

    while ( (line = SlurpLine(ifile, line, &n, &max)), n ) {
	    if ( ( !header && !comment )
	      || ( !header &&  line[0] != comment ) ) {
	    	hdrout(column, ncols, ofile);
		header = 1;
	    }

	    if ( comment && line[0] == comment ) {
		if ( copy && !header ) Print("%s", line);
		continue;
	    }

	    for ( i = 0; i < ncols; i++ ) {
		    for ( j = column[i].start; j < (column[i].finis != -1 
						  ? column[i].finis : n)
					    && j <  n; j++ ) {
			    PutC(ofile, line[j]);
		    }

		    PutC(ofile, i < ncols - 1 ? '\t' : '\n');
	    }
    }

    if ( !header ) hdrout(column, ncols, ofile);
}

