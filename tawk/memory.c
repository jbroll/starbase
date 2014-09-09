
/********************************************
memory.c
copyright 1991, 1992  Michael D. Brennan

This is a source file for mawk, an implementation of
the AWK programming language.

Mawk is distributed without warranty under the terms of
the GNU General Public License, version 2, 1991.
********************************************/


/* memory.c */

#include "mawk.h"
#include "memory.h"

static STRING *PROTO(xnew_STRING, (unsigned)) ;


STRING null_str =
{0, 1, ""} ;

static STRING *
xnew_STRING(len)
   unsigned len ;
{
   STRING *sval = (STRING *) zmalloc(len + STRING_OH) ;

   sval->len = len ;
   sval->ref_cnt = 1 ;
   return sval ;
}

/* allocate space for a STRING */

STRING *
new_STRING0(len)
   unsigned len ;
{
   if (len == 0)
   {
      null_str.ref_cnt++ ;
      return &null_str ;
   }
   else
   {
      STRING *sval = xnew_STRING(len) ;
      sval->str[len] = 0 ;
      return sval ;
   }
}

/* convert char* to STRING* */

STRING *
new_STRING(s)
   char *s ;
{

   if (s[0] == 0)
   {
      null_str.ref_cnt++ ;
      return &null_str ;
   }
   else
   {
      STRING *sval = xnew_STRING(strlen(s)) ;
      strcpy(sval->str, s) ;
      return sval ;
   }
}


#ifdef	 DEBUG

void
DB_free_STRING(sval)
   register STRING *sval ;
{
   if (--sval->ref_cnt == 0)  zfree(sval, sval->len + STRING_OH) ;
}

#endif
