
/********************************************
memory.h
copyright 1991, Michael D. Brennan

This is a source file for mawk, an implementation of
the AWK programming language.

Mawk is distributed without warranty under the terms of
the GNU General Public License, version 2, 1991.
********************************************/


/*  memory.h  */

#ifndef  MEMORY_H
#define  MEMORY_H

#include "zmalloc.h"


STRING *PROTO(new_STRING, (char*)) ;
STRING *PROTO(new_STRING0, (unsigned)) ;

#ifdef   DEBUG
void  PROTO( DB_free_STRING , (STRING *) ) ;

#define  free_STRING(s)  DB_free_STRING(s)

#else

#define  free_STRING(sval)   if ( -- (sval)->ref_cnt == 0 )\
                                zfree(sval, (sval)->len+STRING_OH) ; else
#endif


#endif   /* MEMORY_H */
