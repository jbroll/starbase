/* coproctable
 */
/* Starbase Data Tables	- An Ascii Database for UNIX

   copyright 1991, 1993, 1995, 1999 John B. Roll jr.
 */
#include <xos.h>
#include <xfile.h>
#include <coproc.h>

#include "../tablelib/table.h"

#define MAXQ	100000
#define MAXC	100

#define EnQue(Que, Head, Max, Item)	Que[Head = (Head+1) % Max] = Item
#define DeQue(Que, Tail, Max)		Que[Tail = (Tail+1) % Max]
#define QEmpty(Head, Tail)		(Head == Tail)
#define QFull(Head, Tail)		(Head == Tail-1)

typedef struct _Copy {
	int	from;
	char	*name;
	int	 to;
} *Copy;

#define NCOLFLAGS	1
#define ASSIGN		0

static char *flag   =   "=";
static char *form[] = { "s" };

int main(argc, argv)
	int	 argc;
	char	*argv[];
{
	char	 columns[4096];
	char	 command[4096];
	char	 cols  [4096];
	char	*header = "";
	char	*process = NULL;

	TableHead H = NULL;
	TableRow  R = NULL;

	TableHead iH = NULL;

	void	**Que;
	int	 Q = 0;
	int	 M = MAXQ;

	int	Tail = 0;
	int	Head = 0;

	int	cop, cin, cot, cer;
	File	cfi, cfo;
	File	ifp = Stdin, ofp = Stdout;

	Polls 	polls[2];
	int	npoll = 2;

	int	n;
	int	i;

	int	hread  = 0;
	int	hwrite = 0;
	int	debug  = 0;


	struct _Copy 	copy[MAXC];
	int		coppies = 0;

      H = table_header(ifp, 0);

      for ( i = 1; i < argc ; i++ ) {
	if ( argv[i][0] == '-' ) {
	  switch ( argv[i][1] ) {
	  case 'r' : hread  = 1;			break;
	  case 'w' : hwrite = 1;			break;
	  case 'D' : debug  = 1;			break;
	  default  : {
	      FPrint(Stderr, "%s : bad option %s\n"
		      , argv[0]
		      , argv[i]);
	      exit(1);
	    }
	  }
	} else {
	 if ( process == NULL ) {
	    process = argv[i];
	 } else {
	  /* Non-switch argument	*/

	  int	 	check[NCOLFLAGS];
	  TableColArg	 value[NCOLFLAGS];

	  int	 k, j;
	  char	*col = argv[i];

	    for ( k = 0; k < NCOLFLAGS; k++ ) check[k] = 0; 

	    if ( !(j = table_colarg(H, col, flag, form, value, check)) ) {
	      FPrint(Stderr, "%s : column %s not in input table\n"
		      , argv[0]
		      , argv[i]);
	      exit(1);
	    } else {
		copy[coppies].to = j;
		if ( check[ASSIGN] ) {
		    copy[coppies].name = value[ASSIGN].s;
		} else {
		    copy[coppies].name = strdup(table_colnam(H, j));
		}

		if ( !hread ) copy[coppies].from = atoi(copy[coppies].name);
		coppies++;
	    }
	  }
	}
      }

      if ( process == NULL ) {
	FPrint(stderr, "%s : No coprocess specified\n", argv[0]);
	exit(1);
      }

      if ( debug ) {
	FPrint(stderr, "coprocess: %s\n", process);
      }

	Que = (void **) malloc(MAXQ * sizeof(void *));

	sprintf(command, "%s", process);

	cop = shellproc(command, &cin, &cot, NULL);
	cfi = OpenFd(cin, "w");
	cfo = OpenFd(cot, "r");

	polls[0].f     = cfi;
	polls[0].qmode = XFWRITE;
	polls[1].f     = cfo;
	polls[1].qmode = XFREAD;
	npoll	       = 2;

	table_hdrput(ofp, H, 0, NULL, NULL, 0);

	if ( hwrite ) table_hdrput(cfi, H, 0, NULL, NULL, 0);

	while ( (n = Poll(npoll, polls, 1.0)) >= 0 ) {
	    if ( polls[0].rmode & XFWRITE ) {
		if ( R == NULL )
	    	    R = table_rowget(ifp, H, R, NULL, NULL, 0);

		if ( R == NULL ) {
		    Close(cfi);
		    polls[0].f = NULL;
		} else {
		    table_rowput(cfi, H, R, NULL, NULL, 0);
		    EnQue(Que, Head, M, R);
		    R = NULL;
		}
	    }

	    if ( polls[1].rmode & XFREAD ) {
			TableRow  R = NULL;
			TableRow  r = NULL;

		if ( hread && iH == NULL ) {
			iH = table_header(cfo, 0);

		     for ( i = 0; i < coppies; i++ ) {
			copy[i].from = table_colnum(iH, copy[i].name);
		     }
		}

		if ( QEmpty(Head, Tail) 
		  || (R = DeQue(Que, Tail, M)) 			   == NULL
		  || (r = table_rowget(cfo, iH, r, NULL, NULL, 0)) == NULL )
		    break;
	
		for ( i = 0; i < coppies; i++ ) {
		        char *val = table_colval(r, copy[i].from);

		    if ( val ) table_colset(R, copy[i].to, val, strlen(val));
		}
		table_rowput(ofp, H, R, NULL, NULL, 0);
	    }
	}
	exit(0);
}
