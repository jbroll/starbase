/* histtable.c
 */
/* Starbase Data Tables	- An Ascii Database for UNIX

   copyright 1991, 1993, 1995, 1999 John B. Roll jr.
 */

#include <xos.h>
#include <xfile.h>

#define EPSILON	(2.220e-16)

#include "../tablelib/table.h"

double SAOstrtod();
void	dashes();

int main(argc, argv)
	int	argc;
	char	*argv[];
{
	int have_max = 0;
	int have_min = 0;
	int have_nbins = 0;

	int	nbins;
	double	dz;
	int	flip;

	double	**V = NULL;
	double	 Vmin, Vmax;
	double	 vmin, voff;
	double	 vmax;

	int	 minbin = 0;
	double	*minsum;
	int	 maxbin = 0;
	double	*maxsum;

	int	lt = 0;
	int	gt = 0;

	int	 integer = 1;

	int	 *H;
	double	**S;
	int	 i, n;

	File	ifile = Stdin;
	File	ofile = Stdout;

	TableHead	T = NULL;
	TableRow	R, r;

	int		sum = 0;

	int		column;
	int		av = 0;
	int		nv = 0;

	int		ncol = 0;

	int		*C;

	Calloc(C, argc * sizeof(int ));
	/* open files / allocate terms
	 */
	for ( i = 1; i < argc; i++ ) {
	    if ( argv[i][0] == '-' ) {
		switch ( argv[i][1] ) {
		 case 'i' : 
      			if ( (ifile = (!strcmp("-", argv[i+1]))
				? Stdout : Open(argv[i+1], "r")) == NULL ) {
			    FPrint(Stderr, "histtable: can't open input file: %s\n", argv[i+1]);
			    perror("histtable");
			    exit(1);
			}
			i++;
			break;
		 case 'o' : 
      			if ( (ofile = (!strcmp("-", argv[i+1]))
				? Stdout : Open(argv[i+1], "w")) == NULL ) {
			    FPrint(Stderr, "histtable: can't open output file: %s\n", argv[i+1]);
			    perror("histtable");
			    exit(1);
			}
			i++;
			break;
		 case 'n' : nbins = atoi(argv[++i]);
			    have_nbins++;
			    break;
		 case 'm' :
			if ( !strcmp(&argv[i][1], "max") ) {
				have_max++;
				Vmax = SAOstrtod(argv[++i], NULL);
				break;
			}
			if ( !strcmp(&argv[i][1], "min") ) {
				have_min++;
				Vmin = SAOstrtod(argv[++i], NULL);
				break;
			}
		 case 'I': integer = 0;	break;
		 case 'l': lt++;	break;
		 case 'g': gt++;	break;
		 case 's': sum++;	break;


		 default:
			FPrint(Stderr, "histtable: unknown option -%c\n", argv[i][1]);
			exit(1);
		}
		continue;
	    } else {
		if ( !T ) { T = table_header(ifile, 0); }
		C[ncol++] = table_colnum(T, argv[i]);
	    }
	}

	if ( T == NULL ) {
	  FPrint(Stderr, "usage: smoothtable [-s] [-g] [-l] [-min min] [-max max] [-n bins] Column [Column ...]\n");
	  exit(1);
	}

	R = table_rowloc(NULL, 0, 0);

	Calloc(S, ncol * sizeof(int*));
	Calloc(minsum, ncol * sizeof(double));
	Calloc(maxsum, ncol * sizeof(double));

	if ( have_max ) vmax = Vmax;
	if ( have_min ) vmin = Vmin;
	if ( have_min && have_max ) {
	    if ( !have_nbins ) {
		if ( integer )
		    nbins = Max( 1, (int)(Min( 64000, Abs(vmax-vmin))));
		else
		    nbins = Max( 1, (int)(Min( 64000, Abs(vmax-vmin) +1)));
	    }

	    if ( nbins < 1 )
		exit(0);

	    dz      = (double) (nbins)/(vmax-vmin);
	    flip    = dz < 0.0;

	    voff = vmin;

	    if ( flip ) { vmax = vmax + 2 * EPSILON; }
	    else	{ vmin = vmin + 2 * EPSILON; }

	    Calloc(H, nbins * sizeof(int));
	    for ( i = 0; i < ncol; i++ ) {
	        Calloc(S[i], nbins * sizeof(double));
	    }
	} else {
		Calloc(V, ncol * sizeof(int*));
	}

	for ( i = 0; r = table_rowget(ifile, T, R, NULL, NULL, 0); i++ ) {
	    if ( have_min && have_max ) {
		double v = table_colvald(r, C[0]);
		int bin = Max(0, Min( (int)( (v - voff) * dz ), nbins - 1));

		if ( v < vmin ) {
		    minbin++; 
	    	    for ( i = 0; i < ncol; i++ ) { minsum[i] += table_colvald(r, C[i]); }
		    continue;
		}
		if ( v > vmax ) {
		    maxbin++; 
	    	    for ( i = 0; i < ncol; i++ ) { maxsum[i] += table_colvald(r, C[i]); }
		    continue;
		}

		H[bin]++;
	    	for ( i = 0; i < ncol; i++ ) { S[i][bin] += table_colvald(r, C[i]); }
	    } else {
		if ( av <= nv ) {
		    av *= 3;
		    av += 50;
	    	    for ( i = 0; i < ncol; i++ ) {
		        ReAlloc(V[i], av * sizeof(double));
		    }

		    if ( nv == 0 ) {
			vmin = table_colvald(r, C[0]);
			vmax = table_colvald(r, C[0]);
		    }
		}

	    	for ( i = 0; i < ncol; i++ ) {
		    V[i][nv] = table_colvald(r, C[i]);
		}
		vmin = Min(vmin, V[0][nv]);
		vmax = Max(vmax, V[0][nv]);
		nv++;
	    }
	}

	if ( !have_min || !have_max ) {
	    if ( have_max ) vmax = Vmax;
	    if ( have_min ) vmin = Vmin;

	    if ( !have_min && integer ) vmin = (int) vmin - 0.5;
	    if ( !have_max && integer ) vmax = (int) vmax + 0.5;

	    if ( !have_nbins ) 
		if ( integer )
		    nbins = Max( 1, (int)(Min( 64000, Abs(vmax-vmin))));
		else
		    nbins = Max( 1, (int)(Min( 64000, Abs(vmax-vmin) +1)));

	    if ( nbins < 1 )
		exit(0);

	    Calloc(H, nbins * sizeof(int));
	    for ( i = 0; i < ncol; i++ ) {
	        Calloc(S[i], nbins * sizeof(double));
	    }

	    dz      = (double) (nbins)/(vmax-vmin);
	    flip    = dz < 0.0;

	    voff = vmin;

	    if ( flip ) { vmax = vmax + 2 * EPSILON; }
	    else	{ vmin = vmin + 2 * EPSILON; }

	    for ( n = 0; n < nv; n++ ) {
		int bin = Max(0, Min( (int)( (V[0][n] - voff) * dz ), nbins - 1));

		if ( V[0][n] <  vmin ) {
		    minbin++; 
	    	    for ( i = 0; i < ncol; i++ ) { minsum[i] += V[i][n]; }
		    continue;
		}
		if ( V[0][n] > vmax ) {
		    maxbin++; 
	    	    for ( i = 0; i < ncol; i++ ) { maxsum[i] += V[i][n];; }
		    continue;
		}
		H[bin]++;
		for ( i = 0; i < ncol; i++ ) { S[i][bin] += V[i][n]; }
	    }
	}

	fprintf(ofile, "left	center	right	count	");
	for ( i = 0; i < ncol; i++ ) {
	    fprintf(ofile, "%s%c", table_colnam(T, C[i]), i == ncol -1 ? '\n' : '\t');
	}
	fprintf(ofile, "------	-----	------	-----	");
	for ( i = 0; i < ncol; i++ ) {
	    dashes(ofile, table_colnam(T, C[i]));
	    fprintf(ofile, "%c", i == ncol -1 ? '\n' : '\t');
	}
	if ( lt ) {
	    fprintf(ofile, "		%g	%d	", voff, minbin);
	    for ( i = 0; i < ncol; i++ ) {
		fprintf(ofile, "%g%c"	, minbin ? (minsum[i]/ (sum ? 1 : minbin)) : 0.0
					, i == ncol -1 ? '\n' : '\t');
	    }
	}

	for ( n = 0; n < nbins; n++ ) {
	    fprintf(ofile, "%g	%g	%g	%d\t"
		, vmin +  n 		/ dz
		, vmin + (n + 0.5 ) 	/ dz
		, vmin + (n + 1   )	/ dz
		, H[n]);

	    for ( i = 0; i < ncol; i++ ) {
		fprintf(ofile, "%g%c"	, H[n] ? (S[i][n]/ (sum ? 1 : H[n])) : 0.0
					, !gt && i == ncol -1 ? '\n' : '\t');
	    }
	}
	if ( gt ) {
	    fprintf(ofile, "%g			%d	", vmax, maxbin);
	    for ( i = 0; i < ncol; i++ ) {
		fprintf(ofile, "%g%c"	, maxbin ? (maxsum[i]/ (sum ? 1 : maxbin)) : 0.0
					, i == ncol -1 ? '\n' : '\t');
	    }
	}
}

void dashes(ofile, str)
	File	ofile;
	char	*str;
{
	while ( *str ) { FPrint(ofile, "-"); str++; };
}
