/* indexsorttable.c
 */
/* Starbase Data Tables	- An Ascii Database for UNIX

   copyright 1991, 1993, 1995, 1999 John B. Roll jr.
 */
#include <table.h> 
#include <tablesearch.h> 

void usage() {
	FPrint(Stderr, "usage: indexsorttable file column\n");
	exit(1);
}


int main(argc, argv)
	int	argc;
	char	*argv[];
{
	TableSearch	s;
	File		indx;
	File		tabl;

	TableHead	T;
	TableRow	R = NULL;
	TableRow	r = NULL;

	int		debug = 0;
	int		mode = 0;
	char		*extra;

    if ( argc < 2 ) usage();

    if ( !strcmp(argv[1], "-D") ) { argc--; argv++; debug = 1; }

    if ( !strcmp(argv[1], "-i") ) { argc--; argv++; mode = 1; }
    if ( !strcmp(argv[1], "-s") ) { argc--; argv++; mode = 2; }

    if ( argc != 3 ) usage();

    if ( (s = table_search(argv[1], argv[2], 0, NULL)) == NULL ) {
	FPrint(stderr, "indexsorttable: can't find index: %s column: %s\n"
		, argv[1], argv[2]);
	exit(1);
    }

    extra = table_hdrget(s->IHead, "Extra", 0, 1);

    if ( mode == 0 && s->Tend != s->Iindexd ) {
	    char	command[2048];
	
	sprintf(command, "sorttable -H %s %s %s%s -m "
			 "\"| indexsorttable -i %s %s\" "
			 "\"| indexsorttable -s %s %s | sorttable %s %s%s\""
		, argv[1]
		, s->Numeric ? "-h" : "", argv[2], extra
		, argv[1], argv[2]
		, argv[1], argv[2]
		, s->Numeric ? "-h" : "", argv[2], extra);

	if ( debug ) FPrint(stderr, "%s\n", command);
	system(command);
	exit(0);
    } 

    if ( mode == 0 || mode == 1 ) {
	if ( s->Method == TSEARCH_INDEX ) {
	    Seek(s->Indx, s->Istart, 0);
	    table_hdrput(stdout, s->THead, 0, NULL, NULL, 0);
	    while ( R = table_rowget(s->Indx, s->IHead, R, NULL, NULL, 0) ) {
		    off_t offs;

		offs = atoffset(table_colval(R, s->Ioffcol));
		Seek(s->Tabl, offs, 0);

		r = table_rowget(s->Tabl, s->THead, r, NULL, NULL, 0);
		table_rowput(stdout, s->THead, r, NULL, NULL, 0);
	    }

	    exit(0);
	}

	if ( s->Method == TSEARCH_BINARY ) {
		char	command[2048];

	    sprintf(command, "filebytes 0 %lld < %s", (long long) s->Iindexd, argv[1]);
	    if ( debug ) FPrint(stderr, "%s\n", command);
	    system(command);
	    exit(0);
	}
    }

    if ( mode == 2 ) {
		char	command[2048];

	    sprintf(command, "header < %s; filebytes %lld -1 < %s"
			, argv[1]
			, (long long) s->Iindexd, argv[1]);

	    if ( debug ) FPrint(stderr, "%s\n", command);
	    system(command);
	    exit(0);
	
    }
}
