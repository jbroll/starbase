
/********************************************
bi_funct.h
copyright 1991, Michael D. Brennan

This is a source file for mawk, an implementation of
the AWK programming language.

Mawk is distributed without warranty under the terms of
the GNU General Public License, version 2, 1991.
********************************************/


#ifndef  BI_FUNCT_H
#define  BI_FUNCT_H  1

#include "symtype.h"

extern BI_REC  bi_funct[] ;

void PROTO(bi_init, (void) ) ;

/* builtin string functions */
CELL *PROTO( bi_print, (CELL *) ) ;
CELL *PROTO( bi_printf, (CELL *) ) ;
CELL *PROTO( bi_length, (CELL *) ) ;
CELL *PROTO( bi_index, (CELL *) ) ;
CELL *PROTO( bi_substr, (CELL *) ) ;
CELL *PROTO( bi_sprintf, (CELL *) ) ;
CELL *PROTO( bi_split, (CELL *) ) ;
CELL *PROTO( bi_match, (CELL *) ) ;
CELL *PROTO( bi_getline, (CELL *) ) ;
CELL *PROTO( bi_sub, (CELL *) ) ;
CELL *PROTO( bi_gsub, (CELL *) ) ;
CELL *PROTO( bi_toupper, (CELL*) ) ;
CELL *PROTO( bi_tolower, (CELL*) ) ;

/* builtin arith functions */
CELL *PROTO( bi_sin, (CELL *) ) ;
CELL *PROTO( bi_cos, (CELL *) ) ;
CELL *PROTO( bi_atan2, (CELL *) ) ;
CELL *PROTO( bi_log, (CELL *) ) ;
/* CELL *PROTO( bi_log2, (CELL *) ) ; */
CELL *PROTO( bi_log10, (CELL *) ) ;
CELL *PROTO( bi_exp, (CELL *) ) ;
CELL *PROTO( bi_int, (CELL *) ) ;
CELL *PROTO( bi_sqrt, (CELL *) ) ;
CELL *PROTO( bi_srand, (CELL *) ) ;
CELL *PROTO( bi_rand, (CELL *) ) ;

/* other builtins */
CELL *PROTO( bi_close, (CELL *) ) ;
CELL *PROTO( bi_system, (CELL *) ) ;
CELL *PROTO( bi_fflush, (CELL *) ) ;

CELL *PROTO( bi_asin, (CELL *) ) ;
CELL *PROTO( bi_acos, (CELL *) ) ;
CELL *PROTO( bi_atan, (CELL *) ) ;
CELL *PROTO( bi_tan, (CELL *) ) ;
CELL *PROTO( bi_abs, (CELL *) ) ;
CELL *PROTO( bi_min, (CELL *) ) ;
CELL *PROTO( bi_max, (CELL *) ) ;
CELL *PROTO( bi_sleep, (CELL *) ) ;

#endif  /* BI_FUNCT_H  */

