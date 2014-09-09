/* extracttable.c
 */
/* Starbase Data Tables	- An Ascii Database for UNIX

   copyright 1991, 1993, 1995, 1999 John B. Roll jr.
 */

#include <table.h>
#include <signal.h>

int usesep = 1;
int debug  = 0;

int extract(file, mark, head, data)
	File	 	 file;
	int	 	 mark;
	TableHead	 head;
	void		*data;
{
		static  int sep = 0;
		File 	output = Stdout;
		int	ch;
		int	i;

		static int	prevsep = 0;

		char	command[32768];
		char	value[1024];
		char	trim1[1024];
		char	trim2[1024];

        command[0] = '\0';

	if ( data ) {
	    if ( usesep && prevsep ) {
	        printf("\f");
	        fflush(stdout);
	    }
	    sprintf(value, "export TableName; TableName='%s'\n", head->name);
	    strcat(command, value);
	    for ( i = 0; i < head->nvalue; i++ ) {
		if ( table_isid(head->values[i]->column[1]) && head->values[i]->column[2]) {
		    char *colid = trim(head->values[i]->column[1], trim1);

		       if ( *colid == '#' ) colid++;
		    while ( *colid == ' ' ) colid++;

		    sprintf(value, "export Table_%s; Table_%s='%s'\n"
			    , colid, colid
			    , trim(head->values[i]->column[2], trim2));
		    strcat(command, value);
		}
	    }

	    sprintf(value, "export N; N=%d; ", mark);
	    strcat(command, value);
	    strcat(command, data);

	    if ( debug  ) { fprintf(stderr, "\n%s\n", command); }

	    output = popen(command, "w");
	}

	table_hdrput(output, head, 0, NULL, NULL, 0);

	while ( (ch = GetC(file)) != EOF && ch != '\f' )
	    PutC(output, ch);

	if ( data ) {
		prevsep = !pclose(output);
		sep = 0;
	} else {
		sep = 1;
	}

	if ( usesep && sep ) {
	    printf("\f");
	    fflush(stdout);
	}

	return EOF;
}


void table_extractav(file, argc, argv)
	File	 file;
	int	 argc;
	char	*argv[];
{
	va_list args;  

	TableSection	 sections = NULL;
	int	 	 nsec = 0;
	int	 	 asec = 0;
	char		*sec;

	/* Parse the varargs into a table of name--> command pairs.
	 */
	for ( nsec = 1; (sec = *argv) && nsec <= argc; nsec++, argv++ ) {
	    if ( nsec >= asec ) {
		asec *= 2;
		asec += 5;
		ReAlloc(sections, asec * sizeof(struct _TableSection));
	    }
	    sections[nsec].sec = sec;
	    sections[nsec].fun = extract;
	    sections[nsec].dat = argv[1];

	    if ( !strcmp(sections[nsec].sec, "default") ) {
		sections[0].sec = sections[nsec].sec;
		sections[0].fun = sections[nsec].fun;
		sections[0].dat = sections[nsec].dat;
		nsec--;
	    }
	}

	table_extract(file, sections, nsec);
	Free(sections);
}

int main(argc, argv)
	int	 argc;
	char	*argv[];
{
		char	*template;
		char	*command = NULL;

	signal(SIGPIPE, SIG_IGN);

	if ( argc < 2 ) {
	    FPrint(stderr, "usage: extracttable template [command] ...\n");
	    exit(1);
	}
	if ( argc > 2 && !strcmp(argv[1], "-debug") ) {
		debug = 1;
		argc--;
		argv++;
	}
	if ( argc > 2 && !strcmp(argv[1], "-sep") ) {
		usesep = 0;
		argc--;
		argv++;
	}

	table_extractav(Stdin, argc--, ++argv);

	exit(0);
}
