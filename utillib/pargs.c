/* pargs.c
 */
/* copyright 1991, 1993, 1995, 1999 John B. Roll jr.
 */


#include "xos.h"
#include "xfile.h"
#include "pargs.h"


ArgStream       OpenArgv(nth, separator, argv, argc)
    int             nth;
    char            separator;
    char           *argv[];
    int             argc;
{
    ArgStream       a;

    Malloc(a, sizeof(struct _ArgStream));

    a->ungotten = 0;

    a->here = argv[nth];
    a->this = nth;
    a->argv = argv;
    a->argc = argc;
    a->sep = separator;

    return a;
}


void            CloseArgv(a)
    ArgStream       a;
{
    Free(a);
}
