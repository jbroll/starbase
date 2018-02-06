
#include "slafit.h"
#include "f2c.h"

int sla_svd__(integer *m, integer *n, integer *mp, integer *
	np, doublereal *a, doublereal *w, doublereal *v, doublereal *work, 
	integer *jstat);
int sla_svdcov__(integer *n, integer *np, integer *nc, 
	doublereal *w, doublereal *v, doublereal *work, doublereal *cvm);
int sla_svdsol__(integer *m, integer *n, integer *mp, 
	integer *np, doublereal *b, doublereal *u, doublereal *w, doublereal *v,
	doublereal *work, doublereal *x);
int sla_fitxy__(integer *itype, integer *np, 
	doublereal *xye, doublereal *xym, doublereal *coeffs, 
	integer *j);
int sla_xy2xy__(doublereal *x1, doublereal *y1, doublereal *
	coeffs, doublereal *x2, doublereal *y2);
int sla_dmxv__(doublereal *dm, doublereal *va, doublereal *vb);
int sla_dmat__(integer *n, doublereal *a, doublereal *y, 
	doublereal *d__, integer *jf, integer *iw);
int sla_pxy__(integer *np, doublereal *xye, doublereal *xym, 
	doublereal *coeffs, doublereal *xyp, doublereal *xrms, doublereal *yrms, doublereal *rrms);
int sla_dcmpf__(doublereal *coeffs, doublereal *xz, 
	doublereal *yz, doublereal *xs, doublereal *ys, doublereal *perp, 
	doublereal *orient);
int sla_invf__(doublereal *fwds, doublereal *bkwds, integer *j);

double d_sign(double *x, double *y) {
    return ((*(y) >= 0) ? fabs(*(x)) : -fabs(*(x)));
} 

double d_mod(double *x, double *y) {
    double quotient;
    if( (quotient = *x / *y) >= 0)
	quotient = floor(quotient);
    else
	quotient = -floor(-quotient);
    return(*x - (*y) * quotient );
}


void slaFitxy ( int itype, int np, double xye[][2], double xym[][2],
	double coeffs[6], int *j ) {
    sla_fitxy__(&itype, &np, (double *)xye, (double *)xym, (double *)coeffs, j);
}
void slaDcmpf ( double *coeffs, double *xz, double *yz, double *xs,
	double *ys, double *perp, double *orient ) {
    sla_dcmpf__(coeffs, xz, yz, xs, ys, perp, orient);
}
void slaInvf ( double *fwds, double *bkwds, int *j ) {
    sla_invf__(fwds, bkwds, j);
}

void slaPxy ( int np, double xye[][2], double xym[][2],
	double coeffs[6],
	double xyp[][2], double *xrms, double *yrms, double *rrms ) {
    sla_pxy__(&np, (double *)xye, (double *)xym, (double *)coeffs, (double *)xyp, xrms, yrms, rrms);
}



void slaSvd ( int m, int n, int mp, int np,
	double *a, double *w, double *v, double *work,
	int *jstat ) {
    sla_svd__(&m, &n, &mp, &np, a, w, v, work, jstat);
}

void slaSvdcov ( int n, int np, int nc,
	double *w, double *v, double *work, double *cvm ) {
    sla_svdcov__(&n, &np, &nc, w, v, work, cvm);
}


void slaSvdsol ( int m, int n, int mp, int np,
	double *b, double *u, double *w, double *v,
	double *work, double *x ) {
    sla_svdsol__(&m, &n, &mp, &np, b, u, w, v, work, x);
}
