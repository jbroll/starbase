/* tabletofits.c
 */
/* Starbase Data Tables	- An Ascii Database for UNIX

   copyright 1991, 1993, 1995, 1999 John B. Roll jr.
 */

#include <xos.h>
#include <xfile.h>
#include "../tablelib/table.h"

#define ft_byteswap()			(*(short *)"\001\000" & 0x0001)

#define NCOLFLAGS	4
#define TYPE		0
#define WIDTH		1
#define AXMIN		2
#define AXMAX		3

#define NROW		50000

char	*flag	=   "%#=:";
char	*form[] = { "f LXBIJUVEDAC", "i", "s", "s" };

typedef struct _Column {
	char	*name;
	int	 column;
	int	 type;
	int	 width;
	char	*min;
	char	*max;
} *Column;
 
void unknown(arg)
	char	*arg;
{
    FPrint(Stderr, "tabletofits: unknown option %s\n", arg);
    exit(1);
}



void cardpad(ofile, n)
	File	ofile;
	int	n;
{
		int i;

	if ( n % 36 ) {
	    for ( i = 36 - n % 36; i--; ) {
		    fprintf(ofile, "%-80.80s", "");
	    }
 	}
}

#define swap1 memcpy

void swap2(to, from, nbytes)
	char *to;
	char *from;
	int nbytes;
{
    char c;
    int i;
    for ( i=0; i < nbytes; i += 2, (to += 2), (from += 2) ) {
	c = *from;
	*(to) = *(from+1);
	*(to+1) = c;
    }
}

void  swap4(to, from, nbytes)
	char *to;
	char *from;
	int nbytes;
{
    char c;
    int i;
    for ( i=0; i < nbytes; i += 4, (to += 4), (from += 4) ) {
	c = *from;
	*to = *(from+3);
	*(to+3) = c;
	c = *(from+1);
	*(to+1) = *(from+2);
	*(to+2) = c;
    }
}

void swap8(to, from, nbytes)
	char *to;
	char *from;
	int nbytes;
{
    char c;
    int i;
    for ( i=0; i < nbytes; i += 8, (to += 8), (from += 8) ) {
	c   	= *(from+0);
	*(to+0)	= *(from+7);
	*(to+7) = c;
	c   	= *(from+1);
	*(to+1) = *(from+6);
	*(to+6) = c;
	c 	= *(from+2);
	*(to+2)	= *(from+5);
	*(to+5) = c;
	c 	= *(from+3);
	*(to+3) = *(from+4);
	*(to+4) = c;
    }
}


#define writeouti(ofile, str, convert, swap, type)			\
	{								\
	  type	v = convert(str, NULL, 0);				\
	  type  x;							\
	  if ( ft_byteswap() ) { swap((char *) &x, (char *) &v, sizeof(type));		\
	  } else { x = v; }						\
	  fwrite((char *)&x, sizeof(type), 1, ofile);				\
	}

#define writeoutf(ofile, str, convert, swap, type)			\
	{								\
	  type	v = convert(str, NULL);					\
	  type  x;							\
	  if ( ft_byteswap() ) { swap((char *) &x, (char *) &v, sizeof(type));		\
	  } else { x = v; }						\
	  fwrite((char *)&x, sizeof(type), 1, ofile);				\
	}

int ft_sizeof(c)
        int     c;
{
        switch ( c ) {
         case 'P':      return  8;
         case 'U':      return  2;
         case 'I':      return  2;
         case 'V':      return  4;
         case 'J':      return  4;
         case 'E':      return  4;
         case 'D':      return  8;
         case 'A':      return  1;
         case 'L':      return  1;
         case 'X':      return  1;
         case 'B':      return  1;
         case 'C':      return  8;
         case 'M':      return 16;
	}

	return 0;
}

void tobinary(ofile, str, type, width)
	File	 ofile;
	char	*str;
	int	 type;
	int	 width;
{
	    char	buff[128];

    switch ( type ) {
     case 'P': writeouti(ofile, str, strtoul, swap4, unsigned int);	break;
     case 'U': writeouti(ofile, str, strtoul, swap2, short);		break;
     case 'I': writeouti(ofile, str, strtol,  swap2, short);		break;
     case 'V': writeouti(ofile, str, strtoul, swap4, unsigned int);	break;
     case 'J': writeouti(ofile, str, strtol,  swap4, int);		break;
     case 'L': writeouti(ofile, str, strtol,  swap1, char);		break;
     case 'X': writeouti(ofile, str, strtoul, swap1, char);		break;
     case 'F': writeoutf(ofile, str, SAOstrtod,  swap4, float);		break;
     case 'E': writeoutf(ofile, str, SAOstrtod,  swap4, float);		break;
     case 'D': writeoutf(ofile, str, SAOstrtod,  swap8, double);	break;
     case 'A': 
	       memset(buff, ' ', width);
	       memcpy(buff, str, strlen(str));
	       fwrite(str, sizeof(char), width, ofile);			break;
     case 'B': writeouti(ofile, str, strtoul, swap1, unsigned char);	break;
     case 'C': writeouti(ofile, str, strtol,  swap1, char);		break;
    }
}

void cardo(ofile, name, n, value)
	File	ofile;
	char	*name;
	int	 n;
	char	*value;
{
	char	buff[80];

	if ( n ) sprintf(buff, "%s%d", name, n);
	else     sprintf(buff, "%s", name);

	fprintf(ofile, "%-8.8s%c %-70.70s", buff, *value ? '=' : ' ', value);
}


void cardi(ofile, name, n, value) 
	File	ofile;
	char	*name;
	int	 n;
	int	 value;
{
	char	buff[80];

	sprintf(buff, "%20d", value);
	cardo(ofile, name, n, buff);
}

void cardb(ofile, name, n, value) 
	File	ofile;
	char	*name;
	int	 n;
	int	 value;
{
	char	buff[80];

	sprintf(buff, "%20s", value ? "T" : "F");
	cardo(ofile, name, n, buff);
}

void cards(ofile, name, n, value) 
	File	ofile;
	char	*name;
	int	 n;
	char	*value;
{
	char	buff[80];

	sprintf(buff, "'%s'", value);
	cardo(ofile, name, n, buff);
}

void cardt(ofile, name, n, v, t) 
	File	ofile;
	char	*name;
	int	 n;
	int	 v;
	int	 t;
{
	char	buff[80];

	sprintf(buff, "%d%c", v, t);
	cards(ofile, name, n, buff);
}

void cardr(ofile, name, n, value, prec) 
	File	ofile;
	char	*name;
	int	 n;
	double	 value;
	int	 prec;
{
	char	buff[80];

	sprintf(buff, "%.*f", prec, value);
	cardo(ofile, name, n, buff);
}


void imheader(ofile)
	File	ofile;
{
    	int	k = 0;

	cardb(ofile, "SIMPLE", 0, 1);		k++;
	cardi(ofile, "BITPIX", 0, 8);		k++;
	cardi(ofile, "NAXIS" , 0, 0);		k++;
	cardb(ofile, "EXTEND", 0, 1);		k++;
	cardo(ofile, "END",    0, "");		k++;
	cardpad(ofile, k);
}

int bnheader(ofile, table, name, column, ncols, ntot)
	File		 ofile;
	TableHead	 table;
	char		*name;
	Column		 column;
	int		 ncols;
	int 		 ntot;
{
	int	i, k = 0;
	int	rec = 0;

	for ( i = 0; i < ncols; i++ ) {
		rec += ft_sizeof(column[i].type) * column[i].width;
	}

	cards(ofile, "XTENSION", 0, "BINTABLE");	k++;
	cardi(ofile, "BITPIX",   0, 8);			k++;
	cardi(ofile, "NAXIS",    0, 2);			k++;
	cardi(ofile, "NAXIS",    1, rec);		k++;
	cardi(ofile, "NAXIS",    2, ntot);		k++;
	cardi(ofile, "PCOUNT",   0, 0);			k++;
	cardi(ofile, "GCOUNT",   0, 1);			k++;
 	cardi(ofile, "TFIELDS",	 0, ncols);		k++;
	if ( name ) {
	    cards(ofile, "EXTNAME", 0, name);		k++;
	}

	for ( i = 0; i < ncols; i++ ) {
	    cards(ofile, "TTYPE", i+1, column[i].name);	k++;
	    cardt(ofile, "TFORM", i+1, column[i].width
				     , column[i].type);	k++;
	    cardo(ofile, "TLMIN", i+1, column[i].min);	k++;
	    cardo(ofile, "TLMAX", i+1, column[i].max);	k++;
	}

	for ( i = 1; i <= table_hdrnvalues(table); i++ ) {
	    char	*here;
	    double	 dvalue;
	    int		 ivalue;
	    TableRow R = table_hdrnth(table, i);

	    if ( *R->column[1] ) {
		here = R->column[1];
		dvalue = strtod(R->column[2], &here);

		if ( !*here ) {
		    cardo(ofile, R->column[1], 0, R->column[2]);
		} else {
		    here = R->column[1];
		    ivalue = strtol(R->column[2], &here, 10);

		    if ( !*here ) {
		        cardo(ofile, R->column[1], 0, R->column[2]);
		    } else  {
		        cards(ofile, R->column[1], 0, R->column[2]);
		    }
		}
		k++;
	    }
	}

	cardo(ofile, "END", 0, "");	k++;
        cardpad(ofile, k);

	return rec;
}

int main(argc, argv)
     int         argc;
     char       *argv[];
{

	File	ifile = NULL;
	File	ofile = NULL;
	int	i, j;

	char	*name    = NULL;
	int	 primary = 1;

	int		check[NCOLFLAGS];
	TableColArg	value[NCOLFLAGS];

	int	ncols = 0;
	int	acols = 0;

	Column	column = NULL;
	char	*colon;

	TableHead	table = NULL;
        TableRow	R, row;
	char		*tbuffer;

	int	 	 x;
	char		*zap;
	int		 rec;

  	TableRow	*xrow = NULL;
	int		nrow = NROW;
	int		nred = 0, ntot;
	off_t 		here;

    for ( i = 1; i < argc; i++ ) {
	if ( argv[i][0] == '-' ) {
	    switch ( argv[i][1] ) {
	     case 'i' : 
		    if ( (ifile = (!strcmp("-", argv[i+1]))
			    ? Stdin : Open(argv[i+1], "r")) == NULL ) {
			FPrint(Stderr, "tabletofits: can't open input file: %s\n", argv[i+1]);
			perror("fixtotable");
			exit(1);
		    }
		    i++;
		    break;
	     case 'o' : 
		    if ( (ofile = (!strcmp("-", argv[i+1]))
			    ? Stdout : Open(argv[i+1], "w")) == NULL ) {
			FPrint(Stderr, "tabletofits: can't open output file: %s\n", argv[i+1]);
			perror("fixtotable");
			exit(1);
		    }
		    i++;
		    break;
	     case 'p' : primary = 0;		break;
	     case 'n' : name = argv[++i];	break;
	     default: unknown(argv[i]);
	    }
	    continue;
	}

	if ( !table ) {
	    if ( !ifile ) {
		ifile = stdin;
	    }
	    /* Read in the table header
	     */
	    if ( (table = table_header(ifile, TABLE_PARSE)) == NULL ) {
		FPrint(Stderr, "tabletofits: input file has no header\n");
		exit(1);
	    }
	    ncols = table->header->ncol;

	    Calloc(column, table->header->ncol * sizeof(struct _Column));
	}

	for ( j = 0; j < NCOLFLAGS; j++ ) check[j] = 0;

	if ( !(x = table_colarg(table, argv[i], flag, form, value, check)) ) {
		FPrint(Stderr, "tabletofits: no column %s in input table\n"
			, argv[i]);
		exit(1);
	}

	if ( check[TYPE] )  { column[x-1].type  = form[0][ffs(value[ TYPE].i)+1]; }
	if ( check[WIDTH] ) { column[x-1].width = value[WIDTH].i; }
	if ( check[AXMIN] ) { column[x-1].min   = value[AXMIN].s; }
	if ( check[AXMAX] ) { column[x-1].max   = value[AXMAX].s; }
    }

    if ( !table ) {
	if ( !ifile ) {
	    ifile = stdin;
	}
	/* Read in the table header
	 */
	if ( (table = table_header(ifile, TABLE_PARSE)) == NULL ) {
	    FPrint(Stderr, "tabletofits: input file has no header\n");
	    exit(1);
	}
	ncols = table->header->ncol;

	Calloc(column, table->header->ncol * sizeof(struct _Column));
    }


    int typesDefined = 1;
    for ( i = 0; i < ncols; i++ ) {
	column[i].column = i+1;

	if ( column[i].type ) { typesDefined = 0; }
    }

    /* Read in the first NROWs of the table
     */
    if ( nrow ) Malloc(xrow, sizeof(TableRow) * nrow);

    for ( nred = 0; nred < nrow && (xrow[nred] = table_rowget(ifile, table, NULL, NULL, NULL, 0));
	nred++ );
    ntot = nred;

    if ( !typesDefined ) {
	/* Read and Trim up the rest of the rows if there are any left in 
	   the file.
	 */

	if ( nred == nrow ) {
	    off_t 		here = Tell(ifile);
	    TableRow	R, row;
	    File		otemp;

	  R = row = table_rowloc(NULL, TABLE_LINE, TABLE_NCOL);

	  /* If the Stdin can't seek then create a temp file to store
	   * the row that we read up into.
	   */
	  if ( here < 0 ) otemp = tmpfile();

	  for ( ; row = table_rowget(ifile, table, row, NULL, NULL, 0); ntot++ ) {
	      if ( here < 0 ) table_rowput(otemp, table, row, NULL, NULL, 0);
	  }

	  table_rowfree(R); R = NULL;

	  /* Go back to where we left off.
	   */
	  if ( here < 0 ) {
	    Close(ifile);
	    ifile = otemp;
	    here  = 0; 
	  } else {
	    Seek(ifile, here, 0);
	  }
	}
    }

    for ( i = 0; i < ncols; i++ ) {
        column[i].name   = table_colnam(table, column[i].column);

	if ( !column[i].type ) {

	    switch ( table->mprec[i+1] ) {
	      case -1:	
		    column[i].type  = 'A';
		    if ( !column[i].width ) { column[i].width = table->mwidth[i+1]; }
		    break;
	      case 0:
		    /*--*/ if ( table->mwidth[i+1] <= 2 ) {
			    column[i].type  = 'B';
		    } else if ( table->mwidth[i+1] <= 5 ) {
			    column[i].type  = 'I';
		    } else {
			    column[i].type  = 'J';
		    }
		    break;
	      default:
		    if ( table->mprec[i+1] <= 8 ) {
			    column[i].type  = 'E';
		    } else {
			    column[i].type  = 'D';
		    }
		    break;
	    }
	}

	if ( !column[i].width ) { column[i].width  = 1; }
	if ( !column[i].min   ) { column[i].min    = "1"; }
	if ( !column[i].max   ) { column[i].max    = "1024"; }
    }

    if ( !ofile ) ofile = Stdout;

    Malloc(tbuffer, TABLE_FILEBUF);
    SetBuffer(ofile, _IOFBF, tbuffer, TABLE_FILEBUF);


    if ( primary ) imheader(ofile);
    here = Tell(ofile);
    rec  = bnheader(ofile, table, name, column, ncols, ntot);

    R = row = table_rowloc(NULL, TABLE_LINE, TABLE_NCOL);


    /* Output the rows of the table that were read in
     */
    for ( i = 0; i < nred; i++ ) {
	for ( j = 0; j < ncols; j++ ) {
		tobinary(ofile, table_colval(xrow[i], column[j].column)
					   , column[j].type
					   , column[j].width);
	}
	table_rowfree(xrow[i]);
    }

    /* Read (Re-Read) and output the rows beyond NROW
     */

    table_rowbuf_open(ifile, table);

    while ( row = table_rowget(ifile, table, row, NULL, NULL, 0) ) {
	for ( j = 0; j < ncols; j++ ) {
		tobinary(ofile, table_colval(row, column[j].column)
					   , column[j].type
					   , column[j].width);
	}
	i++;
    }

    table_rowfree(R);

    for ( j = (2880 - (rec*ntot) % 2880) % 2880; j--; ) {
	fputc('\0', ofile);
    }
}
