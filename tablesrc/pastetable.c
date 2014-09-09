/* pastetable.c
 */
/* Starbase Data Tables	- An Ascii Database for UNIX

   copyright 1991, 1993, 1995, 1999 John B. Roll jr.
 */
#include <table.h>

typedef struct Term {
	TableRow	row;
	File		file;
	TableHead	tabl;
} Term;

char *trim();

void emptyrow(ofile, n, c)
	File	ofile;
	int	n;
	int	c;
{
		int	i;

	for ( i = 1; i < n; i++ )
		PutC(ofile, '\t');
	PutC(ofile, c);
}

int main(argc, argv)
	int	argc;
	char	*argv[];
{
	File		ofile = Stdout;

	Term		*terms = NULL;
	int		 nterm = 0;
	int		 aterm = 0;

	int		*lcol = NULL;
	int		 ncol = 0;
	int		 acol = 0;

	int		 i, j, k;

	int		 done = 0;
	int		argsdone = 0;

	int		Force = 0;

	char	 buf[TABLE_TRIMBUF];

	/* open files / allocate terms
	 */
	for ( i = 1; i < argc; i++ ) {
	    if ( !argsdone && argv[i][0] == '-' && argv[i][1] ) {
		switch ( argv[i][1] ) {
		 case 'o' : 
      			if ( (ofile = (!strcmp("-", argv[i+1]))
				? Stdout : Open(argv[i+1], "w")) == NULL ) {
			    FPrint(Stderr, "pastetable: can't open output file: %s\n", argv[i+1]);
			    perror("pastetable");
			    exit(1);
			}
			i++;
			break;

		 case 'f' : i++;
			    argsdone = 1;
			    Force    = 1;
			    break;

	 	 case '-' : i++; 
			    argsdone = 1;
			    break;

		 default:
			FPrint(Stderr, "pastetable: unknown option -%c\n", argv[i][1]);
			exit(1);
		}
	    }

	    if ( nterm + 1 >= aterm ) {
		aterm += 2;
		aterm *= 2;
		ReAlloc(terms, aterm * sizeof(Term));
	    }
	    
	    if ( (terms[nterm].file = Open(argv[i], "r")) == NULL ) {
		perror(argv[i]);
		exit(1);
	    }
	    if ( (terms[nterm].tabl = table_header(terms[nterm].file, 0)) == NULL ) {
		fprintf(stderr, "pastetable: can't read header from: %s\n"
			, argv[i]);
		exit(1);
	    }

	    terms[nterm].row = table_rowloc(NULL, 0, 0);
	    nterm++;
	}

	if ( nterm < 1 ) {
	    fprintf(stderr, "usage: pastetable table [table] ...\n");
	    exit(1);
	}

	table_hdrput(ofile, terms[0].tabl, TBLHDR_VALUES | TBLHDR_TEXT
			, NULL, NULL, 0);
	
	/* rename columns and generate the headline
	 */
	for ( i = 0; i < nterm; i++ ) {
	    if ( i != 0 ) table_ors(terms[i-1].tabl, '\t');

	    for ( j = 1; j <= terms[i].tabl->header->ncol; j++ ) {

		/* look up column name in all other tables.
		 */
	        for ( k = 0; k < nterm; k++ ) {
		    if ( i == k ) continue;

		    if ( table_colnum(terms[k].tabl
				, trim(table_colval(terms[i].tabl->header, j), buf)) )
			break;
		}

		if ( ncol >= acol ) {
		    acol *= 2;
		    acol += 10;
		    ReAlloc(lcol, acol * sizeof(int));
		}

		lcol[ncol++] = table_hdrfix(ofile, table_colval(terms[i].tabl->header, j)
				, Force || k < nterm ? i + 1 : 0
				, ( i == nterm - 1
                                     && j == terms[i].tabl->header->ncol )
                                        ? '\n' : '\t');
	    }
	}

	/* Output dashline
	 */
	for ( i = 0; i < ncol; i++ ) {
	    table_dashes(ofile , lcol[i] , (i == ncol - 1) ? '\n' : '\t');
	}

	/* Paste files
	 */
	while ( !done ) {
	    done = 1;
	    for ( i = 0; i < nterm; i++ ) {
	      if ( terms[i].file != NULL ) {
		if ( table_rowget(terms[i].file, terms[i].tabl, terms[i].row, NULL, NULL, 0) == NULL ) {
		    Close(terms[i].file);
		    terms[i].file = NULL;
		} else {
		    done = 0;
		}
	      }
	    }

	    if ( !done ) {
	        for ( i = 0; i < nterm; i++ ) {
	            if ( terms[i].file == NULL )
		        emptyrow(ofile
			       , terms[i].tabl->header->ncol
			       , terms[i].tabl->ors);
	            else
		        table_rowput(ofile, terms[i].tabl
					   , terms[i].row, NULL, NULL, 0);
	        }
	    }
	}

	exit(0);
}


