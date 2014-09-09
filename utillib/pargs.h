/* pargs.h
**/

#define EOARGV	0


typedef struct _ArgStream {
    int             ungotten;
    char            unchar;
    char            sep;
    char           *here;
    int             this;
    char          **argv;
    int             argc;
}              *ArgStream;

#define ArgvChar(a)	( a->ungotten != 0 ? (a->ungotten = 0), a->unchar : \
			( ( a->this >= a->argc ) ? 0 :			    \
				( *a->here ? *a->here++ : 		    \
				   ((a->here = a->argv[++a->this]), a->sep ))))

#define ArgvUnChar(a, ch) ( a->unchar = ch, a->ungotten = 1 )

#define ArgvSep(a, ch)	a->sep = ch

ArgStream       SAOOpenArgv();

