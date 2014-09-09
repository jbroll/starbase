
/********************************************
sizes.h
copyright 1991, 1992.  Michael D. Brennan

This is a source file for mawk, an implementation of
the AWK programming language.

Mawk is distributed without warranty under the terms of
the GNU General Public License, version 2, 1991.
********************************************/

/*  sizes.h  */

#ifndef  SIZES_H
#define  SIZES_H

#ifndef  MAX__INT
#include <limits.h>
#define  MAX__INT  INT_MAX
#define  MAX__LONG LONG_MAX
#endif   /* MAX__INT */

#if  MAX__INT <= 0x7fff
#define  SHORT_INTS
#define  INT_FMT "%ld"
typedef  long Int ;
#define  Max_Int MAX__LONG
#else
#define  INT_FMT "%d"
typedef  int Int ;
#define  Max_Int  MAX__INT
#endif

#define EVAL_STACK_SIZE  256  /* initial size , can grow */
/* number of fields at startup, must be a power of 2 
   and FBANK_SZ-1 must be divisible by 3! */
#define  FBANK_SZ	256
#define  FB_SHIFT	  8   /* lg(FBANK_SZ) */
#define  NUM_FBANK	128   /* see MAX_FIELD below */


#define  MAX_SPLIT	(FBANK_SZ-1)   /* needs to be divisble by 3*/
#define  MAX_FIELD	(NUM_FBANK*FBANK_SZ - 1)

#define  MIN_SPRINTF	32768*4


#define  BUFFSZ         4096
  /* starting buffer size for input files, grows if 
     necessary */

#define  HASH_PRIME  53
#define  A_HASH_PRIME 199

#define  MAX_COMPILE_ERRORS  5 /* quit if more than 4 errors */

#endif   /* SIZES_H */
