/* extlink.c
 */


double mjd();
double juldate();

typedef void 	Rettype;

#include "xawk.h"
#include "mathmac.h"
#include <stdio.h>

#include "config.h"

#include <math.h>
#include <time.h>

#include <stdlib.h>

#define BITSPERBYTE     8
#define BITS(type)      (BITSPERBYTE * (int)sizeof (type))
#define HIBITL  (1UL << BITS(long) - 1)
#define MAXLONG ((long)(~HIBITL))

void slaDcs2c ( double a, double b, double v[3] ) 
{  
   double cosb;
 
   cosb = cos ( b );
   v[0] = cos ( a ) * cosb;
   v[1] = sin ( a ) * cosb;
   v[2] = sin ( b );
}  

double slaDsep ( double a1, double b1, double a2, double b2 )
{
   int i;
   double d, v1[3], v2[3], s2, c2;
 
   slaDcs2c ( a1, b1, v1 );
   slaDcs2c ( a2, b2, v2 );
 
   s2 = 0.0;
   for ( i = 0; i < 3; i++ ) {
      d = v1[i] - v2[i];
      s2 += d * d;
   }
   s2 /= 4.0; 
 
   c2 = 1.0 - s2;
   return 2.0 * atan2 ( sqrt ( s2 ), sqrt ( Max ( 0.0, c2 )));
}

void Sep(sp, nargs)
        void *sp;
        int  nargs;
{
        double a1 = h2r(xawk_numarg(sp, nargs, 1));
        double b1 = d2r(xawk_numarg(sp, nargs, 2));
        double a2 = h2r(xawk_numarg(sp, nargs, 3));
        double b2 = d2r(xawk_numarg(sp, nargs, 4));
 
        double sep = slaDsep(a1, b1, a2, b2);
 
        sp = xawk_pshnum(sp, r2d(sep), ':');
}



void srandom();
long int  random();

static double seed = 1;

double slaRandom()
{
   return (double) random() / ((double) RAND_MAX);
}

double slaGresid (s)
        double  s;
#define TINY 1e-30
 
{
   static int first   = 1;
   static float x, y;
   float w;
   double rsq;
 
 
/* Second normal deviate of the pair available? */
   if ( first ) {
 
   /* No - generate random x,y in unit-radius circle */
      do {
         x = ( 2.0 * slaRandom() ) - 1.0;
         y = ( 2.0 * slaRandom() ) - 1.0;
         rsq = (double) ( x * x + y * y );
      } while ( rsq >= 1.0 );
 
   /* Box-Muller transformation, generating two deviates */
      w = ( rsq > TINY ) ? (float) sqrt ( - 2.0 * log ( rsq ) / rsq ) : 0.0 ;
      x *= w;
      y *= w;
 
   /* Set flag to indicate availability of next deviate */
      first = 0;
 
   } else {
 
   /* Return second deviate of the pair & reset flag */
      x = y;
      first = 1;
   }
 
/* Scale the deviate by the required standard deviation */
   return x * s;
}


void    Gresid(sp, nargs)
        void *sp;
        int  nargs;
{
        double e;
        double r = xawk_numarg(sp, nargs, 1);
 
        e = slaGresid(r);
 
        sp = xawk_pshnum(sp, e, 0);
}

void	RTtoXY(sp, nargs)
        void *sp;
        int  nargs;
{
        double r = xawk_numarg(sp, nargs, 1);
        double t = xawk_numarg(sp, nargs, 2);

	sp = xawk_pshnum(sp, r * cos(d2r(t)), 0);
	sp = xawk_pshnum(sp, r * sin(d2r(t)), 0);
}

void	XYtoRT(sp, nargs)
        void *sp;
        int  nargs;
{
        double x = xawk_numarg(sp, nargs, 1);
        double y = xawk_numarg(sp, nargs, 2);

	sp = xawk_pshnum(sp, sqrt(x*x + y*y) , 0);
	sp = xawk_pshnum(sp, r2d(atan2(y, x)), 0);
}

void	Rotate(sp, nargs)
        void *sp;
        int  nargs;
{
        double x = xawk_numarg(sp, nargs, 1);
        double y = xawk_numarg(sp, nargs, 2);
        double a = xawk_numarg(sp, nargs, 3);

	sp = xawk_pshnum(sp, x * cos(d2r(a)) -  y * sin(d2r(a)), 0);
	sp = xawk_pshnum(sp, x * sin(d2r(a)) +  y * cos(d2r(a)), 0);
}

	
void    SRandom(sp, nargs)
        void *sp;
        int  nargs;
{
        double e;
        double r = xawk_numarg(sp, nargs, 1);
 
        srandom(r);
}

void    Random(sp, nargs)
        void *sp;
        int  nargs;
{
        double e;
        double r = xawk_numarg(sp, nargs, 1);

	e = slaRandom(r);
 
        sp = xawk_pshnum(sp, e, 0);
}
 
void	R2H(sp, nargs)
	void *sp;
        int  nargs;
{
	double	r = xawk_numarg(sp, nargs, 1);

	sp = xawk_pshnum(sp, r2h(r), ':');
}

void	H2R(sp, nargs)
	void *sp;
        int  nargs;
{
	double	h = xawk_numarg(sp, nargs, 1);

	sp = xawk_pshnum(sp, h2r(h), 0);
}

void	R2D(sp, nargs)
	void *sp;
        int  nargs;
{
	double	r = xawk_numarg(sp, nargs, 1);

	sp = xawk_pshnum(sp, r2d(r), ':');
}

void	D2R(sp, nargs)
	void *sp;
        int  nargs;
{
	double	d = xawk_numarg(sp, nargs, 1);

	sp = xawk_pshnum(sp, d2r(d), 0);
}

void	D2H(sp, nargs)
	void *sp;
        int  nargs;
{
	double	d = xawk_numarg(sp, nargs, 1);

	sp = xawk_pshnum(sp, d / 15, ':');
}

void	H2D(sp, nargs)
	void *sp;
        int  nargs;
{
	double	d = xawk_numarg(sp, nargs, 1);

	sp = xawk_pshnum(sp, d * 15, ':');
}


void MJD(sp, nargs, aargs)
	void *sp;
	int   nargs;
	int   aargs;
{
	int		 stat;

	double	jd = -1;

	if ( aargs == 1 ) {
	    /* Time in as a string */
		char	*str = xawk_strarg(sp, nargs, 1);

	    jd = mjd(str, -1.0, 1);

	    if ( jd == -1 ) {
		fprintf(stderr, "mjd: can't convert '%s' to time\n", str);
	    }
	} else {
	    if ( aargs >= 3 ) {
		    int j = 0;
		    double mm = xawk_numarg(sp, nargs, 1);
		    double dd = xawk_numarg(sp, nargs, 2);
		    double yy = xawk_numarg(sp, nargs, 3);
		    double hrs = 0.0;
		    double min = 0.0;
		    double sec = 0.0;

		/* Time in as YY, MM, DD */

		jd = juldate((int) yy, (int) mm, (int) dd, &j);

		if ( aargs >= 4 ) {	
		    hrs = xawk_numarg(sp, nargs, 4);
		}
		if ( aargs >= 5 ) {
		    min = xawk_numarg(sp, nargs, 5);
		}
		if ( aargs == 6 ) {
		    sec = xawk_numarg(sp, nargs, 6);
		}

		jd += (hrs * 60 * 60
		     + min * 60
		     + sec) / (24.0 * 60 * 60);
	    } 
	}

	sp = xawk_pshnum(sp, jd, 0);
}

#define SECSPERDAY		(24L * 60L * 60L)
#define MJD1970			40587
#define MJDtoUNIXTIME(mjd)	((int) ((mjd - 40587) * SECSPERDAY + 0.5))

void Unx(sp, nargs, aargs)
	void *sp;
        int   nargs;
        int   aargs;
{
        double  jd = -1.0;
	double  unx = -1.0;
 
        if ( aargs == 1 ) {
            /* Time in as a string */
                char    *str = xawk_strarg(sp, nargs, 1);
 
            jd = mjd(str, -1.0, 1);
	    if (  jd >= MJD1970 )
		unx = MJDtoUNIXTIME(jd);
	}

	sp = xawk_pshnum(sp, unx, 0);
}


void cal();
void Cal(sp, nargs, aargs)
	void *sp;
	int   nargs;
	int   aargs;
{
	double	 jd = xawk_numarg(sp, nargs, 1);
	char	*format = NULL;

	char	 tstr[132];

	if ( aargs == 2 ) format = xawk_strarg(sp, nargs, 2);

	cal(tstr, 132, jd, format);

	if ( tstr[0] == '\0' ) fprintf(stderr, "mawk: cal doesn't format date!\n");

	sp = xawk_pshstr(sp, tstr);
}

void caldate();
void Greg(sp, nargs, aargs)
	void *sp;
	int   nargs;
	int   aargs;
{
	int	year;
	int	month;
	int	day;
	double	hour;
	double	mjd = xawk_numarg(sp, nargs, 1);

	caldate(mjd, &year, &month, &day, &hour);

	sp = xawk_pshnum(sp, (double) year , 0);
	sp = xawk_pshnum(sp, (double) month, 0);
	sp = xawk_pshnum(sp, (double) day  , 0);
	sp = xawk_pshnum(sp,          hour , 0);
}

void load(lib, fun)
	char	*lib;
	char	*fun;
{

#ifdef NO_FCNTL_H

	fprintf(stderr, "awk: dynamic loading is not supported on the system\n");
	exit(1);
#else
#include <dlfcn.h>

#ifdef  RTLD_GLOBAL
#define RTLD_FLAGS RTLD_NOW | RTLD_GLOBAL
#else
#define RTLD_FLAGS RTLD_NOW
#endif

	void	*dl;
	void	(*proc)();

	if ( (dl = dlopen(lib, RTLD_FLAGS)) == NULL ) {
	    fprintf(stderr, "awk: can't open file :%s: %s\n", lib, dlerror());
	    return;
	}
	if ( (proc = (void (*)())dlsym(dl, fun)) == NULL ) {
	    fprintf(stderr, "awk: can't load %s from file %s: %s\n", fun, lib, dlerror());
	    perror("awk");
	    return;
	}
	(*proc)();
#endif
}

void Not(sp, nargs, aargs)
	void *sp;
	int   nargs;
	int   aargs;
{
	int	a = xawk_numarg(sp, nargs, 1);

	sp = xawk_pshnum(sp,          !a , 0);
}

void Flag(sp, nargs, aargs) 
	void *sp;
	int   nargs;
	int   aargs;
{
	
}
void And(sp, nargs, aargs)
	void *sp;
	int   nargs;
	int   aargs;
{
	unsigned int	a = xawk_numarg(sp, nargs, 1);
	unsigned int	b = xawk_numarg(sp, nargs, 2);

	sp = xawk_pshnum(sp, (double)(a & b) , 'x' | 0x80);
}

void Or(sp, nargs, aargs)
	void *sp;
	int   nargs;
	int   aargs;
{
	unsigned int	a = xawk_numarg(sp, nargs, 1);
	unsigned int	b = xawk_numarg(sp, nargs, 2);

	sp = xawk_pshnum(sp, (double)(a | b) , 'x' | 0x80);
}

void Xor(sp, nargs, aargs)
	void *sp;
	int   nargs;
	int   aargs;
{
	unsigned int	a = xawk_numarg(sp, nargs, 1);
	unsigned int	b = xawk_numarg(sp, nargs, 2);

	sp = xawk_pshnum(sp, (double)(a ^ b) , 'x' | 0x80);
}

FBLOCK external[] = { 
/* This data structure declairs the "external" functions that
   are available in tawk.  

        Func name,    Magic 1,  C code,     #args,   #ret, argument types s=scaler a=array
 */
  {	"r2d"    , (void *) 1, R2D   , 		1,	1, "s"		}
, {	"d2r"    , (void *) 1, D2R   , 		1, 	1, "s"		}
, {	"r2h"    , (void *) 1, R2H   , 		1, 	1, "s"		}
, {	"h2r"    , (void *) 1, H2R   , 		1,	1, "s"		}
, {	"h2d"    , (void *) 1, H2D   , 		1, 	1, "s"		}
, {	"d2h"    , (void *) 1, D2H   , 		1,	1, "s"		}

, {	"xytort" , (void *) 1, XYtoRT, 		2,	2, "ss"		}
, {	"rttoxy" , (void *) 1, RTtoXY, 		2,	2, "ss"		}
, {	"rotate" , (void *) 1, Rotate, 		3,	2, "sss"	}

, {     "srandom",  (void *) 1, SRandom,        1,      0, "s"          }
, {     "random",  (void *)  1, Random,         1,      1, "s"          }
, {     "gresid",  (void *)  1, Gresid,         1,      1, "s"          }

, {	"mjd"	 , (void *) 1, MJD   , 		6,	1, "ssssss"	}
, {	"cal"	 , (void *) 1, Cal   , 		2,	1, "ss"		}
, {	"unx"	 , (void *) 1, Unx   , 		1, 	1, "s"		}
, {	"greg"	 , (void *) 1, Greg  , 		1, 	4, "s"		}

, {	"sep"	 , (void *) 1, Sep   , 		4, 	1, "ssss"	}

, {	"not"	 , (void *) 1, Not   , 		1, 	1, "s"	}
, {	"and"	 , (void *) 1, And   , 		2, 	1, "ss"	}
, {	"or"	 , (void *) 1, Or   , 		2, 	1, "ss"	}
, {	"xor"	 , (void *) 1, Xor   , 		2, 	1, "ss"	}

, {	 0								}
};
	
void xawk_register();
void extlink_init()
{
	int		 i;

	for ( i = 0; external[i].name; i++ ) {
		xawk_register(&external[i]);
	}
}

