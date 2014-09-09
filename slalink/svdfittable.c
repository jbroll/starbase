/* svdfittable.c
 */
/* Starbase Data Tables	- An Ascii Database for UNIX

   copyright 1991, 1993, 1995, 1999 John B. Roll jr.

 */

#include <xos.h>
#include <xfile.h>
#include <math.h>
#include <table.h>
#include <slalib.h>

#include <unistd.h>

typedef struct _TData {
	File	  f;
	TableHead head;
	TableRow *rows;
	int 	  nrow;
} *TData;

char *stdmodel();
void  dashes();

void w_routine(w_data)
	TData	w_data;
{
	int	i;

    table_hdrput(w_data->f, w_data->head, 0, NULL, NULL, 0);

    for ( i = 0; i < w_data->nrow; i++ ) {
	table_rowput(w_data->f, w_data->head, w_data->rows[i], NULL, NULL, 0);
    }

    Close(w_data->f);
}

int r_routine(f, r_data)
	File	f;
	TData	r_data;
{
	TableHead	 head;
	TableRow	*rows;
	int		 nrow;
	int		 arow = 10;

    Malloc(rows, arow * sizeof(TableRow));

    if ( !(head = table_header(f, 0)) ) {
	fprintf(stderr, "svdfittable : no head read back on design matrix\n");
	exit(1);
    }

    for ( nrow = 0;
	(rows[nrow] = table_rowget(f, head, NULL, NULL, NULL, 0));) {

	nrow++;

	if ( nrow >= arow ) {
	    arow += 100;
	    arow *= 1.5;
	    ReAlloc(rows, arow * sizeof(TableRow));
	}
    }

    r_data->head = head;
    r_data->rows = rows;
    r_data->nrow = nrow;
 
    return 1;
}

int e_routine(f, e_data)
	File	f;
	TData	e_data;
{
	int	red;
	char	buffer[4096];

    if ( (red = Read(f, buffer, 1, 4096)) > 0 ) {
	FPrint(stderr, "svdfittable: error from coprocess\n"); Write(stderr, buffer, 1, red);
	exit(1);
    } else {
	return 1;
    }
}

int main(argc, argv)
        int     argc;
        char    *argv[];
{
	int	nrow = 0;	/* Number of data points	*/
	int	ncol = 0;	/* Number of basis functions	*/
	int	arow = 1;

	struct _TData w_data;
	struct _TData r_data;
	    
	double	**Y, **X, *Var, *A, *Design, *W, *V, *Work, *CVM, **C;
	double	*a;
	double	chisq;
	double	T = 0.00001;
	
	int	i, j, k;

	int	dproger = 0;
	int	design = 0;

	char	**model;
	int	 nmodel = 0;
	int	  mall = 10;
	int	 lmodel = 0;

	TableHead  head;
	TableRow  *rows;

	int	 *just;

	int	  ycol =  0;
	int	 *ycols;
	int	  yall = 10;
	char	**yname;

	int	  order = 0;
	int	  cross = 0;
	int	  zero  = 0;

	char	  stdmodel_str[2048];
	char	 *term[128];
	int	  terms;

	char	*format = "%.15g";


    Malloc(yname, sizeof(char *) * yall);
    Malloc(model, sizeof(char *) * mall);

    for ( i = 1; i < argc; i++ ) {
	if ( argv[i][0] == '-' ) {
	    switch ( argv[i][1] ) {
		case 'f':	 format = argv[++i];		continue;
		case 'p':        dproger++;			continue;
		case 'd':        design++;			continue;
		case 'x':        cross++;			continue;
		case 'n' : i++;  order = atoi(argv[i]); 	continue;
		case 'm' : i++;  model[nmodel++] = argv[i]; lmodel += strlen(argv[i]);	continue;
		case 's' : break;
		case 'z' : 	zero++;				continue;
	    }
	    continue;
	}
	if ( ycol >= yall ) {
	    yall += 10;
	    ReAlloc(yname, sizeof(char *) * yall);
	}
	if ( nmodel >= mall ) {
	    mall += 10;
	    ReAlloc(model, sizeof(char *) * mall);
	}

	yname[ycol++] = argv[i];
    }

    if ( ycol == 0 ) {
	FPrint(stderr, "No independant variable specified\n");
	exit(1);
    }

    if ( order ) {
	model[0] = stdmodel(stdmodel_str, term, &terms, model, order, cross);
    }
    if ( nmodel == 0 ) {
	FPrint(stderr, "No model specified\n");
	exit(1);
    }

    for ( j = 0; j < nmodel; j++ ) {
	int Ncol = 0;

	for ( i = 0, Ncol = 0; model[j][i]; i++ ) {
	    if ( model[j][i] == ',' ) Ncol++;
	}
        Ncol++;

	if ( ncol && Ncol != ncol ) {
	    fprintf(stderr, "svdfittable : each model must have the same number of terms\n");
	    exit(1);
	}

	ncol = Ncol;
    }


    head = table_header(stdin, 0);

    Malloc(just, (table_ncol(head)+1) * sizeof(int));
    for ( i = 0; i <= table_ncol(head); i++ ) {
	just[i] = TAB_DEFAULT;
    }

    Malloc(   Y, ycol * sizeof(double **));
    Malloc(   X, ycol * sizeof(double **));
    Malloc(   C, ycol * sizeof(double **));
    Malloc( Var, ycol * sizeof(double));
    Malloc(ycols, sizeof(int) * yall);

    Malloc(rows, arow * sizeof(TableRow));
    for ( i = 0; i < ycol; i++ ) {
	if ( (ycols[i] = table_colnum(head, yname[i])) == 0 ) {
	    FPrint(stderr, "The dependent variable column name %s wasn't found in the table\n", yname[i]);
	    exit(1);
	}
	just[ycols[i]] = TAB_SKIP;
        Malloc(Y[i], arow * sizeof(double));
    }

    for ( nrow = 0;
	(rows[nrow] = table_rowget(stdin, head, NULL, NULL, NULL, 0));) {

	if ( rows[nrow]->ncol != head->header->ncol ) { 
	    fprintf(stderr, "svdfittable: bad number of columns in row %d skipping\n"
		, nrow+1);
	    continue;
	}

	for ( i = 0; i < ycol; i++ ) {
	    Y[i][nrow]   = table_colvald(rows[nrow], ycols[i]);
	}

	nrow++;

	if ( nrow >= arow ) {
	    arow += 100;
	    arow *= 1.5;
	    ReAlloc(rows, arow * sizeof(TableRow));
	    for ( i = 0; i < ycol; i++ ) {
		ReAlloc(Y[i]   , arow * sizeof(double));
	    }
	}
    }

    {
	    int in, out, err;
	    int coproc, writer;
	    int status;

	    int	ith = 0;

	    char	*args[10];
	    char 	*program;

	    Malloc(program, (lmodel + 20)*nmodel + 50);	program[0] = '\0';
	    strcat(program, "BEGIN { OFMT=\"%.22f\"; CONVFMT=\"%.22f\" }\n");
	    for ( i = 0; i < nmodel; i++ ) {
	    	sprintf(&program[strlen(program)], "{ print %s }\n", model[i]);
	    }

	    if ( dproger == 1 ) {
		fprintf(stderr, "%s\n", program);
		exit(1);
	    }

	    args[ith++] = "table";
	    args[ith++] = "-h";
	    args[ith++] = "-p";
	    if ( dproger == 2 ) args[ith++] = "-D";
	    args[ith++] = program;
	    args[ith++] = NULL;

	/* Start the coprocess which will create the design matrix
	 */
	coproc = coprocess("table", args, &in, &out, &err);

	w_data.f = OpenFd(in, "w");
	w_data.head = head;
	w_data.rows = rows;
	w_data.nrow = nrow;

	/* Start the coroutine which will write the data table
	   to the coprocess.  This completely avoids the dead
	   lock io problem at the expence of a second process.
	 */
	writer = coroutine(w_routine, &w_data);
	close(in);  /* Close the coprocess input here so that it won't hang */

	{
		    int	npolls = 2;
		    Polls	polls[2];

		    int		(*actions[2])();
		    void	 *adata[2];

		polls[0].f     = OpenFd(out, "r");
		polls[0].qmode = XFREAD;
  		polls[1].f     = OpenFd(err, "r");
		polls[1].qmode = XFREAD;

		r_data.f       = polls[0].f;
		r_data.head    = NULL;
		r_data.rows    = NULL;
		r_data.nrow    = 0;

		actions[0]     =  r_routine;
		adata[0]       = &r_data;
		actions[1]     = e_routine;
		adata[1]       = NULL;

		//r_routine(polls[0].f, &r_data);

	    /* Read back the design matrix, while watching
	       for output on stderr.
	     */
	    muxio(npolls, polls, actions, adata, -1.0);
	}

	if ( w_data.nrow*nmodel != r_data.nrow ) {
	    fprintf(stderr, "svdfittable : %d rows read but there are %d rows in the design matrix\n"
		    , w_data.nrow, r_data.nrow);
	    exit(1);
	}

	if ( design ) {
	    for ( i = 1; i <= ncol; i++ ) { fprintf(stdout, "c%d\t", i);	}
	    for ( j = 0; j < ycol; j++ ) {
		fprintf(stdout, "%s%c", yname[j], (j == ycol -1) ? '\n' : '\t');
	    }

	    for ( i = 1; i <= ncol; i++ ) { fprintf(stdout, "---\t");	}
	    for ( j = 0; j < ycol; j++ ) {
		fprintf(stdout, "---%c", (j == ycol -1) ? '\n' : '\t');
	    }

	    table_ors(r_data.head, '\t');
	    for ( i = 0; i < nrow * nmodel; i++ ) {
		table_rowput(stdout, r_data.head, r_data.rows[i], NULL, NULL, 0);
		for ( j = 0; j < ycol; j++ ) {
		    fprintf(stdout, "%f%c", Y[j][i], (j == ycol -1) ? '\n' : '\t');
		}
	    }

	    exit(0);
	}
    }

    Malloc(Design, nmodel * nrow * ncol * sizeof(double));
    Malloc(     A, nmodel * nrow * ncol * sizeof(double));
    Malloc(     W,                 ncol * sizeof(double));
    Malloc(     V, nmodel * nrow * ncol * sizeof(double));
    Malloc(  Work,                 ncol * sizeof(double));
    Malloc(   CVM,          ncol * ncol * sizeof(double));

    a = Design;
    for ( i = 0; i < nrow * nmodel; i++ ) {
        for ( j = 0; j < ncol; j++ ) {
	    a[j] = table_colvald(r_data.rows[i], j+1);
	}
	a += ncol;
    }
    memcpy(A, Design,  nmodel * nrow * ncol * sizeof(double));

    slaSvd(nmodel * nrow, ncol, nmodel * nrow, ncol, A, W, V, Work, &j);
    slaSvdcov(ncol, ncol, ncol, W, V, Work, CVM);

    if ( zero ) {
	int wmax = 0;
	int wmin;

      for ( i = 0; i < ncol; i++ ) { if ( W[i] > wmax ) { wmax = W[i]; } }

      wmin = wmax * 0.000001;
      for ( i = 0; i < ncol; i++ ) { if ( W[i] < wmin ) { W[i] = 0.0;  } }	/* How small is small? */
    }

    if ( j == -1 ) {
	FPrint(stderr, "svd returns an error\n");
	exit(1);
    }

    if ( nmodel == 1 ) {
	for ( i = 0; i < ycol; i++ ) {
	    Malloc(C[i], ncol * sizeof(double));
	    slaSvdsol(nrow, ncol, nrow, ncol, Y[i], A, W, V, Work, C[i]);
	}
    } else {
	double	*c;
	double	*y;

	Malloc(c, ncol * sizeof(double));
	Malloc(y, nmodel * nrow * sizeof(double));
        for ( i = 0; i < ycol; i++ ) {
	    C[i] = c;

	    for ( j = 0; j < nrow; j++ ) {
		y[j * nmodel + i] = Y[i][j];
	    }
	}

        slaSvdsol(nmodel * nrow, ncol, nmodel * nrow, ncol, y, A, W, V, Work, c);
    } 


    table_hdrput(stdout, head, TBLHDR_TEXT, NULL, NULL, 0);
    table_ors(head, '\t');

    Print("\n\n");
    Print("svdfittable Results:\n\n");
    for ( i = 0; i < nmodel; i++ ) {
        Print("Model	%s\n", model[i]);
    }
    Print("\n");
    Print("RMS for each column fit:\n");

    for ( i = 0; i < ycol; i++ ) {
	    double Sd2 = 0.0;

        Malloc(X[i], nrow * sizeof(double));

	a = Design + i * (nmodel - 1) * ncol;
	for ( j = 0; j < nrow; j++ ) {
		double Dx;

	    X[i][j] = 0;
	    for ( k = 0; k < ncol; k++ ) {
		X[i][j] += a[k] * C[i][k];
	    }

	    Dx = Y[i][j] - X[i][j];
	    Sd2 += Dx*Dx;
	    a += ncol * nmodel;
	}
	Var[i] = Sd2/nrow;
	    
	Print("RMS_%s	%g\n", yname[i], sqrt(Var[i]));
    }

    Print("\nCoefficients for each fit:\n");
    for ( i = 0; i < ycol; i++ ) {
	Print("C_%s", yname[i]);
	for ( j = 0; j < ncol-1; j++ ) {
	    Print("	");
	    Print(format, C[i][j]);
	}
	Print("	");
	Print(format, C[i][j]);
	Print("\n");

	if ( nmodel > 1 ) break;
    }

    Print("\n");

    {	int	 ncol	= table_ncol(head);	
	int	 nsel;
	int	*sel	= malloc(sizeof(int) * (ncol+1));

	char	 name[128];

    for ( i = 0; i <= ncol; i++ ) sel[i] = i;

    for ( i = 0; i < ycol; i++ ) {
	sprintf(name, "%s_Fit", yname[i]);
	sel[table_colnum(head, name)] = 0;

	sprintf(name, "%s_Res", yname[i]);
	sel[table_colnum(head, name)] = 0;
    }

    for ( nsel = 1, i = 1; i <= ncol; i++ ) if ( sel[i] ) { sel[nsel++] = sel[i]; };

    table_hdrput(stdout, head, TBLHDR_HEADLINE, NULL, sel, nsel-1);
    for ( i = 0; i < ycol-1; i++ ) {
	Print("%s_Fit	%s_Res\t", yname[i], yname[i]);
    }
    Print("%s_Fit	%s_Res\n", yname[i], yname[i]);

    table_hdrput(stdout, head, TBLHDR_DASHLINE, NULL, sel, nsel-1);
    for ( i = 0; i < ycol-1; i++ ) {
	dashes(4, yname[i]);	Print("\t");
	dashes(4, yname[i]);	Print("\t");
    }
    dashes(4, yname[i]);	Print("\t");
    dashes(4, yname[i]);	Print("\n");

    for ( j = 0; j < nrow; j++ ) {
        table_rowput(stdout, head, rows[j], NULL, sel, nsel-1);
        for ( i = 0; i < ycol-1; i++ ) {
	    Print(format, X[i][j]);
	    Print("	");
	    Print(format, X[i][j]-Y[i][j]);
	    Print("\t");
	}
	Print(format, X[i][j]);
	Print("	");
	Print(format, X[i][j]-Y[i][j]);
	Print("\n");
    }
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

char *stdmodel(stdmodel_str, term, terms, model, order, cross)
	char	*stdmodel_str;
	char	*term[];
	int	*terms;
	char	*model;
	int	 order;
	int	 cross;
{
	char	mod[128];
	char	stdterm[128];
	int	 nterms;
	int	 t;

	int	 i;

	strcpy(mod, model);

        for ( 	nterms = 0, term[nterms++] = strtok(mod, " ,");
	      	term[nterms] = strtok(NULL, " ,");
		nterms++ );
	
	*terms = nterms;

	strcpy(stdmodel_str, "1");
	for ( t = 0; t < nterms; t++ ) {
	    for ( i = 1; i <= order; i++ ) {
		strcat(stdmodel_str, ", ");

		if ( i > 1 ) 	sprintf(stdterm, "%s^%d", term[t], i);
		else		sprintf(stdterm, "%s", term[t]);

		strcat(stdmodel_str, stdterm);
	    }
	}

	return stdmodel_str;
}
