/* Starbase Data Tables	- An Ascii Database for UNIX

   copyright 1991, 1993, 1995, 1999 John B. Roll jr.
 */

/* column.c	- project columns from a starbase data table.
 */

#include <xos.h>
#include <xfile.h>
#include <ctype.h>
#include <table.h>

#include <signal.h>

char *treplace();

#define NROW	100341		/* The number of rows to look at	*/

#define NCOLFLAGS	4
#define WIDTH		0
#define JUSTY		1
#define RNAME		2
#define FORMAT		3

#define MAX_WIDE    10000

static char *flag   =   ",:=%";
static char *form[] = { "i", "f dlcrespx", "s", "S" };


#define NHDRFLAGS	3
#define HDRROWFLAG	0
#define HDRCOLFLAG	1
#define HDRVALFLAG	2

static char *hdrf   =   "#,=\001";
static char *hdrF[] = { "i", "i", "s", "S" };

typedef struct ColData {
	char   *progname;
	File	ifile;
	File	ofile;
	int	argc;
	char  **argv;

	/* Program control flags.
	 */
	int	body;
	int	head;
	int	just;
	int	trim;
	int	chek;
	int	hlin;
	int	dlin;
	int	hval;
	int	verb;
	int	quit;

	int	nrow;
	int	ntab;
	int	ofs;
	int	temp;
} ColData;

#include "argv-fuzz-inl.h"

int main(argc, argv)
			int	 argc;
			char	*argv[];
{
  AFL_INIT_ARGV();


  int columnate();
  int unbuffer = 0;

  char		*strrchr();
  char		*template = NULL;

  ColData	 cd;

  int		 i;

  signal(SIGPIPE, SIG_IGN);

  /* Way Basics.
   */
  cd.ifile= Stdin;
  cd.ofile= Stdout;
  cd.argc = argc;
  cd.argv = argv;

  /* Basic program control options
   */
  cd.body = 1;		/* Print table 		*/
  cd.head = 1;		/* Print header		*/
  cd.just = 0;		/* Do justification	*/
  cd.trim = 0;		/* Trim table		*/
  cd.chek = 0;		/* Check column counts	*/
  cd.hlin = 1;		/* Print HeadLine	*/
  cd.dlin = 1;		/* Print DashLine	*/
  cd.hval = 1;		/* Print HeadValues	*/
  cd.verb = 1;		/* Print check warning	*/
  cd.quit = 0;		/* Quit on check warning*/
  cd.nrow = NROW;	/* Pre Read NROWS	*/
  cd.ofs  = ' ';
  cd.temp = 0;

  cd.ntab = 0;

  if ( argc <= 0 ) {
    fprintf(stderr, "column needs argv\n");
    exit(1);
  }

  if ( (cd.progname = strrchr(argv[0], '/')) )
        cd.progname++;
  else	cd.progname = argv[0];

  /* Change default options based on program name
   */
  if ( !strcmp(cd.progname, "justify") ) 	{  cd.just = 1;			   	    }   
  if ( !strcmp(cd.progname, "trim")    )	{  cd.trim = 1;			   	    }
  if ( !strcmp(cd.progname, "trimtable") )	{  cd.trim = 1;			   	    }
  if ( !strcmp(cd.progname, "headoff") )	{  cd.head = 0;  			    }
  if ( !strcmp(cd.progname, "header")  ) 	{  cd.head = 1;  cd.nrow = 0; cd.body = 0; cd.chek = 0; }
  if ( !strcmp(cd.progname, "check")   ) 	{  cd.chek = 1;  cd.verb = 1; cd.nrow = 0; cd.quit = 1;
					   	   cd.body = 0;  cd.head = 0; 	   	    }
  if ( !strcmp(cd.progname, "headline") )	{  cd.body = 0;  cd.head = 1; cd.chek = 0;
					   cd.hlin = 1;  cd.dlin = 0; cd.hval = 0;  }
  if ( !strcmp(cd.progname, "dashline") )	{  cd.body = 0;  cd.head = 1; cd.chek = 0;
					   cd.hlin = 0;  cd.dlin = 1; cd.hval = 0;  }

  /* Look ahead for file options.
   */
  for ( i = 1; i < argc; i++ ) {
    if ( argv[i][0] == '-' ) 
      switch ( argv[i][1] ) {
	case 'i':
	  if ( ++i >= argc ) { 
	    fprintf(stderr, "column -i needs additional args\n");
	    exit(1);
	  }
#ifdef __AFL_COMPILER
	  if ( getenv("AFL_FUZZING") == NULL ) {
#endif
	  if ( (cd.ifile = (!strcmp("-", argv[i])) ? Stdin : Open(argv[i], "r")) == NULL ) {
	    FPrint(Stderr, "%s: can't open input file: %s\n", cd.progname, argv[i]);
	    perror(cd.progname);
	    exit(1);
	  }
#ifdef __AFL_COMPILER
	  }
#endif
	  break;
	case 'o':
	  if ( ++i >= argc ) { 
	    fprintf(stderr, "column -o needs additional args\n");
	    exit(1);
	  }
#ifdef __AFL_COMPILER
	  if ( getenv("AFL_FUZZING") == NULL ) {
#endif
	  if ( (cd.ofile = (!strcmp("-", argv[i])) ? Stdout : Open(argv[i], "w")) == NULL ) {
	    FPrint(Stderr, "%s: can't open output file: %s\n", cd.progname, argv[i]);
	    perror(cd.progname);
	    exit(1);
	  }
#ifdef __AFL_COMPILER
	  }
#endif
	  i++;
	  break;
	case 'R': 
	  if ( ++i >= argc ) { 
	    fprintf(stderr, "column -R needs additional args\n");
	    exit(1);
	  }
	  cd.nrow = atoi(argv[i]); 
	  break;
    	case 'u': unbuffer = 1; break;
	case 't':
	  if ( ++i >= argc ) { 
	    fprintf(stderr, "column -t needs additional args\n");
	    exit(1);
	  }
	  cd.temp = 1;
	  NewString(template, argv[i]);
	  break;
      }
  }

  {
	    char	*tbuffer;

      if ( unbuffer ) {
	    SetBuffer(cd.ifile, FileNullBuffer, NULL, 0);
      } else {
	  if ( cd.body ) {
	    Malloc(tbuffer, TABLE_FILEBUF);
	    SetBuffer(cd.ifile, _IOFBF, tbuffer, TABLE_FILEBUF);
	  }
      }

      Malloc(tbuffer, TABLE_FILEBUF);
      SetBuffer(cd.ofile, _IOFBF, tbuffer, TABLE_FILEBUF);
  }

  if ( template ) {
	table_extractva(cd.ifile, template, columnate, &cd, NULL);
  } else {
  		TableHead	 table;

      /* Read in the table header
       */
      if ( (table = table_header(cd.ifile, 0)) == NULL ) {
	    FPrint(Stderr, "%s: input file has no header\n", cd.progname);
	    exit(1);
      }

      columnate(cd.ifile, 0, table, &cd);
  }

  /* Close(cd.ifile); */
  Close(cd.ofile); 

  exit(0);
}


/* Run the columator over the table at position on file.
 */
int columnate(ifile, marker, table, cd)
	File		 ifile;
	int		 marker;
	TableHead	 table;
	ColData		*cd;
{
  /* Command line arguments
   */
  int	 argc = cd->argc;
  char **argv = cd->argv;
  File  ofile = cd->ofile;

  int	 body = cd->body;
  int	 head = cd->head;
  int	 just = cd->just;
  int	 trim = cd->trim;
  int	 chek = cd->chek;
  int	 hlin = cd->hlin;
  int	 dlin = cd->dlin;
  int	 hval = cd->hval;
  int	 nrow = cd->nrow;
  int	 ofs  = cd->ofs;
  int	 verb = cd->verb;		/* Print chek errors	*/
  int	 quit = cd->quit;

  int	 icol = table_ncol(table);

  TableRow	*xrow = NULL;

  /* General options.
   */
  int		ptwo = 0;		/* Print each row twice */
  int		numb = 0;		/* Number each row      */

  /* Deal with column selection/ justification
   */
  int		 call = 0;		/* Output all columns	*/
  int		*columns;		/* Requested		*/
  int		 ncol = 0;
  int		 nhdr = 0;
  int		*selection1 = NULL;	/* Selection		*/
  int		*selection2 = NULL;	
  int		*justification1 = NULL;	
  int		*justification2 = NULL;
  
  int		*justify;		/* justification	*/

  int		type;		 	/* current type		*/
  int		wide   = 0;
  int 		strike = 0;		/* strike out count	*/
  int		format = 0;

  int		nred   = 0;

  int		n, i, j, k, l;

  char		*number = "%d\t";
  char		*Number = "Number";

  int	  check[NCOLFLAGS];
  TableColArg value[NCOLFLAGS];

  int		dtype = TAB_NONE;

  if ( just )   dtype = TAB_DEFAULT;

  type = dtype;

  nhdr = table->header->ncol;

  Malloc(columns, sizeof(int) * (table->header->ncol + 1));
  Malloc(justify, sizeof(int) * (table->header->ncol + 1));

  columns[0] = 0;
  justify[0] = 0;


  for ( j = 1; j <= table->header->ncol; j++ ) justify[j] = type;
  for ( i = 1; i < argc ; i++ ) {
    if ( argv[i][0] == '-' ) {
      switch ( argv[i][1] ) {
      case 'a' : {
		int	c;

	  /* Add any column that arn't there
	   */
	  for ( c = 1; c <= table->header->ncol; c++ ) {

	      for ( j = 1; j <= ncol; j++ ) {
		if ( columns[j] == c ) break;
	      }

	      if ( j > ncol ) {
		columns[++ncol] = c;
		justify[c] = type;
		table->owidth[c] = -wide;
	      }
	  }
	  call = 1;
	  break;
      }
      case 'b' : if ( body ) head = 0; body = 1;		break;
      case 'h' : switch ( argv[i][2] ) {
		  case 'v': hval = 1; hlin = 0; dlin = 0;	break;
		  case 'V': hval = 2; hlin = 0; dlin = 0;	break;
		  case 'd': hval = 0; hlin = 0; dlin = 1;	break;
		  case 'l': hval = 0; hlin = 1; dlin = 0;	break;
		  case 'h': hval = 0; hlin = 1; dlin = 1;	break;
		 } 
		 if ( head ) body = 0; chek = 0; head = 1;	break;
      case 'k' : chek = 1;					break;
      case 'K' : chek = 0;					break;
      case 'n' : numb = 1;					break;
      case 'N' : 
	if ( ++i >= argc ) { 
	    fprintf(stderr, "column -N needs additional args\n");
	    exit(1);
	}
	numb = 1; 
	Number = argv[i];			
	break;
      case 'v' : verb = 1; quit = 0;				break;
      case 'q' : verb = 0; 					break;
      case '2' : ptwo = 1;					break;

      case 'd' : wide = 0; just = 1; type = TAB_DEFAULT;	break;
      case 'l' : 	   just = 1; type = TAB_LEFT;		break;
      case 'c' :	   just = 1; type = TAB_CENTER;		break;
      case 'r' : 	   just = 1; type = TAB_RIGHT;		break;
      case 'e' : 	   just = 1; type = TAB_EXPAND;		break;
      case 's' : 	   just = 1; type = TAB_SKIP;		break;
      case 'x' : 	             type = 128;		break;
      case 'p' : 	   just = 1; type = TAB_NONE;		break;
      case 'w' : 	   
	if ( ++i >= argc ) { 
	    fprintf(stderr, "column -w needs additional args\n");
	    exit(1);
	}
        just = 1; 
	wide = Min(MAX_WIDE, atoi(argv[i]));
	break;
      /* Skip global options.
       */
      case 'u':							break;
      case 'i': i++;						break;
      case 'o': i++;						break;
      case 'R': i++;						break;
      case 't': i++;						break;

      case 'H': {	/* Set / Delete header values	*/
		int	row = 1;
		int	col = 1;
		char	*name;

	if ( ++i >= argc ) { 
	    fprintf(stderr, "column -H needs additional args\n");
	    exit(1);
	}

	name = strdup(argv[i]);

	for ( k = 0; k < NHDRFLAGS; k++ ) check[k] = 0; 

	table_colarg(table, name, hdrf, hdrF, value, check);

	if ( check[HDRVALFLAG] ) {
	    if ( check[HDRROWFLAG] && check[HDRCOLFLAG] ) {
		    row = value[HDRROWFLAG].i;
		    col = value[HDRCOLFLAG].i;
	    } else if ( check[HDRROWFLAG] ) {
		    col = value[HDRROWFLAG].i;
	    }
	    if ( col <= 0 || col > 10000 ) {
		FPrint(Stderr, "%s : header value index for %s must be positive and less than 10000\n"
			, cd->progname
			, name);
		exit(1);
	    }
	    if ( row <= 0 ) {
		FPrint(Stderr, "%s : header row index for %s must be positive \n"
			, cd->progname
			, name);
		exit(1);
	    }

	    table_hdrset(table, name, row, col, value[HDRVALFLAG].s, 1);
	} else {
	    if ( check[HDRROWFLAG] ) {
		    row = value[HDRROWFLAG].i;
	    }
	    table_hdrdel(table, name, row);
	}

	free(name);
	break;
      }
      default  : {
	  FPrint(Stderr, "%s : bad option %s\n"
		  , cd->progname
		  , argv[i]);
	  exit(1);
	}
      }
    } else {
	int	list[512];
	char	name[512];
	
	/* Non-switch argument	*/

	char	*col = argv[i];

	for ( k = 0; k < NCOLFLAGS; k++ ) check[k] = 0; 

	if ( !(n = table_colargpat(table, col, name, 512, flag, form, value, check, list, 512)) ) {
	    list[0] = table_coladd(table, check[RNAME]-- ? value[RNAME].s : name);
	    n = 1;

  	    ReAlloc(columns, sizeof(int) * (table->header->ncol + 1));
	    ReAlloc(justify, sizeof(int) * (table->header->ncol + 1));
	}

	for ( k = 0; k < n; k++ ) {
	    j = list[k];

	    if ( check[RNAME] > 0 ) {
		    char	rename[1024];

		    int	len = strlen(treplace(table_colnam(table, list[k]), name, value[RNAME].s, rename, 512));

		    table_colset(table->header, j, rename, len);
		    table_setdash(table, j, len);
	    }

	    if ( check[WIDTH] ) {
		    table->owidth[j] = Max(-MAX_WIDE, -value[WIDTH].i);
		    just = 1;
	    } else  table->owidth[j] = -wide;

	    if ( check[JUSTY] ) {
		    justify[j] = value[JUSTY].i;
		    if ( justify[j] != 128 ) { just = 1; }
	    } else  justify[j] = type;

	    if ( check[FORMAT] ) {
		if ( !table_colfmt(table, j, value[FORMAT].s) ) {
		    fprintf(stderr, "column: warning invalid column format specifier: %s\n"
			    , value[FORMAT].s);
		}
		format = 1;
	    }

	    
	    for ( l = 1; l <= ncol; l++ ) if ( columns[l] == j ) break;
	    if ( l <= ncol ) {
		ncol--;
		for ( ; l <= ncol; l++ ) {
		    columns[l] = columns[l+1];
		}
	    }

	    columns[++ncol] = j;
	}
    }
  }

  /* count the # of strikeouts
   */
  if ( ncol && !call ) {
    for ( i = 1; i <= ncol; i++ ) {
	if ( justify[columns[i]] == 128 ) strike++;
    }

    if ( strike == ncol ) {	/* If all strikeouts output all other cols */
	int	c;

	  /* Add any column that arn't there
	   */
	  for ( c = 1; c <= table->header->ncol; c++ ) {

	      for ( j = 1; j <= ncol; j++ ) {
		if ( columns[j] == c ) break;
	      }

	      if ( j > ncol ) columns[++ncol] = c;
	  }
     }
  }

  if ( ncol ) {
      int nsel = 0;

      justification2 = justify;

      Malloc(selection1, sizeof(int) * (ncol + 1));
      Malloc(selection2, sizeof(int) * (ncol + 1));
      Malloc(justification1, sizeof(int) * (table->header->ncol + 1));

      selection1[0]     = 0;
      selection2[0]     = 0;
      justification1[0] = TAB_DEFAULT;

      for ( i = 1; i <= ncol; i++ ) {
	if ( justification2[columns[i]] != 128 ) {
	    nsel++;
	    selection1[nsel] = columns[i];
	    justification1[columns[i]] = justification2[columns[i]];

	    if ( selection1[nsel] <= nhdr ) {
	        selection2[nsel] = selection1[nsel];

		if ( justification2[columns[i]] & TAB_SKIP )
		     justification1[columns[i]] = dtype;
	    } else {
		 selection2[nsel] = 0;
	    }
	}
      }

      justification2 = NULL;
      ncol = nsel;
  }

  if ( just || trim ) table_mode(table, TABLE_PARSE, 1);
  if ( just ) {
    number = "%6d\t";
    justification2 = justify;
    table_mode(table, TABLE_JUSTIFY, 1);
  } else {
    nrow = 0;
    Free(justification1);
    justification1 = NULL;
  }

  if ( chek ) chek = icol;

  if ( nrow && body ) {
      /* Read in the first NROWs of the table
       */
      if ( nrow ) Malloc(xrow, sizeof(TableRow) * nrow);

      if ( xrow == NULL ) {
	    fprintf(stderr, "cannot allocate %d row pointers\n", nrow);
	    exit(1);
      }

      for ( nred = 0; nred < nrow && (xrow[nred] = table_rowget(ifile, table, NULL, justification2, selection2, ncol));
	    nred++ ) {
	    if ( chek && xrow[nred]->ncol != chek ) {
		if ( verb ) FPrint(Stderr, "%s : table %s row %d has %d columns (%d in header)\n"
		   , cd->progname, table->name == NULL ? "?" : table->name, (nred+1), xrow[nred]->ncol, chek);

		if ( quit == 1 ) exit(1);
	    }
      }

      if ( !format && !cd->temp ) {
	    table_rowbuf_open(ifile, table);
      }


      /* Read and Trim up the rest of the rows if there are any left in 
	 the file.
       */
      if ( just && ( nred == nrow ) ) {
	    off_t 		here = Tell(ifile);
	    TableRow	R, row;
	    File		otemp = NULL;

	R = row = table_rowloc(NULL, TABLE_LINE, TABLE_NCOL);

	/* If the Stdin can't seek then create a temp file to store
	 * the row that we read up into.
	 */
	if ( here < 0 ) {
	    if ( (otemp = tmpfile()) == NULL ) {
		perror("column: tmpfile");
		exit(1);
	    }
	}

        if ( here < 0 && !format && !cd->temp ) {
	    table_rowbuf_open(otemp, table);
        }

	while ( row = table_rowget(ifile, table, row, justification2, selection2, ncol) )
	  if ( here < 0 ) table_rowput(otemp, table, row, NULL, NULL, 0);

	table_rowfree(R); R = NULL;

	/* Go back to where we left off.
	 */
	if ( here < 0 ) {
	    Close(ifile);
	    ifile = otemp;
	    here  = 0; 
	}

	Seek(ifile, here, 0);
      }
  }


  /* Write the header
   */
  if ( head ) {
        int skipval = 0;

  	if ( cd->ntab && hlin && dlin ) {
	    PutC(ofile, '\f');
	    if ( !hval 
	     ||   hval == 2 
             ||  (hval == 1  
	       && table->nvalue
	       && table->values 
	       && table->values[0]
	       && table->values[0]->column
	       && table->values[0]->column[1]
	       && table->values[0]->column[1][0] != '\0') ) {
		PutC(ofile, '\n');
	    }
	}
	if ( !cd->ntab && marker 
	  && table->nvalue
	  && table->values
	  && table->values[0]
	  && table->values[0]->column
	  && table->values[0]->column[1]
	  && table->values[0]->column[1][0] == '\0' ) {
	     skipval++;
	     table->values++;
	     table->nvalue--;
	}
	if ( hval ) {
	    if ( hval == 2 ) {
		hlin = 0;
		dlin = 0;
	        /* FPrint(ofile, "\n"); */
	    }
	    table_hdrput(ofile, table, TBLHDR_VALUES | ((hval == 1) 
					? TBLHDR_TEXT : 0)
			, NULL, NULL, 0);
	}
	if ( skipval ) {
	    table->values--;
	    table->nvalue--;
	}

	if ( numb )	{ FPuts(ofile, Number); PutC(ofile, '\t'); }
	if ( hlin )
	    table_hdrput(ofile, table, TBLHDR_HEADLINE
			, justification1, selection1, ncol);
	if ( numb )	FPuts(ofile, "------\t");
	if ( dlin )
	    table_hdrput(ofile, table, TBLHDR_DASHLINE
			, justification1, selection1, ncol);

  }

  cd->ntab++;

  /* Output the rows of the table that were read in
   */
  if ( body ) {
          char buff[128];

      for ( i = 0; i < nred; i++ ) {
	  if ( numb ) {
                snprintf(buff, 128, number, i+1);
                FPuts(ofile, buff);
          }
	  table_rowput(ofile, table, xrow[i], justification2, selection2, ncol);
	  if ( ptwo ) {
            if ( numb ) FPuts(ofile, buff);
	    table_rowput(ofile, table, xrow[i], justification2, selection2, ncol);
	  }
	  table_rowfree(xrow[i]);
      }
  }

  /* Read (Re-Read) and output the rows beyond NROW
   */
  if ( ( body || chek ) && ( nred == nrow ) ) {
    TableRow	R, row;
    char buff[128];
  
    R = row = table_rowloc(NULL, TABLE_LINE, TABLE_NCOL);

    while ( row = table_rowget(ifile, table, row, justification2, selection2, ncol) ) {
	nred++;
	if ( chek && row->ncol != chek ) {
	    if ( verb ) FPrint(Stderr, "%s : table %s row %d has %d columns (%d in input header)\n"
	       , cd->progname, table->name == NULL ? "?" : table->name, nred, row->ncol, chek);

	    if ( quit == 1 ) exit(1);
	}

	if ( body ) {
	    if ( numb ) {
                snprintf(buff, 128, number, i+1);
                FPuts(ofile, buff);
            }
	    table_rowput(ofile, table, row,  justification2, selection2, ncol);
	    if ( ptwo ) {
                if ( numb ) FPuts(ofile, buff);
		table_rowput(ofile, table, row,  justification2, selection2, ncol);
	    }
	}
	i++;
    }

    table_rowfree(R); R = NULL;
  }

  if ( selection1 )     Free(selection1);
  if ( selection2 )     Free(selection2);
  if ( justification1 ) Free(justification1);

  Free(columns);
  Free(justify);
  Free(xrow);

  if ( body || chek ) return EOF;
  else		      return 0;
}
