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

	double	*V = NULL;
	double	 Vmin, Vmax;
	double	 vmin, voff;
	double	 vmax;

	int	minbin = 0;
	double	minsum = 0;
	int	maxbin = 0;
	double	maxsum = 0;

	int	lt = 0;
	int	gt = 0;

	int	 integer = 1;

	int	*H;
	double	*S;
	int	 i;

	File	ifile = Stdin;
	File	ofile = Stdout;

	TableHead	T = NULL;
	TableRow	R, r;

	int		column;
	int		av = 0;
	int		nv = 0;

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


		 default:
			FPrint(Stderr, "histtable: unknown option -%c\n", argv[i][1]);
			exit(1);
		}
		continue;
	    } else {
		T = table_header(ifile, 0);
		column = table_colnum(T, argv[i]);
	    }
	}

	if ( T == NULL ) {
	  FPrint(Stderr, "usage: histtable [-g] [-l] [-min min] [-max max] [-n bins] Column\n");
	  exit(1);
	}

	R = table_rowloc(NULL, 0, 0);

	if ( have_max ) vmax = Vmax;
	if ( have_min ) vmin = Vmin;
	if ( have_min && have_max ) {
	    if ( !have_nbins ) {
		if ( integer )
		    nbins = Max( 1, (int)(Min( 64000, Abs(vmax-vmin))));
		else
		    nbins = Max( 1, (int)(Min( 64000, Abs(vmax-vmin) +1)));
	    }

	    if ( nbins < 1 ) {
		fprintf(Stderr, "histtable: no bins?\n");
		exit(0);
	    }

	    dz      = (double) (nbins)/(vmax-vmin);
	    flip    = dz < 0.0;

	    voff = vmin;

	    if ( flip ) { vmin = vmin - 2 * EPSILON; }
	    else 	{ vmax = vmax + 2 * EPSILON; }

	    Calloc(S, nbins * sizeof(double));
	    Calloc(H, nbins * sizeof(int));
	}
	for ( i = 0; r = table_rowget(ifile, T, R, NULL, NULL, 0); i++ ) {
	    if ( have_min && have_max ) {
		double v = table_colvald(r, column);
		int bin = Min( (int)( (v - voff) * dz ), nbins - 1 );

		if ( v > vmax ) { maxbin++;  continue; }
		if ( v < vmin ) { minbin++;  continue; }

		S[bin] += v;
		H[bin]++;
	    } else {
		if ( av <= nv ) {
		    av *= 3;
		    av += 50;
		    ReAlloc(V, av * sizeof(double));

		    if ( nv == 0 ) {
			vmin = table_colvald(r, column);
			vmax = table_colvald(r, column);
		    }
		}

		V[nv] = table_colvald(r, column);
		vmin = Min(vmin, V[nv]);
		vmax = Max(vmax, V[nv]);
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

	    Calloc(S, nbins * sizeof(double));
	    Calloc(H, nbins * sizeof(int));

	    dz      = (double) (nbins)/(vmax-vmin);
	    flip    = dz < 0.0;

	    voff = vmin;

	    if ( flip ) { vmin = vmin - 2 * EPSILON; }
	    else 	{ vmax = vmax + 2 * EPSILON; }

	    for ( i = 0; i < nv; i++ ) {
		int bin = Min( (int)( (V[i] - voff) * dz ), nbins - 1 );

		if ( V[i] > vmax ) { maxbin++;  continue; }
		if ( V[i] < vmin ) { minbin++;  continue; }

		S[bin] += V[i];
		H[bin]++;
	    }
	}

	fprintf(ofile, "left	center	right	count	average\n");
	fprintf(ofile, "------	-----	------	-----	-------\n");
	if ( lt ) {
	    fprintf(ofile, "		%g	%d	%g\n", voff, minbin, minbin ? minsum/minbin : 0.0);
	}

	for ( i = 0; i < nbins; i++ ) {
	    fprintf(ofile, "%g	%g	%g	%d	%g\n"
		, vmin + i 		/ dz
		, vmin + (i + 0.5 ) 	/ dz
		, vmin + (i + 1   )	/ dz
		, H[i]
		, H[i] ? S[i]/H[i] : 0.0);
	}
	if ( gt ) {
	    fprintf(ofile, "%g			%d	%g\n", vmax, maxbin, maxbin ? maxsum/maxbin : 0.0);
	}
}
