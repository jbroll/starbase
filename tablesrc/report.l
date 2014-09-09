%{
/* report_lex.l
 */
/* Starbase Data Tables  - An Ascii Database for UNIX

   copyright 1991, 1993, 1995, 1999 John B. Roll jr.
 */
   

#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

#include <xos.h>
#include <xfile.h>

#include "../tablelib/table.h"

TableHead	T = NULL;
TableRow	R = NULL;
int		dollar = 0;

#define LENCMD	16900

int	cmode = 0;
char	command[LENCMD];


void outputs(s)
	 char	*s;
{
	if ( !s ) return;

	if ( cmode ) {
		while ( *s && cmode < LENCMD ) { command[(cmode++)-1] = *s; s++; };
		command[cmode-1] = '\0';
	} else	{
	    while ( *s ) { 
		switch ( *s ) {
		    case '\001':	putchar('\n');	break;
		    case '\002':	putchar('\t');	break;
		    case '\003':	putchar('\r');	break;
		    default:		putchar(*s); 	break;
		}
		s++;
	    }
	}
}


/* This YYINPUT definitions sets the scanner to traverse a string.
** The first '\0' encountered is the "EOF".
**/

static char	*text;

#undef  YY_INPUT
#define YY_INPUT(buf,result,max_size)   {                               \
	        if ( *buf = *text ) {	 				\
			text++;						\
                        result = 1;                                     \
                } else                                                  \
                        result = YY_NULL;                               \
        }

#define YY_SKIP_YYWRAP
#define yywrap()	1

%}

%%

"<#"	{	if ( !cmode ) cmode = 1;	}
"#>"	{	    char	*filename = command;
		    char	*p;
		    static int		 fd = -1;

		if ( cmode == 0 ) {
			fprintf(stderr, "report: unexpected filename closure\n");
			exit(1);
		}
		cmode = 0;

		fflush(stdout);

		while ( *filename && *filename == ' ' ) filename++;
		p = filename;
		while ( *p && *p != ' ' ) p++;
		*p = '\0';

		fflush(stdout);
	        if ( fd != -1 ) close(fd);

		if ( (fd = open(filename, O_APPEND | O_TRUNC | O_CREAT | O_WRONLY, 0644)) == -1 ) {
		    perror(filename);
		    exit(1);
		}

		dup2(fd, 1);
	}
"<!"	{	if ( !cmode ) cmode = 1;	}
"!>"	{	if ( cmode == 0 ) {
			fprintf(stderr, "report: unexpected command closure\n");
			exit(1);
		}
		cmode = 0;
		
		fflush(stdout);
		system(command);
	}

"<"[^!#][^<>]*>	{
		char	 column[1024];
		char	*str;
		int	 n;

		if ( !dollar || ( dollar && yytext[1] == '$' ) ) {
		    strncpy(column, &yytext[1+dollar], yyleng-(2+dollar));
		    column[yyleng-(2+dollar)] = '\0';

		    if ( n = table_colnum(T, column) )
			    outputs(table_colval(R, n));
		    else
			if ( str = table_hdrget(T, column, 0, 1) )
			    outputs(str);
			else
			    outputs(yytext);
		} else {
		    outputs(yytext);
		}
	}

\\\n	{	;	}
\\\\	{	outputs(yytext);	}
.	{	outputs(yytext);	}

<<EOF>> { if ( cmode ) {
		fprintf(stderr, "report: end of file encountered in command substitution\n");
		exit(1);
	  }

	  return 0;
	}

%%


int main(argc, argv)
	int	argc;
	char	*argv[];
{
		File	 infile;
		char	*filetext;

	if ( argc >= 2 && !strcmp(argv[1], "-x") ) {
	    dollar = 1;
	    argc--;
	    argv++;
	}

	if ( argc != 2 ) {
		fprintf(stderr, "usage:	report template < table\n");
		exit(1);
	}

	/* Move our input/output off of stdin/stdout so that if we call "system"
	   the shell won't seek our files.
	 */
	dup2(0, 3);
	infile = fdopen(3, "r");
	close(0);

	if ( (text = filetext = readfile(argv[1])) == NULL ) {
	    perror("report");
	    exit(1);
	}


	if ( !(T = table_header(infile, 0)) ) {
		fprintf(stderr, "report: can't read table from input\n");
		exit(1);
	}

	while ( (R = table_rowget(infile, T, R, NULL, NULL, 0)) != NULL ) {
		(void) yylex();
		text = filetext;
	}
}
