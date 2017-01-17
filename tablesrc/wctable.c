/* statstable.c
 */
/* Starbase Data Tables	- An Ascii Database for UNIX

   copyright 2003 John B. Roll jr.
 */
#include <xos.h>
#include <xfile.h>
#include <table.h>

void wcfile(fp, head, lines, words, chars, file)
	File	 fp;
	int	 head;
	int	 lines;
	int	 words;
	int	 chars;
	char	*file;
{
	TableHead T;
	TableRow  R;
	TableRow  row;

	long l = 0;
	long w = 0;
	long c = 0;

    if ( !(T = table_header(head ? fp : NULL, 0)) ) {
	fprintf(stderr, "wctable: cannot read header from table : %s\n", file);
	exit(1);
    }

    table_rowbuf_open(fp, T);
	
    R = table_rowloc(NULL, 0, 0);

    for ( l = 0; row = table_rowget(fp, T, R, NULL, NULL, 0);
	l++ ) {
	w += row->ncol;
	c += row->nbuf;
    }

    if ( lines ) printf("%8ld ", l);
    if ( words ) printf("%8ld ", w);
    if ( chars ) printf("%8ld ", c);
    if ( fp != stdin ) printf("%s", file);
    printf("\n");

    table_rowfree(R);
    table_hdrfree(T);
}

#include "argv-fuzz-inl.h"

int main(argc, argv)
			int	 argc;
			char	*argv[];
{
	int		i;
	TableHead	T;
	TableRow	row;

	long	lines = 0;
	long	words = 0;
	long	chars = 0;
	long	head  = 1;

    AFL_INIT_ARGV();

    for ( i = 1; i < argc; i++ ) {
	if ( argv[i][0] == '-' ) {
	    switch ( argv[i][1] ) {
		case 'H': 	head = 0;	break;
		case 'l': 	lines++;	break;
		case 'w': 	words++;	break;
		case 'c': 	chars++;	break;
		default:	goto done;
	    }
	} else { break; }
    }

  done:

    if ( !lines && !words && !chars ) {
	lines = words = chars = 1;
    }

    if ( i == argc ) {
	wcfile(stdin, head, lines, words, chars, "stdin");
    } else {
	    for ( ; i < argc; i++ ) {
		    File fp;

		if ( fp = Open(argv[i], "r") ) {
		    wcfile(fp, head, lines, words, chars, argv[i]);
		    Close(fp);
		} else {
		    perror(argv[i]);
		    exit(1);
		}
	    }
    }
    exit(0);
}
