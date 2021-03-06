
/********************************************
print.c
copyright 1991-1993.  Michael D. Brennan

This is a source file for mawk, an implementation of
the AWK programming language.

Mawk is distributed without warranty under the terms of
the GNU General Public License, version 2, 1991.
********************************************/


#include "mawk.h"
#include "bi_vars.h"
#include "bi_funct.h"
#include "memory.h"
#include "field.h"
#include "scan.h"
#include "files.h"

static void PROTO(print_cell, (CELL *, FILE *)) ;
static STRING *PROTO(do_printf, (FILE *, char *, unsigned, CELL *)) ;
static void PROTO(bad_conversion, (int, char *, char *)) ;
static void PROTO(write_error,(void)) ;

/* this can be moved and enlarged  by -W sprintf=num  */
char *sprintf_buff = string_buff ;
char *sprintf_limit = string_buff + SPRINTF_SZ ;

/* Once execute() starts the sprintf code is (belatedly) the only
   code allowed to use string_buff  */

static void
print_cell(p, fp)
   register CELL *p ;
   register FILE *fp ;
{
   int len ;

   switch (p->type)
   {
      case C_NOINIT:
	 break ;
      case C_MBSTRN:
      case C_STRING:
      case C_STRNUM:
	 switch (len = string(p)->len)
	 {
	    case 0:
	       break ;
	    case 1:
	       putc(string(p)->str[0], fp) ;
	       break ;

	    default:
	       fwrite(string(p)->str, 1, len, fp) ;
	 }
	 break ;

      case C_DOUBLE:
	 {
	    Int ival = d_to_I(p->dval) ;

         if ( p->vcnt ) {
                char    buff[256];

                SAOconvert(buff, p->dval, p->vcnt & ~0x80, 0, OFMT->vcnt, p->vcnt & 0x80) ;
                fprintf(fp, "%s", buff);
                break;
         }

	    /* integers print as "%[l]d" */
	    if ((double) ival == p->dval)  fprintf(fp, INT_FMT, ival) ;
	    else  fprintf(fp, string(OFMT)->str, p->dval) ;
	 }
	 break ;

      default:
	 bozo("bad cell passed to print_cell") ;
   }
}

/* on entry to bi_print or bi_printf the stack is:

   sp[0] = an integer k
       if ( k < 0 )  output is to a file with name in sp[-1]
       { so open file and sp -= 2 }

   sp[0] = k >= 0 is the number of print args
   sp[-k]   holds the first argument
*/

CELL *
bi_print(sp)
   CELL *sp ;			 /* stack ptr passed in */
{
   register CELL *p ;
   register int k ;
   FILE *fp ;

   k = sp->type ;
   if (k < 0)
   {
      /* k holds redirection */
      if ((--sp)->type < C_STRING)  cast1_to_s(sp) ;
      fp = (FILE *) file_find(string(sp), k) ;
      free_STRING(string(sp)) ;
      k = (--sp)->type ;
      /* k now has number of arguments */
   }
   else	 fp = stdout ;

   if (k)
   {
      p = sp - k ;		 /* clear k variables off the stack */
      sp = p - 1 ;
      k-- ;

      while (k > 0)
      {
	 print_cell(p,fp) ; print_cell(OFS,fp) ;
	 cell_destroy(p) ;
	 p++ ; k-- ;
      }

      print_cell(p, fp) ;  cell_destroy(p) ;
   }
   else
   {				/* print $0 */
      sp-- ;
      print_cell(&field[0], fp) ;
   }

   print_cell(ORS, fp) ;
   if (ferror(fp)) write_error() ;
   return sp ;
}

/*---------- types and defs for doing printf and sprintf----*/
#define	 PF_C		0	/* %c */
#define	 PF_S		1	/* %s */
#define	 PF_D		2	/* int conversion */
#define	 PF_U		3	/* int conversion */
#define	 PF_F		4	/* float conversion */
#define  PF_AT          5  /* float conversion to sexagasmal */

/* for switch on number of '*' and type */
#define	 AST(num,type)	(256*(num)+(type))

/* some picky ANSI compilers go berserk without this */
#ifdef NO_PROTOS
typedef int (*PRINTER) () ;
#else
typedef int (*PRINTER) (PTR, const char *,...) ;
#endif

/*-------------------------------------------------------*/

static void
bad_conversion(cnt, who, format)
   int cnt ;
   char *who, *format ;
{
   rt_error("improper conversion(number %d) in %s(\"%s\")",
	    cnt, who, format) ;
}

/* the contents of format are preserved,
   caller does CELL cleanup

   This routine does both printf and sprintf (if fp==0)
*/
static STRING *
do_printf(fp, format, argcnt, cp)
   FILE *fp ;
   char *format ;
   unsigned argcnt ;		 /* number of args on eval stack */
   CELL *cp ;			 /* ptr to an array of arguments 
				    (on the eval stack) */
{
   char save ;
   char *p ;
   register char *q = format ;
   register char *target ;
   int l_flag, h_flag ;		 /* seen %ld or %hd  */
   int ast_cnt ;
   int ast[2] ;
   Int Ival ;
   unsigned int Uval ;
   int num_conversion = 0 ;	 /* for error messages */
   char *who ;			 /*ditto*/
   int pf_type ;		 /* conversion type */
   PRINTER printer ;		 /* pts at fprintf() or sprintf() */

   int prec  = -1;
   int width =  0;

   int flags = 0;

#ifdef	 SHORT_INTS
   char xbuff[256] ;		 /* splice in l qualifier here */
#endif

   if (fp == (FILE *) 0)	/* doing sprintf */
   {
      target = sprintf_buff ;
      printer = (PRINTER) sprintf ;
      who = "sprintf" ;
   }
   else	 /* doing printf */
   {
      target = (char *) fp ;	 /* will never change */
      printer = (PRINTER) fprintf ;
      who = "printf" ;
   }

   while (1)
   {
      if (fp)			/* printf */
      {
	 while (*q != '%') {
	    if (*q == 0)  {
	       if (ferror(fp)) write_error() ;
	       /* return is ignored */
	       return (STRING *) 0 ;
	    } else  { putc(*q,fp) ; q++ ; }
	 }
      }
      else  /* sprintf */
      {
	 while (*q != '%')
	    if (*q == 0)
	    {
	       if (target > sprintf_limit)	/* damaged */
	       {
		  /* hope this works */
		  rt_overflow("sprintf buffer",
			      sprintf_limit - sprintf_buff) ;
	       }
	       else  /* really done */
	       {
		  STRING *retval ;
		  int len = target - sprintf_buff ;

		  retval = new_STRING0(len) ;
		  memcpy(retval->str, sprintf_buff, len) ;
		  return retval ;
	       }
	    }
	    else  *target++ = *q++ ;
      }


      /* *q == '%' */
      num_conversion++ ;

      if (*++q == '%')		/* %% */
      {
	 if (fp)  putc(*q, fp) ;
	 else  *target++ = *q ;

	 q++ ; continue ;
      }

      /* mark the '%' with p */
      p = q - 1 ;

      /* eat the flags */
      while (*q == '-' || *q == '+' || *q == ' ' ||
	     *q == '#' || *q == '0') {
	switch ( *q ) {
	    case '-': flags |=  2;	break;
	    case '+': flags |=  4;	break;
	    case ' ': flags |=  8;	break;
	    case '#': flags |= 16;	break;
	    case '0': flags |= 32;	break;
	}
 
	 q++ ;
      }

      ast_cnt = 0 ;
      if (*q == '*')
      {
	 if (cp->type != C_DOUBLE)  cast1_to_d(cp) ;
	 ast[ast_cnt++] = d_to_i(cp++->dval) ;
	 width = ast[ast_cnt-1];
	 argcnt-- ; q++ ;
      }
      else {
	 width = atoi(q);
	 while (scan_code[*(unsigned char *) q] == SC_DIGIT)  q++ ;
      }
      /* width is done */

      if (*q == '.')		/* have precision */
      {
	 q++ ;
	 if (*q == '*')
	 {
	    if (cp->type != C_DOUBLE)  cast1_to_d(cp) ;
	    ast[ast_cnt++] = d_to_i(cp++->dval) ;
	    prec = ast[ast_cnt-1];
	    argcnt-- ; q++ ;
	 }
	 else {
	    prec = atoi(q);
	    while (scan_code[*(unsigned char *) q] == SC_DIGIT)	 q++ ;
	 }
      }

      if (argcnt <= 0)
	 rt_error("not enough arguments passed to %s(\"%s\")",
		  who, format) ;

      l_flag = h_flag = 0 ;

      if (*q == 'l')  { q++ ; l_flag = 1 ; }
      else if (*q == 'h')  { q++ ; h_flag = 1 ; }
      switch (*q++)
      {
         case '@' :
            if ( l_flag )
                bad_conversion(num_conversion,who,format) ;
            if ( cp->type != C_DOUBLE ) cast1_to_d(cp) ;
            pf_type = PF_AT ;
            break;

	 case 's':
	    if (l_flag + h_flag)
	       bad_conversion(num_conversion, who, format) ;
	    if (cp->type < C_STRING)  cast1_to_s(cp) ;
	    pf_type = PF_S ;
	    break ;

	 case 'c':
	    if (l_flag + h_flag)
	       bad_conversion(num_conversion, who, format) ;

	    switch (cp->type)
	    {
	       case C_NOINIT:
		  Ival = 0 ;
		  break ;

	       case C_STRNUM:
	       case C_DOUBLE:
		  Ival =  d_to_I(cp->dval) ;
		  break ;

	       case C_STRING:
		  Ival = string(cp)->str[0] ;
		  break ;

	       case C_MBSTRN:
		  check_strnum(cp) ;
		  Ival = cp->type == C_STRING ?
		     string(cp)->str[0] : d_to_I(cp->dval) ;
		  break ;

	       default:
		  bozo("printf %c") ;
	    }

	    pf_type = PF_C ;
	    break ;

	 case 'd':
	 case 'i':
	    if (cp->type != C_DOUBLE)  cast1_to_d(cp) ;
	    Ival = d_to_I(cp->dval) ;
	    pf_type = PF_D ;
	    break ;

	 case 'o':
	 case 'x':
	 case 'X':
	 case 'u':
	    if (cp->type != C_DOUBLE)  cast1_to_d(cp) ;
	    Uval = (unsigned) cp->dval;
	    pf_type = PF_U ;
	    break ;

	 case 'e':
	 case 'g':
	 case 'f':
	 case 'E':
	 case 'G':
	    if (h_flag + l_flag)
	       bad_conversion(num_conversion, who, format) ;
	    if (cp->type != C_DOUBLE)  cast1_to_d(cp) ;
	    pf_type = PF_F ;
	    break ;

	 default:
	    bad_conversion(num_conversion, who, format) ;
      }

      save = *q ;
      *q = 0 ;

#ifdef	SHORT_INTS
      if (pf_type == PF_D)
      {
	 /* need to splice in long modifier */
	 strcpy(xbuff, p) ;

	 if (l_flag) /* do nothing */ ;
	 else
	 {
	    int k = q - p ;

	    if (h_flag)
	    {
	       Ival = (short) Ival ;
	       /* replace the 'h' with 'l' (really!) */
	       xbuff[k - 2] = 'l' ;
	       if (xbuff[k - 1] != 'd' && xbuff[k - 1] != 'i')
		  Ival &= 0xffff ;
	    }
	    else
	    {
	       /* the usual case */
	       xbuff[k] = xbuff[k - 1] ;
	       xbuff[k - 1] = 'l' ;
	       xbuff[k + 1] = 0 ;
	    }
	 }
      }
#endif

      /* ready to call printf() */
      switch (AST(ast_cnt, pf_type))
      {
	 case AST(0, PF_AT):
	 case AST(1, PF_AT):
	 case AST(2, PF_AT):

	 {
		char	buff[32];
		int	type = h_flag ? 'h' : ':';

		SAOconvert(buff, cp->dval, type, width, prec, flags);
	 	(*printer) ((PTR) target, buff);
	 }
	 break;
	 case AST(0, PF_C):
	    (*printer) ((PTR) target, p, (int) Ival) ;
	    break ;

	 case AST(1, PF_C):
	    (*printer) ((PTR) target, p, ast[0], (int) Ival) ;
	    break ;

	 case AST(2, PF_C):
	    (*printer) ((PTR) target, p, ast[0], ast[1], (int) Ival) ;
	    break ;

	 case AST(0, PF_S):
	    (*printer) ((PTR) target, p, string(cp)->str) ;
	    break ;

	 case AST(1, PF_S):
	    (*printer) ((PTR) target, p, ast[0], string(cp)->str) ;
	    break ;

	 case AST(2, PF_S):
	    (*printer) ((PTR) target, p, ast[0], ast[1], string(cp)->str) ;
	    break ;

#ifdef	SHORT_INTS
#define FMT	xbuff		/* format in xbuff */
#else
#define FMT	p		/* p -> format */
#endif
	 case AST(0, PF_D):
	    (*printer) ((PTR) target, FMT, Ival) ;
	    break ;

	 case AST(1, PF_D):
	    (*printer) ((PTR) target, FMT, ast[0], Ival) ;
	    break ;

	 case AST(2, PF_D):
	    (*printer) ((PTR) target, FMT, ast[0], ast[1], Ival) ;
	    break ;

	 case AST(0, PF_U):
	    (*printer) ((PTR) target, FMT, Uval) ;
	    break ;

	 case AST(1, PF_U):
	    (*printer) ((PTR) target, FMT, ast[0], Uval) ;
	    break ;

	 case AST(2, PF_U):
	    (*printer) ((PTR) target, FMT, ast[0], ast[1], Uval) ;
	    break ;

#undef	FMT


	 case AST(0, PF_F):
	    (*printer) ((PTR) target, p, cp->dval) ;
	    break ;

	 case AST(1, PF_F):
	    (*printer) ((PTR) target, p, ast[0], cp->dval) ;
	    break ;

	 case AST(2, PF_F):
	    (*printer) ((PTR) target, p, ast[0], ast[1], cp->dval) ;
	    break ;
      }
      if (fp == (FILE *) 0)
	 while (*target)  target++ ;
      *q = save ; argcnt-- ; cp++ ;
   }
}

CELL *
bi_printf(sp)
   register CELL *sp ;
{
   register int k ;
   register CELL *p ;
   FILE *fp ;

   k = sp->type ;
   if (k < 0)
   {
      /* k has redirection */
      if ((--sp)->type < C_STRING)  cast1_to_s(sp) ;
      fp = (FILE *) file_find(string(sp), k) ;
      free_STRING(string(sp)) ;
      k = (--sp)->type ;
      /* k is now number of args including format */
   }
   else	 fp = stdout ;

   sp -= k ;			 /* sp points at the format string */
   k-- ;

   if (sp->type < C_STRING)  cast1_to_s(sp) ;
   do_printf(fp, string(sp)->str, k, sp + 1);
   free_STRING(string(sp)) ;

   /* cleanup arguments on eval stack */
   for (p = sp + 1; k; k--, p++)  cell_destroy(p) ;
   return --sp ;
}

CELL *
bi_sprintf(sp)
   CELL *sp ;
{
   CELL *p ;
   int argcnt = sp->type ;
   STRING *sval ;

   sp -= argcnt ;		 /* sp points at the format string */
   argcnt-- ;

   if (sp->type != C_STRING)  cast1_to_s(sp) ;
   sval = do_printf((FILE *) 0, string(sp)->str, argcnt, sp + 1) ;
   free_STRING(string(sp)) ;
   sp->ptr = (PTR) sval ;

   /* cleanup */
   for (p = sp + 1; argcnt; argcnt--, p++)  cell_destroy(p) ;

   return sp ;
}


static void 
write_error()
{
   if ( errno == EPIPE ) { exit(0); }

   perrmsg(errno, "write failure") ;
   mawk_exit(2) ;
}
