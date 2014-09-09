/* filebytes.c
 */

#include <xos.h>
#include <xfile.h>

#include <range.h>
#include <table.h>
#include <tablerange.h>

#define BUFSIZE	1024*64

int main(argc, argv)
	int	 argc;
	char	*argv[];
{
	off_t	start;
	off_t	end;
	int	red;

	char	buffer[BUFSIZE];
	int	blank = 0;
	int	invrt = 0;

    while ( argc > 1 && argv[1][0] == '-' && argv[1][1] != '\0' ) {
	switch ( argv[1][1] ) {
	 case 'b' : 	blank = 1;	break;
	 case 'i' : 	invrt = 1;	break;
	}

	argc--;
	argv++;
    }

    if ( argc == 2 ) {
	    TableHead	 table;
	    Range       *range;
	
	    File	 file = Open(argv[1], "r+");


	range = (Range *) table_readrange(stdin, 0, 1, 2);


	if ( (table = table_header(file, 0)) == NULL ) {
	    fprintf(stderr, "filebytes: cannot read header from input: %s\n", argv[1]);
	}
	if ( invrt ) {
	    range = range_invrt(range, Tell(file), filesize(file));
	}

	if ( blank ) {
	    table_blankrangerows(file, range);
	} else {
	    table_hdrput(stdout, table, 0, NULL, NULL, 0);

	    table_printrangerows(table, file, stdout, range);
	}

	exit(0);
    }

    if ( argc == 3 ) {
	start = atoi(argv[1]);
	end   = atoi(argv[2]);

	Seek(stdin, start, SEEK_SET);
	while ( (red = fread(buffer, 1
		    , end == -1 ?
			      BUFSIZE
			    : Min(BUFSIZE, end - start)
		    , stdin)) > 0 ) {
	    fwrite(buffer, 1, red, stdout);
	    start += red;

	    if ( end != -1 && start >= end ) break;
	}

	if ( red >= 0 ) { exit(0); }
	else            { exit(1); }
    }

    fprintf(stderr, "usage: filebytes [start end]\n");
    exit(1);
}
