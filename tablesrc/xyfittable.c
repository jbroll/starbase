/* xyfittable.c
 */
/* Starbase Data Tables	- An Ascii Database for UNIX

   copyright 1991, 1993, 1995, 1999 John B. Roll jr.

 */

#include <xos.h>
#include <math.h>
#include <table.h>
#include <slafit.h>

void fitxy();
void avgxy();
void dashes();

int main(argc, argv)
	int     argc; char    *argv[]; { int     nrow = 0;
	int     arow = 100;

	double	(*xye)[2], (*xym)[2], (*xyp)[2];

	int	excol, eycol, mxcol, mycol;
	char	*exname, *eyname, *mxname, *myname;
	int	i, ncol = 0;

	int	type = 6;
	int	status;

	double  coeffs[6];
	double  backwd[6];
	double	xrms, yrms, rrms;

	TableRow *rows;

    TableHead head = table_header(stdin, 0);

    for ( i = 1; i < argc; i++ ) {
	if ( argv[i][0] == '-' ) {
	    switch ( argv[i][1] ) {
		case '0' : type = 0;  break;
		case '2' : type = 2;  break;
		case '3' : type = 3;  break;
		case '4' : type = 4;  break;
		case '6' : type = 6;  break;
		default: {
			fprintf(stderr, "xyfittable: unknown option -%c\n", argv[i][1]);
			exit(1);
		}
	    }
	    continue;
	}
	if ( ncol == 0 ) { exname = argv[i]; excol = table_colnum(head, argv[i]);  ncol++; continue; }
	if ( ncol == 1 ) { eyname = argv[i]; eycol = table_colnum(head, argv[i]);  ncol++; continue; }
	if ( ncol == 2 ) { mxname = argv[i]; mxcol = table_colnum(head, argv[i]);  ncol++; continue; }
	if ( ncol == 3 ) { myname = argv[i]; mycol = table_colnum(head, argv[i]);  ncol++; continue; }
	if ( ncol == 4 ) {
	    fprintf(stderr, "xyfittable can only fit two columns to two columns\n");
	    exit(1);
	}
    }

    if ( ncol < 4 ) {
	fprintf(stderr, "usage: xyfittable [-[0|2|3|4|6]] XCol YCol UCol VCol < table\n");
	exit(1);
    }

    Malloc(xye , arow * sizeof(double) * 2);
    Malloc(xym , arow * sizeof(double) * 2);
    Malloc(rows, arow * sizeof(TableRow));

    for ( nrow = 0;
	(rows[nrow] = table_rowget(stdin, head, NULL, NULL, NULL, 0));
        nrow++ ) {

	if ( nrow >= arow-1 ) {
	    arow += 100;
	    arow *= 1.5;
	    ReAlloc(xye , arow * sizeof(double) * 2);
	    ReAlloc(xym , arow * sizeof(double) * 2);
    	    ReAlloc(rows, arow * sizeof(TableRow));
	}

	xye[nrow][0]   = table_colvald(rows[nrow], excol);
	xye[nrow][1]   = table_colvald(rows[nrow], eycol);
	xym[nrow][0]   = table_colvald(rows[nrow], mxcol);
	xym[nrow][1]   = table_colvald(rows[nrow], mycol);
    }

    Malloc(xyp, nrow * sizeof(double) * 2);

    if ( nrow == 1 ) { type = 2; }
    if ( nrow == 2 ) { type = Min(type, 3); }

    Print("xyfittable output.\n");
    Print("\n");
    if ( type == 0 ) {
        Print("%s Coefficient Model:\n\n", "Identity");
	Print("	x = x\n");
	Print("	y = y\n\n");
    }
    if ( type == 2 ) {
        Print("%s Coefficient Model:\n\n", "Two");
	Print("	x = a + x\n");
	Print("	y = b + y\n\n");
    }
    if ( type == 3 ) {
        Print("%s Coefficient Model:\n\n", "Three");
	Print("	x = a + bx + cy\n");
	Print("	y = d + cx + by\n\n");
	Print("	Where the rotation is composed such that the scale == 1.0\n");
    }
    if ( type == 4 ) {
        Print("%s Coefficient Model:\n\n", "Four");
	Print("	x = a + bx + cy\n");
	Print("	y = d + cx + by\n");
	Print("	Where the rotation is composed such that the shear == 0.0\n");
    }
    if ( type == 6 ) {
        Print("%s Coefficient Model:\n\n", "Six");
	Print("	x = a + bx + cy\n");
	Print("	y = d + ex + dy\n");
    }
    Print("\n");

    if ( type == 0 ) {
	coeffs[0] = 0.0;
	coeffs[1] = 1.0;
	coeffs[2] = 0.0;
	coeffs[3] = 0.0;
	coeffs[4] = 0.0;
	coeffs[5] = 1.0;
    }
    if ( type == 2 ) avgxy(nrow, xye, xym, coeffs);
    if ( type == 3 ) fitxy(nrow, xye, xym, coeffs);
    if ( type == 4 
      || type == 6 ) slaFitxy(type, nrow, xye, xym, coeffs, &status);

    slaPxy(nrow, xye, xym, coeffs, xyp, &xrms, &yrms, &rrms);

    Print("RMS_%s	%f\n", mxname, xrms);
    Print("RMS_%s	%f\n", myname, yrms);
    Print("RMS_R	%f\n", rrms);
    Print("\n");

    {
		double xz, yz, xs, ys, perp, orient;

	slaDcmpf(coeffs, &xz, &yz, &xs, &ys, &perp, &orient);
        Print("%sZero	%f\n", mxname, xz);
        Print("%sZero	%f\n", myname, yz);
        Print("%sScale	%f\n", mxname, xs);
        Print("%sScale	%f\n", myname, ys);
	Print("NonPerp	%f\n", perp);
	Print("Orient	%f\n", r2d(orient));
        Print("\n");
    }


    Print("Coeff	%f	%f	%f	%f	%f	%f\n"
		, coeffs[0], coeffs[1], coeffs[2]
		, coeffs[3], coeffs[4], coeffs[5]);

    slaInvf(coeffs, backwd, &status);
    if ( status != -1 )
        Print("Backwd	%f	%f	%f	%f	%f	%f\n"
		, backwd[0], backwd[1], backwd[2]
		, backwd[3], backwd[4], backwd[5]);
    else
	Print("Unable to solve for inverse transform.\n");
	

    Print("\n");

    table_ors(head, '\t');
    table_hdrput(stdout, head, TBLHDR_HEADLINE, NULL, NULL, 0);
    Print("%s_Fit	%s_Res\t", mxname, mxname);
    Print("%s_Fit	%s_Res\n", myname, myname);

    table_hdrput(stdout, head, TBLHDR_DASHLINE, NULL, NULL, 0);
    dashes(4, mxname);  Print("\t"); dashes(4, mxname);
    Print("\t"); dashes(4, myname);  Print("\t"); dashes(4,
    myname);  Print("\n");

    for ( i = 0; i < nrow; i++ ) {
        table_rowput(stdout, head, rows[i], NULL, NULL, 0);
	Print("%f	%f	%f	%f\n"
		, xyp[i][0], xye[i][0] - xyp[i][0]
		, xyp[i][1], xye[i][1] - xyp[i][1]);
    }

    exit(0);
}

void dashes(n, str)
	int	n;
	char	*str;
{
	while ( n-- ) Print("-");
	while ( *str ) { Print("-"); str++; };
}
