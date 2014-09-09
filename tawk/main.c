/********************************************
main.c
copyright 1991, Michael D. Brennan

This is a source file for mawk, an implementation of
the AWK programming language.

Mawk is distributed without warranty under the terms of
the GNU General Public License, version 2, 1991.
********************************************/


/*  main.c  */

#include "mawk.h"
#include "init.h"
#include "code.h"
#include "files.h"


short mawk_state ;		 /* 0 is compiling */
int exit_code ;

int
main(argc, argv)
int argc ; char **argv ;
{

   initialize(argc, argv) ;

   parse() ;

   mawk_state = EXECUTION ;
   execute(execution_start, eval_stack - 1, 0, 0) ;
   /* never returns */
   return 0 ;
}

void
mawk_exit(x)
   int x ;
{
#if  HAVE_REAL_PIPES
   close_out_pipes() ;		 /* no effect, if no out pipes */
#else
#if  HAVE_FAKE_PIPES
   close_fake_pipes() ;
#endif
#endif

   exit(x) ;
}
