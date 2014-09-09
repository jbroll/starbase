/* A Bison parser, made by GNU Bison 2.7.12-4996.  */

/* Bison implementation for Yacc-like parsers in C
   
      Copyright (C) 1984, 1989-1990, 2000-2013 Free Software Foundation, Inc.
   
   This program is free software: you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation, either version 3 of the License, or
   (at your option) any later version.
   
   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.
   
   You should have received a copy of the GNU General Public License
   along with this program.  If not, see <http://www.gnu.org/licenses/>.  */

/* As a special exception, you may create a larger work that contains
   part or all of the Bison parser skeleton and distribute that work
   under terms of your choice, so long as that work isn't itself a
   parser generator using the skeleton or a modified version thereof
   as a parser skeleton.  Alternatively, if you modify or redistribute
   the parser skeleton itself, you may (at your option) remove this
   special exception, which will cause the skeleton and the resulting
   Bison output files to be licensed under the GNU General Public
   License without this special exception.
   
   This special exception was added by the Free Software Foundation in
   version 2.2 of Bison.  */

/* C LALR(1) parser skeleton written by Richard Stallman, by
   simplifying the original so-called "semantic" parser.  */

/* All symbols defined below should begin with yy or YY, to avoid
   infringing on user name space.  This should be done even for local
   variables, as they might otherwise be expanded by user macros.
   There are some unavoidable exceptions within include files to
   define necessary library symbols; they are noted "INFRINGES ON
   USER NAME SPACE" below.  */

/* Identify Bison output.  */
#define YYBISON 1

/* Bison version.  */
#define YYBISON_VERSION "2.7.12-4996"

/* Skeleton name.  */
#define YYSKELETON_NAME "yacc.c"

/* Pure parsers.  */
#define YYPURE 0

/* Push parsers.  */
#define YYPUSH 0

/* Pull parsers.  */
#define YYPULL 1




/* Copy the first part of user declarations.  */
/* Line 371 of yacc.c  */
#line 13 "parse.y"

#include <stdio.h>
#include "mawk.h"
#include "symtype.h"
#include "code.h"
#include "memory.h"
#include "bi_funct.h"
#include "bi_vars.h"
#include "jmp.h"
#include "field.h"
#include "files.h"


#define  YYMAXDEPTH	200

void checker_fcall();
void checker_fcalls();
void load();


extern void  PROTO( eat_nl, (void) ) ;
static void  PROTO( resize_fblock, (FBLOCK *) ) ;
static void  PROTO( switch_code_to_main, (void)) ;
static void  PROTO( code_array, (SYMTAB *) ) ;
static void  PROTO( code_call_id, (CA_REC *, SYMTAB *) ) ;
static void  PROTO( field_A2I, (void)) ;
static void  PROTO( check_var, (SYMTAB *) ) ;
static void  PROTO( check_array, (SYMTAB *) ) ;
static void  PROTO( RE_as_arg, (void)) ;

static int scope ;
static FBLOCK *active_funct ;
      /* when scope is SCOPE_FUNCT  */

#define  code_address(x)  if( is_local(x) ) \
			     code2op(L_PUSHA, (x)->offset) ;\
                          else  code2(_PUSHA, (x)->stval.cp) 

#define  CDP(x)  (code_base+(x))
/* WARNING: These CDP() calculations become invalid after calls
   that might change code_base.  Which are:  code2(), code2op(),
   code_jmp() and code_pop().
*/

/* this nonsense caters to MSDOS large model */
#define  CODE_FE_PUSHA()  code_ptr->ptr = (PTR) 0 ; code1(FE_PUSHA)

FBLOCK          *FReturns = NULL;


/* Line 371 of yacc.c  */
#line 119 "y.tab.c"

# ifndef YY_NULL
#  if defined __cplusplus && 201103L <= __cplusplus
#   define YY_NULL nullptr
#  else
#   define YY_NULL 0
#  endif
# endif

/* Enabling verbose error messages.  */
#ifdef YYERROR_VERBOSE
# undef YYERROR_VERBOSE
# define YYERROR_VERBOSE 1
#else
# define YYERROR_VERBOSE 0
#endif

/* In a future release of Bison, this section will be replaced
   by #include "y.tab.h".  */
#ifndef YY_YY_Y_TAB_H_INCLUDED
# define YY_YY_Y_TAB_H_INCLUDED
/* Enabling traces.  */
#ifndef YYDEBUG
# define YYDEBUG 0
#endif
#if YYDEBUG
extern int yydebug;
#endif

/* Tokens.  */
#ifndef YYTOKENTYPE
# define YYTOKENTYPE
   /* Put the tokens into the symbol table, so that GDB and other debuggers
      know about them.  */
   enum yytokentype {
     UNEXPECTED = 258,
     BAD_DECIMAL = 259,
     NL = 260,
     SEMI_COLON = 261,
     LBRACE = 262,
     RBRACE = 263,
     LBOX = 264,
     RBOX = 265,
     COMMA = 266,
     IO_OUT = 267,
     POW_ASG = 268,
     MOD_ASG = 269,
     DIV_ASG = 270,
     MUL_ASG = 271,
     SUB_ASG = 272,
     ADD_ASG = 273,
     ASSIGN = 274,
     COLON = 275,
     QMARK = 276,
     OR = 277,
     AND = 278,
     IN = 279,
     MATCH = 280,
     GTE = 281,
     GT = 282,
     LTE = 283,
     LT = 284,
     NEQ = 285,
     EQ = 286,
     CAT = 287,
     GETLINE = 288,
     MINUS = 289,
     PLUS = 290,
     MOD = 291,
     DIV = 292,
     MUL = 293,
     UMINUS = 294,
     NOT = 295,
     PIPE = 296,
     IO_IN = 297,
     POW = 298,
     INC_or_DEC = 299,
     FIELD = 300,
     DOLLAR = 301,
     RPAREN = 302,
     LPAREN = 303,
     DOUBLE = 304,
     STRING_ = 305,
     RE = 306,
     ID = 307,
     D_ID = 308,
     FUNCT_ID = 309,
     BUILTIN = 310,
     LENGTH = 311,
     PRINT = 312,
     PRINTF = 313,
     SPLIT = 314,
     MATCH_FUNC = 315,
     SUB = 316,
     GSUB = 317,
     DO = 318,
     WHILE = 319,
     FOR = 320,
     BREAK = 321,
     CONTINUE = 322,
     IF = 323,
     ELSE = 324,
     DELETE = 325,
     BEGIN = 326,
     END = 327,
     EXIT = 328,
     NEXT = 329,
     RETURN = 330,
     FUNCTION = 331,
     LOAD = 332
   };
#endif
/* Tokens.  */
#define UNEXPECTED 258
#define BAD_DECIMAL 259
#define NL 260
#define SEMI_COLON 261
#define LBRACE 262
#define RBRACE 263
#define LBOX 264
#define RBOX 265
#define COMMA 266
#define IO_OUT 267
#define POW_ASG 268
#define MOD_ASG 269
#define DIV_ASG 270
#define MUL_ASG 271
#define SUB_ASG 272
#define ADD_ASG 273
#define ASSIGN 274
#define COLON 275
#define QMARK 276
#define OR 277
#define AND 278
#define IN 279
#define MATCH 280
#define GTE 281
#define GT 282
#define LTE 283
#define LT 284
#define NEQ 285
#define EQ 286
#define CAT 287
#define GETLINE 288
#define MINUS 289
#define PLUS 290
#define MOD 291
#define DIV 292
#define MUL 293
#define UMINUS 294
#define NOT 295
#define PIPE 296
#define IO_IN 297
#define POW 298
#define INC_or_DEC 299
#define FIELD 300
#define DOLLAR 301
#define RPAREN 302
#define LPAREN 303
#define DOUBLE 304
#define STRING_ 305
#define RE 306
#define ID 307
#define D_ID 308
#define FUNCT_ID 309
#define BUILTIN 310
#define LENGTH 311
#define PRINT 312
#define PRINTF 313
#define SPLIT 314
#define MATCH_FUNC 315
#define SUB 316
#define GSUB 317
#define DO 318
#define WHILE 319
#define FOR 320
#define BREAK 321
#define CONTINUE 322
#define IF 323
#define ELSE 324
#define DELETE 325
#define BEGIN 326
#define END 327
#define EXIT 328
#define NEXT 329
#define RETURN 330
#define FUNCTION 331
#define LOAD 332



#if ! defined YYSTYPE && ! defined YYSTYPE_IS_DECLARED
typedef union YYSTYPE
{
/* Line 387 of yacc.c  */
#line 64 "parse.y"

CELL *cp ;
SYMTAB *stp ;
int  start ; /* code starting address as offset from code_base */
PF_CP  fp ;  /* ptr to a (print/printf) or (sub/gsub) function */
BI_REC *bip ; /* ptr to info about a builtin */
FBLOCK  *fbp  ; /* ptr to a function block */
ARG2_REC *arg2p ;
CA_REC   *ca_p  ;
int   ival ;
PTR   ptr ;


/* Line 387 of yacc.c  */
#line 330 "y.tab.c"
} YYSTYPE;
# define YYSTYPE_IS_TRIVIAL 1
# define yystype YYSTYPE /* obsolescent; will be withdrawn */
# define YYSTYPE_IS_DECLARED 1
#endif

extern YYSTYPE yylval;

#ifdef YYPARSE_PARAM
#if defined __STDC__ || defined __cplusplus
int yyparse (void *YYPARSE_PARAM);
#else
int yyparse ();
#endif
#else /* ! YYPARSE_PARAM */
#if defined __STDC__ || defined __cplusplus
int yyparse (void);
#else
int yyparse ();
#endif
#endif /* ! YYPARSE_PARAM */

#endif /* !YY_YY_Y_TAB_H_INCLUDED  */

/* Copy the second part of user declarations.  */

/* Line 390 of yacc.c  */
#line 358 "y.tab.c"

#ifdef short
# undef short
#endif

#ifdef YYTYPE_UINT8
typedef YYTYPE_UINT8 yytype_uint8;
#else
typedef unsigned char yytype_uint8;
#endif

#ifdef YYTYPE_INT8
typedef YYTYPE_INT8 yytype_int8;
#elif (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
typedef signed char yytype_int8;
#else
typedef short int yytype_int8;
#endif

#ifdef YYTYPE_UINT16
typedef YYTYPE_UINT16 yytype_uint16;
#else
typedef unsigned short int yytype_uint16;
#endif

#ifdef YYTYPE_INT16
typedef YYTYPE_INT16 yytype_int16;
#else
typedef short int yytype_int16;
#endif

#ifndef YYSIZE_T
# ifdef __SIZE_TYPE__
#  define YYSIZE_T __SIZE_TYPE__
# elif defined size_t
#  define YYSIZE_T size_t
# elif ! defined YYSIZE_T && (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
#  include <stddef.h> /* INFRINGES ON USER NAME SPACE */
#  define YYSIZE_T size_t
# else
#  define YYSIZE_T unsigned int
# endif
#endif

#define YYSIZE_MAXIMUM ((YYSIZE_T) -1)

#ifndef YY_
# if defined YYENABLE_NLS && YYENABLE_NLS
#  if ENABLE_NLS
#   include <libintl.h> /* INFRINGES ON USER NAME SPACE */
#   define YY_(Msgid) dgettext ("bison-runtime", Msgid)
#  endif
# endif
# ifndef YY_
#  define YY_(Msgid) Msgid
# endif
#endif

#ifndef __attribute__
/* This feature is available in gcc versions 2.5 and later.  */
# if (! defined __GNUC__ || __GNUC__ < 2 \
      || (__GNUC__ == 2 && __GNUC_MINOR__ < 5))
#  define __attribute__(Spec) /* empty */
# endif
#endif

/* Suppress unused-variable warnings by "using" E.  */
#if ! defined lint || defined __GNUC__
# define YYUSE(E) ((void) (E))
#else
# define YYUSE(E) /* empty */
#endif


/* Identity function, used to suppress warnings about constant conditions.  */
#ifndef lint
# define YYID(N) (N)
#else
#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
static int
YYID (int yyi)
#else
static int
YYID (yyi)
    int yyi;
#endif
{
  return yyi;
}
#endif

#if ! defined yyoverflow || YYERROR_VERBOSE

/* The parser invokes alloca or malloc; define the necessary symbols.  */

# ifdef YYSTACK_USE_ALLOCA
#  if YYSTACK_USE_ALLOCA
#   ifdef __GNUC__
#    define YYSTACK_ALLOC __builtin_alloca
#   elif defined __BUILTIN_VA_ARG_INCR
#    include <alloca.h> /* INFRINGES ON USER NAME SPACE */
#   elif defined _AIX
#    define YYSTACK_ALLOC __alloca
#   elif defined _MSC_VER
#    include <malloc.h> /* INFRINGES ON USER NAME SPACE */
#    define alloca _alloca
#   else
#    define YYSTACK_ALLOC alloca
#    if ! defined _ALLOCA_H && ! defined EXIT_SUCCESS && (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
#     include <stdlib.h> /* INFRINGES ON USER NAME SPACE */
      /* Use EXIT_SUCCESS as a witness for stdlib.h.  */
#     ifndef EXIT_SUCCESS
#      define EXIT_SUCCESS 0
#     endif
#    endif
#   endif
#  endif
# endif

# ifdef YYSTACK_ALLOC
   /* Pacify GCC's `empty if-body' warning.  */
#  define YYSTACK_FREE(Ptr) do { /* empty */; } while (YYID (0))
#  ifndef YYSTACK_ALLOC_MAXIMUM
    /* The OS might guarantee only one guard page at the bottom of the stack,
       and a page size can be as small as 4096 bytes.  So we cannot safely
       invoke alloca (N) if N exceeds 4096.  Use a slightly smaller number
       to allow for a few compiler-allocated temporary stack slots.  */
#   define YYSTACK_ALLOC_MAXIMUM 4032 /* reasonable circa 2006 */
#  endif
# else
#  define YYSTACK_ALLOC YYMALLOC
#  define YYSTACK_FREE YYFREE
#  ifndef YYSTACK_ALLOC_MAXIMUM
#   define YYSTACK_ALLOC_MAXIMUM YYSIZE_MAXIMUM
#  endif
#  if (defined __cplusplus && ! defined EXIT_SUCCESS \
       && ! ((defined YYMALLOC || defined malloc) \
	     && (defined YYFREE || defined free)))
#   include <stdlib.h> /* INFRINGES ON USER NAME SPACE */
#   ifndef EXIT_SUCCESS
#    define EXIT_SUCCESS 0
#   endif
#  endif
#  ifndef YYMALLOC
#   define YYMALLOC malloc
#   if ! defined malloc && ! defined EXIT_SUCCESS && (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
void *malloc (YYSIZE_T); /* INFRINGES ON USER NAME SPACE */
#   endif
#  endif
#  ifndef YYFREE
#   define YYFREE free
#   if ! defined free && ! defined EXIT_SUCCESS && (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
void free (void *); /* INFRINGES ON USER NAME SPACE */
#   endif
#  endif
# endif
#endif /* ! defined yyoverflow || YYERROR_VERBOSE */


#if (! defined yyoverflow \
     && (! defined __cplusplus \
	 || (defined YYSTYPE_IS_TRIVIAL && YYSTYPE_IS_TRIVIAL)))

/* A type that is properly aligned for any stack member.  */
union yyalloc
{
  yytype_int16 yyss_alloc;
  YYSTYPE yyvs_alloc;
};

/* The size of the maximum gap between one aligned stack and the next.  */
# define YYSTACK_GAP_MAXIMUM (sizeof (union yyalloc) - 1)

/* The size of an array large to enough to hold all stacks, each with
   N elements.  */
# define YYSTACK_BYTES(N) \
     ((N) * (sizeof (yytype_int16) + sizeof (YYSTYPE)) \
      + YYSTACK_GAP_MAXIMUM)

# define YYCOPY_NEEDED 1

/* Relocate STACK from its old location to the new one.  The
   local variables YYSIZE and YYSTACKSIZE give the old and new number of
   elements in the stack, and YYPTR gives the new location of the
   stack.  Advance YYPTR to a properly aligned location for the next
   stack.  */
# define YYSTACK_RELOCATE(Stack_alloc, Stack)				\
    do									\
      {									\
	YYSIZE_T yynewbytes;						\
	YYCOPY (&yyptr->Stack_alloc, Stack, yysize);			\
	Stack = &yyptr->Stack_alloc;					\
	yynewbytes = yystacksize * sizeof (*Stack) + YYSTACK_GAP_MAXIMUM; \
	yyptr += yynewbytes / sizeof (*yyptr);				\
      }									\
    while (YYID (0))

#endif

#if defined YYCOPY_NEEDED && YYCOPY_NEEDED
/* Copy COUNT objects from SRC to DST.  The source and destination do
   not overlap.  */
# ifndef YYCOPY
#  if defined __GNUC__ && 1 < __GNUC__
#   define YYCOPY(Dst, Src, Count) \
      __builtin_memcpy (Dst, Src, (Count) * sizeof (*(Src)))
#  else
#   define YYCOPY(Dst, Src, Count)              \
      do                                        \
        {                                       \
          YYSIZE_T yyi;                         \
          for (yyi = 0; yyi < (Count); yyi++)   \
            (Dst)[yyi] = (Src)[yyi];            \
        }                                       \
      while (YYID (0))
#  endif
# endif
#endif /* !YYCOPY_NEEDED */

/* YYFINAL -- State number of the termination state.  */
#define YYFINAL  99
/* YYLAST -- Last index in YYTABLE.  */
#define YYLAST   1241

/* YYNTOKENS -- Number of terminals.  */
#define YYNTOKENS  78
/* YYNNTS -- Number of nonterminals.  */
#define YYNNTS  60
/* YYNRULES -- Number of rules.  */
#define YYNRULES  181
/* YYNRULES -- Number of states.  */
#define YYNSTATES  360

/* YYTRANSLATE(YYLEX) -- Bison symbol number corresponding to YYLEX.  */
#define YYUNDEFTOK  2
#define YYMAXUTOK   332

#define YYTRANSLATE(YYX)						\
  ((unsigned int) (YYX) <= YYMAXUTOK ? yytranslate[YYX] : YYUNDEFTOK)

/* YYTRANSLATE[YYLEX] -- Bison symbol number corresponding to YYLEX.  */
static const yytype_uint8 yytranslate[] =
{
       0,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     1,     2,     3,     4,
       5,     6,     7,     8,     9,    10,    11,    12,    13,    14,
      15,    16,    17,    18,    19,    20,    21,    22,    23,    24,
      25,    26,    27,    28,    29,    30,    31,    32,    33,    34,
      35,    36,    37,    38,    39,    40,    41,    42,    43,    44,
      45,    46,    47,    48,    49,    50,    51,    52,    53,    54,
      55,    56,    57,    58,    59,    60,    61,    62,    63,    64,
      65,    66,    67,    68,    69,    70,    71,    72,    73,    74,
      75,    76,    77
};

#if YYDEBUG
/* YYPRHS[YYN] -- Index of the first RHS symbol of rule number YYN in
   YYRHS.  */
static const yytype_uint16 yyprhs[] =
{
       0,     0,     3,     5,     8,    10,    12,    15,    20,    22,
      23,    27,    28,    32,    33,    37,    38,    39,    46,    50,
      54,    56,    58,    60,    63,    65,    68,    70,    73,    76,
      79,    81,    84,    86,    88,    90,    94,    98,   102,   106,
     110,   114,   118,   122,   126,   130,   134,   138,   142,   146,
     147,   152,   153,   158,   159,   160,   168,   170,   173,   175,
     177,   179,   183,   185,   189,   193,   197,   201,   205,   209,
     212,   215,   218,   220,   223,   226,   229,   232,   234,   235,
     237,   239,   243,   249,   251,   252,   258,   260,   262,   264,
     268,   271,   275,   279,   280,   283,   288,   291,   293,   298,
     300,   308,   313,   316,   321,   325,   330,   332,   335,   337,
     340,   344,   350,   356,   362,   369,   377,   381,   388,   391,
     393,   396,   403,   406,   410,   412,   416,   420,   424,   428,
     432,   436,   440,   443,   449,   451,   455,   462,   464,   467,
     471,   474,   480,   484,   488,   492,   494,   497,   500,   504,
     509,   511,   513,   515,   518,   522,   529,   531,   533,   535,
     539,   542,   547,   550,   553,   554,   556,   558,   562,   564,
     571,   581,   585,   589,   591,   595,   598,   601,   603,   607,
     611,   614
};

/* YYRHS -- A `-1'-separated list of the rules' RHS.  */
static const yytype_int16 yyrhs[] =
{
      79,     0,    -1,    80,    -1,    79,    80,    -1,    81,    -1,
     127,    -1,   132,    87,    -1,    77,    50,    50,     5,    -1,
      87,    -1,    -1,    71,    82,    87,    -1,    -1,    72,    83,
      87,    -1,    -1,    92,    84,    88,    -1,    -1,    -1,    92,
      11,    85,    92,    86,    88,    -1,     7,    89,     8,    -1,
       7,     1,     8,    -1,    87,    -1,    91,    -1,    90,    -1,
      89,    90,    -1,    87,    -1,    92,    91,    -1,    91,    -1,
       1,    91,    -1,    66,    91,    -1,    67,    91,    -1,   120,
      -1,    74,    91,    -1,     5,    -1,     6,    -1,    97,    -1,
      99,    19,    92,    -1,    99,    18,    92,    -1,    99,    17,
      92,    -1,    99,    16,    92,    -1,    99,    15,    92,    -1,
      99,    14,    92,    -1,    99,    13,    92,    -1,    92,    31,
      92,    -1,    92,    30,    92,    -1,    92,    29,    92,    -1,
      92,    28,    92,    -1,    92,    27,    92,    -1,    92,    26,
      92,    -1,    92,    25,    92,    -1,    -1,    92,    22,    93,
      92,    -1,    -1,    92,    23,    94,    92,    -1,    -1,    -1,
      92,    21,    95,    92,    20,    96,    92,    -1,    98,    -1,
      97,    98,    -1,    49,    -1,    50,    -1,    52,    -1,    48,
      92,    47,    -1,    51,    -1,    98,    35,    98,    -1,    98,
      34,    98,    -1,    98,    38,    98,    -1,    98,    37,    98,
      -1,    98,    36,    98,    -1,    98,    43,    98,    -1,    40,
      98,    -1,    35,    98,    -1,    34,    98,    -1,   102,    -1,
      52,    44,    -1,    44,    99,    -1,   116,    44,    -1,    44,
     116,    -1,    52,    -1,    -1,   101,    -1,    92,    -1,   101,
      11,    92,    -1,    55,   103,    48,   100,    47,    -1,    56,
      -1,    -1,   104,   103,   105,   107,    91,    -1,    57,    -1,
      58,    -1,   100,    -1,    48,   106,    47,    -1,    48,    47,
      -1,    92,    11,    92,    -1,   106,    11,    92,    -1,    -1,
      12,    92,    -1,    68,    48,    92,    47,    -1,   108,    90,
      -1,    69,    -1,   108,    90,   109,    90,    -1,    63,    -1,
     110,    90,    64,    48,    92,    47,    91,    -1,    64,    48,
      92,    47,    -1,   111,    90,    -1,   112,   113,   114,    90,
      -1,    65,    48,     6,    -1,    65,    48,    92,     6,    -1,
       6,    -1,    92,     6,    -1,    47,    -1,    92,    47,    -1,
      92,    24,    52,    -1,    48,   106,    47,    24,    52,    -1,
      52,   103,     9,   101,    10,    -1,    52,   103,     9,   101,
      10,    -1,    52,   103,     9,   101,    10,    44,    -1,    70,
      52,   103,     9,   101,    10,    91,    -1,    70,    52,    91,
      -1,    65,    48,    52,    24,    52,    47,    -1,   115,    90,
      -1,    45,    -1,    46,    53,    -1,    46,    53,   103,     9,
     101,    10,    -1,    46,    98,    -1,    48,   116,    47,    -1,
     116,    -1,   116,    19,    92,    -1,   116,    18,    92,    -1,
     116,    17,    92,    -1,   116,    16,    92,    -1,   116,    15,
      92,    -1,   116,    14,    92,    -1,   116,    13,    92,    -1,
     117,   118,    -1,    59,    48,    92,    11,    52,    -1,    47,
      -1,    11,    92,    47,    -1,    60,    48,    92,    11,   119,
      47,    -1,    92,    -1,    73,    91,    -1,    73,    92,    91,
      -1,    75,    91,    -1,    75,     9,   121,    10,    91,    -1,
      75,    92,    91,    -1,    92,    11,    92,    -1,   121,    11,
      92,    -1,   122,    -1,   122,   123,    -1,   124,    98,    -1,
      98,    41,    33,    -1,    98,    41,    33,   123,    -1,    33,
      -1,    99,    -1,   116,    -1,   122,    42,    -1,   122,   123,
      42,    -1,   125,    48,   119,    11,    92,   126,    -1,    61,
      -1,    62,    -1,    47,    -1,    11,   123,    47,    -1,   128,
      87,    -1,   129,    48,   130,    47,    -1,    76,    52,    -1,
      76,    54,    -1,    -1,   131,    -1,    52,    -1,   131,    11,
      52,    -1,     1,    -1,     9,   103,   133,    10,    19,   134,
      -1,     9,   103,   133,    10,    19,     9,   121,    10,    91,
      -1,   123,    11,   123,    -1,   133,    11,   123,    -1,   134,
      -1,    54,   103,   135,    -1,    48,    47,    -1,   136,   137,
      -1,    48,    -1,   136,    92,    11,    -1,   136,    52,    11,
      -1,    92,    47,    -1,    52,    47,    -1
};

/* YYRLINE[YYN] -- source line where rule number YYN was defined.  */
static const yytype_uint16 yyrline[] =
{
       0,   142,   142,   143,   146,   147,   148,   149,   154,   160,
     159,   166,   165,   172,   171,   179,   195,   178,   208,   210,
     216,   217,   224,   225,   229,   230,   232,   234,   240,   243,
     246,   250,   258,   258,   261,   262,   263,   264,   265,   266,
     267,   268,   269,   270,   271,   272,   273,   274,   276,   304,
     303,   311,   310,   317,   318,   317,   323,   324,   328,   332,
     334,   342,   346,   350,   351,   352,   353,   354,   355,   356,
     358,   360,   362,   365,   373,   380,   384,   391,   400,   401,
     404,   406,   411,   422,   432,   436,   445,   446,   449,   450,
     454,   458,   463,   467,   468,   475,   480,   484,   488,   497,
     502,   508,   528,   554,   578,   579,   583,   584,   601,   605,
     618,   623,   634,   647,   659,   676,   684,   695,   709,   726,
     728,   737,   751,   753,   757,   761,   762,   763,   764,   765,
     766,   767,   773,   777,   784,   786,   810,   817,   839,   842,
     846,   854,   864,   873,   875,   881,   888,   894,   899,   904,
     911,   913,   913,   915,   919,   927,   946,   947,   951,   956,
     964,   973,   993,  1016,  1023,  1024,  1027,  1033,  1046,  1059,
    1066,  1075,  1077,  1082,  1088,  1104,  1106,  1121,  1123,  1130,
    1139,  1145
};
#endif

#if YYDEBUG || YYERROR_VERBOSE || 0
/* YYTNAME[SYMBOL-NUM] -- String name of the symbol SYMBOL-NUM.
   First, the terminals, then, starting at YYNTOKENS, nonterminals.  */
static const char *const yytname[] =
{
  "$end", "error", "$undefined", "UNEXPECTED", "BAD_DECIMAL", "NL",
  "SEMI_COLON", "LBRACE", "RBRACE", "LBOX", "RBOX", "COMMA", "IO_OUT",
  "POW_ASG", "MOD_ASG", "DIV_ASG", "MUL_ASG", "SUB_ASG", "ADD_ASG",
  "ASSIGN", "COLON", "QMARK", "OR", "AND", "IN", "MATCH", "GTE", "GT",
  "LTE", "LT", "NEQ", "EQ", "CAT", "GETLINE", "MINUS", "PLUS", "MOD",
  "DIV", "MUL", "UMINUS", "NOT", "PIPE", "IO_IN", "POW", "INC_or_DEC",
  "FIELD", "DOLLAR", "RPAREN", "LPAREN", "DOUBLE", "STRING_", "RE", "ID",
  "D_ID", "FUNCT_ID", "BUILTIN", "LENGTH", "PRINT", "PRINTF", "SPLIT",
  "MATCH_FUNC", "SUB", "GSUB", "DO", "WHILE", "FOR", "BREAK", "CONTINUE",
  "IF", "ELSE", "DELETE", "BEGIN", "END", "EXIT", "NEXT", "RETURN",
  "FUNCTION", "LOAD", "$accept", "program", "program_block", "PA_block",
  "$@1", "$@2", "$@3", "$@4", "$@5", "block", "block_or_separator",
  "statement_list", "statement", "separator", "expr", "$@6", "$@7", "$@8",
  "$@9", "cat_expr", "p_expr", "lvalue", "arglist", "args", "builtin",
  "mark", "print", "pr_args", "arg2", "pr_direction", "if_front", "else",
  "do", "while_front", "for1", "for2", "for3", "array_loop_front", "field",
  "split_front", "split_back", "re_arg", "return_statement", "exprlist",
  "getline", "fvalue", "getline_file", "sub_or_gsub", "sub_back",
  "function_def", "funct_start", "funct_head", "f_arglist", "f_args",
  "outside_error", "assignlist", "f_expr", "call_args", "ca_front",
  "ca_back", YY_NULL
};
#endif

# ifdef YYPRINT
/* YYTOKNUM[YYLEX-NUM] -- Internal token number corresponding to
   token YYLEX-NUM.  */
static const yytype_uint16 yytoknum[] =
{
       0,   256,   257,   258,   259,   260,   261,   262,   263,   264,
     265,   266,   267,   268,   269,   270,   271,   272,   273,   274,
     275,   276,   277,   278,   279,   280,   281,   282,   283,   284,
     285,   286,   287,   288,   289,   290,   291,   292,   293,   294,
     295,   296,   297,   298,   299,   300,   301,   302,   303,   304,
     305,   306,   307,   308,   309,   310,   311,   312,   313,   314,
     315,   316,   317,   318,   319,   320,   321,   322,   323,   324,
     325,   326,   327,   328,   329,   330,   331,   332
};
# endif

/* YYR1[YYN] -- Symbol number of symbol that rule YYN derives.  */
static const yytype_uint8 yyr1[] =
{
       0,    78,    79,    79,    80,    80,    80,    80,    81,    82,
      81,    83,    81,    84,    81,    85,    86,    81,    87,    87,
      88,    88,    89,    89,    90,    90,    90,    90,    90,    90,
      90,    90,    91,    91,    92,    92,    92,    92,    92,    92,
      92,    92,    92,    92,    92,    92,    92,    92,    92,    93,
      92,    94,    92,    95,    96,    92,    97,    97,    98,    98,
      98,    98,    98,    98,    98,    98,    98,    98,    98,    98,
      98,    98,    98,    98,    98,    98,    98,    99,   100,   100,
     101,   101,   102,   102,   103,    90,   104,   104,   105,   105,
     105,   106,   106,   107,   107,   108,    90,   109,    90,   110,
      90,   111,    90,    90,   112,   112,   113,   113,   114,   114,
      92,    92,    99,    98,    98,    90,    90,   115,    90,   116,
     116,   116,   116,   116,    98,    92,    92,    92,    92,    92,
      92,    92,    98,   117,   118,   118,    98,   119,    90,    90,
     120,   120,   120,   121,   121,    98,    98,    98,    98,    98,
     122,   123,   123,   124,   124,    98,   125,   125,   126,   126,
     127,   128,   129,   129,   130,   130,   131,   131,   132,    90,
      90,   133,   133,    98,   134,   135,   135,   136,   136,   136,
     137,   137
};

/* YYR2[YYN] -- Number of symbols composing right hand side of rule YYN.  */
static const yytype_uint8 yyr2[] =
{
       0,     2,     1,     2,     1,     1,     2,     4,     1,     0,
       3,     0,     3,     0,     3,     0,     0,     6,     3,     3,
       1,     1,     1,     2,     1,     2,     1,     2,     2,     2,
       1,     2,     1,     1,     1,     3,     3,     3,     3,     3,
       3,     3,     3,     3,     3,     3,     3,     3,     3,     0,
       4,     0,     4,     0,     0,     7,     1,     2,     1,     1,
       1,     3,     1,     3,     3,     3,     3,     3,     3,     2,
       2,     2,     1,     2,     2,     2,     2,     1,     0,     1,
       1,     3,     5,     1,     0,     5,     1,     1,     1,     3,
       2,     3,     3,     0,     2,     4,     2,     1,     4,     1,
       7,     4,     2,     4,     3,     4,     1,     2,     1,     2,
       3,     5,     5,     5,     6,     7,     3,     6,     2,     1,
       2,     6,     2,     3,     1,     3,     3,     3,     3,     3,
       3,     3,     2,     5,     1,     3,     6,     1,     2,     3,
       2,     5,     3,     3,     3,     1,     2,     2,     3,     4,
       1,     1,     1,     2,     3,     6,     1,     1,     1,     3,
       2,     4,     2,     2,     0,     1,     1,     3,     1,     6,
       9,     3,     3,     1,     3,     2,     2,     1,     3,     3,
       2,     2
};

/* YYDEFACT[STATE-NAME] -- Default reduction number in state STATE-NUM.
   Performed when YYTABLE doesn't specify something else to do.  Zero
   means the default is an error.  */
static const yytype_uint8 yydefact[] =
{
       0,   168,     0,   150,     0,     0,     0,     0,   119,     0,
       0,    58,    59,    62,    60,    84,    84,    83,     0,     0,
     156,   157,     9,    11,     0,     0,     0,     2,     4,     8,
      13,    34,    56,     0,    72,   124,     0,   145,     0,     0,
       5,     0,     0,     0,   173,     0,    32,    33,    84,    86,
      87,    99,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    24,     0,    22,    26,     0,    84,     0,     0,     0,
       0,     0,    30,     0,    60,    71,   124,    70,    69,     0,
      77,    74,    76,   120,   122,     0,     0,   124,    73,     0,
       0,     0,     0,     0,     0,     0,   162,   163,     0,     1,
       3,    15,    53,    49,    51,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    57,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    75,     0,   134,   132,
     153,   151,   152,   146,   147,     0,   160,   164,     6,    19,
      27,     0,     0,     0,    28,    29,     0,    84,   138,     0,
      31,     0,   140,     0,     0,    18,    23,    25,    78,    96,
       0,   102,   106,     0,     0,   118,     0,     0,     0,     0,
       0,     0,    61,     0,     0,   123,     0,   177,   174,     0,
      78,     0,     0,    10,    12,     0,     0,     0,     0,     0,
     110,    48,    47,    46,    45,    44,    43,    42,    20,    14,
      21,    64,    63,    67,    66,    65,   148,    68,    41,    40,
      39,    38,    37,    36,    35,   131,   130,   129,   128,   127,
     126,   125,     0,   154,   137,     0,   166,     0,   165,     0,
       0,     0,   104,    60,     0,     0,   116,     0,   139,     0,
       0,   142,     0,    80,    88,    79,    93,    97,     0,     0,
     107,   108,     0,     0,     0,     0,     0,    91,    92,     0,
       0,   175,    60,     0,   176,     0,     0,     0,     7,    16,
       0,    50,    52,   149,   135,     0,   161,     0,     0,     0,
       0,   101,     0,   105,    95,     0,     0,     0,     0,    90,
       0,     0,     0,     0,    98,     0,   109,   103,     0,     0,
       0,   111,   113,   179,   181,   178,   180,    82,   133,     0,
       0,    54,     0,   167,   171,     0,   172,     0,     0,   143,
     141,   144,    89,    81,    94,    85,     0,   113,   112,   121,
     114,   136,    17,     0,     0,   158,   155,     0,   169,   117,
       0,     0,    55,     0,     0,   115,   100,   159,     0,   170
};

/* YYDEFGOTO[NTERM-NUM].  */
static const yytype_int16 yydefgoto[] =
{
      -1,    26,    27,    28,    94,    95,   113,   196,   320,    61,
     209,    62,    63,    64,    65,   198,   199,   197,   343,    31,
      32,    33,   254,   255,    34,    89,    66,   256,    86,   303,
      67,   258,    68,    69,    70,   174,   263,    71,    35,    36,
     139,   235,    72,   250,    37,   143,    38,    39,   346,    40,
      41,    42,   237,   238,    43,   240,    44,   188,   189,   274
};

/* YYPACT[STATE-NUM] -- Index in YYTABLE of the portion describing
   STATE-NUM.  */
#define YYPACT_NINF -129
static const yytype_int16 yypact[] =
{
     319,  -129,   503,  -129,   837,   837,   837,    17,  -129,   747,
     867,  -129,  -129,  -129,   760,  -129,  -129,  -129,   -27,   -23,
    -129,  -129,  -129,  -129,    22,   -17,   273,  -129,  -129,  -129,
    1136,   837,   166,  1025,  -129,   228,   -10,   -18,   837,    -1,
    -129,    31,     5,    31,  -129,    14,  -129,  -129,  -129,  -129,
    -129,  -129,     9,    19,    78,    78,    27,    29,   676,    78,
     645,  -129,   432,  -129,  -129,   979,  -129,   574,   574,   574,
     178,   574,  -129,   867,     8,    25,     6,    25,    25,    -6,
      70,  -129,  -129,    70,  -129,   376,     1,   411,  -129,    77,
      43,    88,   867,   867,    31,    31,  -129,  -129,    68,  -129,
    -129,  -129,  -129,  -129,  -129,    94,   867,   867,   867,   867,
     867,   867,   867,    38,   166,   837,   837,   837,   837,   837,
     115,   837,   867,   867,   867,   867,   867,   867,   867,   867,
     867,   867,   867,   867,   867,   867,  -129,   867,  -129,  -129,
    -129,  -129,  -129,   116,    37,   867,  -129,   102,  -129,  -129,
    -129,    17,   867,   706,  -129,  -129,   867,    78,  -129,   979,
    -129,   867,  -129,   979,    78,  -129,  -129,  -129,   897,   106,
     118,  -129,  -129,  1104,   777,  -129,   990,   171,   146,   176,
     186,   867,  -129,   867,   182,  -129,   867,   161,  -129,   927,
     867,  1157,  1178,  -129,  -129,   210,   867,   867,   867,   867,
    -129,    74,  -129,  -129,  -129,  -129,  -129,  -129,  -129,  -129,
    -129,    18,    18,    25,    25,    25,    17,   174,  1210,  1210,
    1210,  1210,  1210,  1210,  1210,  1210,  1210,  1210,  1210,  1210,
    1210,  1210,  1001,  -129,  1210,   208,  -129,   184,   224,   225,
      84,  1028,  -129,   643,  1115,  1039,  -129,   212,  -129,  1199,
      87,  -129,   807,  1210,  -129,   237,   239,  -129,   574,   201,
    -129,  -129,  1066,   574,   867,   867,   867,  1210,  1210,   206,
     110,  -129,   403,   492,  -129,   213,   209,   867,  -129,  1210,
     575,   646,   966,  -129,  -129,   867,  -129,   211,    17,   243,
      17,  -129,   215,  -129,  -129,   867,   867,    78,   867,  -129,
       2,   867,   867,    78,  -129,   867,  -129,  -129,   128,   140,
     168,  -129,   512,  -129,  -129,  -129,  -129,  -129,  -129,   221,
      38,  -129,   563,  -129,  -129,    -3,  -129,   223,   177,  1210,
    -129,  1210,   182,  1210,  1210,  -129,  1077,   234,  -129,  -129,
    -129,  -129,  -129,   867,    17,  -129,  -129,   867,  -129,  -129,
      78,    78,  1210,   232,   181,  -129,  -129,  -129,    78,  -129
};

/* YYPGOTO[NTERM-NUM].  */
static const yytype_int16 yypgoto[] =
{
    -129,  -129,   255,  -129,  -129,  -129,  -129,  -129,  -129,    46,
     -38,  -129,   -53,   112,     0,  -129,  -129,  -129,  -129,  -129,
     138,     4,    93,  -101,  -129,    16,  -129,  -129,    32,  -129,
    -129,  -129,  -129,  -129,  -129,  -129,  -129,  -129,    -2,  -129,
    -129,    10,  -129,   -58,  -129,  -128,  -129,  -129,  -129,  -129,
    -129,  -129,  -129,  -129,  -129,  -129,   -35,  -129,  -129,  -129
};

/* YYTABLE[YYPACT[STATE-NUM]].  What to do in state STATE-NUM.  If
   positive, shift that token.  If negative, reduce the rule which
   number is the opposite.  If YYTABLE_NINF, syntax error.  */
#define YYTABLE_NINF -113
static const yytype_int16 yytable[] =
{
      30,   137,    76,    76,    76,    82,   347,    76,    87,   166,
      85,    81,   183,   183,   169,   170,   171,   -84,   175,    46,
      47,    92,   149,   239,   140,    93,    30,     8,     9,    76,
      79,    90,    91,    98,    80,   142,    76,   138,     2,     8,
       9,   141,    79,    46,    47,     2,    29,   145,   184,   332,
     136,    15,    88,   147,   117,   118,   119,   152,   159,   120,
     163,   121,     8,     9,   151,    79,   120,   153,   121,    80,
     173,    87,    29,   176,    96,   156,    97,   178,  -113,   -84,
     121,   157,   168,    46,    47,   270,   186,   146,   283,   148,
     177,   187,   191,   192,   289,   290,   179,   297,   298,   180,
     107,   108,   109,   110,   111,   112,   201,   202,   203,   204,
     205,   206,   207,    76,    76,    76,    76,    76,   195,    76,
     312,   301,   218,   219,   220,   221,   222,   223,   224,   225,
     226,   227,   228,   229,   230,   231,   190,   232,   337,   301,
     193,   194,    75,    77,    78,   234,   200,    84,   216,   142,
     338,   301,   241,   244,   236,   141,   245,   150,   233,   208,
     324,   249,   326,   308,   309,   310,   154,   155,   253,   114,
     158,   160,   162,   247,   262,   257,   144,   167,   339,   301,
     264,   267,   259,   268,   172,   265,   253,   350,   301,   273,
     253,   358,   298,   185,   328,   266,   279,   280,   281,   282,
     115,   116,   117,   118,   119,   304,   269,   120,   271,   121,
     307,     3,     4,     5,   142,   278,   353,   121,     6,   285,
     141,   295,     7,     8,     9,   210,    10,    11,    12,    13,
      14,   286,    15,    16,    17,   287,   288,    18,    19,    20,
      21,   129,   130,   131,   132,   133,   134,   135,   301,   305,
      87,   302,    85,   211,   212,   213,   214,   215,   311,   217,
     317,   318,   325,   323,   253,   253,   253,   327,   341,   246,
     349,   248,   136,    99,     1,   251,   150,   234,   340,   357,
       2,   100,   342,   275,   300,   322,   142,   319,   142,   354,
     348,     0,   141,     0,   141,   253,   329,     0,   331,     0,
       0,   333,   334,     0,     0,   336,     3,     4,     5,     0,
       0,     0,     0,     6,     0,     0,     0,     7,     8,     9,
       1,    10,    11,    12,    13,    14,     2,    15,    16,    17,
       0,     0,    18,    19,    20,    21,     0,     0,     0,     0,
       0,     0,   142,   352,    22,    23,     0,   249,   141,    24,
      25,     0,     3,     4,     5,     0,     0,     0,     0,     6,
       0,     0,     0,     7,     8,     9,   208,    10,    11,    12,
      13,    14,     0,    15,    16,    17,     0,     0,    18,    19,
      20,    21,     0,     0,     0,     0,     0,   181,     0,     0,
      22,    23,     0,     0,     0,    24,    25,   102,   103,   104,
     105,   106,   107,   108,   109,   110,   111,   112,     0,   330,
       0,     0,   -84,     0,   313,   335,   -77,   -77,   -77,   -77,
     -77,   -77,   -77,   182,   129,   130,   131,   132,   133,   134,
     135,     0,   210,   164,     0,     0,     0,    46,    47,     2,
     165,    48,     0,     0,     0,     0,     0,    88,     0,     0,
     314,     0,     0,     0,     0,   136,     0,     0,   185,     0,
       0,     0,   355,   356,     0,     3,     4,     5,     0,     0,
     359,     0,     6,     0,     0,     0,     7,     8,     9,     0,
      10,    11,    12,    13,    14,     0,    15,    16,    17,    49,
      50,    18,    19,    20,    21,    51,    52,    53,    54,    55,
      56,     0,    57,   315,    45,    58,    59,    60,    46,    47,
       2,     0,    48,   102,   103,   104,   105,   106,   107,   108,
     109,   110,   111,   112,     0,  -112,  -112,  -112,  -112,  -112,
    -112,  -112,     0,     0,     0,     0,     3,     4,     5,   316,
       0,     0,     0,     6,     0,     0,     0,     7,     8,     9,
       0,    10,    11,    12,    13,    14,   340,    15,    16,    17,
      49,    50,    18,    19,    20,    21,    51,    52,    53,    54,
      55,    56,     0,    57,   344,   164,    58,    59,    60,    46,
      47,     2,     0,    48,   102,   103,   104,   105,   106,   107,
     108,   109,   110,   111,   112,   321,   102,   103,   104,   105,
     106,   107,   108,   109,   110,   111,   112,     3,     4,     5,
     345,     0,     0,     0,     6,     0,     0,     0,     7,     8,
       9,     0,    10,    11,    12,    13,    14,     0,    15,    16,
      17,    49,    50,    18,    19,    20,    21,    51,    52,    53,
      54,    55,    56,     0,    57,     0,     0,    58,    59,    60,
      46,    47,   -84,     0,   161,     0,   -77,   -77,   -77,   -77,
     -77,   -77,   -77,     0,     0,     0,     0,   292,     0,   104,
     105,   106,   107,   108,   109,   110,   111,   112,     3,     4,
       5,    46,    47,     0,     0,     6,     0,    88,     0,     7,
       8,     9,     0,    10,    11,    12,    13,    14,     0,    15,
      16,    17,     0,     0,    18,    19,    20,    21,     0,     3,
       4,     5,   242,     0,     0,     0,     6,     0,     0,     0,
       7,     8,     9,     0,    10,    11,    12,    13,    14,     0,
      15,    16,    17,     0,     0,    18,    19,    20,    21,     3,
       4,     5,     0,     0,     0,     0,     6,     0,     0,     0,
       7,     8,     9,     0,    10,    11,    12,    13,   243,     0,
      15,    16,    17,     0,     0,    18,    19,    20,    21,   -84,
       0,     0,     0,   -77,   -77,   -77,   -77,   -77,   -77,   -77,
       3,     4,     5,     0,     0,     0,     0,     6,     0,     0,
       0,     7,     8,     9,     0,    73,    11,    12,    13,    74,
      83,    15,    16,    17,    88,     0,    18,    19,    20,    21,
       3,     4,     5,     0,     0,     0,     0,     6,     0,     0,
       0,     7,     8,     9,   261,    10,    11,    12,    13,    14,
       0,    15,    16,    17,     0,     0,    18,    19,    20,    21,
       3,     4,     5,     0,     0,     0,     0,     6,     0,     0,
       0,     7,     8,     9,   299,    10,    11,    12,    13,    14,
       0,    15,    16,    17,     0,     0,    18,    19,    20,    21,
       3,     4,     5,     0,     0,     0,     0,     6,     0,     0,
       0,     7,     8,     9,     0,    73,    11,    12,    13,    74,
       0,    15,    16,    17,     0,     0,    18,    19,    20,    21,
       3,     4,     5,     0,     0,     0,     0,     6,     0,     0,
       0,     7,     8,     9,     0,    10,    11,    12,    13,    14,
       0,    15,    16,    17,     0,     0,    18,    19,    20,    21,
       3,     4,     5,     0,     0,     0,     0,     6,     0,     0,
       0,     7,     8,     9,     0,   252,    11,    12,    13,    14,
       0,    15,    16,    17,     0,     0,    18,    19,    20,    21,
       3,     4,     5,     0,     0,     0,     0,     6,     0,     0,
       0,     7,     8,     9,     0,    10,    11,    12,    13,   272,
       0,    15,    16,    17,    46,    47,    18,    19,    20,    21,
     105,   106,   107,   108,   109,   110,   111,   112,     0,     0,
     102,   103,   104,   105,   106,   107,   108,   109,   110,   111,
     112,   102,   103,   104,   105,   106,   107,   108,   109,   110,
     111,   112,   102,   103,   104,   105,   106,   107,   108,   109,
     110,   111,   112,     0,     0,     0,     0,   182,   122,   123,
     124,   125,   126,   127,   128,     0,     0,     0,   284,   102,
     103,   104,   105,   106,   107,   108,   109,   110,   111,   112,
     102,   103,   104,   105,   106,   107,   108,   109,   110,   111,
     112,     0,     0,     0,     0,   291,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   294,   102,   103,   104,
     105,   106,   107,   108,   109,   110,   111,   112,   102,   103,
     104,   105,   106,   107,   108,   109,   110,   111,   112,     0,
     260,     0,     0,   306,     0,     0,     0,     0,     0,     0,
       0,   293,     0,     0,   351,   102,   103,   104,   105,   106,
     107,   108,   109,   110,   111,   112,   102,   103,   104,   105,
     106,   107,   108,   109,   110,   111,   112,   101,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   102,   103,   104,
     105,   106,   107,   108,   109,   110,   111,   112,   276,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   102,   103,
     104,   105,   106,   107,   108,   109,   110,   111,   112,   277,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   102,
     103,   104,   105,   106,   107,   108,   109,   110,   111,   112,
     296,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     102,   103,   104,   105,   106,   107,   108,   109,   110,   111,
     112,   102,   103,   104,   105,   106,   107,   108,   109,   110,
     111,   112
};

#define yypact_value_is_default(Yystate) \
  (!!((Yystate) == (-129)))

#define yytable_value_is_error(Yytable_value) \
  (!!((Yytable_value) == (-113)))

static const yytype_int16 yycheck[] =
{
       0,    11,     4,     5,     6,     7,     9,     9,    10,    62,
      10,     7,    11,    11,    67,    68,    69,     9,    71,     5,
       6,    48,     8,   151,    42,    48,    26,    45,    46,    31,
      48,    15,    16,    50,    52,    37,    38,    47,     7,    45,
      46,    37,    48,     5,     6,     7,     0,    48,    47,    47,
      44,    54,    44,    48,    36,    37,    38,    48,    58,    41,
      60,    43,    45,    46,    48,    48,    41,    48,    43,    52,
      70,    73,    26,    73,    52,    48,    54,    79,    41,     9,
      43,    52,    66,     5,     6,   186,     9,    41,   216,    43,
      74,    48,    92,    93,    10,    11,    80,    10,    11,    83,
      26,    27,    28,    29,    30,    31,   106,   107,   108,   109,
     110,   111,   112,   115,   116,   117,   118,   119,    50,   121,
      10,    11,   122,   123,   124,   125,   126,   127,   128,   129,
     130,   131,   132,   133,   134,   135,    48,   137,    10,    11,
      94,    95,     4,     5,     6,   145,    52,     9,    33,   151,
      10,    11,   152,   153,    52,   151,   156,    45,    42,   113,
     288,   161,   290,   264,   265,   266,    54,    55,   168,    31,
      58,    59,    60,   157,   174,    69,    38,    65,    10,    11,
       9,   181,    64,   183,     6,     9,   186,    10,    11,   189,
     190,    10,    11,    47,   295,     9,   196,   197,   198,   199,
      34,    35,    36,    37,    38,   258,    24,    41,    47,    43,
     263,    33,    34,    35,   216,     5,   344,    43,    40,    11,
     216,     9,    44,    45,    46,   113,    48,    49,    50,    51,
      52,    47,    54,    55,    56,    11,    11,    59,    60,    61,
      62,    13,    14,    15,    16,    17,    18,    19,    11,    48,
     252,    12,   252,   115,   116,   117,   118,   119,    52,   121,
      47,    52,    19,    52,   264,   265,   266,    52,    47,   157,
      47,   159,    44,     0,     1,   163,   164,   277,    44,    47,
       7,    26,   320,   190,   252,   285,   288,   277,   290,   347,
     325,    -1,   288,    -1,   290,   295,   296,    -1,   298,    -1,
      -1,   301,   302,    -1,    -1,   305,    33,    34,    35,    -1,
      -1,    -1,    -1,    40,    -1,    -1,    -1,    44,    45,    46,
       1,    48,    49,    50,    51,    52,     7,    54,    55,    56,
      -1,    -1,    59,    60,    61,    62,    -1,    -1,    -1,    -1,
      -1,    -1,   344,   343,    71,    72,    -1,   347,   344,    76,
      77,    -1,    33,    34,    35,    -1,    -1,    -1,    -1,    40,
      -1,    -1,    -1,    44,    45,    46,   320,    48,    49,    50,
      51,    52,    -1,    54,    55,    56,    -1,    -1,    59,    60,
      61,    62,    -1,    -1,    -1,    -1,    -1,    11,    -1,    -1,
      71,    72,    -1,    -1,    -1,    76,    77,    21,    22,    23,
      24,    25,    26,    27,    28,    29,    30,    31,    -1,   297,
      -1,    -1,     9,    -1,    11,   303,    13,    14,    15,    16,
      17,    18,    19,    47,    13,    14,    15,    16,    17,    18,
      19,    -1,   320,     1,    -1,    -1,    -1,     5,     6,     7,
       8,     9,    -1,    -1,    -1,    -1,    -1,    44,    -1,    -1,
      47,    -1,    -1,    -1,    -1,    44,    -1,    -1,    47,    -1,
      -1,    -1,   350,   351,    -1,    33,    34,    35,    -1,    -1,
     358,    -1,    40,    -1,    -1,    -1,    44,    45,    46,    -1,
      48,    49,    50,    51,    52,    -1,    54,    55,    56,    57,
      58,    59,    60,    61,    62,    63,    64,    65,    66,    67,
      68,    -1,    70,    11,     1,    73,    74,    75,     5,     6,
       7,    -1,     9,    21,    22,    23,    24,    25,    26,    27,
      28,    29,    30,    31,    -1,    13,    14,    15,    16,    17,
      18,    19,    -1,    -1,    -1,    -1,    33,    34,    35,    47,
      -1,    -1,    -1,    40,    -1,    -1,    -1,    44,    45,    46,
      -1,    48,    49,    50,    51,    52,    44,    54,    55,    56,
      57,    58,    59,    60,    61,    62,    63,    64,    65,    66,
      67,    68,    -1,    70,    11,     1,    73,    74,    75,     5,
       6,     7,    -1,     9,    21,    22,    23,    24,    25,    26,
      27,    28,    29,    30,    31,    20,    21,    22,    23,    24,
      25,    26,    27,    28,    29,    30,    31,    33,    34,    35,
      47,    -1,    -1,    -1,    40,    -1,    -1,    -1,    44,    45,
      46,    -1,    48,    49,    50,    51,    52,    -1,    54,    55,
      56,    57,    58,    59,    60,    61,    62,    63,    64,    65,
      66,    67,    68,    -1,    70,    -1,    -1,    73,    74,    75,
       5,     6,     9,    -1,     9,    -1,    13,    14,    15,    16,
      17,    18,    19,    -1,    -1,    -1,    -1,    24,    -1,    23,
      24,    25,    26,    27,    28,    29,    30,    31,    33,    34,
      35,     5,     6,    -1,    -1,    40,    -1,    44,    -1,    44,
      45,    46,    -1,    48,    49,    50,    51,    52,    -1,    54,
      55,    56,    -1,    -1,    59,    60,    61,    62,    -1,    33,
      34,    35,     6,    -1,    -1,    -1,    40,    -1,    -1,    -1,
      44,    45,    46,    -1,    48,    49,    50,    51,    52,    -1,
      54,    55,    56,    -1,    -1,    59,    60,    61,    62,    33,
      34,    35,    -1,    -1,    -1,    -1,    40,    -1,    -1,    -1,
      44,    45,    46,    -1,    48,    49,    50,    51,    52,    -1,
      54,    55,    56,    -1,    -1,    59,    60,    61,    62,     9,
      -1,    -1,    -1,    13,    14,    15,    16,    17,    18,    19,
      33,    34,    35,    -1,    -1,    -1,    -1,    40,    -1,    -1,
      -1,    44,    45,    46,    -1,    48,    49,    50,    51,    52,
      53,    54,    55,    56,    44,    -1,    59,    60,    61,    62,
      33,    34,    35,    -1,    -1,    -1,    -1,    40,    -1,    -1,
      -1,    44,    45,    46,    47,    48,    49,    50,    51,    52,
      -1,    54,    55,    56,    -1,    -1,    59,    60,    61,    62,
      33,    34,    35,    -1,    -1,    -1,    -1,    40,    -1,    -1,
      -1,    44,    45,    46,    47,    48,    49,    50,    51,    52,
      -1,    54,    55,    56,    -1,    -1,    59,    60,    61,    62,
      33,    34,    35,    -1,    -1,    -1,    -1,    40,    -1,    -1,
      -1,    44,    45,    46,    -1,    48,    49,    50,    51,    52,
      -1,    54,    55,    56,    -1,    -1,    59,    60,    61,    62,
      33,    34,    35,    -1,    -1,    -1,    -1,    40,    -1,    -1,
      -1,    44,    45,    46,    -1,    48,    49,    50,    51,    52,
      -1,    54,    55,    56,    -1,    -1,    59,    60,    61,    62,
      33,    34,    35,    -1,    -1,    -1,    -1,    40,    -1,    -1,
      -1,    44,    45,    46,    -1,    48,    49,    50,    51,    52,
      -1,    54,    55,    56,    -1,    -1,    59,    60,    61,    62,
      33,    34,    35,    -1,    -1,    -1,    -1,    40,    -1,    -1,
      -1,    44,    45,    46,    -1,    48,    49,    50,    51,    52,
      -1,    54,    55,    56,     5,     6,    59,    60,    61,    62,
      24,    25,    26,    27,    28,    29,    30,    31,    -1,    -1,
      21,    22,    23,    24,    25,    26,    27,    28,    29,    30,
      31,    21,    22,    23,    24,    25,    26,    27,    28,    29,
      30,    31,    21,    22,    23,    24,    25,    26,    27,    28,
      29,    30,    31,    -1,    -1,    -1,    -1,    47,    13,    14,
      15,    16,    17,    18,    19,    -1,    -1,    -1,    47,    21,
      22,    23,    24,    25,    26,    27,    28,    29,    30,    31,
      21,    22,    23,    24,    25,    26,    27,    28,    29,    30,
      31,    -1,    -1,    -1,    -1,    47,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    47,    21,    22,    23,
      24,    25,    26,    27,    28,    29,    30,    31,    21,    22,
      23,    24,    25,    26,    27,    28,    29,    30,    31,    -1,
       6,    -1,    -1,    47,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,     6,    -1,    -1,    47,    21,    22,    23,    24,    25,
      26,    27,    28,    29,    30,    31,    21,    22,    23,    24,
      25,    26,    27,    28,    29,    30,    31,    11,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    21,    22,    23,
      24,    25,    26,    27,    28,    29,    30,    31,    11,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    21,    22,
      23,    24,    25,    26,    27,    28,    29,    30,    31,    11,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    21,
      22,    23,    24,    25,    26,    27,    28,    29,    30,    31,
      11,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      21,    22,    23,    24,    25,    26,    27,    28,    29,    30,
      31,    21,    22,    23,    24,    25,    26,    27,    28,    29,
      30,    31
};

/* YYSTOS[STATE-NUM] -- The (internal number of the) accessing
   symbol of state STATE-NUM.  */
static const yytype_uint8 yystos[] =
{
       0,     1,     7,    33,    34,    35,    40,    44,    45,    46,
      48,    49,    50,    51,    52,    54,    55,    56,    59,    60,
      61,    62,    71,    72,    76,    77,    79,    80,    81,    87,
      92,    97,    98,    99,   102,   116,   117,   122,   124,   125,
     127,   128,   129,   132,   134,     1,     5,     6,     9,    57,
      58,    63,    64,    65,    66,    67,    68,    70,    73,    74,
      75,    87,    89,    90,    91,    92,   104,   108,   110,   111,
     112,   115,   120,    48,    52,    98,   116,    98,    98,    48,
      52,    99,   116,    53,    98,    92,   106,   116,    44,   103,
     103,   103,    48,    48,    82,    83,    52,    54,    50,     0,
      80,    11,    21,    22,    23,    24,    25,    26,    27,    28,
      29,    30,    31,    84,    98,    34,    35,    36,    37,    38,
      41,    43,    13,    14,    15,    16,    17,    18,    19,    13,
      14,    15,    16,    17,    18,    19,    44,    11,    47,   118,
      42,    99,   116,   123,    98,    48,    87,    48,    87,     8,
      91,   103,    48,    48,    91,    91,    48,    52,    91,    92,
      91,     9,    91,    92,     1,     8,    90,    91,   103,    90,
      90,    90,     6,    92,   113,    90,    92,   103,   116,   103,
     103,    11,    47,    11,    47,    47,     9,    48,   135,   136,
      48,    92,    92,    87,    87,    50,    85,    95,    93,    94,
      52,    92,    92,    92,    92,    92,    92,    92,    87,    88,
      91,    98,    98,    98,    98,    98,    33,    98,    92,    92,
      92,    92,    92,    92,    92,    92,    92,    92,    92,    92,
      92,    92,    92,    42,    92,   119,    52,   130,   131,   123,
     133,    92,     6,    52,    92,    92,    91,   103,    91,    92,
     121,    91,    48,    92,   100,   101,   105,    69,   109,    64,
       6,    47,    92,   114,     9,     9,     9,    92,    92,    24,
     101,    47,    52,    92,   137,   100,    11,    11,     5,    92,
      92,    92,    92,   123,    47,    11,    47,    11,    11,    10,
      11,    47,    24,     6,    47,     9,    11,    10,    11,    47,
     106,    11,    12,   107,    90,    48,    47,    90,   101,   101,
     101,    52,    10,    11,    47,    11,    47,    47,    52,   119,
      86,    20,    92,    52,   123,    19,   123,    52,   101,    92,
      91,    92,    47,    92,    92,    91,    92,    10,    10,    10,
      44,    47,    88,    96,    11,    47,   126,     9,   134,    47,
      10,    47,    92,   123,   121,    91,    91,    47,    10,    91
};

#define yyerrok		(yyerrstatus = 0)
#define yyclearin	(yychar = YYEMPTY)
#define YYEMPTY		(-2)
#define YYEOF		0

#define YYACCEPT	goto yyacceptlab
#define YYABORT		goto yyabortlab
#define YYERROR		goto yyerrorlab


/* Like YYERROR except do call yyerror.  This remains here temporarily
   to ease the transition to the new meaning of YYERROR, for GCC.
   Once GCC version 2 has supplanted version 1, this can go.  However,
   YYFAIL appears to be in use.  Nevertheless, it is formally deprecated
   in Bison 2.4.2's NEWS entry, where a plan to phase it out is
   discussed.  */

#define YYFAIL		goto yyerrlab
#if defined YYFAIL
  /* This is here to suppress warnings from the GCC cpp's
     -Wunused-macros.  Normally we don't worry about that warning, but
     some users do, and we want to make it easy for users to remove
     YYFAIL uses, which will produce warnings from Bison 2.5.  */
#endif

#define YYRECOVERING()  (!!yyerrstatus)

#define YYBACKUP(Token, Value)                                  \
do                                                              \
  if (yychar == YYEMPTY)                                        \
    {                                                           \
      yychar = (Token);                                         \
      yylval = (Value);                                         \
      YYPOPSTACK (yylen);                                       \
      yystate = *yyssp;                                         \
      goto yybackup;                                            \
    }                                                           \
  else                                                          \
    {                                                           \
      yyerror (YY_("syntax error: cannot back up")); \
      YYERROR;							\
    }								\
while (YYID (0))

/* Error token number */
#define YYTERROR	1
#define YYERRCODE	256


/* This macro is provided for backward compatibility. */
#ifndef YY_LOCATION_PRINT
# define YY_LOCATION_PRINT(File, Loc) ((void) 0)
#endif


/* YYLEX -- calling `yylex' with the right arguments.  */
#ifdef YYLEX_PARAM
# define YYLEX yylex (YYLEX_PARAM)
#else
# define YYLEX yylex ()
#endif

/* Enable debugging if requested.  */
#if YYDEBUG

# ifndef YYFPRINTF
#  include <stdio.h> /* INFRINGES ON USER NAME SPACE */
#  define YYFPRINTF fprintf
# endif

# define YYDPRINTF(Args)			\
do {						\
  if (yydebug)					\
    YYFPRINTF Args;				\
} while (YYID (0))

# define YY_SYMBOL_PRINT(Title, Type, Value, Location)			  \
do {									  \
  if (yydebug)								  \
    {									  \
      YYFPRINTF (stderr, "%s ", Title);					  \
      yy_symbol_print (stderr,						  \
		  Type, Value); \
      YYFPRINTF (stderr, "\n");						  \
    }									  \
} while (YYID (0))


/*--------------------------------.
| Print this symbol on YYOUTPUT.  |
`--------------------------------*/

/*ARGSUSED*/
#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
static void
yy_symbol_value_print (FILE *yyoutput, int yytype, YYSTYPE const * const yyvaluep)
#else
static void
yy_symbol_value_print (yyoutput, yytype, yyvaluep)
    FILE *yyoutput;
    int yytype;
    YYSTYPE const * const yyvaluep;
#endif
{
  FILE *yyo = yyoutput;
  YYUSE (yyo);
  if (!yyvaluep)
    return;
# ifdef YYPRINT
  if (yytype < YYNTOKENS)
    YYPRINT (yyoutput, yytoknum[yytype], *yyvaluep);
# else
  YYUSE (yyoutput);
# endif
  YYUSE (yytype);
}


/*--------------------------------.
| Print this symbol on YYOUTPUT.  |
`--------------------------------*/

#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
static void
yy_symbol_print (FILE *yyoutput, int yytype, YYSTYPE const * const yyvaluep)
#else
static void
yy_symbol_print (yyoutput, yytype, yyvaluep)
    FILE *yyoutput;
    int yytype;
    YYSTYPE const * const yyvaluep;
#endif
{
  if (yytype < YYNTOKENS)
    YYFPRINTF (yyoutput, "token %s (", yytname[yytype]);
  else
    YYFPRINTF (yyoutput, "nterm %s (", yytname[yytype]);

  yy_symbol_value_print (yyoutput, yytype, yyvaluep);
  YYFPRINTF (yyoutput, ")");
}

/*------------------------------------------------------------------.
| yy_stack_print -- Print the state stack from its BOTTOM up to its |
| TOP (included).                                                   |
`------------------------------------------------------------------*/

#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
static void
yy_stack_print (yytype_int16 *yybottom, yytype_int16 *yytop)
#else
static void
yy_stack_print (yybottom, yytop)
    yytype_int16 *yybottom;
    yytype_int16 *yytop;
#endif
{
  YYFPRINTF (stderr, "Stack now");
  for (; yybottom <= yytop; yybottom++)
    {
      int yybot = *yybottom;
      YYFPRINTF (stderr, " %d", yybot);
    }
  YYFPRINTF (stderr, "\n");
}

# define YY_STACK_PRINT(Bottom, Top)				\
do {								\
  if (yydebug)							\
    yy_stack_print ((Bottom), (Top));				\
} while (YYID (0))


/*------------------------------------------------.
| Report that the YYRULE is going to be reduced.  |
`------------------------------------------------*/

#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
static void
yy_reduce_print (YYSTYPE *yyvsp, int yyrule)
#else
static void
yy_reduce_print (yyvsp, yyrule)
    YYSTYPE *yyvsp;
    int yyrule;
#endif
{
  int yynrhs = yyr2[yyrule];
  int yyi;
  unsigned long int yylno = yyrline[yyrule];
  YYFPRINTF (stderr, "Reducing stack by rule %d (line %lu):\n",
	     yyrule - 1, yylno);
  /* The symbols being reduced.  */
  for (yyi = 0; yyi < yynrhs; yyi++)
    {
      YYFPRINTF (stderr, "   $%d = ", yyi + 1);
      yy_symbol_print (stderr, yyrhs[yyprhs[yyrule] + yyi],
		       &(yyvsp[(yyi + 1) - (yynrhs)])
		       		       );
      YYFPRINTF (stderr, "\n");
    }
}

# define YY_REDUCE_PRINT(Rule)		\
do {					\
  if (yydebug)				\
    yy_reduce_print (yyvsp, Rule); \
} while (YYID (0))

/* Nonzero means print parse trace.  It is left uninitialized so that
   multiple parsers can coexist.  */
int yydebug;
#else /* !YYDEBUG */
# define YYDPRINTF(Args)
# define YY_SYMBOL_PRINT(Title, Type, Value, Location)
# define YY_STACK_PRINT(Bottom, Top)
# define YY_REDUCE_PRINT(Rule)
#endif /* !YYDEBUG */


/* YYINITDEPTH -- initial size of the parser's stacks.  */
#ifndef	YYINITDEPTH
# define YYINITDEPTH 200
#endif

/* YYMAXDEPTH -- maximum size the stacks can grow to (effective only
   if the built-in stack extension method is used).

   Do not make this value too large; the results are undefined if
   YYSTACK_ALLOC_MAXIMUM < YYSTACK_BYTES (YYMAXDEPTH)
   evaluated with infinite-precision integer arithmetic.  */

#ifndef YYMAXDEPTH
# define YYMAXDEPTH 10000
#endif


#if YYERROR_VERBOSE

# ifndef yystrlen
#  if defined __GLIBC__ && defined _STRING_H
#   define yystrlen strlen
#  else
/* Return the length of YYSTR.  */
#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
static YYSIZE_T
yystrlen (const char *yystr)
#else
static YYSIZE_T
yystrlen (yystr)
    const char *yystr;
#endif
{
  YYSIZE_T yylen;
  for (yylen = 0; yystr[yylen]; yylen++)
    continue;
  return yylen;
}
#  endif
# endif

# ifndef yystpcpy
#  if defined __GLIBC__ && defined _STRING_H && defined _GNU_SOURCE
#   define yystpcpy stpcpy
#  else
/* Copy YYSRC to YYDEST, returning the address of the terminating '\0' in
   YYDEST.  */
#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
static char *
yystpcpy (char *yydest, const char *yysrc)
#else
static char *
yystpcpy (yydest, yysrc)
    char *yydest;
    const char *yysrc;
#endif
{
  char *yyd = yydest;
  const char *yys = yysrc;

  while ((*yyd++ = *yys++) != '\0')
    continue;

  return yyd - 1;
}
#  endif
# endif

# ifndef yytnamerr
/* Copy to YYRES the contents of YYSTR after stripping away unnecessary
   quotes and backslashes, so that it's suitable for yyerror.  The
   heuristic is that double-quoting is unnecessary unless the string
   contains an apostrophe, a comma, or backslash (other than
   backslash-backslash).  YYSTR is taken from yytname.  If YYRES is
   null, do not copy; instead, return the length of what the result
   would have been.  */
static YYSIZE_T
yytnamerr (char *yyres, const char *yystr)
{
  if (*yystr == '"')
    {
      YYSIZE_T yyn = 0;
      char const *yyp = yystr;

      for (;;)
	switch (*++yyp)
	  {
	  case '\'':
	  case ',':
	    goto do_not_strip_quotes;

	  case '\\':
	    if (*++yyp != '\\')
	      goto do_not_strip_quotes;
	    /* Fall through.  */
	  default:
	    if (yyres)
	      yyres[yyn] = *yyp;
	    yyn++;
	    break;

	  case '"':
	    if (yyres)
	      yyres[yyn] = '\0';
	    return yyn;
	  }
    do_not_strip_quotes: ;
    }

  if (! yyres)
    return yystrlen (yystr);

  return yystpcpy (yyres, yystr) - yyres;
}
# endif

/* Copy into *YYMSG, which is of size *YYMSG_ALLOC, an error message
   about the unexpected token YYTOKEN for the state stack whose top is
   YYSSP.

   Return 0 if *YYMSG was successfully written.  Return 1 if *YYMSG is
   not large enough to hold the message.  In that case, also set
   *YYMSG_ALLOC to the required number of bytes.  Return 2 if the
   required number of bytes is too large to store.  */
static int
yysyntax_error (YYSIZE_T *yymsg_alloc, char **yymsg,
                yytype_int16 *yyssp, int yytoken)
{
  YYSIZE_T yysize0 = yytnamerr (YY_NULL, yytname[yytoken]);
  YYSIZE_T yysize = yysize0;
  enum { YYERROR_VERBOSE_ARGS_MAXIMUM = 5 };
  /* Internationalized format string. */
  const char *yyformat = YY_NULL;
  /* Arguments of yyformat. */
  char const *yyarg[YYERROR_VERBOSE_ARGS_MAXIMUM];
  /* Number of reported tokens (one for the "unexpected", one per
     "expected"). */
  int yycount = 0;

  /* There are many possibilities here to consider:
     - Assume YYFAIL is not used.  It's too flawed to consider.  See
       <http://lists.gnu.org/archive/html/bison-patches/2009-12/msg00024.html>
       for details.  YYERROR is fine as it does not invoke this
       function.
     - If this state is a consistent state with a default action, then
       the only way this function was invoked is if the default action
       is an error action.  In that case, don't check for expected
       tokens because there are none.
     - The only way there can be no lookahead present (in yychar) is if
       this state is a consistent state with a default action.  Thus,
       detecting the absence of a lookahead is sufficient to determine
       that there is no unexpected or expected token to report.  In that
       case, just report a simple "syntax error".
     - Don't assume there isn't a lookahead just because this state is a
       consistent state with a default action.  There might have been a
       previous inconsistent state, consistent state with a non-default
       action, or user semantic action that manipulated yychar.
     - Of course, the expected token list depends on states to have
       correct lookahead information, and it depends on the parser not
       to perform extra reductions after fetching a lookahead from the
       scanner and before detecting a syntax error.  Thus, state merging
       (from LALR or IELR) and default reductions corrupt the expected
       token list.  However, the list is correct for canonical LR with
       one exception: it will still contain any token that will not be
       accepted due to an error action in a later state.
  */
  if (yytoken != YYEMPTY)
    {
      int yyn = yypact[*yyssp];
      yyarg[yycount++] = yytname[yytoken];
      if (!yypact_value_is_default (yyn))
        {
          /* Start YYX at -YYN if negative to avoid negative indexes in
             YYCHECK.  In other words, skip the first -YYN actions for
             this state because they are default actions.  */
          int yyxbegin = yyn < 0 ? -yyn : 0;
          /* Stay within bounds of both yycheck and yytname.  */
          int yychecklim = YYLAST - yyn + 1;
          int yyxend = yychecklim < YYNTOKENS ? yychecklim : YYNTOKENS;
          int yyx;

          for (yyx = yyxbegin; yyx < yyxend; ++yyx)
            if (yycheck[yyx + yyn] == yyx && yyx != YYTERROR
                && !yytable_value_is_error (yytable[yyx + yyn]))
              {
                if (yycount == YYERROR_VERBOSE_ARGS_MAXIMUM)
                  {
                    yycount = 1;
                    yysize = yysize0;
                    break;
                  }
                yyarg[yycount++] = yytname[yyx];
                {
                  YYSIZE_T yysize1 = yysize + yytnamerr (YY_NULL, yytname[yyx]);
                  if (! (yysize <= yysize1
                         && yysize1 <= YYSTACK_ALLOC_MAXIMUM))
                    return 2;
                  yysize = yysize1;
                }
              }
        }
    }

  switch (yycount)
    {
# define YYCASE_(N, S)                      \
      case N:                               \
        yyformat = S;                       \
      break
      YYCASE_(0, YY_("syntax error"));
      YYCASE_(1, YY_("syntax error, unexpected %s"));
      YYCASE_(2, YY_("syntax error, unexpected %s, expecting %s"));
      YYCASE_(3, YY_("syntax error, unexpected %s, expecting %s or %s"));
      YYCASE_(4, YY_("syntax error, unexpected %s, expecting %s or %s or %s"));
      YYCASE_(5, YY_("syntax error, unexpected %s, expecting %s or %s or %s or %s"));
# undef YYCASE_
    }

  {
    YYSIZE_T yysize1 = yysize + yystrlen (yyformat);
    if (! (yysize <= yysize1 && yysize1 <= YYSTACK_ALLOC_MAXIMUM))
      return 2;
    yysize = yysize1;
  }

  if (*yymsg_alloc < yysize)
    {
      *yymsg_alloc = 2 * yysize;
      if (! (yysize <= *yymsg_alloc
             && *yymsg_alloc <= YYSTACK_ALLOC_MAXIMUM))
        *yymsg_alloc = YYSTACK_ALLOC_MAXIMUM;
      return 1;
    }

  /* Avoid sprintf, as that infringes on the user's name space.
     Don't have undefined behavior even if the translation
     produced a string with the wrong number of "%s"s.  */
  {
    char *yyp = *yymsg;
    int yyi = 0;
    while ((*yyp = *yyformat) != '\0')
      if (*yyp == '%' && yyformat[1] == 's' && yyi < yycount)
        {
          yyp += yytnamerr (yyp, yyarg[yyi++]);
          yyformat += 2;
        }
      else
        {
          yyp++;
          yyformat++;
        }
  }
  return 0;
}
#endif /* YYERROR_VERBOSE */

/*-----------------------------------------------.
| Release the memory associated to this symbol.  |
`-----------------------------------------------*/

/*ARGSUSED*/
#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
static void
yydestruct (const char *yymsg, int yytype, YYSTYPE *yyvaluep)
#else
static void
yydestruct (yymsg, yytype, yyvaluep)
    const char *yymsg;
    int yytype;
    YYSTYPE *yyvaluep;
#endif
{
  YYUSE (yyvaluep);

  if (!yymsg)
    yymsg = "Deleting";
  YY_SYMBOL_PRINT (yymsg, yytype, yyvaluep, yylocationp);

  YYUSE (yytype);
}




/* The lookahead symbol.  */
int yychar;


#ifndef YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
# define YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
# define YY_IGNORE_MAYBE_UNINITIALIZED_END
#endif
#ifndef YY_INITIAL_VALUE
# define YY_INITIAL_VALUE(Value) /* Nothing. */
#endif

/* The semantic value of the lookahead symbol.  */
YYSTYPE yylval YY_INITIAL_VALUE(yyval_default);

/* Number of syntax errors so far.  */
int yynerrs;


/*----------.
| yyparse.  |
`----------*/

#ifdef YYPARSE_PARAM
#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
int
yyparse (void *YYPARSE_PARAM)
#else
int
yyparse (YYPARSE_PARAM)
    void *YYPARSE_PARAM;
#endif
#else /* ! YYPARSE_PARAM */
#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
int
yyparse (void)
#else
int
yyparse ()

#endif
#endif
{
    int yystate;
    /* Number of tokens to shift before error messages enabled.  */
    int yyerrstatus;

    /* The stacks and their tools:
       `yyss': related to states.
       `yyvs': related to semantic values.

       Refer to the stacks through separate pointers, to allow yyoverflow
       to reallocate them elsewhere.  */

    /* The state stack.  */
    yytype_int16 yyssa[YYINITDEPTH];
    yytype_int16 *yyss;
    yytype_int16 *yyssp;

    /* The semantic value stack.  */
    YYSTYPE yyvsa[YYINITDEPTH];
    YYSTYPE *yyvs;
    YYSTYPE *yyvsp;

    YYSIZE_T yystacksize;

  int yyn;
  int yyresult;
  /* Lookahead token as an internal (translated) token number.  */
  int yytoken = 0;
  /* The variables used to return semantic value and location from the
     action routines.  */
  YYSTYPE yyval;

#if YYERROR_VERBOSE
  /* Buffer for error messages, and its allocated size.  */
  char yymsgbuf[128];
  char *yymsg = yymsgbuf;
  YYSIZE_T yymsg_alloc = sizeof yymsgbuf;
#endif

#define YYPOPSTACK(N)   (yyvsp -= (N), yyssp -= (N))

  /* The number of symbols on the RHS of the reduced rule.
     Keep to zero when no symbol should be popped.  */
  int yylen = 0;

  yyssp = yyss = yyssa;
  yyvsp = yyvs = yyvsa;
  yystacksize = YYINITDEPTH;

  YYDPRINTF ((stderr, "Starting parse\n"));

  yystate = 0;
  yyerrstatus = 0;
  yynerrs = 0;
  yychar = YYEMPTY; /* Cause a token to be read.  */
  goto yysetstate;

/*------------------------------------------------------------.
| yynewstate -- Push a new state, which is found in yystate.  |
`------------------------------------------------------------*/
 yynewstate:
  /* In all cases, when you get here, the value and location stacks
     have just been pushed.  So pushing a state here evens the stacks.  */
  yyssp++;

 yysetstate:
  *yyssp = yystate;

  if (yyss + yystacksize - 1 <= yyssp)
    {
      /* Get the current used size of the three stacks, in elements.  */
      YYSIZE_T yysize = yyssp - yyss + 1;

#ifdef yyoverflow
      {
	/* Give user a chance to reallocate the stack.  Use copies of
	   these so that the &'s don't force the real ones into
	   memory.  */
	YYSTYPE *yyvs1 = yyvs;
	yytype_int16 *yyss1 = yyss;

	/* Each stack pointer address is followed by the size of the
	   data in use in that stack, in bytes.  This used to be a
	   conditional around just the two extra args, but that might
	   be undefined if yyoverflow is a macro.  */
	yyoverflow (YY_("memory exhausted"),
		    &yyss1, yysize * sizeof (*yyssp),
		    &yyvs1, yysize * sizeof (*yyvsp),
		    &yystacksize);

	yyss = yyss1;
	yyvs = yyvs1;
      }
#else /* no yyoverflow */
# ifndef YYSTACK_RELOCATE
      goto yyexhaustedlab;
# else
      /* Extend the stack our own way.  */
      if (YYMAXDEPTH <= yystacksize)
	goto yyexhaustedlab;
      yystacksize *= 2;
      if (YYMAXDEPTH < yystacksize)
	yystacksize = YYMAXDEPTH;

      {
	yytype_int16 *yyss1 = yyss;
	union yyalloc *yyptr =
	  (union yyalloc *) YYSTACK_ALLOC (YYSTACK_BYTES (yystacksize));
	if (! yyptr)
	  goto yyexhaustedlab;
	YYSTACK_RELOCATE (yyss_alloc, yyss);
	YYSTACK_RELOCATE (yyvs_alloc, yyvs);
#  undef YYSTACK_RELOCATE
	if (yyss1 != yyssa)
	  YYSTACK_FREE (yyss1);
      }
# endif
#endif /* no yyoverflow */

      yyssp = yyss + yysize - 1;
      yyvsp = yyvs + yysize - 1;

      YYDPRINTF ((stderr, "Stack size increased to %lu\n",
		  (unsigned long int) yystacksize));

      if (yyss + yystacksize - 1 <= yyssp)
	YYABORT;
    }

  YYDPRINTF ((stderr, "Entering state %d\n", yystate));

  if (yystate == YYFINAL)
    YYACCEPT;

  goto yybackup;

/*-----------.
| yybackup.  |
`-----------*/
yybackup:

  /* Do appropriate processing given the current state.  Read a
     lookahead token if we need one and don't already have one.  */

  /* First try to decide what to do without reference to lookahead token.  */
  yyn = yypact[yystate];
  if (yypact_value_is_default (yyn))
    goto yydefault;

  /* Not known => get a lookahead token if don't already have one.  */

  /* YYCHAR is either YYEMPTY or YYEOF or a valid lookahead symbol.  */
  if (yychar == YYEMPTY)
    {
      YYDPRINTF ((stderr, "Reading a token: "));
      yychar = YYLEX;
    }

  if (yychar <= YYEOF)
    {
      yychar = yytoken = YYEOF;
      YYDPRINTF ((stderr, "Now at end of input.\n"));
    }
  else
    {
      yytoken = YYTRANSLATE (yychar);
      YY_SYMBOL_PRINT ("Next token is", yytoken, &yylval, &yylloc);
    }

  /* If the proper action on seeing token YYTOKEN is to reduce or to
     detect an error, take that action.  */
  yyn += yytoken;
  if (yyn < 0 || YYLAST < yyn || yycheck[yyn] != yytoken)
    goto yydefault;
  yyn = yytable[yyn];
  if (yyn <= 0)
    {
      if (yytable_value_is_error (yyn))
        goto yyerrlab;
      yyn = -yyn;
      goto yyreduce;
    }

  /* Count tokens shifted since error; after three, turn off error
     status.  */
  if (yyerrstatus)
    yyerrstatus--;

  /* Shift the lookahead token.  */
  YY_SYMBOL_PRINT ("Shifting", yytoken, &yylval, &yylloc);

  /* Discard the shifted token.  */
  yychar = YYEMPTY;

  yystate = yyn;
  YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
  *++yyvsp = yylval;
  YY_IGNORE_MAYBE_UNINITIALIZED_END

  goto yynewstate;


/*-----------------------------------------------------------.
| yydefault -- do the default action for the current state.  |
`-----------------------------------------------------------*/
yydefault:
  yyn = yydefact[yystate];
  if (yyn == 0)
    goto yyerrlab;
  goto yyreduce;


/*-----------------------------.
| yyreduce -- Do a reduction.  |
`-----------------------------*/
yyreduce:
  /* yyn is the number of a rule to reduce with.  */
  yylen = yyr2[yyn];

  /* If YYLEN is nonzero, implement the default value of the action:
     `$$ = $1'.

     Otherwise, the following line sets YYVAL to garbage.
     This behavior is undocumented and Bison
     users should not rely upon it.  Assigning to YYVAL
     unconditionally makes the parser a bit smaller, and it avoids a
     GCC warning that YYVAL may be used uninitialized.  */
  yyval = yyvsp[1-yylen];


  YY_REDUCE_PRINT (yyn);
  switch (yyn)
    {
        case 7:
/* Line 1787 of yacc.c  */
#line 149 "parse.y"
    {
		load(((STRING*) (yyvsp[(2) - (4)].ptr))->str, ((STRING*)(yyvsp[(3) - (4)].ptr))->str);
		}
    break;

  case 8:
/* Line 1787 of yacc.c  */
#line 155 "parse.y"
    { /* this do nothing action removes a vacuous warning
                  from Bison */
             }
    break;

  case 9:
/* Line 1787 of yacc.c  */
#line 160 "parse.y"
    { be_setup(scope = SCOPE_BEGIN) ; }
    break;

  case 10:
/* Line 1787 of yacc.c  */
#line 163 "parse.y"
    { switch_code_to_main() ; }
    break;

  case 11:
/* Line 1787 of yacc.c  */
#line 166 "parse.y"
    { be_setup(scope = SCOPE_END) ; }
    break;

  case 12:
/* Line 1787 of yacc.c  */
#line 169 "parse.y"
    { switch_code_to_main() ; }
    break;

  case 13:
/* Line 1787 of yacc.c  */
#line 172 "parse.y"
    { code_jmp(_JZ, (INST*)0) ; }
    break;

  case 14:
/* Line 1787 of yacc.c  */
#line 175 "parse.y"
    { patch_jmp( code_ptr ) ; }
    break;

  case 15:
/* Line 1787 of yacc.c  */
#line 179 "parse.y"
    { 
	       INST *p1 = CDP((yyvsp[(1) - (2)].start)) ;
             int len ;

	       code_push(p1, code_ptr - p1, scope, active_funct) ;
               code_ptr = p1 ;

               code2op(_RANGE, 1) ;
               code_ptr += 3 ;
               len = code_pop(code_ptr) ;
             code_ptr += len ;
               code1(_STOP) ;
             p1 = CDP((yyvsp[(1) - (2)].start)) ;
               p1[2].op = code_ptr - (p1+1) ;
             }
    break;

  case 16:
/* Line 1787 of yacc.c  */
#line 195 "parse.y"
    { code1(_STOP) ; }
    break;

  case 17:
/* Line 1787 of yacc.c  */
#line 198 "parse.y"
    { 
	       INST *p1 = CDP((yyvsp[(1) - (6)].start)) ;
	       
	       p1[3].op = CDP((yyvsp[(6) - (6)].start)) - (p1+1) ;
               p1[4].op = code_ptr - (p1+1) ;
             }
    break;

  case 18:
/* Line 1787 of yacc.c  */
#line 209 "parse.y"
    { (yyval.start) = (yyvsp[(2) - (3)].start) ; }
    break;

  case 19:
/* Line 1787 of yacc.c  */
#line 211 "parse.y"
    { (yyval.start) = code_offset ; /* does nothing won't be executed */
              print_flag = getline_flag = paren_cnt = 0 ;
              yyerrok ; }
    break;

  case 21:
/* Line 1787 of yacc.c  */
#line 218 "parse.y"
    { (yyval.start) = code_offset ;
                       code1(_PUSHINT) ; code1(0) ;
                       code2(_PRINT, bi_print) ;
                     }
    break;

  case 25:
/* Line 1787 of yacc.c  */
#line 231 "parse.y"
    { code1(_POP) ; }
    break;

  case 26:
/* Line 1787 of yacc.c  */
#line 233 "parse.y"
    { (yyval.start) = code_offset ; }
    break;

  case 27:
/* Line 1787 of yacc.c  */
#line 235 "parse.y"
    { (yyval.start) = code_offset ;
                print_flag = getline_flag = 0 ;
                paren_cnt = 0 ;
                yyerrok ;
              }
    break;

  case 28:
/* Line 1787 of yacc.c  */
#line 241 "parse.y"
    { (yyval.start) = code_offset ; BC_insert('B', code_ptr+1) ;
               code2(_JMP, 0) /* don't use code_jmp ! */ ; }
    break;

  case 29:
/* Line 1787 of yacc.c  */
#line 244 "parse.y"
    { (yyval.start) = code_offset ; BC_insert('C', code_ptr+1) ;
               code2(_JMP, 0) ; }
    break;

  case 30:
/* Line 1787 of yacc.c  */
#line 247 "parse.y"
    { if ( scope != SCOPE_FUNCT )
                     compile_error("return outside function body") ;
             }
    break;

  case 31:
/* Line 1787 of yacc.c  */
#line 251 "parse.y"
    { if ( scope != SCOPE_MAIN )
                   compile_error( "improper use of next" ) ;
                (yyval.start) = code_offset ; 
                code1(_NEXT) ;
              }
    break;

  case 35:
/* Line 1787 of yacc.c  */
#line 262 "parse.y"
    { code1(_ASSIGN) ; }
    break;

  case 36:
/* Line 1787 of yacc.c  */
#line 263 "parse.y"
    { code1(_ADD_ASG) ; }
    break;

  case 37:
/* Line 1787 of yacc.c  */
#line 264 "parse.y"
    { code1(_SUB_ASG) ; }
    break;

  case 38:
/* Line 1787 of yacc.c  */
#line 265 "parse.y"
    { code1(_MUL_ASG) ; }
    break;

  case 39:
/* Line 1787 of yacc.c  */
#line 266 "parse.y"
    { code1(_DIV_ASG) ; }
    break;

  case 40:
/* Line 1787 of yacc.c  */
#line 267 "parse.y"
    { code1(_MOD_ASG) ; }
    break;

  case 41:
/* Line 1787 of yacc.c  */
#line 268 "parse.y"
    { code1(_POW_ASG) ; }
    break;

  case 42:
/* Line 1787 of yacc.c  */
#line 269 "parse.y"
    { code1(_EQ) ; }
    break;

  case 43:
/* Line 1787 of yacc.c  */
#line 270 "parse.y"
    { code1(_NEQ) ; }
    break;

  case 44:
/* Line 1787 of yacc.c  */
#line 271 "parse.y"
    { code1(_LT) ; }
    break;

  case 45:
/* Line 1787 of yacc.c  */
#line 272 "parse.y"
    { code1(_LTE) ; }
    break;

  case 46:
/* Line 1787 of yacc.c  */
#line 273 "parse.y"
    { code1(_GT) ; }
    break;

  case 47:
/* Line 1787 of yacc.c  */
#line 274 "parse.y"
    { code1(_GTE) ; }
    break;

  case 48:
/* Line 1787 of yacc.c  */
#line 276 "parse.y"
    {
	    INST *p3 = CDP((yyvsp[(3) - (3)].start)) ;

            if ( p3 == code_ptr - 2 ) {
               if ( p3->op == _MATCH0 )  p3->op = _MATCH1 ;

               else /* check for string */
               if ( p3->op == _PUSHS ) {
		   CELL *cp = ZMALLOC(CELL);

                 cp->type = C_STRING; 
                 cp->ptr = p3[1].ptr;
                 cast_to_RE(cp);
                 code_ptr -= 2;
                 code2(_MATCH1, cp->ptr);
                 ZFREE(cp);
               } else {
		   code1(_MATCH2);
	       }
	    } else {
		code1(_MATCH2);
	    }

	    if ( !(yyvsp[(2) - (3)].ival) ) code1(_NOT) ;
      }
    break;

  case 49:
/* Line 1787 of yacc.c  */
#line 304 "parse.y"
    { code1(_TEST) ;
                code_jmp(_LJNZ, (INST*)0) ;
              }
    break;

  case 50:
/* Line 1787 of yacc.c  */
#line 308 "parse.y"
    { code1(_TEST) ; patch_jmp(code_ptr) ; }
    break;

  case 51:
/* Line 1787 of yacc.c  */
#line 311 "parse.y"
    { code1(_TEST) ; 
		code_jmp(_LJZ, (INST*)0) ;
	      }
    break;

  case 52:
/* Line 1787 of yacc.c  */
#line 315 "parse.y"
    { code1(_TEST) ; patch_jmp(code_ptr) ; }
    break;

  case 53:
/* Line 1787 of yacc.c  */
#line 317 "parse.y"
    { code_jmp(_JZ, (INST*)0) ; }
    break;

  case 54:
/* Line 1787 of yacc.c  */
#line 318 "parse.y"
    { code_jmp(_JMP, (INST*)0) ; }
    break;

  case 55:
/* Line 1787 of yacc.c  */
#line 320 "parse.y"
    { patch_jmp(code_ptr) ; patch_jmp(CDP((yyvsp[(7) - (7)].start))) ; }
    break;

  case 57:
/* Line 1787 of yacc.c  */
#line 325 "parse.y"
    { code1(_CAT) ; }
    break;

  case 58:
/* Line 1787 of yacc.c  */
#line 329 "parse.y"
    {  (yyval.start) = code_offset ;
                 code2(_PUSHD,     (yyvsp[(1) - (1)].ptr)); code1(SAOdtype); break;
          }
    break;

  case 59:
/* Line 1787 of yacc.c  */
#line 333 "parse.y"
    { (yyval.start) = code_offset ; code2(_PUSHS, (yyvsp[(1) - (1)].ptr)) ; }
    break;

  case 60:
/* Line 1787 of yacc.c  */
#line 335 "parse.y"
    { check_var((yyvsp[(1) - (1)].stp)) ;
            (yyval.start) = code_offset ;
            if ( is_local((yyvsp[(1) - (1)].stp)) )
            { code2op(L_PUSHI, (yyvsp[(1) - (1)].stp)->offset) ; }
            else code2(_PUSHI, (yyvsp[(1) - (1)].stp)->stval.cp) ;
          }
    break;

  case 61:
/* Line 1787 of yacc.c  */
#line 343 "parse.y"
    { (yyval.start) = (yyvsp[(2) - (3)].start) ; }
    break;

  case 62:
/* Line 1787 of yacc.c  */
#line 347 "parse.y"
    { (yyval.start) = code_offset ; code2(_MATCH0, (yyvsp[(1) - (1)].ptr)) ; }
    break;

  case 63:
/* Line 1787 of yacc.c  */
#line 350 "parse.y"
    { code1(_ADD) ; }
    break;

  case 64:
/* Line 1787 of yacc.c  */
#line 351 "parse.y"
    { code1(_SUB) ; }
    break;

  case 65:
/* Line 1787 of yacc.c  */
#line 352 "parse.y"
    { code1(_MUL) ; }
    break;

  case 66:
/* Line 1787 of yacc.c  */
#line 353 "parse.y"
    { code1(_DIV) ; }
    break;

  case 67:
/* Line 1787 of yacc.c  */
#line 354 "parse.y"
    { code1(_MOD) ; }
    break;

  case 68:
/* Line 1787 of yacc.c  */
#line 355 "parse.y"
    { code1(_POW) ; }
    break;

  case 69:
/* Line 1787 of yacc.c  */
#line 357 "parse.y"
    { (yyval.start) = (yyvsp[(2) - (2)].start) ; code1(_NOT) ; }
    break;

  case 70:
/* Line 1787 of yacc.c  */
#line 359 "parse.y"
    { (yyval.start) = (yyvsp[(2) - (2)].start) ; code1(_UPLUS) ; }
    break;

  case 71:
/* Line 1787 of yacc.c  */
#line 361 "parse.y"
    { (yyval.start) = (yyvsp[(2) - (2)].start) ; code1(_UMINUS) ; }
    break;

  case 73:
/* Line 1787 of yacc.c  */
#line 366 "parse.y"
    { check_var((yyvsp[(1) - (2)].stp)) ;
             (yyval.start) = code_offset ;
             code_address((yyvsp[(1) - (2)].stp)) ;

             if ( (yyvsp[(2) - (2)].ival) == '+' )  code1(_POST_INC) ;
             else  code1(_POST_DEC) ;
           }
    break;

  case 74:
/* Line 1787 of yacc.c  */
#line 374 "parse.y"
    { (yyval.start) = (yyvsp[(2) - (2)].start) ; 
              if ( (yyvsp[(1) - (2)].ival) == '+' ) code1(_PRE_INC) ;
              else  code1(_PRE_DEC) ;
            }
    break;

  case 75:
/* Line 1787 of yacc.c  */
#line 381 "parse.y"
    { if ((yyvsp[(2) - (2)].ival) == '+' ) code1(F_POST_INC ) ; 
             else  code1(F_POST_DEC) ;
           }
    break;

  case 76:
/* Line 1787 of yacc.c  */
#line 385 "parse.y"
    { (yyval.start) = (yyvsp[(2) - (2)].start) ; 
             if ( (yyvsp[(1) - (2)].ival) == '+' ) code1(F_PRE_INC) ;
             else  code1( F_PRE_DEC) ; 
           }
    break;

  case 77:
/* Line 1787 of yacc.c  */
#line 392 "parse.y"
    { (yyval.start) = code_offset ; 
          check_var((yyvsp[(1) - (1)].stp)) ;
          code_address((yyvsp[(1) - (1)].stp)) ;
        }
    break;

  case 78:
/* Line 1787 of yacc.c  */
#line 400 "parse.y"
    { (yyval.ival) = 0 ; }
    break;

  case 80:
/* Line 1787 of yacc.c  */
#line 405 "parse.y"
    { (yyval.ival) = 1 ; }
    break;

  case 81:
/* Line 1787 of yacc.c  */
#line 407 "parse.y"
    { (yyval.ival) = (yyvsp[(1) - (3)].ival) + 1 ; }
    break;

  case 82:
/* Line 1787 of yacc.c  */
#line 412 "parse.y"
    { BI_REC *p = (yyvsp[(1) - (5)].bip) ;
          (yyval.start) = (yyvsp[(2) - (5)].start) ;
          if ( (int)p->min_args > (yyvsp[(4) - (5)].ival) || (int)p->max_args < (yyvsp[(4) - (5)].ival) )
            compile_error(
            "wrong number of arguments in call to %s" ,
            p->name ) ;
          if ( p->min_args != p->max_args ) /* variable args */
              { code1(_PUSHINT) ;  code1((yyvsp[(4) - (5)].ival)) ; }
          code2(_BUILTIN , p->fp) ;
        }
    break;

  case 83:
/* Line 1787 of yacc.c  */
#line 423 "parse.y"
    {
	    (yyval.start) = code_offset ;
	    code1(_PUSHINT) ; code1(0) ;
	    code2(_BUILTIN, (yyvsp[(1) - (1)].bip)->fp) ;
	  }
    break;

  case 84:
/* Line 1787 of yacc.c  */
#line 432 "parse.y"
    { (yyval.start) = code_offset ; }
    break;

  case 85:
/* Line 1787 of yacc.c  */
#line 437 "parse.y"
    { code2(_PRINT, (yyvsp[(1) - (5)].fp)) ; 
              if ( (yyvsp[(1) - (5)].fp) == bi_printf && (yyvsp[(3) - (5)].ival) == 0 )
                    compile_error("no arguments in call to printf") ;
              print_flag = 0 ;
              (yyval.start) = (yyvsp[(2) - (5)].start) ;
            }
    break;

  case 86:
/* Line 1787 of yacc.c  */
#line 445 "parse.y"
    { (yyval.fp) = bi_print ; print_flag = 1 ;}
    break;

  case 87:
/* Line 1787 of yacc.c  */
#line 446 "parse.y"
    { (yyval.fp) = bi_printf ; print_flag = 1 ; }
    break;

  case 88:
/* Line 1787 of yacc.c  */
#line 449 "parse.y"
    { code2op(_PUSHINT, (yyvsp[(1) - (1)].ival)) ; }
    break;

  case 89:
/* Line 1787 of yacc.c  */
#line 451 "parse.y"
    { (yyval.ival) = (yyvsp[(2) - (3)].arg2p)->cnt ; zfree((yyvsp[(2) - (3)].arg2p),sizeof(ARG2_REC)) ; 
             code2op(_PUSHINT, (yyval.ival)) ; 
           }
    break;

  case 90:
/* Line 1787 of yacc.c  */
#line 455 "parse.y"
    { (yyval.ival)=0 ; code2op(_PUSHINT, 0) ; }
    break;

  case 91:
/* Line 1787 of yacc.c  */
#line 459 "parse.y"
    { (yyval.arg2p) = (ARG2_REC*) zmalloc(sizeof(ARG2_REC)) ;
             (yyval.arg2p)->start = (yyvsp[(1) - (3)].start) ;
             (yyval.arg2p)->cnt = 2 ;
           }
    break;

  case 92:
/* Line 1787 of yacc.c  */
#line 464 "parse.y"
    { (yyval.arg2p) = (yyvsp[(1) - (3)].arg2p) ; (yyval.arg2p)->cnt++ ; }
    break;

  case 94:
/* Line 1787 of yacc.c  */
#line 469 "parse.y"
    { code2op(_PUSHINT, (yyvsp[(1) - (2)].ival)) ; }
    break;

  case 95:
/* Line 1787 of yacc.c  */
#line 476 "parse.y"
    {  (yyval.start) = (yyvsp[(3) - (4)].start) ; eat_nl() ; code_jmp(_JZ, (INST*)0) ; }
    break;

  case 96:
/* Line 1787 of yacc.c  */
#line 481 "parse.y"
    { patch_jmp( code_ptr ) ;  }
    break;

  case 97:
/* Line 1787 of yacc.c  */
#line 484 "parse.y"
    { eat_nl() ; code_jmp(_JMP, (INST*)0) ; }
    break;

  case 98:
/* Line 1787 of yacc.c  */
#line 489 "parse.y"
    { patch_jmp(code_ptr) ; 
		  patch_jmp(CDP((yyvsp[(4) - (4)].start))) ; 
		}
    break;

  case 99:
/* Line 1787 of yacc.c  */
#line 498 "parse.y"
    { eat_nl() ; BC_new() ; }
    break;

  case 100:
/* Line 1787 of yacc.c  */
#line 503 "parse.y"
    { (yyval.start) = (yyvsp[(2) - (7)].start) ;
          code_jmp(_JNZ, CDP((yyvsp[(2) - (7)].start))) ; 
          BC_clear(code_ptr, CDP((yyvsp[(5) - (7)].start))) ; }
    break;

  case 101:
/* Line 1787 of yacc.c  */
#line 509 "parse.y"
    { eat_nl() ; BC_new() ;
                  (yyval.start) = (yyvsp[(3) - (4)].start) ;

                  /* check if const expression */
                  if ( code_ptr - 3 == CDP((yyvsp[(3) - (4)].start)) &&
                       code_ptr[-3].op == _PUSHD &&
                       *(double*)code_ptr[-2].ptr != 0.0 
                     )
                     code_ptr -= 3 ;
                  else
		  { INST *p3 = CDP((yyvsp[(3) - (4)].start)) ;
		    code_push(p3, code_ptr-p3, scope, active_funct) ;
		    code_ptr = p3 ;
                    code2(_JMP, (INST*)0) ; /* code2() not code_jmp() */
		  }
                }
    break;

  case 102:
/* Line 1787 of yacc.c  */
#line 529 "parse.y"
    { 
		  int  saved_offset ;
		  int len ;
		  INST *p1 = CDP((yyvsp[(1) - (2)].start)) ;
		  INST *p2 = CDP((yyvsp[(2) - (2)].start)) ;

                  if ( p1 != p2 )  /* real test in loop */
		  {
		    p1[1].op = code_ptr-(p1+1) ;
		    saved_offset = code_offset ;
		    len = code_pop(code_ptr) ;
		    code_ptr += len ;
		    code_jmp(_JNZ, CDP((yyvsp[(2) - (2)].start))) ;
		    BC_clear(code_ptr, CDP(saved_offset)) ;
		  }
		  else /* while(1) */
		  {
		    code_jmp(_JMP, p1) ;
		    BC_clear(code_ptr, CDP((yyvsp[(2) - (2)].start))) ;
		  }
                }
    break;

  case 103:
/* Line 1787 of yacc.c  */
#line 555 "parse.y"
    { 
		  int cont_offset = code_offset ;
                  unsigned len = code_pop(code_ptr) ;
		  INST *p2 = CDP((yyvsp[(2) - (4)].start)) ;
		  INST *p4 = CDP((yyvsp[(4) - (4)].start)) ;

                  code_ptr += len ;

		  if ( p2 != p4 )  /* real test in for2 */
		  {
                    p4[-1].op = code_ptr - p4 + 1 ;
		    len = code_pop(code_ptr) ;
		    code_ptr += len ;
                    code_jmp(_JNZ, CDP((yyvsp[(4) - (4)].start))) ;
		  }
		  else /*  for(;;) */
		  code_jmp(_JMP, p4) ;

		  BC_clear(code_ptr, CDP(cont_offset)) ;

                }
    break;

  case 104:
/* Line 1787 of yacc.c  */
#line 578 "parse.y"
    { (yyval.start) = code_offset ; }
    break;

  case 105:
/* Line 1787 of yacc.c  */
#line 580 "parse.y"
    { (yyval.start) = (yyvsp[(3) - (4)].start) ; code1(_POP) ; }
    break;

  case 106:
/* Line 1787 of yacc.c  */
#line 583 "parse.y"
    { (yyval.start) = code_offset ; }
    break;

  case 107:
/* Line 1787 of yacc.c  */
#line 585 "parse.y"
    { 
             if ( code_ptr - 3 == CDP((yyvsp[(1) - (2)].start)) &&
                  code_ptr[-3].op == _PUSHD &&
                  * (double*) code_ptr[-2].ptr != 0.0
                )
                    code_ptr -= 3 ;
             else   
	     {
	       INST *p1 = CDP((yyvsp[(1) - (2)].start)) ;
	       code_push(p1, code_ptr-p1, scope, active_funct) ;
	       code_ptr = p1 ;
	       code2(_JMP, (INST*)0) ;
	     }
           }
    break;

  case 108:
/* Line 1787 of yacc.c  */
#line 602 "parse.y"
    { eat_nl() ; BC_new() ;
	     code_push((INST*)0,0, scope, active_funct) ;
	   }
    break;

  case 109:
/* Line 1787 of yacc.c  */
#line 606 "parse.y"
    { INST *p1 = CDP((yyvsp[(1) - (2)].start)) ;
	   
	     eat_nl() ; BC_new() ; 
             code1(_POP) ;
             code_push(p1, code_ptr - p1, scope, active_funct) ;
             code_ptr -= code_ptr - p1 ;
           }
    break;

  case 110:
/* Line 1787 of yacc.c  */
#line 619 "parse.y"
    { check_array((yyvsp[(3) - (3)].stp)) ;
             code_array((yyvsp[(3) - (3)].stp)) ; 
             code1(A_TEST) ; 
            }
    break;

  case 111:
/* Line 1787 of yacc.c  */
#line 624 "parse.y"
    { (yyval.start) = (yyvsp[(2) - (5)].arg2p)->start ;
             code2op(A_CAT, (yyvsp[(2) - (5)].arg2p)->cnt) ;
             zfree((yyvsp[(2) - (5)].arg2p), sizeof(ARG2_REC)) ;

             check_array((yyvsp[(5) - (5)].stp)) ;
             code_array((yyvsp[(5) - (5)].stp)) ;
             code1(A_TEST) ;
           }
    break;

  case 112:
/* Line 1787 of yacc.c  */
#line 635 "parse.y"
    { 
             if ( (yyvsp[(4) - (5)].ival) > 1 )
             { code2op(A_CAT, (yyvsp[(4) - (5)].ival)) ; }

             check_array((yyvsp[(1) - (5)].stp)) ;
             if( is_local((yyvsp[(1) - (5)].stp)) )
             { code2op(LAE_PUSHA, (yyvsp[(1) - (5)].stp)->offset) ; }
             else code2(AE_PUSHA, (yyvsp[(1) - (5)].stp)->stval.array) ;
             (yyval.start) = (yyvsp[(2) - (5)].start) ;
           }
    break;

  case 113:
/* Line 1787 of yacc.c  */
#line 648 "parse.y"
    { 
             if ( (yyvsp[(4) - (5)].ival) > 1 )
             { code2op(A_CAT, (yyvsp[(4) - (5)].ival)) ; }

             check_array((yyvsp[(1) - (5)].stp)) ;
             if( is_local((yyvsp[(1) - (5)].stp)) )
             { code2op(LAE_PUSHI, (yyvsp[(1) - (5)].stp)->offset) ; }
             else code2(AE_PUSHI, (yyvsp[(1) - (5)].stp)->stval.array) ;
             (yyval.start) = (yyvsp[(2) - (5)].start) ;
           }
    break;

  case 114:
/* Line 1787 of yacc.c  */
#line 660 "parse.y"
    { 
             if ( (yyvsp[(4) - (6)].ival) > 1 )
             { code2op(A_CAT,(yyvsp[(4) - (6)].ival)) ; }

             check_array((yyvsp[(1) - (6)].stp)) ;
             if( is_local((yyvsp[(1) - (6)].stp)) )
             { code2op(LAE_PUSHA, (yyvsp[(1) - (6)].stp)->offset) ; }
             else code2(AE_PUSHA, (yyvsp[(1) - (6)].stp)->stval.array) ;
             if ( (yyvsp[(6) - (6)].ival) == '+' )  code1(_POST_INC) ;
             else  code1(_POST_DEC) ;

             (yyval.start) = (yyvsp[(2) - (6)].start) ;
           }
    break;

  case 115:
/* Line 1787 of yacc.c  */
#line 677 "parse.y"
    { 
               (yyval.start) = (yyvsp[(3) - (7)].start) ;
               if ( (yyvsp[(5) - (7)].ival) > 1 ) { code2op(A_CAT, (yyvsp[(5) - (7)].ival)) ; }
               check_array((yyvsp[(2) - (7)].stp)) ;
               code_array((yyvsp[(2) - (7)].stp)) ;
               code1(A_DEL) ;
             }
    break;

  case 116:
/* Line 1787 of yacc.c  */
#line 685 "parse.y"
    {
		(yyval.start) = code_offset ;
		check_array((yyvsp[(2) - (3)].stp)) ;
		code_array((yyvsp[(2) - (3)].stp)) ;
		code1(DEL_A) ;
	     }
    break;

  case 117:
/* Line 1787 of yacc.c  */
#line 696 "parse.y"
    { eat_nl() ; BC_new() ;
                      (yyval.start) = code_offset ;

                      check_var((yyvsp[(3) - (6)].stp)) ;
                      code_address((yyvsp[(3) - (6)].stp)) ;
                      check_array((yyvsp[(5) - (6)].stp)) ;
                      code_array((yyvsp[(5) - (6)].stp)) ;

                      code2(SET_ALOOP, (INST*)0) ;
                    }
    break;

  case 118:
/* Line 1787 of yacc.c  */
#line 710 "parse.y"
    { 
		INST *p2 = CDP((yyvsp[(2) - (2)].start)) ;

	        p2[-1].op = code_ptr - p2 + 1 ;
                BC_clear( code_ptr+2 , code_ptr) ;
		code_jmp(ALOOP, p2) ;
		code1(POP_AL) ;
              }
    break;

  case 119:
/* Line 1787 of yacc.c  */
#line 727 "parse.y"
    { (yyval.start) = code_offset ; code2(F_PUSHA, (yyvsp[(1) - (1)].cp)) ; }
    break;

  case 120:
/* Line 1787 of yacc.c  */
#line 729 "parse.y"
    { check_var((yyvsp[(2) - (2)].stp)) ;
             (yyval.start) = code_offset ;
             if ( is_local((yyvsp[(2) - (2)].stp)) )
             { code2op(L_PUSHI, (yyvsp[(2) - (2)].stp)->offset) ; }
             else code2(_PUSHI, (yyvsp[(2) - (2)].stp)->stval.cp) ;

	     CODE_FE_PUSHA() ;
           }
    break;

  case 121:
/* Line 1787 of yacc.c  */
#line 738 "parse.y"
    { 
             if ( (yyvsp[(5) - (6)].ival) > 1 )
             { code2op(A_CAT, (yyvsp[(5) - (6)].ival)) ; }

             check_array((yyvsp[(2) - (6)].stp)) ;
             if( is_local((yyvsp[(2) - (6)].stp)) )
             { code2op(LAE_PUSHI, (yyvsp[(2) - (6)].stp)->offset) ; }
             else code2(AE_PUSHI, (yyvsp[(2) - (6)].stp)->stval.array) ;

	     CODE_FE_PUSHA()  ;

             (yyval.start) = (yyvsp[(3) - (6)].start) ;
           }
    break;

  case 122:
/* Line 1787 of yacc.c  */
#line 752 "parse.y"
    { (yyval.start) = (yyvsp[(2) - (2)].start) ;  CODE_FE_PUSHA() ; }
    break;

  case 123:
/* Line 1787 of yacc.c  */
#line 754 "parse.y"
    { (yyval.start) = (yyvsp[(2) - (3)].start) ; }
    break;

  case 124:
/* Line 1787 of yacc.c  */
#line 758 "parse.y"
    { field_A2I() ; }
    break;

  case 125:
/* Line 1787 of yacc.c  */
#line 761 "parse.y"
    { code1(F_ASSIGN) ; }
    break;

  case 126:
/* Line 1787 of yacc.c  */
#line 762 "parse.y"
    { code1(F_ADD_ASG) ; }
    break;

  case 127:
/* Line 1787 of yacc.c  */
#line 763 "parse.y"
    { code1(F_SUB_ASG) ; }
    break;

  case 128:
/* Line 1787 of yacc.c  */
#line 764 "parse.y"
    { code1(F_MUL_ASG) ; }
    break;

  case 129:
/* Line 1787 of yacc.c  */
#line 765 "parse.y"
    { code1(F_DIV_ASG) ; }
    break;

  case 130:
/* Line 1787 of yacc.c  */
#line 766 "parse.y"
    { code1(F_MOD_ASG) ; }
    break;

  case 131:
/* Line 1787 of yacc.c  */
#line 767 "parse.y"
    { code1(F_POW_ASG) ; }
    break;

  case 132:
/* Line 1787 of yacc.c  */
#line 774 "parse.y"
    { code2(_BUILTIN, bi_split) ; }
    break;

  case 133:
/* Line 1787 of yacc.c  */
#line 778 "parse.y"
    { (yyval.start) = (yyvsp[(3) - (5)].start) ;
              check_array((yyvsp[(5) - (5)].stp)) ;
              code_array((yyvsp[(5) - (5)].stp))  ;
            }
    break;

  case 134:
/* Line 1787 of yacc.c  */
#line 785 "parse.y"
    { code2(_PUSHI, &fs_shadow) ; }
    break;

  case 135:
/* Line 1787 of yacc.c  */
#line 787 "parse.y"
    { 
                  if ( CDP((yyvsp[(2) - (3)].start)) == code_ptr - 2 )
                  {
                    if ( code_ptr[-2].op == _MATCH0 )
                        RE_as_arg() ;
                    else
                    if ( code_ptr[-2].op == _PUSHS )
                    { CELL *cp = ZMALLOC(CELL) ;

                      cp->type = C_STRING ;
                      cp->ptr = code_ptr[-1].ptr ;
                      cast_for_split(cp) ;
                      code_ptr[-2].op = _PUSHC ;
                      code_ptr[-1].ptr = (PTR) cp ;
                    }
                  }
                }
    break;

  case 136:
/* Line 1787 of yacc.c  */
#line 811 "parse.y"
    { (yyval.start) = (yyvsp[(3) - (6)].start) ; 
          code2(_BUILTIN, bi_match) ;
        }
    break;

  case 137:
/* Line 1787 of yacc.c  */
#line 818 "parse.y"
    {
	       INST *p1 = CDP((yyvsp[(1) - (1)].start)) ;

               if ( p1 == code_ptr - 2 ) 
               {
                 if ( p1->op == _MATCH0 ) RE_as_arg() ;
                 else
                 if ( p1->op == _PUSHS )
                 { CELL *cp = ZMALLOC(CELL) ;

                   cp->type = C_STRING ;
                   cp->ptr = p1[1].ptr ;
                   cast_to_RE(cp) ;
                   p1->op = _PUSHC ;
                   p1[1].ptr = (PTR) cp ;
                 } 
               }
             }
    break;

  case 138:
/* Line 1787 of yacc.c  */
#line 840 "parse.y"
    { (yyval.start) = code_offset ;
                      code1(_EXIT0) ; }
    break;

  case 139:
/* Line 1787 of yacc.c  */
#line 843 "parse.y"
    { (yyval.start) = (yyvsp[(2) - (3)].start) ; code1(_EXIT) ; }
    break;

  case 140:
/* Line 1787 of yacc.c  */
#line 847 "parse.y"
    { (yyval.start) = code_offset ;
			if ( active_funct == NULL ) {
	    fprintf(stderr, "tawk: return statement not in function block\n");
			    exit(1);
			}
		      active_funct->nrets = 0;
                      code1(_RET0) ; }
    break;

  case 141:
/* Line 1787 of yacc.c  */
#line 855 "parse.y"
    { 
			if ( active_funct == NULL ) {
	    fprintf(stderr, "tawk: return statement not in function block\n");
			    exit(1);
			}
                        active_funct->nrets = (yyvsp[(3) - (5)].ival);
                        code1(_RET2);
                        (yyval.start) = code_offset; 
                    }
    break;

  case 142:
/* Line 1787 of yacc.c  */
#line 865 "parse.y"
    { 
			if ( active_funct == NULL ) {
	    fprintf(stderr, "tawk: return statement not in function block\n");
			    exit(1);
			}
			(yyval.start) = (yyvsp[(2) - (3)].start) ; active_funct->nrets = 1; code1(_RET) ; }
    break;

  case 143:
/* Line 1787 of yacc.c  */
#line 874 "parse.y"
    {       (yyval.ival) = 2; }
    break;

  case 144:
/* Line 1787 of yacc.c  */
#line 876 "parse.y"
    {       (yyval.ival)++;   }
    break;

  case 145:
/* Line 1787 of yacc.c  */
#line 882 "parse.y"
    { (yyval.start) = code_offset ;
            code2(F_PUSHA, &field[0]) ;
            code1(_PUSHINT) ; code1(0) ; 
            code2(_BUILTIN, bi_getline) ;
            getline_flag = 0 ;
          }
    break;

  case 146:
/* Line 1787 of yacc.c  */
#line 889 "parse.y"
    { (yyval.start) = (yyvsp[(2) - (2)].start) ;
            code1(_PUSHINT) ; code1(0) ;
            code2(_BUILTIN, bi_getline) ;
            getline_flag = 0 ;
          }
    break;

  case 147:
/* Line 1787 of yacc.c  */
#line 895 "parse.y"
    { code1(_PUSHINT) ; code1(F_IN) ;
            code2(_BUILTIN, bi_getline) ;
            /* getline_flag already off in yylex() */
          }
    break;

  case 148:
/* Line 1787 of yacc.c  */
#line 900 "parse.y"
    { code2(F_PUSHA, &field[0]) ;
            code1(_PUSHINT) ; code1(PIPE_IN) ;
            code2(_BUILTIN, bi_getline) ;
          }
    break;

  case 149:
/* Line 1787 of yacc.c  */
#line 905 "parse.y"
    { 
            code1(_PUSHINT) ; code1(PIPE_IN) ;
            code2(_BUILTIN, bi_getline) ;
          }
    break;

  case 150:
/* Line 1787 of yacc.c  */
#line 911 "parse.y"
    { getline_flag = 1 ; }
    break;

  case 153:
/* Line 1787 of yacc.c  */
#line 916 "parse.y"
    { (yyval.start) = code_offset ;
                   code2(F_PUSHA, field+0) ;
                 }
    break;

  case 154:
/* Line 1787 of yacc.c  */
#line 920 "parse.y"
    { (yyval.start) = (yyvsp[(2) - (3)].start) ; }
    break;

  case 155:
/* Line 1787 of yacc.c  */
#line 928 "parse.y"
    {
	     INST *p5 = CDP((yyvsp[(5) - (6)].start)) ;
	     INST *p6 = CDP((yyvsp[(6) - (6)].start)) ;

             if ( p6 - p5 == 2 && p5->op == _PUSHS  )
             { /* cast from STRING to REPL at compile time */
               CELL *cp = ZMALLOC(CELL) ;
               cp->type = C_STRING ;
               cp->ptr = p5[1].ptr ;
               cast_to_REPL(cp) ;
               p5->op = _PUSHC ;
               p5[1].ptr = (PTR) cp ;
             }
             code2(_BUILTIN, (yyvsp[(1) - (6)].fp)) ;
             (yyval.start) = (yyvsp[(3) - (6)].start) ;
           }
    break;

  case 156:
/* Line 1787 of yacc.c  */
#line 946 "parse.y"
    { (yyval.fp) = bi_sub ; }
    break;

  case 157:
/* Line 1787 of yacc.c  */
#line 947 "parse.y"
    { (yyval.fp) = bi_gsub ; }
    break;

  case 158:
/* Line 1787 of yacc.c  */
#line 952 "parse.y"
    { (yyval.start) = code_offset ;
                  code2(F_PUSHA, &field[0]) ; 
                }
    break;

  case 159:
/* Line 1787 of yacc.c  */
#line 957 "parse.y"
    { (yyval.start) = (yyvsp[(2) - (3)].start) ; }
    break;

  case 160:
/* Line 1787 of yacc.c  */
#line 965 "parse.y"
    { 
		   resize_fblock((yyvsp[(1) - (2)].fbp)) ;
                   restore_ids() ;
		   switch_code_to_main() ;
                 }
    break;

  case 161:
/* Line 1787 of yacc.c  */
#line 974 "parse.y"
    { eat_nl() ;
                   scope = SCOPE_FUNCT ;
                   active_funct = (yyvsp[(1) - (4)].fbp) ;
		   active_funct->nrets = 0;
                   *main_code_p = active_code ;

		   (yyvsp[(1) - (4)].fbp)->nargs = (yyvsp[(3) - (4)].ival) ;
                   if ( (yyvsp[(3) - (4)].ival) )
                        (yyvsp[(1) - (4)].fbp)->typev = (char *)
			memset( zmalloc((yyvsp[(3) - (4)].ival)), ST_LOCAL_NONE, (yyvsp[(3) - (4)].ival)) ;
                   else (yyvsp[(1) - (4)].fbp)->typev = (char *) 0 ;

		   code_ptr = code_base =
                       (INST *) zmalloc(INST_BYTES(PAGESZ));
		   code_limit = code_base + PAGESZ ;
		   code_warn = code_limit - CODEWARN ;
                 }
    break;

  case 162:
/* Line 1787 of yacc.c  */
#line 994 "parse.y"
    { FBLOCK  *fbp ;

                   if ( (yyvsp[(2) - (2)].stp)->type == ST_NONE )
                   {
                         (yyvsp[(2) - (2)].stp)->type = ST_FUNCT ;
                         fbp = (yyvsp[(2) - (2)].stp)->stval.fbp = 
                             (FBLOCK *) zmalloc(sizeof(FBLOCK)) ;
                         fbp->name = (yyvsp[(2) - (2)].stp)->name ;
			 fbp->code = (INST*) 0 ;
                   }
                   else
                   {
                         type_error( (yyvsp[(2) - (2)].stp) ) ;

                         /* this FBLOCK will not be put in
                            the symbol table */
                         fbp = (FBLOCK*) zmalloc(sizeof(FBLOCK)) ;
                         fbp->name = "" ;
                   }
                   (yyval.fbp) = fbp ;
                 }
    break;

  case 163:
/* Line 1787 of yacc.c  */
#line 1017 "parse.y"
    { (yyval.fbp) = (yyvsp[(2) - (2)].fbp) ; 
                   if ( (yyvsp[(2) - (2)].fbp)->code ) 
                       compile_error("redefinition of %s" , (yyvsp[(2) - (2)].fbp)->name) ;
                 }
    break;

  case 164:
/* Line 1787 of yacc.c  */
#line 1023 "parse.y"
    { (yyval.ival) = 0 ; }
    break;

  case 166:
/* Line 1787 of yacc.c  */
#line 1028 "parse.y"
    { (yyvsp[(1) - (1)].stp) = save_id((yyvsp[(1) - (1)].stp)->name) ;
                (yyvsp[(1) - (1)].stp)->type = ST_LOCAL_NONE ;
                (yyvsp[(1) - (1)].stp)->offset = 0 ;
                (yyval.ival) = 1 ;
              }
    break;

  case 167:
/* Line 1787 of yacc.c  */
#line 1034 "parse.y"
    { if ( is_local((yyvsp[(3) - (3)].stp)) ) 
                  compile_error("%s is duplicated in argument list",
                    (yyvsp[(3) - (3)].stp)->name) ;
                else
                { (yyvsp[(3) - (3)].stp) = save_id((yyvsp[(3) - (3)].stp)->name) ;
                  (yyvsp[(3) - (3)].stp)->type = ST_LOCAL_NONE ;
                  (yyvsp[(3) - (3)].stp)->offset = (yyvsp[(1) - (3)].ival) ;
                  (yyval.ival) = (yyvsp[(1) - (3)].ival) + 1 ;
                }
              }
    break;

  case 168:
/* Line 1787 of yacc.c  */
#line 1047 "parse.y"
    {  /* we may have to recover from a bungled function
		       definition */
		   /* can have local ids, before code scope
		      changes  */
		    restore_ids() ;

		    switch_code_to_main() ;
		 }
    break;

  case 169:
/* Line 1787 of yacc.c  */
#line 1060 "parse.y"
    { (yyval.start) = (yyvsp[(2) - (6)].start);
	  code1(_ASSIGN2);
	  xcode2(-(yyvsp[(3) - (6)].ival), FReturns);
	}
    break;

  case 170:
/* Line 1787 of yacc.c  */
#line 1067 "parse.y"
    {
	  (yyval.start) = (yyvsp[(2) - (9)].start);
          code1(_ASSIGNL) ;
          code1(-(yyvsp[(3) - (9)].ival));
          code1(-(yyvsp[(7) - (9)].ival));
        }
    break;

  case 171:
/* Line 1787 of yacc.c  */
#line 1076 "parse.y"
    {       (yyval.ival) = 2;         }
    break;

  case 172:
/* Line 1787 of yacc.c  */
#line 1078 "parse.y"
    {       (yyval.ival)++;           }
    break;

  case 173:
/* Line 1787 of yacc.c  */
#line 1083 "parse.y"
    {
		checker_fcall(-1, FReturns);
	}
    break;

  case 174:
/* Line 1787 of yacc.c  */
#line 1089 "parse.y"
    { (yyval.start) = (yyvsp[(2) - (3)].start) ;
             code2(_CALL, (yyvsp[(1) - (3)].fbp)) ;

             if ( (yyvsp[(3) - (3)].ca_p) )  code1((yyvsp[(3) - (3)].ca_p)->arg_num+1) ;
             else  code1(0) ;
               
	     /* This passes the FBLOCK up over the expression production
              */
             FReturns =  (yyvsp[(1) - (3)].fbp);
 
             check_fcall((yyvsp[(1) - (3)].fbp), scope, code_move_level, active_funct, 
                         (yyvsp[(3) - (3)].ca_p), token_lineno) ;
           }
    break;

  case 175:
/* Line 1787 of yacc.c  */
#line 1105 "parse.y"
    { (yyval.ca_p) = (CA_REC *) 0 ; }
    break;

  case 176:
/* Line 1787 of yacc.c  */
#line 1107 "parse.y"
    { (yyval.ca_p) = (yyvsp[(2) - (2)].ca_p) ;
                 (yyval.ca_p)->link = (yyvsp[(1) - (2)].ca_p) ;
                 (yyval.ca_p)->arg_num = (yyvsp[(1) - (2)].ca_p) ? (yyvsp[(1) - (2)].ca_p)->arg_num+1 : 0 ;
               }
    break;

  case 177:
/* Line 1787 of yacc.c  */
#line 1122 "parse.y"
    { (yyval.ca_p) = (CA_REC *) 0 ; }
    break;

  case 178:
/* Line 1787 of yacc.c  */
#line 1124 "parse.y"
    { (yyval.ca_p) = ZMALLOC(CA_REC) ;
                (yyval.ca_p)->link = (yyvsp[(1) - (3)].ca_p) ;
                (yyval.ca_p)->type = CA_EXPR  ;
                (yyval.ca_p)->arg_num = (yyvsp[(1) - (3)].ca_p) ? (yyvsp[(1) - (3)].ca_p)->arg_num+1 : 0 ;
		(yyval.ca_p)->call_offset = code_offset ;
              }
    break;

  case 179:
/* Line 1787 of yacc.c  */
#line 1131 "parse.y"
    { (yyval.ca_p) = ZMALLOC(CA_REC) ;
                (yyval.ca_p)->link = (yyvsp[(1) - (3)].ca_p) ;
                (yyval.ca_p)->arg_num = (yyvsp[(1) - (3)].ca_p) ? (yyvsp[(1) - (3)].ca_p)->arg_num+1 : 0 ;

                code_call_id((yyval.ca_p), (yyvsp[(2) - (3)].stp)) ;
              }
    break;

  case 180:
/* Line 1787 of yacc.c  */
#line 1140 "parse.y"
    { (yyval.ca_p) = ZMALLOC(CA_REC) ;
                (yyval.ca_p)->type = CA_EXPR ;
		(yyval.ca_p)->call_offset = code_offset ;
              }
    break;

  case 181:
/* Line 1787 of yacc.c  */
#line 1146 "parse.y"
    { (yyval.ca_p) = ZMALLOC(CA_REC) ;
                code_call_id((yyval.ca_p), (yyvsp[(1) - (2)].stp)) ;
              }
    break;


/* Line 1787 of yacc.c  */
#line 3508 "y.tab.c"
      default: break;
    }
  /* User semantic actions sometimes alter yychar, and that requires
     that yytoken be updated with the new translation.  We take the
     approach of translating immediately before every use of yytoken.
     One alternative is translating here after every semantic action,
     but that translation would be missed if the semantic action invokes
     YYABORT, YYACCEPT, or YYERROR immediately after altering yychar or
     if it invokes YYBACKUP.  In the case of YYABORT or YYACCEPT, an
     incorrect destructor might then be invoked immediately.  In the
     case of YYERROR or YYBACKUP, subsequent parser actions might lead
     to an incorrect destructor call or verbose syntax error message
     before the lookahead is translated.  */
  YY_SYMBOL_PRINT ("-> $$ =", yyr1[yyn], &yyval, &yyloc);

  YYPOPSTACK (yylen);
  yylen = 0;
  YY_STACK_PRINT (yyss, yyssp);

  *++yyvsp = yyval;

  /* Now `shift' the result of the reduction.  Determine what state
     that goes to, based on the state we popped back to and the rule
     number reduced by.  */

  yyn = yyr1[yyn];

  yystate = yypgoto[yyn - YYNTOKENS] + *yyssp;
  if (0 <= yystate && yystate <= YYLAST && yycheck[yystate] == *yyssp)
    yystate = yytable[yystate];
  else
    yystate = yydefgoto[yyn - YYNTOKENS];

  goto yynewstate;


/*------------------------------------.
| yyerrlab -- here on detecting error |
`------------------------------------*/
yyerrlab:
  /* Make sure we have latest lookahead translation.  See comments at
     user semantic actions for why this is necessary.  */
  yytoken = yychar == YYEMPTY ? YYEMPTY : YYTRANSLATE (yychar);

  /* If not already recovering from an error, report this error.  */
  if (!yyerrstatus)
    {
      ++yynerrs;
#if ! YYERROR_VERBOSE
      yyerror (YY_("syntax error"));
#else
# define YYSYNTAX_ERROR yysyntax_error (&yymsg_alloc, &yymsg, \
                                        yyssp, yytoken)
      {
        char const *yymsgp = YY_("syntax error");
        int yysyntax_error_status;
        yysyntax_error_status = YYSYNTAX_ERROR;
        if (yysyntax_error_status == 0)
          yymsgp = yymsg;
        else if (yysyntax_error_status == 1)
          {
            if (yymsg != yymsgbuf)
              YYSTACK_FREE (yymsg);
            yymsg = (char *) YYSTACK_ALLOC (yymsg_alloc);
            if (!yymsg)
              {
                yymsg = yymsgbuf;
                yymsg_alloc = sizeof yymsgbuf;
                yysyntax_error_status = 2;
              }
            else
              {
                yysyntax_error_status = YYSYNTAX_ERROR;
                yymsgp = yymsg;
              }
          }
        yyerror (yymsgp);
        if (yysyntax_error_status == 2)
          goto yyexhaustedlab;
      }
# undef YYSYNTAX_ERROR
#endif
    }



  if (yyerrstatus == 3)
    {
      /* If just tried and failed to reuse lookahead token after an
	 error, discard it.  */

      if (yychar <= YYEOF)
	{
	  /* Return failure if at end of input.  */
	  if (yychar == YYEOF)
	    YYABORT;
	}
      else
	{
	  yydestruct ("Error: discarding",
		      yytoken, &yylval);
	  yychar = YYEMPTY;
	}
    }

  /* Else will try to reuse lookahead token after shifting the error
     token.  */
  goto yyerrlab1;


/*---------------------------------------------------.
| yyerrorlab -- error raised explicitly by YYERROR.  |
`---------------------------------------------------*/
yyerrorlab:

  /* Pacify compilers like GCC when the user code never invokes
     YYERROR and the label yyerrorlab therefore never appears in user
     code.  */
  if (/*CONSTCOND*/ 0)
     goto yyerrorlab;

  /* Do not reclaim the symbols of the rule which action triggered
     this YYERROR.  */
  YYPOPSTACK (yylen);
  yylen = 0;
  YY_STACK_PRINT (yyss, yyssp);
  yystate = *yyssp;
  goto yyerrlab1;


/*-------------------------------------------------------------.
| yyerrlab1 -- common code for both syntax error and YYERROR.  |
`-------------------------------------------------------------*/
yyerrlab1:
  yyerrstatus = 3;	/* Each real token shifted decrements this.  */

  for (;;)
    {
      yyn = yypact[yystate];
      if (!yypact_value_is_default (yyn))
	{
	  yyn += YYTERROR;
	  if (0 <= yyn && yyn <= YYLAST && yycheck[yyn] == YYTERROR)
	    {
	      yyn = yytable[yyn];
	      if (0 < yyn)
		break;
	    }
	}

      /* Pop the current state because it cannot handle the error token.  */
      if (yyssp == yyss)
	YYABORT;


      yydestruct ("Error: popping",
		  yystos[yystate], yyvsp);
      YYPOPSTACK (1);
      yystate = *yyssp;
      YY_STACK_PRINT (yyss, yyssp);
    }

  YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
  *++yyvsp = yylval;
  YY_IGNORE_MAYBE_UNINITIALIZED_END


  /* Shift the error token.  */
  YY_SYMBOL_PRINT ("Shifting", yystos[yyn], yyvsp, yylsp);

  yystate = yyn;
  goto yynewstate;


/*-------------------------------------.
| yyacceptlab -- YYACCEPT comes here.  |
`-------------------------------------*/
yyacceptlab:
  yyresult = 0;
  goto yyreturn;

/*-----------------------------------.
| yyabortlab -- YYABORT comes here.  |
`-----------------------------------*/
yyabortlab:
  yyresult = 1;
  goto yyreturn;

#if !defined yyoverflow || YYERROR_VERBOSE
/*-------------------------------------------------.
| yyexhaustedlab -- memory exhaustion comes here.  |
`-------------------------------------------------*/
yyexhaustedlab:
  yyerror (YY_("memory exhausted"));
  yyresult = 2;
  /* Fall through.  */
#endif

yyreturn:
  if (yychar != YYEMPTY)
    {
      /* Make sure we have latest lookahead translation.  See comments at
         user semantic actions for why this is necessary.  */
      yytoken = YYTRANSLATE (yychar);
      yydestruct ("Cleanup: discarding lookahead",
                  yytoken, &yylval);
    }
  /* Do not reclaim the symbols of the rule which action triggered
     this YYABORT or YYACCEPT.  */
  YYPOPSTACK (yylen);
  YY_STACK_PRINT (yyss, yyssp);
  while (yyssp != yyss)
    {
      yydestruct ("Cleanup: popping",
		  yystos[*yyssp], yyvsp);
      YYPOPSTACK (1);
    }
#ifndef yyoverflow
  if (yyss != yyssa)
    YYSTACK_FREE (yyss);
#endif
#if YYERROR_VERBOSE
  if (yymsg != yymsgbuf)
    YYSTACK_FREE (yymsg);
#endif
  /* Make sure YYID is used.  */
  return YYID (yyresult);
}


/* Line 2050 of yacc.c  */
#line 1154 "parse.y"


/* resize the code for a user function */

static void  resize_fblock( fbp )
  FBLOCK *fbp ;
{ 
  CODEBLOCK *p = ZMALLOC(CODEBLOCK) ;
  unsigned dummy ;

  code2op(_RET0, _HALT) ;
	/* make sure there is always a return */

  *p = active_code ;
  fbp->code = code_shrink(p, &dummy) ;
      /* code_shrink() zfrees p */

  if ( dump_code_flag ) add_to_fdump_list(fbp) ;
}


/* convert FE_PUSHA  to  FE_PUSHI
   or F_PUSH to F_PUSHI
*/

static void  field_A2I()
{ CELL *cp ;

  if ( code_ptr[-1].op == FE_PUSHA &&
       code_ptr[-1].ptr == (PTR) 0)
  /* On most architectures, the two tests are the same; a good
     compiler might eliminate one.  On LM_DOS, and possibly other
     segmented architectures, they are not */
  { code_ptr[-1].op = FE_PUSHI ; }
  else
  {
    cp = (CELL *) code_ptr[-1].ptr ;

    if ( cp == field  ||

#ifdef  MSDOS
	 SAMESEG(cp,field) &&
#endif
         cp > NF && cp <= LAST_PFIELD )
    {
         code_ptr[-2].op = _PUSHI  ;
    }
    else if ( cp == NF )
    { code_ptr[-2].op = NF_PUSHI ; code_ptr-- ; }

    else
    { 
      code_ptr[-2].op = F_PUSHI ;
      code_ptr -> op = field_addr_to_index( code_ptr[-1].ptr ) ;
      code_ptr++ ;
    }
  }
}

/* we've seen an ID in a context where it should be a VAR,
   check that's consistent with previous usage */

static void check_var( p )
  register SYMTAB *p ;
{
      switch(p->type)
      {
        case ST_NONE : /* new id */
            p->type = ST_VAR ;
            p->stval.cp = ZMALLOC(CELL) ;
            p->stval.cp->type = C_NOINIT ;
            break ;

        case ST_LOCAL_NONE :
            p->type = ST_LOCAL_VAR ;
            active_funct->typev[p->offset] = ST_LOCAL_VAR ;
            break ;

        case ST_VAR :
        case ST_LOCAL_VAR :  break ;

        default :
            type_error(p) ;
            break ;
      }
}

/* we've seen an ID in a context where it should be an ARRAY,
   check that's consistent with previous usage */
static  void  check_array(p)
  register SYMTAB *p ;
{
      switch(p->type)
      {
        case ST_NONE :  /* a new array */
            p->type = ST_ARRAY ;
            p->stval.array = new_ARRAY() ;
            break ;

        case  ST_ARRAY :
        case  ST_LOCAL_ARRAY :
            break ;

        case  ST_LOCAL_NONE  :
            p->type = ST_LOCAL_ARRAY ;
            active_funct->typev[p->offset] = ST_LOCAL_ARRAY ;
            break ;

        default : type_error(p) ; break ;
      }
}

static void code_array(p)
  register SYMTAB *p ;
{ 
  if ( is_local(p) ) code2op(LA_PUSHA, p->offset) ; 
  else  code2(A_PUSHA, p->stval.array) ;
}


/* we've seen an ID as an argument to a user defined function */

static void  code_call_id( p, ip )
  register CA_REC *p ;
  register SYMTAB *ip ;
{ static CELL dummy ;

  p->call_offset = code_offset ;
     /* This always get set now.  So that fcall:relocate_arglist
	works. */

  switch( ip->type )
  {
    case  ST_VAR  :
            p->type = CA_EXPR ;
            code2(_PUSHI, ip->stval.cp) ;
            break ;

    case  ST_LOCAL_VAR  :
            p->type = CA_EXPR ;
            code2op(L_PUSHI, ip->offset) ;
            break ;

    case  ST_ARRAY  :
            p->type = CA_ARRAY ;
            code2(A_PUSHA, ip->stval.array) ;
            break ;

    case  ST_LOCAL_ARRAY :
            p->type = CA_ARRAY ;
            code2op(LA_PUSHA, ip->offset) ;
            break ;

    /* not enough info to code it now; it will have to
       be patched later */

    case  ST_NONE :
            p->type = ST_NONE ;
            p->sym_p = ip ;
            code2(_PUSHI, &dummy) ;
            break ;

    case  ST_LOCAL_NONE :
            p->type = ST_LOCAL_NONE ;
            p->type_p = & active_funct->typev[ip->offset] ;
            code2op(L_PUSHI, ip->offset) ;
            break ;

  
#ifdef   DEBUG
    default :
            bozo("code_call_id") ;
#endif

  }
}

/* an RE by itself was coded as _MATCH0 , change to
   push as an expression */

static void RE_as_arg()
{ CELL *cp = ZMALLOC(CELL) ;

  code_ptr -= 2 ;
  cp->type = C_RE ;
  cp->ptr = code_ptr[1].ptr ;
  code2(_PUSHC, cp) ;
}

/* reset the active_code back to the MAIN block */
static void
switch_code_to_main()
{
   switch(scope)
   {
     case SCOPE_BEGIN :
	*begin_code_p = active_code ;
	active_code = *main_code_p ;
	break ;

     case SCOPE_END :
	*end_code_p = active_code ;
	active_code = *main_code_p ;
	break ;

     case SCOPE_FUNCT :
	active_code = *main_code_p ;
	break ;

     case SCOPE_MAIN :
	break ;
   }
   active_funct = (FBLOCK*) 0 ;
   scope = SCOPE_MAIN ;
}

typedef struct checker_rec {
        struct checker_rec      *next;
        int                      n;
        FBLOCK                  *func;
} checker_rec;
 
checker_rec *checker_list = NULL;
 

void
parse()
{ 
   if ( yyparse() || compile_error_count != 0 ) mawk_exit(2) ;

   scan_cleanup() ;
   set_code() ; 
   /* code must be set before call to resolve_fcalls() */
   if ( resolve_list )  resolve_fcalls() ;

   if ( compile_error_count != 0 ) mawk_exit(2) ;
   if ( checker_list )  checker_fcalls() ;
   if ( dump_code_flag ) { dump_code() ; mawk_exit(0) ; }
}


void checker_fcall(nlist, func)
        int     nlist;
        FBLOCK *func;
{
        if ( func->code == NULL ) {
                checker_rec *x = zmalloc(sizeof(checker_rec));
 
                x->n    = nlist;
                x->func = func;
                x->next = checker_list;
                checker_list = x;
 
                return;
        }
 
        if ( nlist == -1 ) {
            if ( func->nrets > 1 ) {
                compile_error("Function %s returns %d values to expression"
                        , func->name, func->nrets) ;
            }
        } else
        if ( nlist != func->nrets ) {
           compile_error("Function %s returns %d values to assignment list of %d values"
                , func->name, func->nrets, nlist) ;
        }
}
 
void checker_fcalls()
{
        for ( ; checker_list; checker_list = checker_list->next ) {
                checker_fcall(checker_list->n, checker_list->func);
       }
}

