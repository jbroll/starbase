
void slaFitxy ( int itype, int np, double xye[][2], double xym[][2],
	double coeffs[6], int *j );
void slaDcmpf ( double *coeffs, double *xz, double *yz, double *xs,
	double *ys, double *perp, double *orient );
void slaInvf ( double *fwds, double *bkwds, int *j );
void slaPxy ( int np, double xye[][2], double xym[][2],
	double coeffs[6],
	double xyp[][2], double *xrms, double *yrms, double *rrms );


void slaSvd ( int m, int n, int mp, int np,
	double *a, double *w, double *v, double *work,
	int *jstat );
void slaSvdcov ( int n, int np, int nc,
	double *w, double *v, double *work, double *cvm );

void slaSvdsol ( int m, int n, int mp, int np,
	double *b, double *u, double *w, double *v,
	double *work, double *x );
