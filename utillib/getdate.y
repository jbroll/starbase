%{
/* $Revision: 1.3 $
**
**  Originally written by Steven M. Bellovin <smb@research.att.com> while
**  at the University of North Carolina at Chapel Hill.  Later tweaked by
**  a couple of people on Usenet.  Completely overhauled by Rich $alz
**  <rsalz@bbn.com> and Jim Berets <jberets@bbn.com> in August, 1990;
**  send any email to Rich.
**
**  This grammar has eight shift/reduce conflicts.
**
**  This code is in the public domain and has no copyright.
*/
/* SUPPRESS 287 on yaccpar_sccsid *//* Unusd static variable */
/* SUPPRESS 288 on yyerrlab *//* Label unused */


struct tm *breakmjd();

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#ifdef __GNUC__
#define alloca __builtin_alloca
#else
#ifdef HAVE_ALLOCA_H
#include <alloca.h>
#else
#ifdef _AIX /* for Bison */
 #pragma alloca
#else
#define yyoverflow	dateoverflow

static int dateoverflow() {
	return 1;
}

char *alloca ();
#endif
#endif
#endif


#include <stdio.h>
#include <ctype.h>
#include <stdlib.h>

void   caldate();
double juldate();

/* The code at the top of get_date which figures out the offset of the
   current time zone checks various CPP symbols to see if special
   tricks are need, but defaults to using the gettimeofday system call.
   Include <sys/time.h> if that will be used.  */

#if !defined (USG) && !defined (sgi) && !defined (__FreeBSD__)
#include <sys/time.h>
#endif

#if	defined(vms)

#include <types.h>
#include <time.h>

#else

#include <sys/types.h>

#if	defined(BSD4_2) || defined(BSD4_1C) || (defined (hp9000) && !defined (hpux))
#include <sys/time.h>
#else
#if defined(_AIX)
#include <sys/time.h>
#endif
#include <time.h>
#endif	/* defined(BSD4_2) */
#endif	/* defined(vms) */

#if defined (STDC_HEADERS) || defined (USG)
#include <string.h>
#endif

#if sgi
#undef timezone
#endif

#define yyparse getdate_yyparse
#define yylex   getdate_yylex
#define yyerror getdate_yyerror
#define yylval  getdate_yylval

#define HOUR(x)		((int)(x) * 60)
#define SECSPERDAY	(24L * 60L * 60L)

#define MJD1970			  40587
#define MJD2038			  65424

#define MJDtoUNIXTIME(mjd)	((int   ) (((mjd - MJD1970) * SECSPERDAY)))
#define UNIXTIMEtoMJD(unx)	((double)unx/SECSPERDAY + MJD1970)


/*
**  An entry in the lexical lookup table.
*/
typedef struct _TABLE {
    char	*name;
    int		type;
    time_t	value;
} TABLE;


/*
**  Daylight-savings mode:  on, off, or not yet known.
*/
typedef enum _DSTMODE {
    DSTon, DSToff, DSTmaybe
} DSTMODE;

/*
**  Meridian:  am, pm, or 24-hour style.
*/
typedef enum _MERIDIAN {
    MERam, MERpm, MER24
} MERIDIAN;


/*
**  Global variables.  We could get rid of most of these by using a good
**  union as the yacc stack.  (This routine was originally written before
**  yacc had the %union construct.)  Maybe someday; right now we only use
**  the %union very rarely.
*/
static char	*yyInput;
static DSTMODE	yyDSTmode;
static time_t	yyDayOrdinal;
static time_t	yyDayNumber;
static int	yyHaveDate;
static int	yyHaveMJD;
static int	yyHaveDay;
static int	yyHaveRel;
static int	yyHaveTime;
static int	yyHaveZone;
static double	yyMJD;
static int	yyNow;
static time_t	yyTimezone;
static time_t	yyDay;
static time_t	yyHour;
static time_t	yyMinutes;
static time_t	yyMonth;
static double	yySeconds;
static time_t	yyYear;
static MERIDIAN	yyMeridian;
static time_t	yyRelMonth;
static time_t	yyRelSeconds;

static int FitsDate = 1;

static int getdate_yyerror();
static int getdate_yylex();
%}

%union {
    double		Number;
    enum _MERIDIAN	Meridian;
}

%token	tAGO tDAY tDAYZONE tID tMERIDIAN tMINUTE_UNIT tMONTH tMONTH_UNIT
%token	tSEC_UNIT tSNUMBER tUNUMBER tFNUMBER tZONE tDST tT

%type	<Number>	tDAY tDAYZONE tMINUTE_UNIT tMONTH tMONTH_UNIT
%type	<Number>	tSEC_UNIT tSNUMBER tUNUMBER tFNUMBER tZONE UorFNumber
%type	<Meridian>	tMERIDIAN o_merid

%%

spec	: /* NULL */
	| spec item
	;

item	: 
	  date tT time { yyHaveDate++; yyHaveTime++; }
	| tMONTH tUNUMBER time tUNUMBER {
	    yyHaveDate++; yyHaveTime++;

	    yyMonth = $1;
	    yyDay   = $2;
	    yyYear  = $4;
	}
	| date { yyHaveDate++;	}
	| time { yyHaveTime++;	}
	| mjd  { yyHaveMJD++;	}
	| zone { yyHaveZone++;	}
	| day  { yyHaveDay++;	}
	| rel  { yyHaveRel++;	}
	| number
	;

time	: tUNUMBER tMERIDIAN {
	    yyHour = $1;
	    yyMinutes = 0;
	    yySeconds = 0;
	    yyMeridian = $2;
	}
	| tUNUMBER ':' tUNUMBER o_merid {
	    yyHour = $1;
	    yyMinutes = $3;
	    yySeconds = 0;
	    yyMeridian = $4;
	}
	| tUNUMBER ':' tUNUMBER tSNUMBER {
	    yyHour = $1;
	    yyMinutes = $3;
	    yySeconds = 0;
	    yyMeridian = MER24;
	    yyDSTmode = DSToff;
	    yyTimezone = - ((int)$4 % 100 + ((int)$4 / 100) * 60);
	}
	| tUNUMBER ':' tUNUMBER ':' UorFNumber o_merid {
		double	f;

	    yyHour = $1;
	    yyMinutes = $3;
	    yySeconds = $5;
	    yyMeridian = $6;
	}
	| tUNUMBER ':' tUNUMBER ':' UorFNumber tSNUMBER {
		double	f;

	    yyHour = $1;
	    yyMinutes = $3;
	    yySeconds = $5;
	    yyMeridian = MER24;
	    yyDSTmode = DSToff;
	    yyTimezone = - ((int)$6 % 100 + ((int)$6 / 100) * 60);
	}
	;

UorFNumber : tUNUMBER {
	    $<Number>$ = $1;
	}
	| tFNUMBER {
	    $<Number>$ = $1;
	}
	;


mjd	: tFNUMBER {
	    if ( $1 > 5000 && $1 < 100000 ) {
		yyMJD = $1;
	    }
	}
	;

zone	: tZONE {
	    yyTimezone = $1;
	    yyDSTmode = DSToff;
	}
	| tDAYZONE {
	    yyTimezone = $1;
	    yyDSTmode = DSTon;
	}
	|
	  tZONE tDST {
	    yyTimezone = $1;
	    yyDSTmode = DSTon;
	}
	;

day	: tDAY {
	    yyDayOrdinal = 1;
	    yyDayNumber = $1;
	}
	| tDAY ',' {
	    yyDayOrdinal = 1;
	    yyDayNumber = $1;
	}
	| tUNUMBER tDAY {
	    yyDayOrdinal = $1;
	    yyDayNumber = $2;
	}
	;

date	: tUNUMBER '/' tUNUMBER {
	    yyMonth = $1;
	    yyDay = $3;
	}
	| tUNUMBER '/' tUNUMBER '/' tUNUMBER {
	    if ( FitsDate ) {
		    yyDay = $1;
		    yyMonth = $3;
	    } else {
		    yyMonth = $1;
		    yyDay = $3;
	    }
	    yyYear = $5;
	}
	| tUNUMBER tSNUMBER tSNUMBER {
	    /* ISO 8601 format.  yyyy-mm-dd.  */
	    yyYear = $1;
	    yyMonth = -$2;
	    yyDay = -$3;
	}
	| tMONTH tUNUMBER {
	    yyMonth = $1;
	    yyDay = $2;
	}
	| tUNUMBER '-' tMONTH tSNUMBER {
	    yyDay   =  $1;
	    yyMonth =  $3;
	    yyYear  = -$4;
	}
	| tMONTH tSNUMBER tSNUMBER {
	    yyMonth =  $1;
	    yyDay   = -$2;
	    yyYear  = -$3;
	}
	| tMONTH tUNUMBER ',' tUNUMBER {
	    yyMonth = $1;
	    yyDay = $2;
	    yyYear = $4;
	}
	| tMONTH tUNUMBER tUNUMBER {
	    yyMonth = $1;
	    yyDay = $2;
	    yyYear = $3;
	}
	| tUNUMBER tMONTH {
	    yyMonth = $2;
	    yyDay = $1;
	}
	| tUNUMBER tMONTH tUNUMBER {
	    yyDay = $1;
	    yyMonth = $2;
	    yyYear = $3;
	}
	;

rel	: relunit tAGO {
	    yyRelSeconds = -yyRelSeconds;
	    yyRelMonth = -yyRelMonth;
	}
	| relunit
	;

relunit	: tUNUMBER tMINUTE_UNIT {
	    yyRelSeconds += $1 * $2 * 60L;
	}
	| tSNUMBER tMINUTE_UNIT {
	    yyRelSeconds += $1 * $2 * 60L;
	}
	| tMINUTE_UNIT {
	    yyRelSeconds += $1 * 60L;
	}
	| tSNUMBER tSEC_UNIT {
	    yyRelSeconds += $1;
	}
	| tUNUMBER tSEC_UNIT {
	    yyRelSeconds += $1;
	}
	| tSEC_UNIT {
	    yyRelSeconds++;
	}
	| tSNUMBER tMONTH_UNIT {
	    yyRelMonth += $1 * $2;
	}
	| tUNUMBER tMONTH_UNIT {
	    yyRelMonth += $1 * $2;
	}
	| tMONTH_UNIT {
	    yyRelMonth += $1;
	}
	;

number	: tUNUMBER {
	    if (yyHaveTime && yyHaveDate && !yyHaveRel)
		yyYear = $1;
	    else {
		if( $1 > 5000 && $1 < 100000 ) {
		    yyHaveMJD++;
		    yyMJD = $1;
		} else 
		if( $1 > 30000000 ) {
		    yyHaveMJD++;
		    yyMJD = UNIXTIMEtoMJD($1);
		} else 
		if($1>100000) {
		    yyHaveDate++;
		    yyDay= ((int)$1)%100;
		    yyMonth= ((int)$1/100)%100;
	    	    yyYear = $1/10000;
		} else {
		    yyHaveTime++;
		    if ($1 < 100) {
			yyHour = $1;
			yyMinutes = 0;
		    }
		    else {
			yyHour = $1 / 100;
			yyMinutes = (int)$1 % 100;
		    }
		}
		yySeconds = 0;
		yyMeridian = MER24;
	    }
	}
	;

o_merid	: /* NULL */ {
	    $$ = MER24;
	}
	| tMERIDIAN {
	    $$ = $1;
	}
	;

%%

/* Month and day table. */
static TABLE const MonthDayTable[] = {
    { "january",	tMONTH,  1 },
    { "february",	tMONTH,  2 },
    { "march",		tMONTH,  3 },
    { "april",		tMONTH,  4 },
    { "may",		tMONTH,  5 },
    { "june",		tMONTH,  6 },
    { "july",		tMONTH,  7 },
    { "august",		tMONTH,  8 },
    { "september",	tMONTH,  9 },
    { "sept",		tMONTH,  9 },
    { "october",	tMONTH, 10 },
    { "november",	tMONTH, 11 },
    { "december",	tMONTH, 12 },
    { "sunday",		tDAY, 0 },
    { "monday",		tDAY, 1 },
    { "tuesday",	tDAY, 2 },
    { "tues",		tDAY, 2 },
    { "wednesday",	tDAY, 3 },
    { "wednes",		tDAY, 3 },
    { "thursday",	tDAY, 4 },
    { "thur",		tDAY, 4 },
    { "thurs",		tDAY, 4 },
    { "friday",		tDAY, 5 },
    { "saturday",	tDAY, 6 },
    { NULL }
};

/* Time units table. */
static TABLE const UnitsTable[] = {
    { "year",		tMONTH_UNIT,	12 },
    { "month",		tMONTH_UNIT,	1 },
    { "fortnight",	tMINUTE_UNIT,	14 * 24 * 60 },
    { "week",		tMINUTE_UNIT,	7 * 24 * 60 },
    { "day",		tMINUTE_UNIT,	1 * 24 * 60 },
    { "hour",		tMINUTE_UNIT,	60 },
    { "minute",		tMINUTE_UNIT,	1 },
    { "min",		tMINUTE_UNIT,	1 },
    { "second",		tSEC_UNIT,	1 },
    { "sec",		tSEC_UNIT,	1 },
    { NULL }
};

/* Assorted relative-time words. */
static TABLE const OtherTable[] = {
    { "tomorrow",	tMINUTE_UNIT,	1 * 24 * 60 },
    { "yesterday",	tMINUTE_UNIT,	-1 * 24 * 60 },
    { "today",		tMINUTE_UNIT,	0 },
    { "now",		tMINUTE_UNIT,	0 },
    { "last",		tUNUMBER,	-1 },
    { "this",		tMINUTE_UNIT,	0 },
    { "next",		tUNUMBER,	2 },
    { "first",		tUNUMBER,	1 },
/*  { "second",		tUNUMBER,	2 }, */
    { "third",		tUNUMBER,	3 },
    { "fourth",		tUNUMBER,	4 },
    { "fifth",		tUNUMBER,	5 },
    { "sixth",		tUNUMBER,	6 },
    { "seventh",	tUNUMBER,	7 },
    { "eighth",		tUNUMBER,	8 },
    { "ninth",		tUNUMBER,	9 },
    { "tenth",		tUNUMBER,	10 },
    { "eleventh",	tUNUMBER,	11 },
    { "twelfth",	tUNUMBER,	12 },
    { "ago",		tAGO,	1 },
    { NULL }
};

/* The timezone table. */
/* Some of these are commented out because a time_t can't store a float. */
static TABLE const TimezoneTable[] = {
    { "gmt",	tZONE,     HOUR( 0) },	/* Greenwich Mean */
    { "ut",	tZONE,     HOUR( 0) },	/* Universal (Coordinated) */
    { "utc",	tZONE,     HOUR( 0) },
    { "wet",	tZONE,     HOUR( 0) },	/* Western European */
    { "bst",	tDAYZONE,  HOUR( 0) },	/* British Summer */
    { "wat",	tZONE,     HOUR( 1) },	/* West Africa */
    { "at",	tZONE,     HOUR( 2) },	/* Azores */
#if	0
    /* For completeness.  BST is also British Summer, and GST is
     * also Guam Standard. */
    { "bst",	tZONE,     HOUR( 3) },	/* Brazil Standard */
    { "gst",	tZONE,     HOUR( 3) },	/* Greenland Standard */
#endif
#if 0
    { "nft",	tZONE,     HOUR(3.5) },	/* Newfoundland */
    { "nst",	tZONE,     HOUR(3.5) },	/* Newfoundland Standard */
    { "ndt",	tDAYZONE,  HOUR(3.5) },	/* Newfoundland Daylight */
#endif
    { "ast",	tZONE,     HOUR( 4) },	/* Atlantic Standard */
    { "adt",	tDAYZONE,  HOUR( 4) },	/* Atlantic Daylight */
    { "est",	tZONE,     HOUR( 5) },	/* Eastern Standard */
    { "edt",	tDAYZONE,  HOUR( 5) },	/* Eastern Daylight */
    { "cst",	tZONE,     HOUR( 6) },	/* Central Standard */
    { "cdt",	tDAYZONE,  HOUR( 6) },	/* Central Daylight */
    { "mst",	tZONE,     HOUR( 7) },	/* Mountain Standard */
    { "mdt",	tDAYZONE,  HOUR( 7) },	/* Mountain Daylight */
    { "pst",	tZONE,     HOUR( 8) },	/* Pacific Standard */
    { "pdt",	tDAYZONE,  HOUR( 8) },	/* Pacific Daylight */
    { "yst",	tZONE,     HOUR( 9) },	/* Yukon Standard */
    { "ydt",	tDAYZONE,  HOUR( 9) },	/* Yukon Daylight */
    { "hst",	tZONE,     HOUR(10) },	/* Hawaii Standard */
    { "hdt",	tDAYZONE,  HOUR(10) },	/* Hawaii Daylight */
    { "cat",	tZONE,     HOUR(10) },	/* Central Alaska */
    { "ahst",	tZONE,     HOUR(10) },	/* Alaska-Hawaii Standard */
    { "nt",	tZONE,     HOUR(11) },	/* Nome */
    { "idlw",	tZONE,     HOUR(12) },	/* International Date Line West */
    { "cet",	tZONE,     -HOUR(1) },	/* Central European */
    { "met",	tZONE,     -HOUR(1) },	/* Middle European */
    { "mewt",	tZONE,     -HOUR(1) },	/* Middle European Winter */
    { "mest",	tDAYZONE,  -HOUR(1) },	/* Middle European Summer */
    { "swt",	tZONE,     -HOUR(1) },	/* Swedish Winter */
    { "sst",	tDAYZONE,  -HOUR(1) },	/* Swedish Summer */
    { "fwt",	tZONE,     -HOUR(1) },	/* French Winter */
    { "fst",	tDAYZONE,  -HOUR(1) },	/* French Summer */
    { "eet",	tZONE,     -HOUR(2) },	/* Eastern Europe, USSR Zone 1 */
    { "bt",	tZONE,     -HOUR(3) },	/* Baghdad, USSR Zone 2 */
#if 0
    { "it",	tZONE,     -HOUR(3.5) },/* Iran */
#endif
    { "zp4",	tZONE,     -HOUR(4) },	/* USSR Zone 3 */
    { "zp5",	tZONE,     -HOUR(5) },	/* USSR Zone 4 */
#if 0
    { "ist",	tZONE,     -HOUR(5.5) },/* Indian Standard */
#endif
    { "zp6",	tZONE,     -HOUR(6) },	/* USSR Zone 5 */
#if	0
    /* For completeness.  NST is also Newfoundland Stanard, and SST is
     * also Swedish Summer. */
    { "nst",	tZONE,     -HOUR(6.5) },/* North Sumatra */
    { "sst",	tZONE,     -HOUR(7) },	/* South Sumatra, USSR Zone 6 */
#endif	/* 0 */
    { "wast",	tZONE,     -HOUR(7) },	/* West Australian Standard */
    { "wadt",	tDAYZONE,  -HOUR(7) },	/* West Australian Daylight */
#if 0
    { "jt",	tZONE,     -HOUR(7.5) },/* Java (3pm in Cronusland!) */
#endif
    { "cct",	tZONE,     -HOUR(8) },	/* China Coast, USSR Zone 7 */
    { "jst",	tZONE,     -HOUR(9) },	/* Japan Standard, USSR Zone 8 */
#if 0
    { "cast",	tZONE,     -HOUR(9.5) },/* Central Australian Standard */
    { "cadt",	tDAYZONE,  -HOUR(9.5) },/* Central Australian Daylight */
#endif
    { "east",	tZONE,     -HOUR(10) },	/* Eastern Australian Standard */
    { "eadt",	tDAYZONE,  -HOUR(10) },	/* Eastern Australian Daylight */
    { "gst",	tZONE,     -HOUR(10) },	/* Guam Standard, USSR Zone 9 */
    { "nzt",	tZONE,     -HOUR(12) },	/* New Zealand */
    { "nzst",	tZONE,     -HOUR(12) },	/* New Zealand Standard */
    { "nzdt",	tDAYZONE,  -HOUR(12) },	/* New Zealand Daylight */
    { "idle",	tZONE,     -HOUR(12) },	/* International Date Line East */
    {  NULL  }
};

/* Military timezone table. */
static TABLE const MilitaryTable[] = {
    { "a",	tZONE,	HOUR(  1) },
    { "b",	tZONE,	HOUR(  2) },
    { "c",	tZONE,	HOUR(  3) },
    { "d",	tZONE,	HOUR(  4) },
    { "e",	tZONE,	HOUR(  5) },
    { "f",	tZONE,	HOUR(  6) },
    { "g",	tZONE,	HOUR(  7) },
    { "h",	tZONE,	HOUR(  8) },
    { "i",	tZONE,	HOUR(  9) },
    { "k",	tZONE,	HOUR( 10) },
    { "l",	tZONE,	HOUR( 11) },
    { "m",	tZONE,	HOUR( 12) },
    { "n",	tZONE,	HOUR(- 1) },
    { "o",	tZONE,	HOUR(- 2) },
    { "p",	tZONE,	HOUR(- 3) },
    { "q",	tZONE,	HOUR(- 4) },
    { "r",	tZONE,	HOUR(- 5) },
    { "s",	tZONE,	HOUR(- 6) },
    { "t",	tZONE,	HOUR(- 7) },
    { "u",	tZONE,	HOUR(- 8) },
    { "v",	tZONE,	HOUR(- 9) },
    { "w",	tZONE,	HOUR(-10) },
    { "x",	tZONE,	HOUR(-11) },
    { "y",	tZONE,	HOUR(-12) },
    { "z",	tZONE,	HOUR(  0) },
    { NULL }
};

/* ARGSUSED */
static int
yyerror(s)
    char	*s;
{
  return 0;
}

#define ToTOD(Hours, Minutes, Seconds, Meridian)	\
     ( ToSeconds(Hours, Minutes, Seconds, Meridian) / SECSPERDAY )

double ToSeconds(Hours, Minutes, Seconds, Meridian)
    int	Hours;
    int	Minutes;
    double	Seconds;
    MERIDIAN	Meridian;
{
    if (Minutes < 0 || Minutes > 59 || Seconds < 0 || Seconds > 60)
	return -1.0;
    switch (Meridian) {
    case MER24:
	if (Hours < 0 
	 || Hours > 24
	 ||(Hours > 23 && Minutes != 0.0 && Seconds != 0.0))
	    return -1.0;
	return (Hours * 60L + Minutes) * 60L + Seconds;
    case MERam:
	if (Hours < 1 || Hours > 12)
	    return -1.0;
	return (Hours * 60L + Minutes) * 60L + Seconds;
    case MERpm:
	if (Hours < 1 || Hours > 12)
	    return -1.0;
	return ((Hours + 12) * 60L + Minutes) * 60L + Seconds;
    }
    /* NOTREACHED */
}


static double
Convert(Month, Day, Year, Hours, Minutes, Seconds, Meridian, DSTmode)
    int	Month;
    int	Day;
    int	Year;
    int	Hours;
    int	Minutes;
    double	Seconds;
    MERIDIAN	Meridian;
    DSTMODE	DSTmode;
{
    double	Julian;
    double	tod;
    int		j;
    time_t	unx;

    if (Year <   0) Year = -Year;
    if (Year <  50) Year += 2000;
    if (Year < 100) Year += 1900;

    Julian = juldate(Year, Month, Day, &j);
    Julian += ((double ) yyTimezone * 60L) / SECSPERDAY;

    if ((tod = ToTOD(Hours, Minutes, Seconds, Meridian)) < 0)
	return -1;
    else
    	Julian += tod;


    if ( Julian >= MJD1970 && Julian <= MJD2038 ) {
	unx = MJDtoUNIXTIME(Julian);

	if (DSTmode == DSTon
	 || (DSTmode == DSTmaybe && localtime(&unx)->tm_isdst))
	    Julian -= ((double) (60 * 60)) / SECSPERDAY;
    }

    return Julian;
}


static time_t
DSTcorrect(Start, Future)
    time_t	Start;
    time_t	Future;
{
    time_t	StartDay;
    time_t	FutureDay;

    StartDay = (localtime(&Start)->tm_hour + 1) % 24;
    FutureDay = (localtime(&Future)->tm_hour + 1) % 24;
    return (Future - Start) + (StartDay - FutureDay) * 60L * 60L;
}


static double
RelativeDate(Start, DayOrdinal, DayNumber)
    double	Start;
    time_t	DayOrdinal;
    time_t	DayNumber;
{
    struct tm	*tm;
    time_t	unx;
    time_t	now;

    if ( Start >= MJD1970 ) {
	unx = MJDtoUNIXTIME(Start);

	tm = localtime(&unx);
	now += SECSPERDAY * ((DayNumber - tm->tm_wday + 7) % 7);
	now += 7 * SECSPERDAY * (DayOrdinal <= 0 ? DayOrdinal : DayOrdinal - 1);
	return ((double) DSTcorrect(unx, now)) / SECSPERDAY;
    }

    return 0.0;
}


static double RelativeMonth(Start, RelMonth)
    double	Start;
    time_t	RelMonth;
{
    struct tm	tm;
    time_t	Month;
    time_t	Year;
    time_t	unx;
    double	mjd;
    time_t	now;

    if (RelMonth == 0 || Start < MJD1970 )
	return 0.0;

    unx = MJDtoUNIXTIME(Start);
    
    breakmjd(Start, &tm);
    Month = 12 * tm.tm_year + tm.tm_mon + RelMonth;
    Year = Month / 12;
    Month = Month % 12 + 1;
    mjd = Convert(Month, (time_t)tm.tm_mday, Year,
	(time_t)tm.tm_hour, (time_t)tm.tm_min, (double) (time_t)tm.tm_sec,
	MER24, DSTmaybe);

    now = MJDtoUNIXTIME(mjd);

    return ((double) DSTcorrect(unx, now)) / SECSPERDAY;
}


static int
LookupWord(buff)
    char		*buff;
{
    register char	*p;
    register char	*q;
    register const TABLE	*tp;
    int			i;
    int			abbrev;

    if (strcmp(buff, "T") == 0) 
	return tT;

    /* Make it lowercase. */
    for (p = buff; *p; p++)
	if (isupper(*p))
	    *p = tolower(*p);

    if (strcmp(buff, "am") == 0 || strcmp(buff, "a.m.") == 0) {
	yylval.Meridian = MERam;
	return tMERIDIAN;
    }
    if (strcmp(buff, "pm") == 0 || strcmp(buff, "p.m.") == 0) {
	yylval.Meridian = MERpm;
	return tMERIDIAN;
    }

    /* See if we have an abbreviation for a month. */
    if (strlen(buff) == 3)
	abbrev = 1;
    else if (strlen(buff) == 4 && buff[3] == '.') {
	abbrev = 1;
	buff[3] = '\0';
    }
    else
	abbrev = 0;

    for (tp = MonthDayTable; tp->name; tp++) {
	if (abbrev) {
	    if (strncmp(buff, tp->name, 3) == 0) {
		yylval.Number = tp->value;
		return tp->type;
	    }
	}
	else if (strcmp(buff, tp->name) == 0) {
	    yylval.Number = tp->value;
	    return tp->type;
	}
    }

    for (tp = TimezoneTable; tp->name; tp++)
	if (strcmp(buff, tp->name) == 0) {
	    yylval.Number = tp->value;
	    return tp->type;
	}

    if (strcmp(buff, "dst") == 0) 
	return tDST;

    for (tp = UnitsTable; tp->name; tp++)
	if (strcmp(buff, tp->name) == 0) {
	    yylval.Number = tp->value;
	    return tp->type;
	}

    /* Strip off any plural and try the units table again. */
    i = strlen(buff) - 1;
    if (buff[i] == 's') {
	buff[i] = '\0';
	for (tp = UnitsTable; tp->name; tp++)
	    if (strcmp(buff, tp->name) == 0) {
		yylval.Number = tp->value;
		return tp->type;
	    }
	buff[i] = 's';		/* Put back for "this" in OtherTable. */
    }

    for ( tp = OtherTable; tp->name; tp++ )
	if ( strcmp(buff, tp->name) == 0 ) {
	    if ( !strcmp(tp->name, "now") ) {
		yyNow = 1;
	    }
	    yylval.Number = tp->value;
	    return tp->type;
	}

    /* Military timezones. */
    if (buff[1] == '\0' && isalpha(*buff)) {
	for (tp = MilitaryTable; tp->name; tp++)
	    if (strcmp(buff, tp->name) == 0) {
		yylval.Number = tp->value;
		return tp->type;
	    }
    }

    /* Drop out any periods and try the timezone table again. */
    for (i = 0, p = q = buff; *q; q++)
	if (*q != '.')
	    *p++ = *q;
	else
	    i++;
    *p = '\0';
    if (i)
	for (tp = TimezoneTable; tp->name; tp++)
	    if (strcmp(buff, tp->name) == 0) {
		yylval.Number = tp->value;
		return tp->type;
	    }

    return tID;
}

int TZX() {
#if HAVE_TM_ZONE
        time_t  unx;
        struct tm *tmx;
        time(&unx);
        tmx = localtime(&unx);
        return tmx->tm_gmtoff;
#else
	return timezone;
#endif
}
	

static int
yylex()
{
    register char	c;
    register char	*p;
    char		buff[20];
    int			Count;
    int			sign;
    int			floa = 0;

    if ( FitsDate == 1 ) 
	if ( getenv("STARBASE_CALDATE") ) FitsDate = 0;
	else				  FitsDate = 2;

    for ( ; ; ) {
	while (isspace(*yyInput))
	    yyInput++;

	if (isdigit(c = *yyInput) || c == '-' || c == '+') {
	    if (c == '-' || c == '+') {
		sign = c == '-' ? -1 : 1;
		if (!isdigit(*++yyInput))
		    /* skip the '-' sign */
		    return '-';
	    }
	    else
		sign = 0;
	    for (yylval.Number = 0; isdigit(c = *yyInput++); )
		yylval.Number = 10 * yylval.Number + (c - '0');
	    yyInput--;

	    if ( *yyInput == '.' ) {
		double div = 10 * (sign < 0 ? -1 : 1);

		floa = 1;

		yyInput++;
		for ( ; isdigit(c = *yyInput++); ) {
		    yylval.Number += (c - '0') / div;
		    div *= 10;
		}
		yyInput--;
	    }

	    if (sign < 0)
		yylval.Number = -yylval.Number;

	    if ( floa ) { return tFNUMBER; }

	    return sign ? tSNUMBER : tUNUMBER;
	}
	if (isalpha(c)) {
	    for (p = buff; isalpha(c = *yyInput++) || c == '.'; )
		if (p < &buff[sizeof buff - 1])
		    *p++ = c;
	    *p = '\0';
	    yyInput--;
	    return LookupWord(buff);
	}
	if (c != '(')
	    return *yyInput++;
	Count = 0;
	do {
	    c = *yyInput++;
	    if (c == '\0')
		return c;
	    if (c == '(')
		Count++;
	    else if (c == ')')
		Count--;
	} while (Count > 0);
    }
}


double juldate();

/* Return the MJD as a double.
 */
double mjd(p, now, gmt)
    char		*p;
    double		 now;
    int			 gmt;
{
    double		Start;

    int	day, month, year, hour, min, sec;
    time_t unx;

    localtime(&unx);

    yyInput = p;

    yyMJD	= 0;
    yyNow	= 0;
    yyYear 	= 0;
    yyMonth 	= 0;
    yyDay 	= 0;
    yyTimezone 	= gmt ? 0 : TZX() / 60L;	/* In minutes	*/
    yyDSTmode  	= gmt ? DSToff : DSTmaybe;
    yyHour 	= 0;
    yyMinutes 	= 0;
    yySeconds 	= 0;
    yyMeridian 	= MER24;


    yyRelSeconds = 0;
    yyRelMonth 	= 0;

    yyHaveMJD	= 0;
    yyHaveDate 	= 0;
    yyHaveDay 	= 0;
    yyHaveRel 	= 0;
    yyHaveTime 	= 0;
    yyHaveZone 	= 0;

    if (yyparse()
     || yyHaveMJD > 1 || yyHaveTime > 1 || yyHaveZone > 1 || yyHaveDate > 1 || yyHaveDay > 1) {
	return -1;
    }

    if ( yyHaveMJD ) {
	return yyMJD;
    }

    if ( yyMonth > 12 ) {
	return -1;
    }

    if ( now == -1 ) {
	struct timeval date;
	struct timezone tz;

	gettimeofday(&date, &tz);

	now = date.tv_sec + date.tv_usec / 1000000.0;
	now = UNIXTIMEtoMJD(now);
    }

    if ( yyHaveDate ) {
	Start = Convert(yyMonth, yyDay, yyYear, yyHour, yyMinutes, yySeconds,
		    yyMeridian, yyDSTmode);
    } else {
	if ( yyNow ) {
	    Start = now;
	} else {
	    Start = (int) now;

	    if ( yyHaveTime || yyHaveDay ) {
		Start += ToTOD(yyHour, yyMinutes, yySeconds, yyMeridian);
	    }
	}
    }


    if ( Start < 0 ) return -1;

    Start += ((double) yyRelSeconds) / SECSPERDAY;

    if ( yyRelMonth ) 
	if ( Start < MJD1970 || Start > MJD2038 ) return -1;
    	else
	    Start += RelativeMonth(Start, yyRelMonth);

    if (yyHaveDay && !yyHaveDate) {
	if ( Start < MJD1970 || Start > MJD2038 ) return -1;
	else
	    Start += RelativeDate(Start, yyDayOrdinal, yyDayNumber);
    }

    return Start;
}

struct tm *breakmjd(mjd, tm)
    double	mjd;
    struct tm	*tm;
{
    double	hour;
    double	min;
    int		yjd;
    int		j;

    /* MJD % 7 tells the day of the week
       (4=Sunday, 5=Monday, 6=Tuesday, 0=Wednesday, ...)  */

    caldate(mjd, &tm->tm_year, &tm->tm_mon, &tm->tm_mday, &hour);
    yjd = juldate(tm->tm_year, 1, 1);

    tm->tm_year -= 1900;
    tm->tm_mon--;

    tm->tm_hour = (int) hour + 0.5;
           min  =       (hour - tm->tm_hour) * 60;
    tm->tm_min  = (int)  min + 0.5;
    tm->tm_sec  =      ((min  - tm->tm_min)  * 60) + 0.5;
    tm->tm_wday = ((int) (mjd + 3) % 7) - 1;
    tm->tm_yday = (int)  (mjd - yjd)    - 1;  
    tm->tm_isdst = 0; 

    if ( tm->tm_sec >= 60 ) { tm->tm_sec = 0; tm->tm_min++;  }
    if ( tm->tm_min >= 60 ) { tm->tm_min = 0; tm->tm_hour++; }

    return tm;
}


char *cal(buffer, max, mjd, format)
	char	*buffer;
	int	 max;
	double	 mjd;
	char	*format;
{
	struct   tm tstc;
	struct   tm*tstp = &tstc;

	if ( !format ) format = "%b %e %T %Y";

	{
		int	 iy, im, id, j;
		double	 fd;

	    tstc.tm_mday = -1;
	    tstc.tm_wday = -1;
	    tstc.tm_yday = -1;

	    caldate(mjd, &iy, &im, &id, &fd);

	    tstc.tm_year = iy - 1900;
	    tstc.tm_mon  = im - 1;
	    tstc.tm_mday = id;
	    tstc.tm_hour =  ((int) fd / 24) * 24;
	    tstc.tm_min  = (((int) ((fd - tstc.tm_hour) / 60) * 60) + .5);
	    tstc.tm_sec  = ((int) (((fd - (tstc.tm_hour + tstc.tm_min))
			    / 60 * 60 ) * 60 * 60) + .5);
	    if ( tstc.tm_sec >= 60 ) { tstc.tm_sec = 0; tstc.tm_min++;  }
	    if ( tstc.tm_min >= 60 ) { tstc.tm_min = 0; tstc.tm_hour++; }

	    tstc.tm_isdst = 0;

	    breakmjd(mjd, &tstc);
	}

	strftime(buffer, max, format, tstp);

	return buffer;
}
