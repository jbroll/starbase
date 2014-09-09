%{
/* table_lex.l
 */
/* Starbase Data Tables  - An Ascii Database for UNIX

   copyright 1991, 1993, 1995, 1999 John B. Roll jr.
 */

#include <xos.h>
#include <xfile.h>
#include <xlist.h>
#include "filelist.h"

#include <table.h>
#include <signal.h>

#define TABLEFUN	"$TABLEFUN:" TABLELIB "/tablefun"
#define TABLEINC	"$HOME:$TABLELIB"  ":" TABLELIB
#define TABLEAWK         "TABLEAWK"


void copytext();
void include();

extern char *getenv();

#define YY_NEVER_INTERACTIVE 1
#define YY_SKIP_YYWRAP
#define yywrap()	1

/* This YYINPUT definitions sets the scanner to traverse a string.
** The first '\0' encountered is the "EOF".
**/

static char	*tprog;


#undef  YY_INPUT
#define YY_INPUT(buf,result,max_size)   {                               \
	        if ( *buf = *tprog ) {	 				\
			tprog++;					\
                        result = 1;                                     \
                } else                                                  \
                        result = YY_NULL;                               \
        }

extern filelist	 function();
extern filelist  FileList();

extern char	*readfile();

char		*progname, *strrchr();

typedef struct Program {
	char	*buff;
	int	 leng;
	int	 here;
} Program;

typedef	struct	Arrylist {
	struct Arrylist	*next;
	char	*colnam;
	int	 colnum;
	int	 column;
} Arrylist;
	

/********************************/
/* Parser symbols and state	*/
/********************************/

char	 cnumber[40];
char	 colnam[1024];
int	 colnum;

int	StackRef = 0;

TableHead	         table = NULL, T;
static	filelist	 includes = NULL;
static	Arrylist	*arrylist = NULL;
static  Arrylist	*arryitem = NULL;

int	Napp = 0;
int	Nins = 0;
int	Nsel = 0;
char	*colapp[128];
char	*colins[128];
char	*colsel[128];
char	*prints[4096];

char	*stack = NULL;

static	Program		*program;
%}

%x COMMENT QUOTE ESCAPE HV CV
%s CI

ALPH [a-zA-Z_]
OTHR [a-zA-Z0-9_]

%%
[1-9][0-9]*e[+-]?[0-9] { copytext(program, yytext); }
\+\+ { copytext(program, yytext);  /* Allow ++x without invoking the +x add a column feature */ }
[.+]{ALPH}{OTHR}*/.*= {	/* Match possible column names		*/

		  strcpy(colnam, &yytext[1]);
		  if ( colnum = table_colnum(table, colnam) ) {
				BEGIN(CV);
		  } else {
		  	copytext(program, colnam);

			if ( *yytext == '+' ) {
			    colapp[Napp++] = strdup(colnam);
			} else {
			    colins[Nins++] = strdup(colnam);
			}
		  } 
		}

{ALPH}{OTHR}* {		/* Match possible column names		*/

		  strcpy(colnam, yytext);
		  if ( colnum = table_colnum(table, colnam) ) {
				BEGIN(CV);
		  } else {
		  	copytext(program, colnam);
			/* The key word was not a column name
		   	   See if it is a header value.
		 	 */
	  		if ( table_hdrget(table, colnam, 0, 1) ) {
				BEGIN(HV);
			}
		  } 
		}

{ALPH}{OTHR}*\( {	/* Match possible functions names	*/

				filelist fp;

		  copytext(program, yytext);
		  if ( fp = function(yytext, yyleng -1) ) {
				filelist cp;

			Malloc(cp, sizeof(struct _filelist));
			cp->next = NULL;
			cp->name = fp->name;
			cp->path = fp->path;

			includes = ListAdd(includes, cp);
		  }
		}

<CV>[ \t]*\[	{	/* The column value is indexed			*/

		  	Arrylist *al;

		  for ( al = arrylist; al; al = al->next )
			if ( !strcmp(al->colnam, colnam) ) break;
	  
		  if ( al == NULL ) {
		      New(al, Arrylist);
		      NewString(al->colnam, colnam);
		      al->colnum = -colnum;
		      al->column =  colnum;
		      al->next = arrylist;
		      arrylist = al;
		  }

		  arryitem = arrylist;

		  copytext(program, colnam);
		  copytext(program, yytext);

		  BEGIN(CI); 					}

<CV>.|\n	{	/* The column value is not indexed		*/
		  sprintf(cnumber, "$%d", colnum);
		  copytext(program, cnumber);
		  copytext(program, yytext);

		  if ( *yytext == '\"' ) { BEGIN(QUOTE);
		  } else { 		   BEGIN(INITIAL);				
		  }
		}

<CI>=		{ if ( arryitem->colnum < 0 ) arryitem->colnum = -arryitem->colnum;

		  copytext(program, yytext);
		  BEGIN(INITIAL); 					}

<CI>;|\n	{ copytext(program, yytext);
		  BEGIN(INITIAL); 					}

		

<HV>[ \t]*\[	{	/* The header value is already indexed.		*/
		  copytext(program, yytext);
		  BEGIN(INITIAL);				}

<HV>.|\n 	{	/* The header value is not indexed.		*/
		  copytext(program, "[1]");
		  copytext(program, yytext);
		  if ( *yytext == '\"' ) { BEGIN(QUOTE);
		  } else {		   BEGIN(INITIAL);				
		  }
		}

@		{ if ( stack ) {
    			StackRef = 1;
		        copytext(program, "_Stack");
		        BEGIN(HV);
		  } else {
		  	copytext(program, yytext);
		  }
		}

#		{	/* Match and skip any text within comments	*/

		  copytext(program, yytext);  BEGIN(COMMENT);	}
<COMMENT>\n	{ copytext(program, yytext);  BEGIN(INITIAL);	}
<COMMENT>.	{ copytext(program, yytext);			}


\"		{ 	/* Match and skip any text within quotes	*/
		  copytext(program, yytext);  BEGIN(QUOTE);	}
<QUOTE>\\	{ copytext(program, yytext);  BEGIN(ESCAPE);	}
<ESCAPE>.	{ copytext(program, yytext);  BEGIN(QUOTE);	}
<QUOTE>\"	{ copytext(program, yytext);  BEGIN(INITIAL);	}
<QUOTE>.|\n	{ copytext(program, yytext);  			}

.|\n		{ 	/* Match any other text				*/

		  copytext(program, yytext);
		}

<<EOF>>		{ 
		  if ( YY_START == HV ) {
			copytext(program, "[1]");
		  }
		  if ( YY_START == CV ) {
		      sprintf(cnumber, "$%d", colnum);
		      copytext(program, cnumber);
		  }
			
		  return 1;
		}
%%


void parse(txt)
     char	*txt;
{
  static int parser = 0;

  tprog= txt;

  if ( parser ) {
    yyrestart((void *)  txt);
    BEGIN(0);
  } else {
    parser = 1;
  }

  if ( !yylex() ) {
    FPrint(Stderr, "%s : can't parse program\n", progname);
    exit(1);
  }
}

void copytext(program, text)
    Program	*program;
    char	*text;
{
  while ( *text ) {
	if ( program->buff == NULL || program->here >= program->leng - 1 ) {
	    program->leng *= 2;
	    program->leng += 128;
	    ReAlloc(program->buff, program->leng);
	}

	program->buff[program->here++] = *text++;
  }
  program->buff[program->here] = '\0';
}


static filelist fpinc = NULL;
static filelist fptab = NULL;

int main(argc, argv)
     int	 argc;
     char	*argv[];
{
  int	what;
  
  static Program	 preamble = { NULL, 0, 0 };
  static Program	 prog     = { NULL, 0, 0 };
  static Program	*prep     = &preamble;

  int		 argi;
  int		 argj	 = 0;
  int		 argk;
  char		*argx[1024];
  int		 i, j;

  char		*text;
  char          *talk;

  char		*ifile = NULL;
  char		*ofile = NULL;
  char		*pfile = NULL;
  char		*head = NULL;

  char		*cols = NULL;
  int		 debug= 0;

  char		 stkval[1024];
  char		 buffer[1024];

  File		tfp = NULL;
  File		ifp = Stdin;
  File		ofp = Stdout;

  int		progarg = 0;
  int		inter   = 0;
  
  char	*tableinc;
  char	*tablefun; 


  struct stat statbuf;

  char	ncolstr[32];


typedef struct _DoWhat {
  char	*name;
  char	*prec;
  char	*post;
  int	 proc;
} DoWhat;

#define HEADER	0x0001
#define HDRVAL	0x0002
#define PRINTH	0x0004
#define PARSEH	0x0008
#define CATARG	0x0010
#define HASSTK	0x0020
#define ONEEXP	0x0040
#define ARRINP	0x0080
#define ARROUT	0x0100
#define COLSEL	0x0200

#define	TBLPRG	( HEADER | HDRVAL | PARSEH | PRINTH | ARRINP )

  static DoWhat dx[] = {
   {"reckon"	, "BEGIN { "	   , " }",		0			 }
 , {"calc"	, "BEGIN { _ANS = ", "\nprint _ANS }",	CATARG | HASSTK		 }
 , {"table"	,  NULL		   , NULL,		0			 }
 , {"row"	,  NULL		   , NULL,		TBLPRG | COLSEL | ONEEXP }
 , {"select"	,  NULL		   , NULL,		TBLPRG | COLSEL | ONEEXP }
 , {"compute"	, "{"		   , "}\n",		TBLPRG | COLSEL | ARROUT }
 , {"validate"	,  NULL		   , NULL,		TBLPRG | COLSEL	| ARROUT }
 , { NULL	,  NULL	  	   , NULL,		0			 }
};

  program = &prog;
  signal(SIGPIPE, SIG_IGN);

  if ( (progname = strrchr(argv[0], '/')) )
    progname++;
  else	progname = argv[0];

  {
	char	*binpath = strdup(argv[0]);
	char	*endpath = strrchr(binpath, '/');

	if ( endpath ) *endpath = '\0';
	else 	       *binpath = '\0';

      tableinc = malloc(strlen(binpath) + strlen(TABLEINC) + 2);
      strcpy(tableinc, TABLEINC);
      if ( *binpath ) {
	strcat(tableinc, ":");
      	strcat(tableinc, binpath);
      }
  }

  fpinc = FileList(tableinc, "*");
  if ( !((tablefun = getenv("TABLEFUN")) && *tablefun == '\0') ) {
      fptab = FileList(TABLEFUN, "*");
  }

  for ( what = 0; dx[what].name; what++ ) {
    if ( !strcmp(progname, dx[what].name) ) break;
  }

  if ( dx[what].name == NULL ) {
    FPrint(Stderr, "table : don't know how to %s\n", progname);
    exit(1);
  }

    /* Handle command line arguments
     */
    for ( argi = 1;  argi < argc && argv[argi][0] == '-'; ) {
	switch ( argv[argi++][1] ) {
          case 'f': {	int j;

	       for ( j = 2; argv[argi-1][j]; j++ ) {
	           switch ( argv[argi-1][j] ) {
		     case 'D':	debug = 1;				break;
		     case 'r':   dx[what].proc |= HEADER;		break;
		     case 'x':   dx[what].proc &= ~HDRVAL;		break;
		     case 'p':   dx[what].proc |= PRINTH;		break;
		     case 'h':	dx[what].proc |= PARSEH | HDRVAL | ARRINP | ARROUT;	break;
		   }
		}
		pfile  = argv[argi++];
		break;	
	    }
	  case 'H':	head   = argv[argi++];   		break;
	  case 'D':  debug = 1;				break;
	  case 'r':  dx[what].proc |= HEADER;		break;
	  case 'p':  dx[what].proc |= PRINTH;		break;
	  case 'h':  dx[what].proc |= PARSEH | HDRVAL;	break;
	  case 'x':  dx[what].proc &= ~HDRVAL;		break;
	  case 's':  stack = argv[argi++];		break;
	  case 'i':  ifile = argv[argi++];		break;
	  case 'I':  inter = 1;				break;
		     argi++;				break;
	  case 'o':  ofile = argv[argi++]; 		break;

	  case 'v':  if ( argj == 0 ) argj = argi-1;       
		     argi++;                            break;

	  case 'F': {
			filelist cp;

		Malloc(cp, sizeof(struct _filelist));
		cp->next = NULL;
		cp->name = argv[argi++];
		cp->path = "";

		includes = ListAdd(includes, cp);
		break;
	  }

	  default:   goto done_with_args;
	}
    }
    done_with_args:

    argk = argi;


    if ( pfile ) {
	if ( (text = readfile(pfile)) == NULL ) {
	    FPrint(Stderr, "%s : reading file %s\n", progname, pfile);
	    perror(progname);
	    exit(1);
	}
    } else {
	if ( argi >= argc ) {
	  FPrint(Stderr
		  , "%s : Usage : %s [-f source | 'cmds']\n"
		  , progname, progname);
	  exit(1);
	}

	if ( dx[what].proc & CATARG ) {
	    int	len = 0;

	  for ( i = argi; i < argc; i++ )
	    len += strlen(argv[i]) + 2;

	  Malloc(text, len);
	  text[0] = '\0';
	  for ( i = argi; i < argc; i++ ) {
	    strcat(text, argv[i]);
	    strcat(text, " ");
	  }
	  argi = argc;
	} else {
	  if ( dx[what].proc & COLSEL ) {
	      while ( argi < argc - 1 ) {
		  colsel[Nsel++] = argv[argi++];
	      }
	  } 

	  /* Malloc(text, strlen(argv[argc-1])) + 2; */
	  text = argv[argc-1];
	  argc--;
	}

	if ( dx[what].proc & ONEEXP ) {
	  for ( i = 0; text[i]; i++ ) {
	    if ( text[i] == '\n' || text[i] == '\r' ) { text[i] = ' '; }
	  }
	}
    }

      if ( head || dx[what].proc & PRINTH || dx[what].proc & PARSEH ) 
	    dx[what].proc |= HEADER;

      if ( head ) {
	    if ( (tfp = Open(head, "r")) == NULL ) {
	      FPrint(Stderr, "%s: can't open header reference file: %s\n", progname, head);
	      exit(1);
	    }
      }

      if ( ifile ) {
	    if ( (ifp = Open(ifile, "r")) == NULL ) {
	      FPrint(Stderr, "%s: can't open input file: %s\n", progname, ifile);
	      exit(1);
	    }
      }
      SetBuffer(ifp, FileNullBuffer, NULL, 0);

      if ( ofile ) {
	    if ( (ofp = Open(ofile, "w")) == NULL ) {
	      FPrint(Stderr, "%s: can't open input file: %s\n", progname, ofile);
	      exit(1);
	    }
      }

      if ( dx[what].proc & HASSTK && stack == NULL && getenv("HOME") != NULL ) {
	    stack = stkval;
	    sprintf(stkval, "%s/.calc", getenv("HOME"));
      }

      {
		filelist fp;
		char	path[2048];
		char	*text;
		int		gotinc = 0;

	    for ( fp = fpinc; fp; fp = ListNext(fp) ) {
		if ( !strcmp(fp->name, ".table") ) {
		    strcpy(path, fp->path);
		    strcat(path, "/");
		    strcat(path, fp->name);

		    if ( text = readfile(path) )
			copytext(prep, text);
		}
		if ( !gotinc ) {
		    if ( !strcmp(fp->name, "table.inc") ) {
			strcpy(path, fp->path);
			strcat(path, "/");
			strcat(path, fp->name);

			if ( text = readfile(path) ) {
			    copytext(prep, text);
			    gotinc = 1;
			}
		    }
		}
	    }
      }


      /* Output the initial BEGIN clause to set thing up
       */
      copytext(prep, "BEGIN {\n"	);
	
      /* Read in Header, Headline/Wideline
       */
      if ( dx[what].proc & HEADER ) {
	if ( tfp == NULL ) tfp = ifp;

	if ( (table = T = table_header(tfp, 0)) == NULL ) {
	  FPrint(Stderr, "%s : can't read table header from input\n", progname);
	  exit(1);
	}
	if ( T && !(T->mode & TABLE_NICETAB) )
	  copytext(prep, "\tFS=OFS=\"\t\"\n");

	copytext(prep, "\tHeader=\""	);
	for ( i = 1; i < table->header->ncol; i++ ) {
	  copytext(prep, table->header->column[i]);
	  copytext(prep, "\t");
	}
	copytext(prep, table->header->column[i]);
	copytext(prep, "\"\n");
	copytext(prep, "\tDashes=\""	);
	for ( i = 1; i < table->dashes->ncol; i++ ) {
	  copytext(prep, table->dashes->column[i]);
	  copytext(prep, "\t");
	}
	copytext(prep, table->dashes->column[i]);
	copytext(prep, "\"\n");

	if ( table->name ) {
	  copytext(prep, "\tTableName=\"");
	  copytext(prep, table->name);
	  copytext(prep, "\"\n\n");
	}

	  copytext(prep, "\tNValues=");
	  sprintf(buffer, "%d\n\n", table->nvalue);
	  copytext(prep, buffer);


	  if ( dx[what].proc & HDRVAL ) {
	    /* Set up the header values
	     */
	    for ( i = 0; i < table->nvalue; i++ ) {
	     if ( table->values[i]->column[1][0] != '\0' ) {
		    char *colid = table->values[i]->column[1];

		       if ( *colid == '#' ) colid++;
		    while ( *colid == ' ' ) colid++;

	      if ( table->values[i]->ncol >= 2 
		&& table_isid(colid) ) {

		copytext(prep, "\t");
		copytext(prep, "N_");
		copytext(prep, colid);
		copytext(prep, " = split(\"");
		for ( j = 2; j < table->values[i]->ncol; j++ ) {
		    copytext(prep, table->values[i]->column[j]);
		    if ( table->values[i]->ncol > 2 ) copytext(prep, "\t");
		}
		copytext(prep, table->values[i]->column[j]);
		copytext(prep, "\", ");
		
		copytext(prep, colid);
		copytext(prep, ")\n");
	      }
	     }
	    }
	  }
      }

      copytext(prep, "}\n");


      /* disable the $column text replacement.
       */
      if ( !(dx[what].proc & PARSEH) ) {
	    table = NULL;
      }

      /* Process awk program text
       */
      if ( dx[what].prec ) copytext(program, dx[what].prec);
      parse(text);
      if ( dx[what].post ) copytext(program, dx[what].post);

      if ( !StackRef ) { stack = NULL; }

      if ( stack && !stat(stack, &statbuf) ) {
	copytext(prep, "\nBEGIN {\n");
	copytext(prep, " stack = \""); copytext(prep, stack); copytext(prep, "\"\n");
	copytext(prep, " for ( i = 1; getline v < stack > 0; i++ )\n");
	copytext(prep, "  _Stack[i] = v\n");
	copytext(prep, " _Stack[0] = i;\n");
	copytext(prep, " i = \"\"; v = \"\"; stack = \"\"\n");
	copytext(prep, "}\n");
      }

      if ( dx[what].proc & ARRINP >= 2 && arrylist ) {
	    Arrylist	*al;

	  copytext(prep, "{\n");
	  for ( al = arrylist; al; al = al->next ) {
		    char	cnumber[32];
	    copytext(prep, "\tN_"); copytext(prep, al->colnam);
	    copytext(prep, " = split($");
	    sprintf(cnumber, "%d", al->column);
	    copytext(prep, cnumber); 
	    copytext(prep, ", ");
	    copytext(prep, al->colnam);
	    copytext(prep, ", ARRDEL)\n");
	  }
	  copytext(prep, "\n}\n");
      }

      if ( T && T->mode & TABLE_NICETAB ) {
	    copytext(prep, "\n/^[ 	]*$/ {\texit\t}\n");
      }

      copytext(prep, program->buff);
      program = prep;


  if ( dx[what].proc & ARROUT && arrylist ) {
	Arrylist	*al;
	int		 output = 0;


      for ( al = arrylist; al; al = al->next ) {
	if ( al->colnum > 0 ) {
		    char	cnumber[32];

      	    if ( !output ) copytext(prep, "{\n");
	    output = 1;

	    copytext(prep, "\t_a = ");
	    copytext(prep, al->colnam);
	    copytext(prep, "[1]; for ( _i = 2; _i <= N_");
	    copytext(prep, al->colnam);
	    copytext(prep, "; _i++ ) _a = _a ARRDEL ");
	    copytext(prep, al->colnam);
	    copytext(prep, "[_i]; $");
	    sprintf(cnumber, "%d", al->column);
	    copytext(prep, cnumber); 
	    copytext(prep, " = _a\n");
	}
      }
      if ( output ) copytext(prep, "\n}");
  }
  if ( dx[what].proc & COLSEL ) {
	copytext(prep, " { ");
	copytext(prep, "print ");

	for ( i = 0; i < Nins; i++ ) {
	    copytext(prep, colins[i]);
	    copytext(prep, ", ");
	}

	if ( Nsel ) {
	    for ( i = 0; i < Nsel; i++ ) {
		int col = table_colnum(T, colsel[i]);

		if ( col ) {
		    char name[256];

		    sprintf(name, "$%d", col);
		    copytext(prep, name);
		    if ( i < Nsel-1 ) copytext(prep, ", ");
		} else {
		    copytext(prep, colsel[i]);
		    if ( i < Nsel-1 ) copytext(prep, ", ");

		    table_coladd(T, colsel[i]);
		} 
	    }
	} else {
	    copytext(prep, "$0");
	}
	for ( i = 0; i < Napp; i++ ) {
	    copytext(prep, ", ");
	    copytext(prep, colapp[i]);
	}
	copytext(prep, " }\n");
  }

  /* Process any included functions
   */
  table = NULL;		/* This disables the $column text
			 * replacement in the parser	*/
  include();


  if ( stack && !stat(stack, &statbuf) ) {
    copytext(prep, "\nBEGIN {\n");
    copytext(prep, " stack = \""); copytext(program, stack); copytext(program, "\"\n");
    copytext(prep, " print _ANS > stack\n");
    copytext(prep, " _Stack[0]--\n");
    copytext(prep, " for ( i = 1; i < _Stack[0]; i++ )\n");
    copytext(prep, "  print _Stack[i] > stack\n");
    copytext(prep, "}\n");
  }

  /* Construct awk command
   */
  (talk = getenv(TABLEAWK)) == NULL && (talk = "tawk");

  j = 0;
  argx[j++]	= dx[what].name;

  if ( T && T->mode & TABLE_NICETAB ) {
      sprintf(ncolstr, "%d", T->header->ncol);
      argx[j++]     = "-C";
      argx[j++]     = ncolstr;
  }

  if ( inter ) {
      argx[j++]     = "-W";
      argx[j++]     = "interactive";
  }
  if ( arrylist ) {
      argx[j++]	    = "-v";
      argx[j++]	    = "ARRDEL= ";
  }


  if ( argj ) {
      for ( i = argj; i < argk; i++ ) 
	    if ( argv[i][0] ) argx[j++] = argv[i];
  }

  progarg = j;
  argx[j++]	= prep->buff;

  for ( i = argi; i < argc; i++ ) 
	if ( argv[i][0] ) argx[j++] = argv[i];
  argx[j] = NULL;

  if ( debug ) {
    FPrint(Stderr, "#!/bin/sh\n#\n\n mawk ");
    for ( i = 0; i < j; i++ ) {
      if ( i == progarg ) FPrint(Stderr, "'\n");
      FPrint(Stderr, "%s ", argx[i]);
      if ( i == progarg ) FPrint(Stderr, "'\n");
    }
  }

  if ( dx[what].proc & PRINTH ) {
	int	 Ncol   = table_ncol(T);
	int	*select = malloc((Nins + Napp + Ncol + 1) * sizeof(int *));

    i = 0;
    for ( j = 0; j <  Nins; j++ ) {
	int col = table_colnum(T, colins[j]);

	if ( col ) select[++i] = col;
	else       select[++i] = table_coladd(T, colins[j]);
    }
    if ( Nsel ) {
	for ( j = 0; j <  Nsel; j++ ) { select[++i] = table_colnum(T, colsel[j]); }
    } else {
	for ( j = 1; j <= Ncol; j++ ) { select[++i] = j; }
    }
    for ( j = 0; j <  Napp; j++ ) {
	int col = table_colnum(T, colapp[j]);

	if ( col ) select[++i] = col;
	else       select[++i] = table_coladd(T, colapp[j]);
    }

    table_hdrput(ofp, T, 0, NULL, select, i);
    Flush(ofp);
  }

  dup2(FileFd(ifp), 0);
  dup2(FileFd(ofp), 1);
  execvp(talk, argx);
  perror(progname);
  fprintf(stderr, "error trying to exec : %s\n", talk);
  exit(1);
}



filelist function(yytext, yyleng)
			char	*yytext;
			int	 yyleng;
{
		filelist	fp;


	for ( fp = fptab; fp; fp = ListNext(fp) ) {
	    if ( strlen(fp->name) == yyleng
	     && !strncmp(yytext, fp->name, yyleng) ) break;
	}

	if ( fp != NULL && !fp->mark ) {
		fp->mark = 1;
		return fp;
	} else	return NULL;
}

void include()
{
	char            funct[1024]; char            *text;

	for ( ; includes; includes = ListPop(includes) ) {
		funct[0] = '\0';

		if ( includes->path[0] ) {
		    strcpy(funct, includes->path);
		    strcat(funct, "/");
		}
		strcat(funct, includes->name);

		copytext(program, "\n");

		if ( text = readfile(funct) ) {
			parse(text);
			Free(text);
		} else {
			fprintf(stderr, "%s : could not read include file %s\n"
				      , progname, funct);
		}
	}
}

