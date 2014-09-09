
/********************************************
mawk.h
copyright 1991-94, Michael D. Brennan

This is a source file for mawk, an implementation of
the AWK programming language.

Mawk is distributed without warranty under the terms of
the GNU General Public License, version 2, 1991.
********************************************/



extern double	SAOstrtod();
extern int	SAOdtype;
extern char    *SAOconvert();

/*  mawk.h  */

#ifndef  MAWK_H
#define  MAWK_H   

#include  "nstd.h"
#include <stdio.h>
#include "types.h"

#ifdef   DEBUG
#define  YYDEBUG  1
extern  int   yydebug ;  /* print parse if on */
extern  int   dump_RE ;
#endif

#define DEBUG 1

extern  short  posix_space_flag , interactive_flag ;


/*----------------
 *  GLOBAL VARIABLES
 *----------------*/

/* a well known string */
extern STRING  null_str ;

#ifndef TEMPBUFF_GOES_HERE
#define EXTERN	extern
#else
#define EXTERN   /* empty */
#endif

/* a useful scratch area */
EXTERN  union {
STRING  *_split_buff[MAX_SPLIT] ;
char    _string_buff[MIN_SPRINTF] ;
} tempbuff ;

/* anonymous union */
#define  string_buff	tempbuff._string_buff
#define  split_buff	tempbuff._split_buff

#define  SPRINTF_SZ	sizeof(tempbuff)

/* help with casts */
extern int mpow2[] ;


 /* these are used by the parser, scanner and error messages
    from the compile  */

extern  char *pfile_name ; /* program input file */
extern  int current_token ;
extern  unsigned  token_lineno ; /* lineno of current token */
extern  unsigned  compile_error_count ;
extern  int  paren_cnt, brace_cnt ;
extern  int  print_flag, getline_flag ;
extern  short mawk_state ;
#define EXECUTION       1  /* other state is 0 compiling */


extern  char *progname ; /* for error messages */
extern  unsigned rt_nr , rt_fnr ; /* ditto */

/* macro to test the type of two adjacent cells */
#define TEST2(cp)  (mpow2[(cp)->type]+mpow2[((cp)+1)->type])

/* macro to get at the string part of a CELL */
#define string(cp) ((STRING *)(cp)->ptr)

#ifdef   DEBUG
#define cell_destroy(cp)  DB_cell_destroy(cp)
#else

#define cell_destroy(cp)   if ( (cp)->type >= C_STRING &&\
                           -- string(cp)->ref_cnt == 0 )\
                        zfree(string(cp),string(cp)->len+STRING_OH);else
#endif

/*  prototypes  */

void  PROTO( cast1_to_s, (CELL *) ) ;
void  PROTO( cast1_to_d, (CELL *) ) ;
void  PROTO( cast2_to_s, (CELL *) ) ;
void  PROTO( cast2_to_d, (CELL *) ) ;
void  PROTO( cast_to_RE, (CELL *) ) ;
void  PROTO( cast_for_split, (CELL *) ) ;
void  PROTO( check_strnum, (CELL *) ) ;
void  PROTO( cast_to_REPL, (CELL *) ) ;
Int   PROTO( d_to_I, (double)) ;

#define d_to_i(d)     ((int)d_to_I(d))

int   PROTO( test, (CELL *) ) ; /* test for null non-null */
CELL *PROTO( cellcpy, (CELL *, CELL *) ) ;
CELL *PROTO( repl_cpy, (CELL *, CELL *) ) ;
void  PROTO( DB_cell_destroy, (CELL *) ) ;
void  PROTO( overflow, (char *, unsigned) ) ;
void  PROTO( rt_overflow, (char *, unsigned) ) ;
void  PROTO( rt_error, ( char *, ...) ) ;
void  PROTO( mawk_exit, (int) ) ;
void PROTO( da, (INST *, FILE *)) ;
char *PROTO( str_str, (char*, char*, unsigned) ) ;
char *PROTO( rm_escape, (char *) ) ;
char *PROTO( re_pos_match, (char *, PTR, unsigned *) ) ;
int   PROTO( binmode, (void)) ;


int   PROTO( close, (int) ) ;
/* int   PROTO( read, (int , PTR, unsigned) ) ; */

void PROTO ( parse, (void) ) ;
int  PROTO ( yylex, (void) ) ;
int  PROTO( yyparse, (void) ) ;
void PROTO( yyerror, (char *) ) ;
void PROTO( scan_cleanup, (void)) ;

void PROTO( bozo, (char *) ) ;
void PROTO( perrmsg , (int, char*, ...) ) ;
void PROTO( compile_error, ( char *, ...) ) ;

void  PROTO( execute, (INST *, CELL *, CELL *, int)) ;
char *PROTO( find_kw_str, (int) ) ;

#ifdef HAVE_STRTOD_OVF_BUG
double PROTO(strtod_with_ovf_bug, (const char*, char**)) ;
#define strtod  strtod_with_ovf_bug
#endif

#endif  /* MAWK_H */
