/* index.c
 */
/* Starbase Data Tables	- An Ascii Database for UNIX

   copyright 1991, 1993, 1995, 1999 John B. Roll jr.
 */

#include <unistd.h>

#include <signal.h>
#include <xos.h>
#include <xfile.h>

#include <table.h> 
#include <tablesearch.h>

char *SlurpLine();
char *trim();

char	tmpname[1024];

void sig_handler(sig)
        int     sig;
{
    if ( tmpname[0] ) {
	unlink(tmpname);
    }

    kill (getpid (), sig);
}


void usage() {
	FPrint(Stderr, "usage: index -m[sbihr] [-n] [-h] [-r] [-L] file column [extra cols]\n");
	exit(1);
}

int main(argc, argv)
	int	argc;
	char	*argv[];
{
	TableSearch	 s;
	char	search = 's';
	int	numeric	= 0;
	int	reverse = 0;
	int	reindex = 0;
	int	columns = 1;
	int	debug   = 0;
	int	touch   = 0;

	char	*filename;
	char	*colmname;
	char	*col2name = NULL;
	char	 Colmname[128];
	char	 Col2name[128];
	double	 modulus  = 0.0;
	char	 indxname[1024];
	char	 modulstr[1024];

	TableHead	thead;

	File	file = NULL;
	File	indx = NULL;

	int	Column;
	int	Colum2 = 0;
	off_t	Tstart;
	off_t	Tend;
	int	Width;
	int	Rows = 0;

	char   *memory = NULL;
	char   *zipper = NULL;

	char	buffer[25];
	char	command[4096];
	char	extra[1024];

	char	*tbuffer;

	char	 buf[TABLE_TRIMBUF];

	int	i;

	tmpname[0] = '\0';
	extra[0]   = '\0';

	sigset_t        mask;
        sigfillset(&mask);
        SigAction(SIGHUP , sig_handler, &mask, 0);
        SigAction(SIGINT , sig_handler, &mask, 0);
        SigAction(SIGQUIT, sig_handler, &mask, 0);
        SigAction(SIGPIPE, sig_handler, &mask, 0);
        SigAction(SIGSEGV, sig_handler, &mask, 0);
        SigAction(SIGTERM, sig_handler, &mask, 0);

	for ( i = 1; i < argc; i++ ) {
	    if ( argv[i][0] == '-' ) {
		switch ( argv[i][1] ) {
		 case 'm' :
		  switch ( argv[i][2] ) {
		   case '\0':
		   case 's':	search = 's';	break;
		   case 'b':	search = 'b';	break;
		   case 'B':	search = 'b';	touch = 1; break;
		   case 'i':	search = 'i';	break;
		   case 'h':	search = 'h';	break;
		   case 'r':	search = 'r';	break;
		   default:
			FPrint(Stderr, "index: bad search type options\n");
			usage();
			break;
		  }
		  break;

		 case 'a':	numeric = 0;	break;
		 case 'n':	numeric = 1;	break;
		 case 'h':	numeric = 2;	break;
		 case 'r':	reverse = 1;	break;
		 case 'R':	reindex = 1;	
				break;

		 case 'D':	debug   = 1;	break;
		 case 'L':	columns = 0;	break;
		 case 'V':	columns = 2;	break;

		 case 'M':	memory  = argv[i+1];  i++;  break;
		 case 'Z':	zipper  = argv[i+1];  i++;  break;

		 default:
			FPrint(Stderr, "index: unrecognised option -%c\n"
				, argv[i][1]);
			usage();
			break;
		}
	    } else {
#define NCOLFLAGS	 1
#define ZONES		 0

			char	*flag	=   "%";
			char	*form[] = { "r" };

			TableColArg	value[NCOLFLAGS];
			int		check[NCOLFLAGS];
			int		k;

		    for ( k = 0; k < NCOLFLAGS; k++ ) check[k] = 0;
		    value[ZONES].f = 0;

		filename = argv[i++];

		if ( (file = Open(filename, "r")) == NULL ) {
		    FPrint(Stderr, "index: can't open file: %s\n", filename);
		    usage();
		}

	        Malloc(tbuffer, TABLE_FILEBUF);
	        SetBuffer(file, _IOFBF, tbuffer, TABLE_FILEBUF);

		if ( (thead = table_header(file, 0)) == NULL ) {
		    FPrint(Stderr, "index: can't read table header from: %s\n"
				, filename);
		    usage();
		}

		if ( i >= argc ) {
		    FPrint(Stderr, "index: no index column given\n");
		    usage();
		}
		colmname = argv[i++];

		if ( (Column = table_colarg(thead, colmname
					    , flag, form, value, check)) == 0 ) {
		    FPrint(Stderr, "index : index column not given \"%s\"\n"
			   , colmname);
		    exit(1);
		}
		strcpy(Colmname, trim(table_colnam(thead, Column), buf));

		if ( check[ZONES] ) {
		    modulus = value[ZONES].f;

		    if ( i >= argc ) {
			FPrint(Stderr, "index: second index column not given\n");
			usage();
		    }
		    col2name = argv[i++];

		    if ( (Colum2 = table_colnum(thead, col2name)) == 0 ) {
			FPrint(Stderr, "index: index column: %s not in file: %s\n"
				, col2name, filename);
			usage();
		    }
		    strcpy(Col2name, trim(table_colnam(thead, Colum2), buf));
		} 

		if ( reindex ) {
		    if ( (s = table_search(filename, colmname, 0, NULL))
			  == NULL ) {
			FPrint(stderr
				, "index: can't find index: %s column: %s\n"
				, filename, colmname);
			exit(1);
		    }
    		    if ( s->Tend == s->Iindexd ) {
			exit(0);
		    }

		    search = s->Method == TSEARCH_SEQUENTIAL ? 's'
			 : ( s->Method == TSEARCH_BINARY     ? 'b'
			 : ( s->Method == TSEARCH_INDEX	     ? 'i'
			 : ( s->Method == TSEARCH_ROW	     ? 'r'
			 : ( s->Method == TSEARCH_HASH	     ? 'h' : ' ' ))));
		    numeric = s->Numeric;
		    reverse = s->Reverse;

		    { char *ex = table_hdrget(s->IHead, "Extra", 0, 1);

			if ( ex ) strcpy(extra, ex);
		    }

		} else {
		    while ( i < argc ) {
			    strcat(extra, " ");
			    strcat(extra, argv[i++]);
		    }
		}
		if ( !search ) {
			FPrint(Stderr, "index: no search mode selected\n");
			usage();
		}

		Tstart = Seek(file, 0, 1);
		Tend   = Seek(file, 0, 2);
		Seek(file, Tstart, 0);
		snprintf(buffer, sizeof(buffer), "%lld", (long long) Tend); 
		Width = strlen(buffer);
		if ( Width < 6 ) Width = 6;

		if ( search == 'r' ) {
		    colmname = "Row";
		}
	    }
	}

	if ( file == NULL ) {
	    FPrint(Stderr, "index: no file to index!\n");
	    usage();
	}

	snprintf(indxname, sizeof(indxname), "%s.%s%s%s%c%c", filename
		, Colmname
		, col2name == NULL ? "" : "."
		, col2name == NULL ? "" : Col2name
		, ( numeric == 0 ? '-' 
		: ( numeric == 1 ? '.' 
		: ( numeric == 2 ? '+' : '-' )))
		, search);	

	if ( search == 'i' ) {
		snprintf(command, sizeof(command), "sorttable %s %s%s %s %s %s %s %s %s %s %s %s"
				, debug == 1 	? "-X" : ""
			        , memory != NULL    ? "-M "  : ""
			        , memory != NULL    ? memory : ""
			        , zipper != NULL    ? "-Z "  : ""
			        , zipper != NULL    ? zipper : ""
				, numeric == 0	? "" :
				 (numeric == 1  ? "-n" : "-h" )
				, reverse == 0  ? "" : "-r"
				, Colmname
			    	, col2name == NULL ? "" : Col2name
				, extra
				, columns ? "| column Offset Length >"
				 	  : "| column Offset >"
				, indxname);
		if ( debug ) FPrint(stderr, "%s\n", command);

		indx = popen(command, "w");

	        Malloc(tbuffer, TABLE_FILEBUF);
	        SetBuffer(indx, _IOFBF, tbuffer, TABLE_FILEBUF);
	} else if ( search != 'b' ) {
	    if ( (indx = Open(indxname, "w+")) == NULL ) {
		    FPrint(Stderr
			, "index: can't open index file: %s\n", indxname);
		    exit(1);
	    }
	}

	if ( search != 'b' && search != 's' ) {
	    FPrint(indx, "%s\n\n", filename);
	    {	time_t	t;

		    time(&t);
		    FPrint(indx, "Date	%s", ctime(&t));
	    }
	    FPrint(indx, "Index	" "%lld\n\n", (long long) Tend);
	    FPrint(indx, "Column	%s\n", colmname);
	    FPrint(indx, "Extra	%s\n\n", extra);

	    if ( search == 'r' || search == 'h' ) {
		    FPrint(indx, "%*s\n", Width, "Offset");
		    FPrint(indx, "%*s\n", Width, "------");
	    } else {
		FPrint(indx, "Offset	");
		if ( columns ) FPrint(indx, "Length	");
		FPrint(indx, "%s%c", Colmname, Colum2 ? '\t' : '\n');
		if ( Colum2 ) FPrint(indx, "%s\n", Col2name);

		FPrint(indx, "------	");
		if ( columns ) FPrint(indx, "------	");
		FPrint(indx, "-%c", Colum2 ? '\t' : '\n');
		if ( Colum2 ) FPrint(indx, "-\n");
	    }
	}

	if ( search == 's' ) {
		Close(indx);
		exit(0);
	}


    if ( search == 'b' ) {
	    char	command[4096];
	    char	tmpname[1024];
	    int		pid = getpid();

	Close(file);
	snprintf(tmpname, sizeof(tmpname), "%d.%s", pid, "index");

	if ( reindex ) {
	    snprintf(command, sizeof(command), "indexsorttable %s %s %s > %s"
			    , debug == 1 	? "-D" : ""
			    , filename
			    , colmname
			    , tmpname);
	} else {
	    snprintf(command, sizeof(command), "sorttable %s %s%s %s %s %s %s %s %s %s < %s > %s"
			    , debug == 1 	? "-X" : ""
			    , memory != NULL    ? "-M "  : ""
			    , memory != NULL    ? memory : ""
			    , zipper != NULL    ? "-Z "  : ""
			    , zipper != NULL    ? zipper : ""
			    , numeric == 0	? "" :
			     (numeric == 1  ? "-n" : "-h" )
			    , reverse == 0  ? "" : "-r"
			    , colmname
			    , col2name == NULL ? "" : col2name
			    , extra
			    , filename
			    , tmpname);
	} 
	if ( debug ) FPrint(stderr, "%s\n", command);
	if ( touch || !system(command) ) {
	    if ( (indx = Open(indxname, "w+")) == NULL ) {
		FPrint(Stderr, "index: can't open index file: %s\n", indxname);
		exit(1);
	    }

	    FPrint(indx, "%s\n\n", filename);
	    {	time_t	t;

		    time(&t);
		    FPrint(indx, "Date	%s", ctime(&t));
	    }
	    FPrint(indx, "Index	" "%lld\n\n", (long long) Tend);
	    FPrint(indx, "Column	%s\n", colmname);
	    FPrint(indx, "Extra	%s\n\n", extra);

	    FPrint(indx, "%s\n", "Index");
	    FPrint(indx, "%s\n", "-----");

	    Close(indx);


	    if ( !touch ) {
		    if ( unlink(filename) == -1 )          perror("index");
		    if ( rename(tmpname, filename) == -1 ) perror("index");
		    tmpname[0] = '\0';
	    }
	} else {
		FPrint(Stderr, "index: sort exits abnormally\n");
		exit(1);
	}
	

	exit(0);
    }

    if ( search == 'r' ) {
	    off_t	offset = Tstart;
	    char	*line  = NULL;
	    int	 n   = 0;
	    int	 max = 0;
		    
	while ( (line = SlurpLine(file, line, &n, &max)), n ) {
		FPrint(indx, "%*lld\n", Width, (long long) offset);
		offset += n + 1;
	}

	Close(file);
	Close(indx);
	exit(0);
    }

    if ( search == 'h' ) {
	    off_t	 offset = Tstart;
	    char	*line = NULL;
	    int	 n   = 0;
	    int	 max = 0;

	TableRow row = table_rowloc(NULL, 0, 0);

	/* Count the rows
	 */
	while ( line = SlurpLine(file, line, &n, &max), n ) Rows++;
	Rows *= 2;
	Seek(file, Tstart, 0);

	/* Initialize and Clear out the hash table.
	 */
	for ( i = 0; i < Rows; i++ ) 
		FPrint(indx, "%*s\n", Width, "");

	while ( table_rowget(file, thead, row, NULL, NULL, 0) ) {
			int	value = 1;
			int	seekat;
			int	seekto;

		seekat = table_hash(table_colval(row, Column), Rows);

		while ( value != 0 ) {
			seekto = (seekat + 2) * ( Width + 1 );
			Seek(indx, seekto, 0);
			value = atoi(line 
				= SlurpLine(indx, line, &n, &max));

			if ( ++seekat >= Rows ) seekat = 0;
		}
		Seek(indx, seekto, 0);

		FPrint(indx, "%*lld\n", Width, (long long) offset);
		offset += row->nbuf;
	}

	Close(file);
	Close(indx);
	exit(0);
    }

    if ( search == 'i' ) {
	    off_t offset = Tstart;
	    int	       r = 0;

	TableRow row = table_rowloc(NULL, 0, 0);

	table_rowbuf_open(file, thead);

	while ( table_rowget(file, thead, row, NULL, NULL, 0) ) {
		char	*v = table_colval(row, Column);
	    r++;

	    if ( v == NULL ) {
		    FPrint(Stderr, "index: no data at row %d col %d\n", r, Column);
		    exit(1);
	    }

	    FPrint(indx, "%lld\t", (long long) offset);
	    if ( columns ) FPrint(indx, "%d\t", row->nbuf);

	    FPrint(indx, "%s%c", v, Colum2 ? '\t' : '\n');
	    if ( Colum2 ) FPrint(indx, "%s\n", table_colval(row, Colum2));

	    offset += row->nbuf;
	}

	Close(file);
	pclose(indx);

	exit(0);
    }	

    exit(1);
}
