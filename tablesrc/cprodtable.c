/* cprodtable -  compute the Cartesian product of a list of input tables.
 */
/* Starbase Data Tables	- An Ascii Database for UNIX

   copyright 1991, 1993, 1995, 1999 John B. Roll jr.

   Starbase Data Tables is distributed without warranty under the terms of
   the GNU General Public License, version 2, 1991.
 */

#include <table.h>

typedef struct Term {
	TableRow	*row;
	int		nrow;
	int		arow;
	int		crow;

	File		file;
	TableHead	tabl;
} Term;

char *trim();

int nextrow(term)
	Term	*term;
{
	/* Get the next row from this term
	 */
	if ( term->file ) {

	    /* read a row */
	    if ( term->nrow >= term->arow ) { /* Allocate more row space */
		term->arow *= 2;
		term->arow += 128;
		ReAlloc(term->row, term->arow * sizeof(TableRow *));
	    }
	    if ( (term->row[term->nrow] = table_rowget(term->file
			, term->tabl, NULL, NULL, NULL, 0)) == NULL ) {
		if ( term->file != stdin ) Close(term->file);
		term->file = NULL;
		term->crow = -1;
		return 0;
	    } else {
		term->nrow++;
		term->crow++;
		return 1;
	    }
	} else
	    if ( term->crow >= term->nrow  - 1) {
		term->crow = -1;
		return 0;
	    } else {
	        term->crow++;
	        return 1;
	    }
}


void product(ofile, terms, term)
	File		 ofile;
	Term		*terms;
	Term		*term;
{
		Term	*tx;

	if ( term[1].tabl == NULL ) {	 /* If this is the last term */
		/* Multiply by this row.
		 */
		while ( nextrow(term) ) {
		    for ( tx = terms; tx->tabl != NULL; tx++ ) {
			table_rowput(ofile, tx->tabl, tx->row[tx->crow]
				, NULL, NULL, 0);
		    }
		}
	} else {
		/* Get a new row and call down to 
		   the next table.
		 */
		while ( nextrow(term) ) {
		    product(ofile, terms, &term[1]);
		    if ( terms == term && term->file ) {
			table_rowfree(term->row[0]);
			term->crow = -1;
			term->nrow =  0;
		    }
		}
	}
}

int main(argc, argv)
	int	argc;
	char	*argv[];
{
	File		ofile = Stdout;

	TableRow	row, R;
	Term		*terms = NULL;
	int		 nterm = 0;
	int		 aterm = 0;

	int		*lcol = NULL;
	int		 ncol = 0;
	int		 acol = 0;

	int		 len;

	int		 i, j, k;

	int		Force = 0;

	Term		*interm = NULL;

	/* open files / allocate terms
	 */
	for ( i = 1; i < argc; i++ ) {
	    if ( argv[i][0] == '-' && argv[i][1] ) {
		switch ( argv[i][1] ) {
		 case 'o' : 
      			if ( (ofile = (!strcmp("-", argv[i+1]))
				? Stdout : Open(argv[i+1], "w")) == NULL ) {
			    FPrint(Stderr, "cprodtable: can't open output file: %s\n", argv[i+1]);
			    perror("cprodtable");
			    exit(1);
			}
			i++;
			break;
	         case 'f' : Force = 1; break;
		 default:
			FPrint(Stderr, "cprodtable: unknown option -%c\n", argv[i][1]);
			exit(1);
		}
		continue;
	    }

	    if ( nterm != 0 ) table_ors(terms[nterm-1].tabl, '\t');

	    if ( nterm + 1 >= aterm ) {
		aterm += 2;
		aterm *= 2;
		ReAlloc(terms, aterm * sizeof(struct Term));
	    }

	    
	    if ( (terms[nterm].file = Open(argv[i], "r")) == NULL ) {
		perror("cprodtable");
		exit(1);
	    }

	    if ( terms[nterm].file == stdin ) {

	        if ( interm != NULL ) {
		    while ( nextrow(interm) );
		}

		interm = &terms[nterm];
	    } 
		
	    if ( (terms[nterm].tabl = table_header(terms[nterm].file, 0)) == NULL ) {
		fprintf(stderr, "cprodtable: can't read header from: %s\n"
			, argv[i]);
		exit(1);
	    }

	    terms[nterm].row = NULL;
	    terms[nterm].nrow=  0;
	    terms[nterm].arow=  0;
	    terms[nterm].crow= -1;

	    nterm++;
	    terms[nterm].tabl = NULL;

	    if ( nterm == 0 )
		table_hdrput(ofile, terms[0].tabl, TBLHDR_VALUES | TBLHDR_TEXT
				, NULL, NULL, 0);
	}

	if ( nterm < 2 ) {
	    fprintf(stderr, "usage: cprodtable term term ...\n");
	    exit(1);
	}

	/* rename columns and generate the headline
	 */
	for ( i = 0; i < nterm; i++ )
	    for ( j = 1; j <= terms[i].tabl->header->ncol; j++ ) {
		char buf[TABLE_TRIMBUF];

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

	/* Output dashline
	 */
	for ( i = 0; i < ncol; i++ ) {
	    table_dashes(ofile , lcol[i] , (i == ncol - 1) ? '\n' : '\t');
	}

	product(ofile, &terms[0], &terms[0]);
	exit(0);
}

