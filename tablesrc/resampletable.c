/* resample - resample numeric columns in a table 
 */
/* Starbase Data Tables	- An Ascii Database for UNIX

   copyright 1991, 1993, 1995, 1999 John B. Roll jr.
 */


#include "table.h"

#define NCOLS	32

#define NCOLFLAGS	1
#define COLUMN1		0

char	*flag   =   ":";
char	*form[] = { "s" };


int main(argc, argv)
	int	 argc;
	char	*argv[];
	
{
	File		 CFile;
	File             RFile = Stdin;
        File             OFile = Stdout;
	TableHead	 InTable = NULL;
	TableHead        CTabl;
	TableHead        RTabl;
	char		*keynames;
	char            *column0;
        char            *column1;
	int		 colnum;

	int	control0;
	int	control1;
	int     ncolumns;
	int	column[NCOLS];
	char   *colname[NCOLS];

	int	value[NCOLFLAGS];
	int	check[NCOLFLAGS];

	char	*p;

	int	i, j;

		double	**data = NULL;
		double   *key = NULL;
		double    controlval;
		int	  ntab = 0;
		int	  atab = 0;
		int       lowindex = 0;
		TableRow  r;
		TableRow  R;
		char     *s;
		double    frac;
		double    interpval;

	if ( argc < 2 ) { 
		FPrint(Stderr, "Usage: resample controltable keycol[:keycontrolcol] col ... [col] < input\n");
		exit(1);
	}

	if ( !(CFile = Open(argv[1], "r"))) {
	        FPrint(Stderr, "resample: can't open control table %s\n", argv[1]);
		exit(1);
	}

	if ( !(CTabl = table_header(CFile, 0))) {
		FPrint(Stderr, "resample: can't read control table\n");
		exit(1);
	}

	if ( !(RTabl = table_header(RFile, 0))) {
                FPrint(Stderr, "resample: can't read table from input\n");
                exit(1);
        }

	NewString(keynames, argv[2]);
	column0 = keynames;
	column1 = keynames;
	if ( p = strchr(keynames, ':') ) {
		*p = '\0';
		 column1 = ++p;
	}

	if ( !(control0 = table_colnum(RTabl, column0))) {
		FPrint(Stderr, "resample: keycolumn %s not in input table\n", column0);
		exit(1);
	}

	if ( !(control1 = table_colnum(CTabl, column1))) {
		FPrint(Stderr, "resample: keycolumn %s not in control table\n",column1);
		exit(1);
	}

	for ( i = 0; i < argc - 3; i++ ) {
	        colname[i] = argv[i+3];
		if ( !(column[i] = table_colnum(RTabl, colname[i]))) {
		FPrint(Stderr, "resample: keycolumn %s not in input table\n", colname[i]);
		exit(1);
		}
	}

	ncolumns = argc - 3;

	{

	    /* Read in the entire input table
	     */

	    Calloc(data, sizeof(double*) * ncolumns);

	    R = table_rowloc(NULL, 0, 0);

	    while ( r = table_rowget(RFile, RTabl, R, NULL, NULL, 0) ) {
		if ( ntab >= atab ) {

		    atab *= 2;
		    atab += 128;

		    for ( i = 0; i < ncolumns; i++ ) {
			ReAlloc(data[i], sizeof(double) * atab);
		    }
		    ReAlloc(key, sizeof(double) * atab);
		}

		for ( i = 0; i < ncolumns; i++ ) {
		    data[i][ntab] = table_colvald(r, column[i]);
		}
		key[ntab] = table_colvald(r, control0);

		ntab++;
		    
	    }

	    if ( ntab < 2 ) {
		FPrint(Stderr, "Input table must have at least two rows\n");
		exit(1);
	    }
	    
	    FPrint(OFile, "%s", column0);
	    for ( i = 0; i < ncolumns; i++ ) {
		FPrint(OFile, "\t%s", colname[i] );
	    }
	    FPrint(OFile,"\n");
	    
	    s = column0;
            while ( *s++ ) FPrint(OFile, "-");
	    for ( i = 0; i < ncolumns; i++ ) {
		FPrint(OFile, "\t");
		s = colname[i];
		while ( *s++ ) FPrint(OFile, "-");
	    }
	    FPrint(OFile, "\n");

	    /* Process the control table one line at a time
	     */

	    while ( r = table_rowget(CFile, CTabl, R, NULL, NULL, 0) ) {
		controlval = table_colvald(r, control1);
		FPrint(OFile, "%g", controlval);
		
		/* Search for the two surrounding points
		 */

		while ( key[lowindex + 1] < controlval ) {
		    if ( lowindex >= ntab - 2 ) {
			break;
		    }
		    lowindex++;
		}

		/* Do the interpolation for each of the output columns
		 */

		for ( i = 0; i < ncolumns; i++ ) {
		
		    frac = ( controlval - key[lowindex] ) / 
		      ( key[lowindex+1] - key[lowindex] ) ; 
		
		    interpval = (1.0 - frac) * data[i][lowindex] + 
		      frac * data[i][lowindex+1];
		    FPrint(OFile, "\t%g",interpval);
		}
		FPrint(OFile, "\n");

	    }
	}

	exit(0);
    
}

