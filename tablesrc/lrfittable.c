/* lrfittable.c
 */
/* Starbase Data Tables	- An Ascii Database for UNIX

   copyright 1991, 1993, 1995, 1999 John B. Roll jr.
 */
#include <xos.h>
#include <xfile.h>
#include <table.h>

#include <math.h>

#if HAVE_FLOAT_H
#include <float.h>
#else
#include <values.h>
#define DBL_MAX MAXDOUBLE
#define DBL_MIN MINDOUBLE
#endif


int main(argc, argv)
			int	 argc;
			char	*argv[];
{
	TableHead	T;
	TableRow	R = NULL;

	int		xcol;
	int		ycol;


	double	sumx  = 0;
	double	sumy  = 0;
	double	sumxx = 0;
	double	sumyy = 0;
	double	sumxy = 0;

	int  	n;
	double	x;
	double	y;
	double	m;
	double	b;
	double	r;


    if ( argc <= 2 ) {
	fprintf(stderr, "usage: lrfittable xcol ycol\n");
	exit(1);
    }
    if ( (T = table_header(stdin, TABLE_PARSE)) == NULL ) {
	exit(1);
    }
    if ( !(xcol = table_colnum(T, argv[1])) ) {
	fprintf(stderr, "lrfittable: cannot find %s column in input table\n"
			, argv[1]);
	exit(1);
    }
    if ( !(ycol = table_colnum(T, argv[2])) ) {
	fprintf(stderr, "lrfittable: cannot find %s column in input table\n"
			, argv[2]);
	exit(1);
    }

    for ( n = 0; R = table_rowget(stdin, T, R, NULL, NULL, 0); n++ ) {
	x = table_colvald(R, xcol);
	y = table_colvald(R, ycol);

	sumx += x;  sumxx += x * x;
	sumy += y;  sumyy += y * y;
	sumxy += x * y;
    }
    
    m =   ( n*sumxy - sumx * sumy ) 
       	/ ( n*sumxx - sumx * sumx );
    b =   ( sumy  - m * sumx ) / n;
    r =   ( n * sumxy - sumx * sumy ) / 
    	  sqrt((n*sumxx - sumx * sumx) * (n*sumyy - sumy * sumy));

    printf("M	B	R\n");
    printf("-	-	-\n");
    printf("%3.12f	%3.12f	%3.12f\n", m, b, r);

    exit(0);
}

