/* slalink.c

 Starbase Data Tables  - An Ascii Database for UNIX

   copyright 1991, 1993, 1995, 1999 John B. Roll jr.

 */
#include <stdio.h>

#include <slalib.h>
#include "../include/xos.h"

int toupper();
int  islower();
void xawk_register();

#ifdef TCL

/* TCL VERSION */
typedef int Rettype;

#include <tcl.h>
#define xawk_register(fbp) Tcl_CreateCommand(interp, (fbp)->name, (fbp)->addr, \
			     (ClientData) NULL, (Tcl_CmdDeleteProc *) NULL)
#define slainit() Sla_Init(Tcl_Interp *interp)
#define argstuff  (ClientData clientData, Tcl_Interp *interp, int aargs, char *argv[])
#define xawk_strarg(sp, nargs, i)  argv[i]


#define xawk_numarg(sp,nargs,i) tcl_numarg(interp, argv, i)
double tcl_numarg(Tcl_Interp *interp, char *argv[], int i) 
{
    double d;
    int error = Tcl_GetDouble(interp, argv[i], &d);
    return d;
}

#define xawk_pshnum(sp,d,c) tcl_pshnum(interp,  d,  c)
int tcl_pshnum(Tcl_Interp *interp, double d, char c)
{
    char str[TCL_DOUBLE_SPACE];
    Tcl_PrintDouble(interp, d, str);
    Tcl_AppendElement(interp, str);
    return 0;
}

#define xawk_pshstr(sp, str)  tcl_pshstr(interp, str)
int tcl_pshstr(Tcl_Interp *interp, char *str)
{
    Tcl_AppendElement(interp, str);
    return 0;
}

#define ReturnOK()  return TCL_OK
#define ReturnErr(str) Tcl_SetResult(interp,str,TCL_VOLATILE); return TCL_ERROR;

static int sp;

#define checkArgs(min,max,str) 		\
    aargs--;				\
    if (aargs<min || aargs>max) { 	\
       Tcl_SetResult(interp, "wrong # args: should be \"", TCL_STATIC); \
       Tcl_AppendResult(interp, str); 	\
       Tcl_AppendResult(interp, "\"");  \
       return TCL_ERROR; 		\
    }							      

#else

/* AWK VERSION */
#define argstuff (sp, nargs, aargs) void *sp; int   nargs; int   aargs;
typedef void Rettype;
#define ReturnOK()     return;
#define ReturnErr(str) fputs(str, stderr); exit(1);

#define checkArgs(min,max,str)

#endif


#include "xawk.h"

#define TROPOLAPSERATE 0.0065   /* Value suggested by slalib */
#define EPSILON  1e-9           /* value suggested by slalib */
#define MMTTEMP (273+10)           /* 50F */ 
#define MMTHUMID (0.5)
#define MMTPRESSURE 737 /* mbar, from Smithsonian Meteorological Table 63*/

Rettype Refro argstuff
{
    checkArgs(3,3,"refro place el wave");
    {
        char    *place = xawk_strarg(sp, nargs, 1);
        char    *px;
	char	 name[132];
	double	 w, p, h;
	double	ref;

	double el   = xawk_numarg(sp, nargs, 2);
	double wave = xawk_numarg(sp, nargs, 3);

	slaObs(0, place, name, &w, &p, &h);


	slaRefro(d2r(90-el), h, MMTTEMP,
	     MMTPRESSURE, MMTHUMID, wave, p, TROPOLAPSERATE,
	     EPSILON, &ref);

	sp = xawk_pshnum(sp, r2d(ref) * 3600, 0);
	ReturnOK();
    }
}

Rettype	Airmas argstuff 
{
	double airmas;
        double zd;

	checkArgs(1,1,"airmass zd");
	zd = xawk_numarg(sp, nargs, 1);
	airmas = slaAirmas(d2r(zd));

	sp = xawk_pshnum(sp, airmas, 0);

	ReturnOK();
}

Rettype Precess argstuff 
{
    checkArgs(5,5,"precess system epoch0 epoch1 ra0 dec0");
    {
	char  *sys = xawk_strarg(sp, nargs, 1);
	double ep0 = xawk_numarg(sp, nargs, 2);
	double ep1 = xawk_numarg(sp, nargs, 3);
        double ra  = h2r(xawk_numarg(sp, nargs, 4));
	double dc  = d2r(xawk_numarg(sp, nargs, 5));

	if ( strcmp(sys, "FK5nut") ) {
	    slaPreces(sys, ep0, ep1, &ra, &dc);
	} else {
		double pm[3][3], v1[3], v2[3];

	   slaPrenut(ep0, (ep1 - 2000) * 365.25 + 51544.5, pm);

	  slaDcs2c(ra, dc, v1);
	  slaDmxv(pm, v1, v2);
	  slaDcc2s(v2, &ra, &dc);
	  ra = slaDranrm(ra);
	}

	sp = xawk_pshnum(sp, r2h(ra), ':');
	sp = xawk_pshnum(sp, r2d(dc), ':');
	ReturnOK();
    }
}

Rettype Fk54z argstuff 
{
     checkArgs(3,3, "Fk54z ra dec epoch");
     {
	double	r2000  = h2r(xawk_numarg(sp, nargs, 1));
	double	d2000  = d2r(xawk_numarg(sp, nargs, 2));
	double	bepoch =     xawk_numarg(sp, nargs, 3);
	double	r1950;
	double	d1950;
	double	dr1950;
	double	dd1950;

	slaFk54z(r2000,  d2000,  bepoch
	      , &r1950, &d1950, &dr1950, &dd1950);
 
	sp = xawk_pshnum(sp, r2h (r1950), ':');
	sp = xawk_pshnum(sp, r2d (d1950), ':');
	sp = xawk_pshnum(sp, r2h(dr1950), ':');
	sp = xawk_pshnum(sp, r2d(dd1950), ':');
	ReturnOK();
    }
}


Rettype Fk45z argstuff 
{
     checkArgs(3,3, "Fk45z ra dec epoch");
     {
	double	r1950  = h2r(xawk_numarg(sp, nargs, 1));
	double	d1950  = d2r(xawk_numarg(sp, nargs, 2));
	double	bepoch =     xawk_numarg(sp, nargs, 3);
	double	r2000;
	double	d2000;
	double	dr2000;
	double	dd2000;

	slaFk45z(r1950,  d1950,  bepoch
	      , &r2000, &d2000);
 
	sp = xawk_pshnum(sp, r2h (r2000), ':');
	sp = xawk_pshnum(sp, r2d (d2000), ':');
	ReturnOK();
    }
}

Rettype Fk425 argstuff 
{
     checkArgs(6,6, "fk425 r1950 d1950 dr1950 dd1950 p1950 v1950");
     {
	double	r1950  = h2r(xawk_numarg(sp, nargs, 1));
	double	d1950  = d2r(xawk_numarg(sp, nargs, 2));
	double	dr1950 =     xawk_numarg(sp, nargs, 3);
	double	dd1950 =     xawk_numarg(sp, nargs, 4);
	double	p1950  =     xawk_numarg(sp, nargs, 5);
	double	v1950  =     xawk_numarg(sp, nargs, 6);
	double	r2000;
	double	d2000;
	double	dr2000;
	double	dd2000;
	double	p2000;
	double	v2000;


	slaFk425(r1950,  d1950,  dr1950,  dd1950,  p1950,  v1950
	      , &r2000, &d2000, &dr2000, &dd2000, &p2000, &v2000);
 
	sp = xawk_pshnum(sp, r2h (r2000), ':');
	sp = xawk_pshnum(sp, r2d (d2000), ':');
	sp = xawk_pshnum(sp, r2h(dr2000), ':');
	sp = xawk_pshnum(sp, r2d(dd2000), ':');
	sp = xawk_pshnum(sp, r2h (p2000), ':');
	sp = xawk_pshnum(sp, r2d (v2000), ':');
	ReturnOK();
    }
}

Rettype GalEq argstuff 
{
     checkArgs(2,2, "galeq l b");
     {
	double	l = d2r(xawk_numarg(sp, nargs, 1));
	double	b = d2r(xawk_numarg(sp, nargs, 2));
	double	r2000;
	double	d2000;

	slaGaleq(l, b, &r2000,  &d2000);
 
	sp = xawk_pshnum(sp, r2h(r2000), ':');
	sp = xawk_pshnum(sp, r2d(d2000), ':');
	ReturnOK();
    }
}

Rettype EqGal argstuff 
{
     checkArgs(2,2, "eqgal ra dec");
     {
	double	r2000  = h2r(xawk_numarg(sp, nargs, 1));
	double	d2000  = d2r(xawk_numarg(sp, nargs, 2));
	double	l;
	double	b;

	slaEqgal(r2000,  d2000,  &l, &b);
 
	sp = xawk_pshnum(sp, r2d(l), ':');
	sp = xawk_pshnum(sp, r2d(b), ':');
	ReturnOK();
    }
}

Rettype EqEcl argstuff 
{
     checkArgs(3,3, "fk5ecl ra dec date");
     {
	double	r2000  = h2r(xawk_numarg(sp, nargs, 1));
	double	d2000  = d2r(xawk_numarg(sp, nargs, 2));
	double	date   =     xawk_numarg(sp, nargs, 2);
	double	l;
	double	b;

	slaEqecl(r2000,  d2000,  date, &l, &b);
 
	sp = xawk_pshnum(sp, r2d(l), ':');
	sp = xawk_pshnum(sp, r2d(b), ':');
	ReturnOK();
    }
}
Rettype EclEq argstuff 
{
     checkArgs(2,2, "eclfk5 ra dec");
     {
	double	r2000  = h2r(xawk_numarg(sp, nargs, 1));
	double	d2000  = d2r(xawk_numarg(sp, nargs, 2));
	double	date   =     xawk_numarg(sp, nargs, 2);
	double	l;
	double	b;

	slaEcleq(r2000,  d2000,  date, &l, &b);
 
	sp = xawk_pshnum(sp, r2d(l), ':');
	sp = xawk_pshnum(sp, r2d(b), ':');
	ReturnOK();
    }
}

Rettype GE1950 argstuff 
{
     checkArgs(2,2, "ge1950 l b");
     {
	double	l = d2r(xawk_numarg(sp, nargs, 1));
	double	b = d2r(xawk_numarg(sp, nargs, 2));
	double	r1950;
	double	d1950;

	slaGe50(l, b, &r1950,  &d1950);
 
	sp = xawk_pshnum(sp, r2h (r1950), ':');
	sp = xawk_pshnum(sp, r2d (d1950), ':');
	ReturnOK();
    }
}

Rettype EG1950 argstuff 
{
     checkArgs(2,2, "eg1950 ra dec");
     {
	double	r1950  = h2r(xawk_numarg(sp, nargs, 1));
	double	d1950  = d2r(xawk_numarg(sp, nargs, 2));
	double	l;
	double	b;

	slaEg50(r1950,  d1950,  &l, &b);
 
	sp = xawk_pshnum(sp, r2d (l), ':');
	sp = xawk_pshnum(sp, r2d (b), ':');
	ReturnOK();
    }
}

Rettype Fk524 argstuff 
{
     checkArgs(2, 2, "fk524 r2000 d2000 dr2000 dd2000 p2000 v2000");
     {
	double	r2000  = h2r(xawk_numarg(sp, nargs, 1));
	double	d2000  = d2r(xawk_numarg(sp, nargs, 2));
	double	dr2000 =     xawk_numarg(sp, nargs, 3);
	double	dd2000 =     xawk_numarg(sp, nargs, 4);
	double	p2000  =     xawk_numarg(sp, nargs, 5);
	double	v2000  =     xawk_numarg(sp, nargs, 6);
	double	r1950;
	double	d1950;
	double	dr1950;
	double	dd1950;
	double	p1950;
	double	v1950;

	slaFk524(r2000,  d2000,  dr2000,  dd2000,  p2000,  v2000
	      , &r1950, &d1950, &dr1950, &dd1950, &p1950, &v1950);
 
	sp = xawk_pshnum(sp, r2h (r1950), ':');
	sp = xawk_pshnum(sp, r2d (d1950), ':');
	sp = xawk_pshnum(sp, r2h(dr1950), ':');
	sp = xawk_pshnum(sp, r2d(dd1950), ':');
	sp = xawk_pshnum(sp, r2h (p1950), ':');
	sp = xawk_pshnum(sp, r2d (v1950), ':');
	ReturnOK();
    }
}

Rettype TP2S argstuff 
{
    checkArgs(6, 6, "tp2s x y rzero dzero scale rot");
    {
	double  x =     xawk_numarg(sp, nargs, 1);
	double  y =     xawk_numarg(sp, nargs, 2);
	double rz = h2r(xawk_numarg(sp, nargs, 3));
	double dz = d2r(xawk_numarg(sp, nargs, 4));
	double sc =     xawk_numarg(sp, nargs, 5);
	double rt = d2r(xawk_numarg(sp, nargs, 6));

	double ra, dc;
	double xi, eta;

	sc = d2r(sc/60/60);

	xi =  ( x * cos(rt) + y * sin(rt)) * sc;
	eta = (-x * sin(rt) + y * cos(rt)) * sc;

	slaDtp2s (xi, eta, rz, dz, &ra, &dc);
 
	sp = xawk_pshnum(sp, r2h(ra), ':');
	sp = xawk_pshnum(sp, r2d(dc), ':');
	ReturnOK();
    }
}
 
Rettype S2TP argstuff 
{
    checkArgs(6, 6, "tp2s ra dec rzero dzero scale rot");
    {
	double ra = h2r(xawk_numarg(sp, nargs, 1));
	double dc = d2r(xawk_numarg(sp, nargs, 2));
	double rz = h2r(xawk_numarg(sp, nargs, 3));
	double dz = d2r(xawk_numarg(sp, nargs, 4));
	double sc =     xawk_numarg(sp, nargs, 5);
	double rt = d2r(xawk_numarg(sp, nargs, 6));

	double xi, eta;
	double x, y;
	int	j;

	sc = d2r(sc/60/60);

	slaDs2tp(ra, dc, rz, dz, &xi, &eta, &j);

	x = ( xi * cos(rt) + eta * -sin(rt))/sc;
	y = ( xi * sin(rt) + eta *  cos(rt))/sc;
 
	sp = xawk_pshnum(sp, x, 0);
	sp = xawk_pshnum(sp, y, 0);
	ReturnOK();
    }
}

Rettype Bear argstuff 
{
    checkArgs(4, 4, "bear a1 b1 a2 b2");
    {
	double a1 = h2r(xawk_numarg(sp, nargs, 1));
	double b1 = d2r(xawk_numarg(sp, nargs, 2));
	double a2 = h2r(xawk_numarg(sp, nargs, 3));
	double b2 = d2r(xawk_numarg(sp, nargs, 4));

	double ber = slaDbear(a1, b1, a2, b2);

	sp = xawk_pshnum(sp, r2d(ber), ':');
	ReturnOK();
    }
}

Rettype Sep argstuff 
{
    checkArgs(4, 4, "sep a1 b1 a2 b2");
    {
	double a1 = h2r(xawk_numarg(sp, nargs, 1));
	double b1 = d2r(xawk_numarg(sp, nargs, 2));
	double a2 = h2r(xawk_numarg(sp, nargs, 3));
	double b2 = d2r(xawk_numarg(sp, nargs, 4));

	double sep = slaDsep(a1, b1, a2, b2);

	sp = xawk_pshnum(sp, r2d(sep), ':');
	ReturnOK();
    }
}

Rettype Obs argstuff
{
    checkArgs(1,1,"obs place");
    {
        char    *place = xawk_strarg(sp, nargs, 1);
        char    *px;
	char	 name[132];
	double	 w, p, h;


	for ( px = place; *px; px++ ) if ( islower(*px) ) *px = toupper(*px);

	/* look up place */
	slaObs(0, place, name, &w, &p, &h);

	sp = xawk_pshstr(sp, name);
	sp = xawk_pshnum(sp, r2d(w), ':');
	sp = xawk_pshnum(sp, r2d(p), ':');
	sp = xawk_pshnum(sp,     h,   0);
	ReturnOK();
    }
}
	
Rettype St argstuff
{
    checkArgs(2, 2, "st place mjd");
    {
	char 	*place = xawk_strarg(sp, nargs, 1);
	char	*px;
	double	 date  = xawk_numarg(sp, nargs, 2);
	double	 st;

	char	 name[132];
	double	 w, p, h;
	double	 aoprms[14];

	for ( px = place; *px; px++ ) if ( islower(*px) ) *px = toupper(*px);

	/* look up place */
	slaObs(0, place, name, &w, &p, &h);
	if ( name[0] == '?' ) st = -1;
	else {
	    st = slaGmst(date) + -w + slaEqeqx ( date );
	    st = r2h(slaDranrm(st));
	}

	sp = xawk_pshnum(sp, st, ':');
	ReturnOK();
    }
}

Rettype Pa argstuff
{
	char 	*place = xawk_strarg(sp, nargs, 1);
	char	*px;
	double	 ha    = h2r(xawk_numarg(sp, nargs, 2));
	double	 dc    = d2r(xawk_numarg(sp, nargs, 3));
	char	 name[132];
	double	 w, p, h;
	double	 rot;

    checkArgs(3, 3, "pa place ha dec");

    for ( px = place; *px; px++ ) if ( islower(*px) ) *px = toupper(*px);

    slaObs(0, place, name, &w, &p, &h);

    if ( name[0] == '?' ) {
	char err[200];
	sprintf(err,"pa: no observatory named: %s\n", place);
	ReturnErr(err);
    } else {
	rot = slaPa(ha, dc, p);

	sp = xawk_pshnum(sp, r2d(rot), 0);
	ReturnOK();
    }
}

Rettype RADec argstuff
{
    checkArgs(4, 5, "radec place el az mjd ?epoch?");
    {

	char 	*place = xawk_strarg(sp, nargs, 1);
	char	*px;
	double	 el    = d2r(xawk_numarg(sp, nargs, 2));
	double	 az    = d2r(xawk_numarg(sp, nargs, 3));
	double	 date  = xawk_numarg(sp, nargs, 4);
	double	 rm, dm, rot, rob, dob;
	double	 zob, hob;

	char	 name[132];
	double	 w, p, h;

	if ( aargs != 4 && aargs != 5 ) {
	    ReturnErr("[ alt az rot ] = altaz(obs, el, az, date, [equinox])\n");
	}

	for ( px = place; *px; px++ ) if ( islower(*px) ) *px = toupper(*px);

	/* look up place */
	slaObs(0, place, name, &w, &p, &h);
	if ( name[0] == '?' ) {
	    char err[200];
	    sprintf(err,"altaz: no observatory named: %s\n", place);
	    ReturnErr(err);
	} else {
		double ra, da, eq;

	    if ( aargs == 5 ) eq = xawk_numarg(sp, nargs, 5);
	    else	      eq = 2000;

	    {
		double	MAParams[21];
		double	AOParams[14];

	     slaAoppa( date, 0.0
		    , -w, p, h
		    , 0.0, 0.0
		    , 273.155, 1013.25, .33
		    , 5000.0, 0.0065, AOParams);
	     slaMappa( eq, date, MAParams);

	     slaOapqk("A", az, M_PI/2.0-el, AOParams, &ra, &da);
	     slaAmpqk(ra, da, MAParams, &rm, &dm);
	    }
	}

	sp = xawk_pshnum(sp, r2h(rm),  ':');
	sp = xawk_pshnum(sp, r2d(dm),  ':');
	ReturnOK();
    }
}

Rettype AltAz argstuff
{
    checkArgs(4, 5, "altaz place ra dec mjd ?epoch?");
    {

	char 	*place = xawk_strarg(sp, nargs, 1);
	char	*px;
	double	 rm    = h2r(xawk_numarg(sp, nargs, 2));
	double	 dm    = d2r(xawk_numarg(sp, nargs, 3));
	double	 date  = xawk_numarg(sp, nargs, 4);
	double	 alt, azm, rot, rob, dob;
	double	 zob, hob;

	char	 name[132];
	double	 w, p, h;

	if ( aargs != 4 && aargs != 5 ) {
	    ReturnErr("[ alt az rot ] = altaz(obs, ra, dec, date, [equinox])\n");
	}

	for ( px = place; *px; px++ ) if ( islower(*px) ) *px = toupper(*px);

	/* look up place */
	slaObs(0, place, name, &w, &p, &h);
	if ( name[0] == '?' ) {
	    char err[200];
	    sprintf(err,"altaz: no observatory named: %s\n", place);
	    ReturnErr(err);
	} else {
		double ra, da, eq;

	    if ( aargs == 5 ) eq = xawk_numarg(sp, nargs, 5);
	    else	      eq = 2000;

	    {
		double	MAParams[21];
		double	AOParams[14];

	     slaMappa( eq, date, MAParams);
	     slaAoppa( date, 0.0
		    , -w, p, h
		    , 0.0, 0.0
		    , 273.155, 1013.25, .33
		    , 5000.0, 0.0065, AOParams);

	     slaMapqkz(rm, dm, MAParams, &ra, &da);
	     slaAopqk(ra, da, AOParams, &azm, &zob, &hob, &dob, &rob);
	    }

	    alt = M_PI/2 - zob;
	    rot = slaPa(hob, dob, p);
	}

	sp = xawk_pshnum(sp, r2d(alt),  0);
	sp = xawk_pshnum(sp, r2d(azm),  0);
	sp = xawk_pshnum(sp, r2d(rot),  0);
	sp = xawk_pshnum(sp, r2h(hob), ':');
	sp = xawk_pshnum(sp, r2d(zob),  0);
	ReturnOK();
    }
}

Rettype RDPlan argstuff
{
    checkArgs(3, 3, "rdplan place planet mjd");
    {

	char 	*place  = xawk_strarg(sp, nargs, 1);
	char 	*planet = xawk_strarg(sp, nargs, 2);
	char	*px;
	double	 date  = xawk_numarg(sp, nargs, 3);
	double	 alt, azm, rot, rob, dob, diam;

	char	 name[132];
	double	 w, p, h;
	double ra, da;
	double zob, hob;

	for ( px = place; *px; px++ ) if ( islower(*px) ) *px = toupper(*px);

	/* look up place */
	slaObs(0, place, name, &w, &p, &h);
	if ( name[0] == '?' ) {
	    char err[200];
	    sprintf(err,"rdplan: no observatory named: %s\n", place);
	    ReturnErr(err);
	} else {

	    {
		double	AOParams[14];
		int np = 0;

		if ( !strcmp("mercury", planet) ) np =  1;
		if ( !strcmp("venus"  , planet) ) np =  2;
		if ( !strcmp("moon"   , planet) ) np =  3;
		if ( !strcmp("mars"   , planet) ) np =  4;
		if ( !strcmp("jupiter", planet) ) np =  5;
		if ( !strcmp("saturn" , planet) ) np =  6;
		if ( !strcmp("uranus" , planet) ) np =  7;
		if ( !strcmp("neptune", planet) ) np =  8;
		if ( !strcmp("pluto"  , planet) ) np =  9;
		if ( !strcmp("sun"    , planet) ) np = 10;
		if ( np == 0 ) {
	    	    ReturnErr("planet must be one of: mercury venus moon mars jupiter saturn uranu pluto sun");
		}

		slaRdplan(date, np, -w, p , &ra, &da, &diam);
	    }
	}

	sp = xawk_pshnum(sp, r2h(ra)  , ':');
	sp = xawk_pshnum(sp, r2d(da)  , ':');
	sp = xawk_pshnum(sp, r2d(diam), ':');
	ReturnOK();
    }
}
FBLOCK slafun[] = { 
/* This data structure declairs the "external" functions that
   are available in tawk.  

        Func name,    Magic 1,  C code,     #args,   #ret, argument types s=scaler a=array
 */
#if 0
  {     NULL }
#endif
  {	"st"	 , (void *) 1, St    , 		2, 	1, "ss"		}
, {	"obs"	 , (void *) 1, Obs   , 		1, 	4, "s"		}
, {     "radec"  , (void *) 1, RADec , 		5, 	2, "sssss"	}
, {     "altaz"  , (void *) 1, AltAz , 		5, 	5, "sssss"	}
, {     "s2tp"   , (void *) 1, S2TP  , 		6, 	2, "ssssss"	}
, {     "tp2s"   , (void *) 1, TP2S  , 		6, 	2, "ssssss"	}
, {	"precess", (void *) 1, Precess, 	5, 	2, "sssss"	}
, {	"air"    , (void *) 1, Airmas, 		1, 	1, "s"		}
, {	"fk425"  , (void *) 1, Fk425 ,		6, 	6, "ssssss"	}
, {	"fk524"  , (void *) 1, Fk524 , 		6, 	6, "ssssss"	}
, {	"fk45z"  , (void *) 1, Fk45z , 		3, 	2, "ssssss"	}
, {	"fk54z"  , (void *) 1, Fk54z , 		3, 	4, "ssssss"	}
, {	"ge1950" , (void *) 1, GE1950, 		2, 	2, "ss"		}
, {	"eg1950" , (void *) 1, EG1950, 		2, 	2, "ss"		}
, {	"galeq"  , (void *) 1, GalEq, 		2, 	2, "ss"		}
, {	"eqgal"  , (void *) 1, EqGal, 		2, 	2, "ss"		}
, {	"ecleq"  , (void *) 1, EclEq, 		3, 	3, "sss"	}
, {	"eqecl"  , (void *) 1, EqEcl, 		3, 	3, "sss"	}
, {	"bear"   , (void *) 1, Bear  , 		4, 	1, "ssss"	}
, {	"sep"    , (void *) 1, Sep   , 		4, 	1, "ssss"	}
, {	"refro"	 , (void *) 1, Refro ,		3,	1, "sss"	}
, {	"parangle",(void *) 1, Pa   , 		3, 	1, "sss"	}
, {	"rdplanet",(void *) 1, RDPlan, 		3, 	3, "sss"	}
, {     NULL }
};

Rettype slainit()
{
	int		 i;

#ifdef TCL
    if (Tcl_InitStubs(interp, "8.4", 0) == NULL) {
	return TCL_ERROR;
    }
    Tcl_PkgProvide(interp, "sla", "1.0");
#endif
	for ( i = 0; slafun[i].name; i++ ) {
		xawk_register(&slafun[i]); }

	ReturnOK();
}

