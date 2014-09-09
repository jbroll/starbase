
/********************************************
vargs.h
copyright 1992 Michael D. Brennan

This is a source file for mawk, an implementation of
the AWK programming language.

Mawk is distributed without warranty under the terms of
the GNU General Public License, version 2, 1991.
********************************************/

/* provides common interface to <stdarg.h> or <varargs.h> 
   only used for error messages
*/

#ifdef     NO_PROTOS
#ifndef    NO_STDARG_H   
#define    NO_STDARG_H  1
#endif
#endif

#if     NO_STDARG_H
#include <varargs.h>

#ifndef  VA_ALIST

#define  VA_ALIST(type, arg)  (va_alist) va_dcl { type arg ;
#define  VA_ALIST2(t1,a1,t2,a2) (va_alist) va_dcl { t1 a1 ; t2 a2 ;

#endif

#define  VA_START(p,type, last)  va_start(p) ;\
                                 last = va_arg(p,type)


#define  VA_START2(p,t1,a1,t2,a2)  va_start(p) ;\
                                  a1 = va_arg(p,t1);\
                                  a2 = va_arg(p,t2)

#else  /* have stdarg.h */
#include <stdarg.h>

#ifndef  VA_ALIST
#define  VA_ALIST(type, arg)  (type arg, ...) {
#define  VA_ALIST2(t1,a1,t2,a2)  (t1 a1,t2 a2,...) {
#endif

#define  VA_START(p,type,last)   va_start(p,last)

#define  VA_START2(p,t1,a1,t2,a2)  va_start(p,a2)

#endif

