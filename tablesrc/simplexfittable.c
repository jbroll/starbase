/* simplexfittable.c
 */
/* Starbase Data Tables	- An Ascii Database for UNIX

   copyright 2004 John B. Roll jr.
 */

#include <xos.h>
#include <math.h>
#include <table.h>

#include "simplex.h"

void	error();
void	dashes();
int 	coprocess();

typedef struct FitPar {
	char	*name;
	int	xmin;
	double	 min;
	int	xmax;
	double	 max;
	int	xdel;
} *FitPar;


typedef struct FData {
	File	co_in;
	File	coout;

	TableHead	 head;
	TableRow	*rows;
	int		 nrow;

	FitPar		 par;
	double		*fit;
	double		*val;
} *FData;

double funct(fdata, nvec, vec)
	FData	 fdata;
	int	 nvec;
	double	*vec;
{
	char	line[128];

	double	sum = 0.0;
	double	val;
	int	i;
	int	n;

    for ( i = 0; i < nvec; i++ ) {
	if ( fdata->par->xmax && vec[i] > fdata->par->max ) vec[i] = fdata->par->max;
	if ( fdata->par->xmin && vec[i] < fdata->par->min ) vec[i] = fdata->par->min;
    }

    for ( i = 0; i < fdata->nrow; i++ ) {
	table_rowput(fdata->co_in, fdata->head, fdata->rows[i], 0, NULL, 0);

	for ( n = 0; n < nvec - 1; n++ ) {
	    fprintf(fdata->co_in, "%.16f\t", vec[n]); 
	}
	fprintf(fdata->co_in, "%.16f\n", vec[n]);
	 fflush(fdata->co_in);

	ReadLine(fdata->coout, line, 128);

	fdata->val[i] = val = strtod(line, NULL);
	val -= fdata->fit[i];
	sum += val * val;
    }

    return sum;
}


Simplex *simplex_create();
double  *simplex_search();
void     simplex_display();

void main(argc, argv)
        int     argc;
        char    *argv[];
{
	int	i;
	int	n;

        int      iterations	= 500;
        int      extras		=   1;
        double   tolerence	=   0.00001;

#define	NHDRFLAGS	4

	char *flag   =   "=#:%\001";
	char *form[] = { "r", "r", "r", "r" };

	struct FitPar	par[10];
	double		start[10];
	double		sdelt[10];
        int	  	check[NHDRFLAGS];
        TableColArg 	value[NHDRFLAGS];
	int		k;

	char	*expr = argv[1];
	char	*fnam = argv[2];
	int	 fcol;

	char	*program;
	char	*args[10];

	TableHead	 head;
	TableRow	*rows;
	int		 nrow;
	int		 arow = 10;

	int		coproc;
	int		in;
	int		out;


	struct FData	fdata;

	Simplex		*sim;
	double		*bet;

	int		display  = 0;
	int		debug    = 0;
 
    if ( argc < 4 ) {
	fprintf(stderr, "usage: simplexfittable < data.tab expression fit_column [-t tol] [-d] [-D] -[i iters] fit_variable=start#min:max%%delta ...\n");
	exit(1);
    }

    /* Read in the data table 
     */
    if ( !(head = fdata.head = head = table_header(stdin, 0)) ) {
	error("simplexfittable: cannot read data table");
    }

    if ( !(fcol = table_colnum(head, fnam)) ) {
	error("simplexfittable: fit column not in input table");
    }

    Malloc(rows, arow * sizeof(TableRow));

    for ( nrow = 0;
	(rows[nrow] = table_rowget(stdin, head, NULL, NULL, NULL, 0));) {

	nrow++;

	if ( nrow >= arow ) {
	    arow += 100;
	    arow *= 1.5;
	    ReAlloc(rows, arow * sizeof(TableRow));
	}
    }

    /* Convert the column of data to fit to double
     */
    Malloc(fdata.fit, nrow * sizeof(double));
    Malloc(fdata.val, nrow * sizeof(double));

    for ( i = 0; i < nrow; i++ ) {
	fdata.fit[i] = table_colvald(rows[i], fcol);
    }

    fdata.nrow = nrow;
    fdata.rows = rows;

    /* Parse the arguments and the free parameter columns
     */
    n = 0;
    for ( i = 3; i < argc; i++ ) {
	if ( argv[i][0] == '-' ) {
	    switch ( argv[i][1] ) {
		case 'i' : iterations = atoi(argv[++i]);	break;
		case 'e' : extras     = atoi(argv[++i]);	break;
		case 't' : tolerence  = atof(argv[++i]);	break;
		case 'd' : display    = 4;			break;
		case 'D' : debug      = 1;			break;
	    }
	} else {
	    for ( k = 0; k < NHDRFLAGS; k++ ) check[k]   = 0; 
	    for ( k = 0; k < NHDRFLAGS; k++ ) value[k].f = 0.0; 

	    if ( table_colarg(head, argv[i], flag, form, value, check) ) {
		error("simplexfittable: fit parameter name may not already exits in the input");
	    } else {
		par[n].name   = argv[i];
		par[n].min    = value[1].f;
		par[n].max    = value[2].f;

		par[n].xmin   = check[1];
		par[n].xmax   = check[2];

		start[n]      = value[0].f;
		if ( !(par[n].xdel = check[3]) ) {
		    if ( start[n] == 0.0 ) {
			sdelt[n] = 1.0;
		    } else {
			sdelt[n] = start[n] * 0.1;
		    }
		} else {
		    sdelt[n] = value[3].f;
		}
		   
		n++;
	    }
        }
    }

    fdata.par = par;

    Malloc(program, strlen(expr) + 500);
    sprintf(program, "BEGIN { OFMT=\"%%22f\"; CONVFMT=\"%%22f\"; fflush(\"/dev/stderr\") }\n{ print %s; }\n", expr);

    i = 0;
    args[i++] = "table";
    if ( debug ) args[i++] = "-D";
    args[i++] = "-I";
    args[i++] = "-h";
    args[i++] = program;
    args[i++] = NULL;

    /* Start the coprocess which will evaluate the user function
     */
    coproc = coprocess("table", args, &in, &out, NULL);

    fdata.co_in = OpenFd(in,  "w");
    fdata.coout = OpenFd(out, "r");

    /* Send the table header to the coprocess
     */
    table_hdrput(fdata.co_in, head, TBLHDR_VALUES | TBLHDR_TEXT, NULL, NULL, 0);

    table_ors(head, '\t');
    table_hdrput(fdata.co_in, head, TBLHDR_HEADLINE, NULL, NULL, 0);
    for ( i = 0; i < n - 1; i++ ) {
	fprintf(fdata.co_in, "%s\t", par[i].name);
    }
    fprintf(fdata.co_in, "%s\n", par[i].name);

    table_hdrput(fdata.co_in, head, TBLHDR_DASHLINE, NULL, NULL, 0);
    for ( i = 0; i < n - 1; i++ ) {
	fprintf(fdata.co_in, "%s\t", "-");
    }
    fprintf(fdata.co_in, "%s\n", "-");
    fflush(fdata.co_in);

    double y0 = funct(&fdata, n, start);

    sim = simplex_create(n, y0, start, sdelt, NULL, &iterations
		    , funct, fdata
		    , NULL, NULL);

    bet = simplex_search(sim, tolerence, &iterations, extras
		   , funct, fdata
		   , simplex_display, &display);

    y0        = funct(&fdata, n, &bet[1]);

    Print("\n\nsimplexfittable	%s	%s	", expr, fnam);
    for ( i = 0; i < n; i++ ) {
	Print("%s=%f", par[i].name, start[i]);
	if ( par[i].xmin ) Print("#%f",  par[i].min);
	if ( par[i].xmax ) Print("#%f",  par[i].max);
	if ( par[i].xdel ) Print("%%%f", sdelt);
        Print(" ");
    }
    Print("\n\n");

    Print("RMS_%s	%g\n\n", fnam, sqrt(y0));

    Print("Coeff\t");
    for ( i = 1; i < n; i++ ) {
	Print("%.17f\t", bet[i]);
    }
    Print("%.17f\n\n", bet[i]);

    table_hdrput(stdout, head, TBLHDR_HEADLINE, NULL, NULL, 0);
    Print("%s_Fit	%s_Res\n", fnam, fnam);

    table_hdrput(stdout, head, TBLHDR_DASHLINE, NULL, NULL, 0);
    dashes(4, fnam);	Print("\t");
    dashes(4, fnam);	Print("\n");

    for ( i = 0; i < nrow; i++ ) {
        table_rowput(stdout, head, fdata.rows[i], NULL, NULL, 0);
	Print("%g	%g\n", fdata.fit[i], fdata.val[i] - fdata.fit[i]);
    }

    exit(0);
}

void dashes(n, str)
	int	n;
	char	*str;
{
	while ( n-- )    Print("-");
	while ( *str ) { Print("-"); str++; };
}
