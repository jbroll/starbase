/* statstable.c
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


int dcmp(a, b)
	const void *a;
	const void *b;
{
	double *pa = (double *) a;
	double *pb = (double *) b;

	if ( *pa  > *pb ) return  1;
	if ( *pa  < *pb ) return -1;

	return  0;
}

void printprec(T)
	TableHead	T;
{
	int	i;

    for ( i = 1; i < T->header->ncol; i++ ) {
      printf("%d\t", T->mprec[i]);
    }
      printf("%d\n", T->mprec[i]);
}

void printwide(T)
	TableHead	T;
{
	int	i;

    for ( i = 1; i < T->header->ncol; i++ ) {
      printf("%d\t", T->mwidth[i]);
    }
      printf("%d\n", T->mwidth[i]);
}

void printcols(T, v, ncol)
	TableHead	 T;
	double		*v;
	int		 ncol;
{
	int	i;

    for ( i = 1; i < ncol; i++ ) {
      if ( T->mprec[i] >= 0 && T->mwidth[i] > 0 )
           printf("%.15g\t", v[i]);
      else printf("\t");
    }
    if ( T->mprec[i] >= 0 && T->mwidth[i] > 0 ) 
         printf("%.15g\n", v[i]);
    else printf("\n");
}

int main(argc, argv)
			int	 argc;
			char	*argv[];
{
	TableHead	 T;
	TableRow	*xrow;
	TableRow	  row;
	int		 ncol;
	double		*vector;
	double		*sum, *squ, *min, *max, *rms, *zms, *mean, *med, *std, *rng;

	char		*wcolname = NULL;
	int		 wcol	  = 0;
	double 		 w = 1;
	double 		 weight = 0;

	int		  x_mean = 0
			, x_squ  = 0
			, x_min  = 0
			, x_max  = 0
			, x_rms  = 0
			, x_rmsz  = 0
			, x_med  = 0
			, x_std  = 0
			, x_rng  = 0
			, x_sum  = 0
			, x_prec = 0
			, x_wide = 0
			, x_cnt  = 0
			, stats  = 0;
	int	arow    = 1000;
	int	nred    = 0;
	double  nval	= 0;

	int	i, n;

	char	*progname;

    if ( (progname = strrchr(argv[0], '/')) )
	progname++;
    else	progname = argv[0];

    if ( !strncmp(progname, "stats", 3) ) {
	stats  =
	x_mean = x_squ =
	x_min  = x_max =
	x_rms  = x_rmsz = x_med =
	x_std  = x_rng = x_sum = 1;
	x_prec = x_wide = 0;
    } 
    else if ( !strncmp(progname, "avg",   3) ) x_mean  = 1;
    else if ( !strncmp(progname, "mean",  4) ) x_mean  = 1;
    else if ( !strncmp(progname, "med",   3) ) x_med   = 1;
    else if ( !strncmp(progname, "rms",   3) ) x_rms   = 1;
    else if ( !strncmp(progname, "rmsz",  4) ) x_rmsz  = 1;
    else if ( !strncmp(progname, "rms0",  4) ) x_rmsz  = 1;
    else if ( !strncmp(progname, "stdev", 5) ) x_std   = 1;
    else if ( !strncmp(progname, "min",   3) ) x_min   = 1;
    else if ( !strncmp(progname, "max",   3) ) x_max   = 1;
    else if ( !strncmp(progname, "range", 5) ) x_rng   = 1;
    else if ( !strncmp(progname, "rng",   3) ) x_rng   = 1;
    else if ( !strncmp(progname, "sum",   3) ) x_sum   = 1;
    else if ( !strncmp(progname, "tot",   3) ) x_sum   = 1;
    else if ( !strncmp(progname, "cnt",   3) ) x_cnt   = 1;
    else if ( !strncmp(progname, "prec",  4) ) x_prec  = 1;
    else if ( !strncmp(progname, "width", 5) ) x_wide  = 1;

    for ( i = 1; i < argc; i++ ) {
	if ( argv[i][0] == '-' ) {
	    switch ( argv[i][1] ) {
		case 'l' : { stats = 1; break; }
		case 'w' : {
			       wcolname = argv[++i];
			       break;
			   }
	    }
	}
	if ( argv[i][0] == '-' || argv[i][0] == '+' ) {
	    if ( !strcmp(&argv[i][1], "mean")  ) x_mean = argv[i][0] == '+';
	    if ( !strcmp(&argv[i][1], "med")   ) x_med  = argv[i][0] == '+';
	    if ( !strcmp(&argv[i][1], "median")) x_med  = argv[i][0] == '+';
	    if ( !strcmp(&argv[i][1], "rms")   ) x_rms  = argv[i][0] == '+';
	    if ( !strcmp(&argv[i][1], "rmsz")  ) x_rmsz = argv[i][0] == '+';
	    if ( !strcmp(&argv[i][1], "stdev") ) x_std  = argv[i][0] == '+';
	    if ( !strcmp(&argv[i][1], "min")   ) x_min  = argv[i][0] == '+';
	    if ( !strcmp(&argv[i][1], "max")   ) x_max  = argv[i][0] == '+';
	    if ( !strcmp(&argv[i][1], "range") ) x_rng  = argv[i][0] == '+';
	    if ( !strcmp(&argv[i][1], "rng")   ) x_rng  = argv[i][0] == '+';
	    if ( !strcmp(&argv[i][1], "sum")   ) x_sum  = argv[i][0] == '+';
	    if ( !strcmp(&argv[i][1], "tot")   ) x_sum  = argv[i][0] == '+';
	    if ( !strcmp(&argv[i][1], "cnt")   ) x_cnt  = argv[i][0] == '+';
	    if ( !strcmp(&argv[i][1], "total") ) x_sum  = argv[i][0] == '+';
	    if ( !strcmp(&argv[i][1], "prec")  ) x_prec = argv[i][0] == '+';
	    if ( !strcmp(&argv[i][1], "width") ) x_wide = argv[i][0] == '+';
	}
    }

    if ( (T = table_header(stdin, TABLE_PARSE)) == NULL ) {
	exit(1);
    }
    ncol = table_ncol(T);

    if ( wcolname ) {
	if ( !(wcol = table_colnum(T, wcolname)) ) {
	    fprintf(stderr, "%s: cannot find weight colmn: %s in input table\n"
		, argv[0], wcolname);
	    exit(1);
	}
    }
  
    Malloc(xrow,   sizeof(TableRow) * arow);

    Malloc(sum,  sizeof(double) * (ncol+1));
    Malloc(squ,  sizeof(double) * (ncol+1));
    Malloc(min,  sizeof(double) * (ncol+1));
    Malloc(max,  sizeof(double) * (ncol+1));
    Malloc(rms,  sizeof(double) * (ncol+1));
    Malloc(zms,  sizeof(double) * (ncol+1));
    Malloc(mean, sizeof(double) * (ncol+1));
    Malloc(med,  sizeof(double) * (ncol+1));
    Malloc(std,  sizeof(double) * (ncol+1));
    Malloc(rng,  sizeof(double) * (ncol+1));

    for ( i = 1; i <= ncol; i++ ) {
	sum[i] = 0.0;
	squ[i] = 0.0;
	rms[i] = 0.0;
	min[i] =  DBL_MAX;
	max[i] = -DBL_MAX;
    }

    row = NULL;
    for ( nred = 0; xrow[nred] = table_rowget(stdin, T, row, NULL, NULL, 0);
	  nred++ ) {
	if ( wcol ) w = table_colvald(xrow[nred], wcol);


	if ( nred >= (arow-1) ) {
	    arow += 100;
	    arow *= 1.5;
  	    ReAlloc(xrow, sizeof(TableRow) * arow);
	}

	for ( i = 1; i <= ncol; i++ ) {
		double wx;

	    if ( i == wcol ) {  wx = 1;
	    } else { 		wx = w; }


	    if ( T->mprec[i] >= 0 && T->mwidth[i] > 0 ) {
		    double v = table_colvald(xrow[nred], i);


		min[i]  = Min(v, min[i]);
		max[i]  = Max(v, max[i]);

		sum[i] += v * wx;
		squ[i] += v * v * wx;
	    }
	}
	if ( !x_med ) row = xrow[nred];

	weight += w;
    }

  Malloc(vector, sizeof(double) * nred);

  nval = nred;

  if ( wcol ) nval = weight;


  if ( nred )
    for ( i = 1; i <= ncol; i++ ) {
      if ( T->mprec[i] >= 0 && T->mwidth[i] > 0 ) {
	if ( nval != 0 ) {
	    mean[i] = sum[i]/nval;
	} else {
	    mean[i] = 0;
	}

	rng[i]  = max[i] - min[i];

	if ( nred > 1 ) {
	    rms[i] = sqrt((squ[i] 
			- 2 * mean[i] * sum[i]
			+ nval * mean[i] * mean[i])/(nval-1));

	    zms[i] = sqrt(squ[i] / (nval-1));
	} else {
	    rms[i] = table_colvald(xrow[0], i);
	    zms[i] = table_colvald(xrow[0], i);
	}

	if ( x_med ) {
	    for ( n = 0; n < nred; n++ ) {
		vector[n] = table_colvald(xrow[n], i);
	    }
	    qsort(vector, nred, sizeof(double), dcmp);

	    if ( nred == 1 ) med[i] = vector[0];
	    else
		if ( nred % 2 )
		    med[i] =  vector[ nred/2];
		else
		    med[i] = (vector[(nred/2)-1] + vector[nred/2])/2;
	}


	if ( nred > 1 ) {
		double	temp;

            temp = (squ[i] - mean[i] * sum[i]) / (nval - 1);

            if (temp < 0)               /* possible with roundoff error */
                std[i] = 0.0;
            else
                std[i] = sqrt(temp);
	} else
	    std[i] = 0.0;
       }
    }

    if ( stats ) printf("Stat	");
    table_hdrput(stdout, T, TBLHDR_HEADLINE, NULL, NULL, 0);
    if ( stats ) printf("----	");
    table_hdrput(stdout, T, TBLHDR_DASHLINE, NULL, NULL, 0);

    if ( x_cnt  ) {
	if ( x_cnt ) printf("Count	");
	printcols(T, nred, ncol);
    }
    if ( x_mean  ) {
	if ( stats ) printf("Mean	");
	printcols(T, mean, ncol);
    }
    if ( x_med   ) {
	if ( stats ) printf("Median	"); 
	printcols(T, med,  ncol);
    }
    if ( x_rms   ) {
	if ( stats ) printf("RMS	"); 
	printcols(T, rms,  ncol);
    }
    if ( x_rmsz  ) {
	if ( stats ) printf("RMS_0	"); 
	printcols(T, zms,  ncol);
    }
    if ( x_std   ) {
	if ( stats ) printf("Stdev	"); 
	printcols(T, std,  ncol);
    }
    if ( x_min   ) {
	if ( stats ) printf("Min	"); 
	printcols(T, min,  ncol);
    }
    if ( x_max   ) {
	if ( stats ) printf("Max	"); 
	printcols(T, max,  ncol);
    }
    if ( x_rng   ) {
	if ( stats ) printf("Range	"); 
	printcols(T, rng,  ncol);
    }
    if ( x_sum  ) {
	if ( stats ) printf("Sum	");
	printcols(T, sum, ncol);
    }
    if ( x_prec   ) {
	if ( stats ) printf("Prec	"); 
	printprec(T);
    }
    if ( x_wide   ) {
	if ( stats ) printf("Width	"); 
	printwide(T);
    }

    exit(0);
}

