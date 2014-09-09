
/********************************************
cast.c
copyright 1991, Michael D. Brennan

This is a source file for mawk, an implementation of
the AWK programming language.

Mawk is distributed without warranty under the terms of
the GNU General Public License, version 2, 1991.
********************************************/

/*  cast.c  */

#include "mawk.h"
#include "field.h"
#include "memory.h"
#include "scan.h"
#include "repl.h"

#include <math.h>
#ifdef	USE_IEEEFP_H
#include <ieeefp.h>
#endif


double xstrtod(str)
	char	*str;
{
	char	*ptr;

	double value = SAOstrtod(str, &ptr);

    return value;
}

int mpow2[NUM_CELL_TYPES] =
{1, 2, 4, 8, 16, 32, 64, 128, 256, 512} ;


void
cast1_to_d(cp)
   register CELL *cp ;
{
   SAOdtype = 0;

   switch (cp->type)
   {
      case C_NOINIT:
	 cp->dval = 0.0 ;
	 break ;

      case C_DOUBLE:
	 return ;

      case C_MBSTRN:
      case C_STRING:
	 {
	    register STRING *s = (STRING *) cp->ptr ;

	    cp->dval = xstrtod(s->str) ;
	    cp->vcnt = SAOdtype;
#if FPE_TRAPS_ON		/* look for overflow error */
	    errno = 0 ;
	     if (errno && cp->dval != 0.0)	/* ignore underflow */
	       rt_error("overflow converting %s to double", s->str) ;
#endif
	    free_STRING(s) ;

	 }
	 break ;

      case C_STRNUM:
	 /* don't need to convert, but do need to free the STRING part */
	 free_STRING(string(cp)) ;
	 break ;

      default:
	 bozo("cast on bad type") ;
   }
   cp->type = C_DOUBLE ;
   cp->vcnt = SAOdtype;
}

void
cast2_to_d(cp)
   register CELL *cp ;
{
   register STRING *s ;

   switch (cp->type)
   {
      case C_NOINIT:
	 cp->dval = 0.0 ;
	 break ;

      case C_DOUBLE:
	 goto two ;
      case C_STRNUM:
	 free_STRING(string(cp)) ;
	 break ;

      case C_MBSTRN:
      case C_STRING:
	 s = (STRING *) cp->ptr ;

	 cp->dval = xstrtod(s->str) ;
	 cp->vcnt = SAOdtype;
#if FPE_TRAPS_ON		/* look for overflow error */
	 errno = 0 ;
	 if (errno && cp->dval != 0.0)	/* ignore underflow */
	    rt_error("overflow converting %s to double", s->str) ;
#endif
	 free_STRING(s) ;
	 break ;

      default:
	 bozo("cast on bad type") ;
   }
   cp->type = C_DOUBLE ;
   cp->vcnt = SAOdtype;

 two:cp++ ;

   switch (cp->type)
   {
      case C_NOINIT:
	 cp->dval = 0.0 ;
	 break ;

      case C_DOUBLE:
	 return ;
      case C_STRNUM:
	 free_STRING(string(cp)) ;
	 break ;

      case C_MBSTRN:
      case C_STRING:
	 s = (STRING *) cp->ptr ;

#if FPE_TRAPS_ON		/* look for overflow error */
	 errno = 0 ;
	 cp->dval = xstrtod(s->str) ;
	 cp->vcnt = SAOdtype;
	 if (errno && cp->dval != 0.0)	/* ignore underflow */
	    rt_error("overflow converting %s to double", s->str) ;
#else
	 cp->dval = xstrtod(s->str) ;
	 cp->vcnt = SAOdtype;
#endif
	 free_STRING(s) ;
	 break ;

      default:
	 bozo("cast on bad type") ;
   }
   cp->type = C_DOUBLE ;
   cp->vcnt = SAOdtype;
}

void
cast1_to_s(cp)
   register CELL *cp ;
{
   register Int ival ;
   char xbuff[260] ;

   switch (cp->type)
   {
      case C_NOINIT:
	 null_str.ref_cnt++ ;
	 cp->ptr = (PTR) & null_str ;
	 break ;

      case C_DOUBLE:

	 if ( cp->vcnt ) SAOconvert(xbuff, cp->dval, cp->vcnt & ~0x80
				, 0,  CONVFMT->vcnt, cp->vcnt &  0x80);
	 else {
		 ival = d_to_I(cp->dval) ;
		 if (ival == cp->dval)	sprintf(xbuff, INT_FMT, ival) ;
		 else  sprintf(xbuff, string(CONVFMT)->str, cp->dval) ;
	 }

	 cp->ptr = (PTR) new_STRING(xbuff) ;
	 break ;

      case C_STRING:
	 return ;

      case C_MBSTRN:
      case C_STRNUM:
	 break ;

      default:
	 bozo("bad type on cast") ;
   }

   cp->type = C_STRING ;
}

void
cast2_to_s(cp)
   register CELL *cp ;
{
   register Int ival ;
   char xbuff[260] ;

   switch (cp->type)
   {
      case C_NOINIT:
	 null_str.ref_cnt++ ;
	 cp->ptr = (PTR) & null_str ;
	 break ;

      case C_DOUBLE:

	 if ( cp->vcnt ) SAOconvert(xbuff, cp->dval
				, cp->vcnt, 0, CONVFMT->vcnt, 0);
	 else {
		 ival = d_to_I(cp->dval) ;
		 if (ival == cp->dval)	sprintf(xbuff, INT_FMT, ival) ;
		 else  sprintf(xbuff, string(CONVFMT)->str, cp->dval) ;
	 }

	 cp->ptr = (PTR) new_STRING(xbuff) ;
	 break ;

      case C_STRING:
	 goto two ;

      case C_MBSTRN:
      case C_STRNUM:
	 break ;

      default:
	 bozo("bad type on cast") ;
   }
   cp->type = C_STRING ;

two:
   cp++ ;

   switch (cp->type)
   {
      case C_NOINIT:
	 null_str.ref_cnt++ ;
	 cp->ptr = (PTR) & null_str ;
	 break ;

      case C_DOUBLE:

	 if ( cp->vcnt ) SAOconvert(xbuff, cp->dval, cp->vcnt
					, 0, CONVFMT->vcnt, 0);
	 else {
		 ival = d_to_I(cp->dval) ;
		 if (ival == cp->dval)	sprintf(xbuff, INT_FMT, ival) ;
		 else  sprintf(xbuff, string(CONVFMT)->str, cp->dval) ;
	 }

	 cp->ptr = (PTR) new_STRING(xbuff) ;
	 break ;

      case C_STRING:
	 return ;

      case C_MBSTRN:
      case C_STRNUM:
	 break ;

      default:
	 bozo("bad type on cast") ;
   }
   cp->type = C_STRING ;
}

void
cast_to_RE(cp)
   register CELL *cp ;
{
   register PTR p ;

   if (cp->type < C_STRING)  cast1_to_s(cp) ;

   p = re_compile(string(cp)) ;
   free_STRING(string(cp)) ;
   cp->type = C_RE ;
   cp->ptr = p ;

}

void
cast_for_split(cp)
   register CELL *cp ;
{
   static char meta[] = "^$.*+?|[]()" ;
   static char xbuff[] = "\\X" ;
   int c ;
   unsigned len ;

   if (cp->type < C_STRING)  cast1_to_s(cp) ;

   if ((len = string(cp)->len) == 1)
   {
      if ((c = string(cp)->str[0]) == ' ')
      {
	 free_STRING(string(cp)) ;
	 cp->type = C_SPACE ;
	 return ;
      }
      else if (strchr(meta, c))
      {
	 xbuff[1] = c ;
	 free_STRING(string(cp)) ;
	 cp->ptr = (PTR) new_STRING(xbuff) ;
      }
   }
   else if (len == 0)
   {
      free_STRING(string(cp)) ;
      cp->type = C_SNULL ;
      return ;
   }

   cast_to_RE(cp) ;
}

/* input: cp-> a CELL of type C_MBSTRN (maybe strnum)
   test it -- casting it to the appropriate type
   which is C_STRING or C_STRNUM
*/

void
check_strnum(cp)
   CELL *cp ;
{
   char *test ;
   register unsigned char *s, *q ;

   cp->type = C_STRING ;	 /* assume not C_STRNUM */
   s = (unsigned char *) string(cp)->str ;
   q = s + string(cp)->len ;
   while (scan_code[*s] == SC_SPACE)  s++ ;
   if (s == q)	return ;

   while (scan_code[q[-1]] == SC_SPACE)	 q-- ;
   if (scan_code[q[-1]] != SC_DIGIT &&
       q[-1] != '.')
      return ;

   switch (scan_code[*s])
   {
	extern int	Trap;
      case SC_DIGIT:
      case SC_PLUS:
      case SC_MINUS:
      case SC_DOT:

#if FPE_TRAPS_ON
	 errno = 0 ;
Trap = 0;
	 cp->dval = SAOstrtod((char *) s, &test) ;
	 cp->vcnt = SAOdtype;
Trap = 1;
#else
	 cp->dval = SAOstrtod((char *) s, &test) ;
	 cp->vcnt = SAOdtype;
#endif

	 if ((char *) q <= test)  cp->type = C_STRNUM ;
	 /*  <= instead of == , for some buggy strtod
		 e.g. Apple Unix */
   }
}

/* cast a CELL to a replacement cell */

void
cast_to_REPL(cp)
   register CELL *cp ;
{
   register STRING *sval ;

   if (cp->type < C_STRING)  cast1_to_s(cp) ;
   sval = (STRING *) cp->ptr ;

   cellcpy(cp, repl_compile(sval)) ;
   free_STRING(sval) ;
}


/* convert a double to Int (this is not as simple as a
   cast because the results are undefined if it won't fit).
   Truncate large values to +Max_Int or -Max_Int
   Send nans to -Max_Int
*/

Int
d_to_I(d)
   double d;
{
   if (d >= Max_Int)	return Max_Int ;
   if (d > -Max_Int)	return (Int) d ;
   return -Max_Int ;
}
