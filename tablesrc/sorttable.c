/* sorttable.c
 */
/* Starbase Data Tables	- An Ascii Database for UNIX

   copyright 1991, 1993, 1995, 1999 John B. Roll jr.
 */

#include <table.h>
#include <unistd.h>

#define FLEN		32
#define NCOLFLAGS	 4
#define START		 0
#define FINIS		 1
#define FLAGS		 2
#define ZONES		 3

char	*flag	=   "+-:%";
char	*form[] = { "i", "i", "f bdfiMnqrh", "r" };

int main(argc, argv)
     int	 argc;
     char	*argv[];
{
  File		ifile = Stdin;
  File		hfile = NULL;
  File		ofile = Stdout;
  int		hfake = 0;

  TableHead	table;
  char		*argx[512];
  int		 nargx = 0;

  int		debug = 0, done = 0;
  int		i;
  int		colargs = argc;
  int		count = 0;
  int		setct = 0;
  int		check = 0;
  int		merge = 0;

  /* Look ahead for file option.
   */
  for ( i = 1; i < argc - 1 ; i++ ) {
    if ( argv[i][0] == '-' && argv[i][1] == 'i' ) {
      if ( (ifile = (!strcmp("-", argv[i+1])) ? Stdin : Open(argv[i+1], "r")) == NULL ) {
	FPrint(Stderr, "%s: can't open input file\n", "sorttable");
	perror("sorttable");
	exit(1);
      } else {
	for ( ; i < argc - 2; i++ ) 
		  argv[i] = argv[i+2];	/* Ooch these over	*/
	argc   -= 2;
  	colargs = argc;
	break;
      }
    }
  }

  /* Scan for merge files.
   */
  for ( i = 1; i < argc; i++ ) {
      if ( argv[i][0] == '-' && argv[i][1] == 'H' ) {
	if ( (hfile = Open(argv[i+1], "r")) == NULL ) {
	    FPrint(Stderr, "sorttable: can't open input %s\n", argv[i+1]);
	    exit(1);
	}
	hfake = 1;
      }
  }
  for ( i = 1; i < argc; i++ ) {
      if ( argv[i][0] == '-' && argv[i][1] == 'm' ) {
	colargs = i;
        merge = 1;
      }
  }
  if ( merge ) {
      for ( i = colargs + 1; i < argc && (!hfile || hfile == Stdin); i++ ) {
	if ( argv[i][0] == '|' ) continue;

	if ( (hfile = Open(argv[i], "r")) == NULL ) {
	    FPrint(Stderr, "sorttable: can't open input %s\n", argv[i]);
	    exit(1);
	} else
	    if ( hfile != Stdin ) break;
      }
  }

  if ( merge && hfile == NULL ) {
	FPrint(Stderr, "sorttable: no file\n");
	exit(1);
  }

  if ( !hfile ) hfile = ifile;

  SetBuffer(hfile, FileNullBuffer, NULL, 0);


  if ( (table = table_header(hfile, 0)) == NULL ) {
	fprintf(stderr, "sorttable: can't read header from table\n");
	exit(1);
  };

  if ( !hfake && merge && hfile == Stdin && (fseek(hfile, 0, 0) == -1) ) {
	fprintf(stderr, "sorttable: can't seek header table\n");
	exit(1);
  }

    argx[nargx++] = "sorttable";
    argx[nargx++] = "-t\t";

    for ( i = 1; i < colargs; i++ ) {
		    int col;

	if ( !done && argv[i][0] == '-' ) {
	    switch ( argv[i][1] )  {
	     case '-':  done  = 1;				break;
	     case 'X':  debug = 1;				
	    		SetBuffer(Stdin, FileNullBuffer, NULL, 0);
			break;
	     case 't': argx[1][2] = argv[i][2];		break;
	     case 'I': argx[nargx++] = "-i";		break;
	     case 'S': argx[nargx++] = "-S"; setct = 1;	break;
	     case 'C': argx[nargx++] = "-C"; count = 1;	break;
	     case 'Z': argx[nargx++] = "-Z";
	     	       argx[nargx++] = argv[i+1];
		       i++;
		       break;
	     case 'M': argx[nargx++] = "-X";
	     	       argx[nargx++] = argv[i+1];
		       i++;
		       break;
	     case 'c': argx[nargx++] = "-c"; check = 1; 	break;
	     case 'H': i++;					break;
	     case 'o' : 
		    if ( (ofile = (!strcmp("-", argv[i+1]))
			    ? Stdout : Open(argv[i+1], "w")) == NULL ) {
			FPrint(Stderr, "sorttable: can't open output file: %s\n", argv[i+1]);
			perror("sorttable");
			exit(1);
		    }
		    i++;
		    break;
	     default: argx[nargx++] = argv[i];		break;
	    }
	    continue;
	} else {
		    int		k;
		    TableColArg	value[NCOLFLAGS];
		    int		check[NCOLFLAGS];
		    char		flags[64];

		for ( k = 0; k < NCOLFLAGS; k++ ) check[k] = 0;
		value[START].i = 0;

		if ( (col = table_colarg(table, argv[i]
					    , flag, form, value, check)) == 0 ) {
		    FPrint(Stderr, "sorttable : requested column \"%s\" not in table\n"
			   , argv[i]);
		    exit(1);
		}

		/* Re-constitute the sort type string.
		 */
		if ( check[FLAGS] ) {
			    int	j, k = 0;

		    for ( j = 2; form[FLAGS][j]; j++ )
			if ( value[FLAGS].i & 1 << j - 2 ) flags[k++] = form[FLAGS][j];
		    flags[k] = '\0';
		} else flags[0] = '\0';

		if ( check[ZONES] ) {
			char zone[32];

		    sprintf(zone, "z%f", value[ZONES].f);
		    strcat(flags, zone);
		}

		Malloc(argx[nargx], FLEN);
		sprintf(argx[nargx++], "+%d.%d",       col - 1, value[START].i);
		Malloc(argx[nargx], FLEN);
		if ( check[FINIS] )
		    sprintf(argx[nargx++], "-%d.%d%s", col - 1, value[FINIS].i, flags);
		else
		    sprintf(argx[nargx++], "-%d%s",    col,                   flags);
	}
    }


    for ( ; i < argc; i++ )
	    argx[nargx++] = argv[i];

    argx[nargx++] = NULL;

    if ( debug ) {
	for ( i = 0; i < nargx - 1; i++ )
	    FPrint(Stdout, "%s ", argx[i]);
	FPrint(Stdout, "\n");
	exit(0);
    }

    if ( count && setct ) {
	    fprintf(stderr, "Row count and Set count can't both be used!\n");
	    exit(1);
    }

    if ( !check ) {
	    table_hdrput(ofile, table, TBLHDR_VALUES | TBLHDR_TEXT, NULL, NULL, 0);
	    if ( count ) fprintf(ofile, "Count\t");
	    if ( setct ) fprintf(ofile, "SetNumber\t");
	    table_hdrput(ofile, table, TBLHDR_HEADLINE, NULL, NULL, 0);
	    if ( count ) fprintf(ofile, "-----\t");
	    if ( setct ) fprintf(ofile, "---------\t");
	    table_hdrput(ofile, table, TBLHDR_DASHLINE, NULL, NULL, 0);
    }
    Flush(ofile);

    
    /* Put my input/output file onto the stdin/stdout of sort
     */
    
    if ( ifile != Stdin  ) dup2(FileFd(ifile), 0);
    if ( ofile != Stdout ) dup2(FileFd(ofile), 1);

  {
	char *sorttable = getenv("STARBASE_SORTTABLE");

    execvp(sorttable ? sorttable : "_sorttable", argx);
  }
    perror("sorttable");
    exit(1);
}
