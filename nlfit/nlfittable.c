/* nlfittable.c

    > nlfittable 'x-a + (y-b)^2' z a=4,-4:8 b=12,0:10

*/
#include <xos.h>
#include <math.h>
#include <table.h>

#include "cmpfit-1.2/mpfit.h"

void    error();
void    dashes();
int     coprocess();

typedef struct FData {
        File    co_in;
        File    coout;
} FData;


int func(int m, int n, double *p, double *dy, double **dvec, void *data) {
    char line[128];
    int i;

    FData *fdata = (FData *) data;

    for ( i = 0; i < n-1; i++ ) { fprintf(fdata->co_in, "%.15f\t", p[i]); }
    fprintf(fdata->co_in, "%.15f\n", p[i]);
     fflush(fdata->co_in);

    for ( i = 0; i < m; i++ ) {
	dy[i] = strtod(fgets(line, 128, fdata->coout), NULL);
    }    

    fgets(line, 128, fdata->coout);

    return 0;
}

char *strrepl(char *str0, char *str1, char *repl) {
	char *here;
	char *this;

    if ( !(here = strstr(str0, str1)) ) {
	return str0;
    }

    this = malloc(strlen(str0) + strlen(repl));

    strncpy(this, str0, here-str0);
     strcpy(&this[here-str0], repl);
     strcpy(&this[here-str0+strlen(repl)], here+strlen(str1));

    free(str0);
    return this;
}

int main(argc, argv)
        int     argc;
        char    *argv[];
{
    char  fitargs[512];
    char  fitvals[512];
    char  datinit[4096];
    
    TableHead        head;
    TableRow        *rows;
    int              nrow;
    int              arow = 10;

    int			npar =  0;
    int			apar = 10;
    mp_par		*par = (mp_par *) calloc(10, sizeof(mp_par));
    double		*p   = (double *) calloc(10, sizeof(double));

    int		     mall = 10;

    int			fcol;
    int			debug = 0;
    int	i;

    FData fdata;
    mp_config config;

    memset(&config, 0, sizeof(config));

    char **xargv = malloc(argc*sizeof(char*));
    int    xargc = argc;

    if ( argc < 4 ) {
	fprintf(stderr, "\n"
"    nlfittable [options] expression fitcol param=initial[,[min]:[max]] < data\n"
"\n"
"	Reads a starbase table from the file \"data\" and uses the \"MPFIT\" package\n"
"	to fit \"expression\" to the data in \"fitcol\".  Parameters are provided\n"
"	with thier initial values and may have thier ranges limited using the min\n"
"	and max	notation.  A parameter's value may be fixed by omitting both the\n"
"	min and max values.\n"
"\n"
"	The expression is evaluated using the \"tawk\" scripting language and\n"
"	may contain references to fit parameters and columns in the data table.\n"
"\n"
"	It is often desirable to have the fit function in a separate file to allow\n"
"	clear formatting and comments.  This can be achived using tawk's automated\n"
"	function include facility.  Define the function in a spearate file with the same\n"
"	name as the function.  Set the TABLEFUN environemnt variable with the path to \n"
"	the directory where the function definition can be found.  Call the function in\n"
"	nlfittable's expression.\n"
"\n"
"	A new function \"merit\" is defined in a file also named \"merit\", in the current \n"
"	directory:\n"
"\n"
"	contents of file \"merit\":\n"
"	    function merit(X, K) {\n"
"		return X**K\n"
"	    }\n"
"\n"
"	call nlfittable:\n"
"\n"
"		> export TABLEFUN=.\n"
"		> nlfittable 'merit(X, K)' Y K=1,0:10 < data.tab\n"
"\n"
"\n"
"\n"
"\n"
"	Options:\n"
"\n"
"	  -i value	Max iterations\n"
"\n"
"	  -f value 	Relative Chi Square convergence         	Default: 1e-10\n"
"	  -x value	Relative parameter convergence criterium	Default: 1e-10\n"
"	  -g value	Orthogonality convergence criterium     	Default: 1e-10\n"
"	  -s value	Initial step bound                       	Default: 100.0\n"
"	  -c value	Range tolerance for covariance calculation	Default: 1e-14\n"
"	  -e value	Finite derivative step size               Default: MP_MACHEP0\n"
"\n"
"	  -d      	Debug\n"
"	  -D      	Debug more\n"
"\n"
"	Example:\n"
"\n"
"\n"
"\n"
"\n"
"\n");
	exit(1);
    }

    for ( i = 0; i < argc; i++ ) {
	xargv[i] = strdup(argv[i]);
    }

    config.nprint = 0;

    while ( argc ) {
        if ( argv[1][0] == '-' ) {
            switch ( argv[1][1] ) {
	     case 'd' : debug      = 1;                      			break;
	     case 'D' : debug      = 2;                      			break;
	     case 'f' : config.ftol   = strtod(argv[2], NULL); argv++; argc--;	break;
	     case 'x' : config.xtol   = strtod(argv[2], NULL); argv++; argc--;	break;
	     case 'g' : config.gtol   = strtod(argv[2], NULL); argv++; argc--;	break;
	     case 'c' : config.covtol = strtod(argv[2], NULL); argv++; argc--;	break;
	     case 'e' : config.epsfcn = strtod(argv[2], NULL); argv++; argc--;	break;
	     case 's' : config.epsfcn = strtod(argv[2], NULL); argv++; argc--;	break;
	     case 'i' : config.maxiter = atoi(argv[2]); argv++; argc--; 	break;
            }
	    argv++;
	    argc--;

            continue;
        } 

	break;
    }

    char *expression = argv[1];
    char *fitcol     = argv[2];



    fitargs[0] = '\0';
    fitvals[0] = '\0';
    datinit[0] = '\0';

    /* Read in the data table
     */
    if ( !(head = table_header(stdin, 0)) ) {
        error("nlfittable: cannot read data table");
    }
    if ( !(fcol = table_colnum(head, fitcol)) ) {
        error("nlfittable: fit column not in input table");
    }

    for ( i = 1; i <= head->header->ncol; i++ ) {
	sprintf(&fitargs[strlen(fitargs)],           "%s%s", fitargs[0] ? ", " : "",  head->header->column[i]);
	sprintf(&fitvals[strlen(fitvals)],      "%s_%s[_i]", fitvals[0] ? ", " : "",  head->header->column[i]);
	sprintf(&datinit[strlen(datinit)], "%s_%s[i] = $%d\n", datinit[0] ? "\t\t  " : "", head->header->column[i], i);
    }


    Malloc(rows, arow * sizeof(TableRow));

    for ( nrow = 0; (rows[nrow] = table_rowget(stdin, head, NULL, NULL, NULL, 0));) {
        nrow++;

        if ( nrow >= arow ) {
            arow += 100;
            arow *= 1.5;
            ReAlloc(rows, arow * sizeof(TableRow));
        }
    }

    npar = 0;

    for ( i = 3; i < argc; i++ ) {
#define NCOLFLAGS	5
#define EQUAL           0
#define MIN	        1
#define MAX    	    	2
#define STEP		3
#define PERCENT		4

	TableColArg     value[NCOLFLAGS];
	int             check[NCOLFLAGS];


	char    *flag   =   "=,:+%\001";
	char    *form[] = { "s", "s", "s", "r", "r" };

	int k;

        if ( argv[i][0] == '-' ) {
            switch ( argv[i][1] ) {
	     case 'd' : debug      = 1;                      break;
	     case 'D' : debug      = 2;                      break;
            }
            continue;
        }


	for ( k = 0; k < NCOLFLAGS; k++ ) check[k]   = 0;
	for ( k = 0; k < NCOLFLAGS; k++ ) value[k].f = 0.0;
	
	if ( table_colarg(head, argv[i], flag, form, value, check) ) {
	    error("nlfittable: fit parameter name may not be a column in the input");
	} 

	if ( npar >= apar ) {
	    apar *= 2;
            ReAlloc(par, apar * sizeof(mp_par));
            ReAlloc(p,   apar * sizeof(double));
	}

	if ( check[EQUAL] ) { p[npar] = strtod(value[EQUAL].s, NULL); }
	if ( check[MIN] ) {
	    par[npar].limited[0] = value[MIN].s[0] != '\0';
	    par[npar].limits[0]  = strtod(value[MIN].s, NULL);
	}
	if ( check[MAX] ) {
	    par[npar].limited[1] = value[MAX].s[0] != '\0';
	    par[npar].limits[1]  = strtod(value[MAX].s, NULL);
	}

	par[npar].fixed = check[MAX] && !par[npar].limited[0] && !par[npar].limited[1];

	if ( check[STEP]    ) { par[npar].step    = value[STEP].f; }
	if ( check[PERCENT] ) { par[npar].relstep = value[PERCENT].f; }

	par[npar].parname = strdup(argv[i]);

	if ( debug ) {
	    fprintf(stderr, "constrants : %s start=%f, fixed : %d, limited : (%d, %d), limits (%f, %f)\n"
			, par[npar].parname, p[npar], par[npar].fixed
			, par[npar].limited[0], par[npar].limited[1]
			, par[npar].limits[0] , par[npar].limits[1]);
	}
	npar++;
    }

    /* Make a program to evaluate the user function
     */
    char *program = strdup("					\n\
	function evaluate(_FITARGS_) {				\n\
	     return _FITCOL_ - ( _EXPRESSION_ )			\n\
	}							\n\
								\n\
	BEGIN {							\n\
		i = 0						\n\
								\n\
		while ( getline > 0 ) {				\n\
		  if ( $1 == \"\" ) { break }			\n\
		  i++						\n\
		  _DATINIT_					\n\
		}						\n\
								\n\
		_NData = i					\n\
	}							\n\
	{							\n\
		for ( _i = 1; _i <= _NData; _i++ ) {		\n\
		    printf(\"%.15f\\n\", evaluate(_FITVALS_))	\n\
		}						\n\
		print \"\"					\n\
		fflush();					\n\
	}");

    program = strrepl(program, "_FITARGS_"   , fitargs);
    program = strrepl(program, "_FITCOL_"    , fitcol);
    program = strrepl(program, "_EXPRESSION_", expression);
    program = strrepl(program, "_DATINIT_"   , datinit);
    program = strrepl(program, "_FITVALS_"   , fitvals);

    if ( debug ) { printf("%s\n", program); }

    { char 	*args[10];
      int	coproc;
      int	in;
      int	out;

	/* Start the coprocess which will evaluate the user function
	 */
	i = 0;
	args[i++] = "table";
	if ( debug == 2 ) args[i++] = "-D";
	args[i++] = "-I";
	args[i++] = "-h";
	args[i++] = program;
	args[i++] = NULL;

	coproc = coprocess("table", args, &in, &out, NULL);

	fdata.co_in = OpenFd(in,  "w");
	fdata.coout = OpenFd(out, "r");

	for ( i = 0 ; i < npar-1; i++ ) { fprintf(fdata.co_in, "%s\t", par[i].parname); }
	fprintf(fdata.co_in, "%s\n", par[i].parname);

	for ( i = 0 ; i < npar-1; i++ ) { fprintf(fdata.co_in, "-\t"); }
	fprintf(fdata.co_in, "-\n");
	    

	for ( i = 0; i < nrow; i++ ) {
	    table_rowput(fdata.co_in, head, rows[i], 0, NULL, 0);
	}
	fprintf(fdata.co_in, "\n");
	 fflush(fdata.co_in);
    }

    {
	char *statusCodes[] = {
	  "", "Convergence in chi-square value"
	    , "Convergence in parameter value"
	    , "Convergence in chi-square and parameter value"
	    , "Convergence in orthogonality"
	    , "Maximum number of iterations reached"
	    , "ftol is too small; no further improvement"
	    , "xtol is too small; no further improvement"
	    , "gtol is too small; no further improvement"
	};


	mp_result result;

	memset(&result, 0, sizeof(result));

	int status = mpfit(func, nrow, npar, p, par, &config, &fdata, &result);

	switch ( status ) {
	 case MP_ERR_INPUT: 	 error("General input parameter error 0");
	 case MP_ERR_NAN:        error("User function produced non-finite values");
	 case MP_ERR_FUNC:       error("No user function was supplied");
	 case MP_ERR_NPOINTS:    error("No user data points were supplied");
	 case MP_ERR_NFREE:      error("No free parameters");
	 case MP_ERR_MEMORY:     error("Memory allocation error");
	 case MP_ERR_INITBOUNDS: error("Initial values inconsistent w constraints");
	 case MP_ERR_BOUNDS:     error("Initial constraints inconsistent");
	 case MP_ERR_PARAM:      error("General input parameter error");
	 case MP_ERR_DOF:        error("not enough degrees of freedom");
	}
	if ( status < 0 ) {
	    error("Unknown error status : %d\n", status);
	}


	printf("\n");
	printf("Fit	");
	for ( i = 0; i < xargc; i++ ) {
	    printf(" %s", xargv[i]);
        }
	printf("\n");
	printf("\n");
	printf("FitChi	%f\n", result.bestnorm);
	printf("NIters	%d\n", result.niter);
	printf("Status	%d	%s\n", status, statusCodes[result.status]);

	printf("Params");
	for ( i = 0 ; i < npar; i++ ) {
	    printf("\t%s", par[i].parname);
	}
	printf("\n");
	printf("Params");
	for ( i = 0 ; i < npar; i++ ) {
	    printf("\t");
	    dashes(0, par[i].parname);
	}
	printf("\n");
	printf("Params");
	for ( i = 0 ; i < npar; i++ ) {
	    printf("\t%f", p[i]);
	}
	printf("\n");
	printf("\n");
    }

    {
	double *dy = malloc(nrow*sizeof(double));
	double Sd2;

	func(nrow, npar, p, dy, NULL, &fdata);

	for ( i = 0; i < nrow; i++ ) {
	   Sd2 = dy[i]*dy[i];
	}

	printf("RMS	%f\n", sqrt(Sd2/nrow));
	printf("\n");

	table_ors(head, '\t');

	table_hdrput(stdout, head, TBLHDR_HEADLINE, NULL, NULL, 0);
	Print("%s_Fit	%s_Res\n", fitcol, fitcol);

	table_hdrput(stdout, head, TBLHDR_DASHLINE, NULL, NULL, 0);
	dashes(4, fitcol);    Print("\t");
	dashes(4, fitcol);    Print("\n");

	for ( i = 0; i < nrow; i++ ) {
	    table_rowput(stdout, head, rows[i], NULL, NULL, 0);

	    Print("%f	%f\n", table_colvald(rows[i], fcol)-dy[i], dy[i]);
	}
    }
}

void dashes(n, str)
        int     n;
        char    *str;
{
        while ( n-- )    Print("-");
        while ( *str ) { Print("-"); str++; };
}

