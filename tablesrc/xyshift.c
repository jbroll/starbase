
double A = 0.0;
double B = 1.0;
double C = 0.0;
double D = 0.0;
double E = 0.0;
double F = 1.0;


#include <xos.h>
#include <xfile.h>
#include <table.h>

#include <float.h>
#include <math.h>

#include "stargrid.h"
#include "stargrid.c"


double SAOstrtod();
#define atof(x)	SAOstrtod(x, NULL);



Stars read_file(char *fname, int tx)
{
    TableHead	th;
    TableRow   row = NULL;

    File fp;
    Stars s;

    int	xcol;
    int	ycol;
    int	zcol;

    if ( !(fp = Open(fname, "r")) ) {
      fprintf(stderr,"can't open %s.\n", fname);
      exit(1);
    }

    if ( !(th = table_header(fp, 0)) ) {
      fprintf(stderr,"no header in input table : %s\n", fname);
      exit(1);
    }


    if ( !(xcol = table_colnum(th, "x")) ) {
      fprintf(stderr,"no x column in input table : %s\n", fname);
      exit(1);
    }
    if ( !(ycol = table_colnum(th, "y")) ) {
      fprintf(stderr,"no y column in input table : %s\n", fname);
      exit(1);
    }
    zcol = table_colnum(th, "z");

    Malloc(s, sizeof(struct _Stars));
    s->astar = 100;
    Malloc(s->star, sizeof(struct _Star) * s->astar);
    s->nstar = 0;

    s->xmin =  DBL_MAX;
    s->xmax = -DBL_MAX;
    s->ymin =  DBL_MAX;
    s->ymax = -DBL_MAX;

    while ( row = table_rowget(fp, th, row, NULL, NULL, 0) ) {
	if ( s->nstar >= s->astar ) {
	    s->astar = s->astar*1.5 + 100;
	    ReAlloc(s->star, sizeof(struct _Star) * s->astar);
	}
	s->star[s->nstar].x0 = table_colvald(row, xcol);
	s->star[s->nstar].y0 = table_colvald(row, ycol);
	s->star[s->nstar].z  = zcol ? table_colvald(row, zcol) : 1;

	s->star[s->nstar].row  = s->nstar+1;

	if ( tx ) {
	    s->star[s->nstar].x = A + B*s->star[s->nstar].x0 + C*s->star[s->nstar].y0;
	    s->star[s->nstar].y = D + E*s->star[s->nstar].x0 + F*s->star[s->nstar].y0;
	} else {
	    s->star[s->nstar].x = s->star[s->nstar].x0;
	    s->star[s->nstar].y = s->star[s->nstar].y0;
	}

	if ( s->star[s->nstar].x < s->xmin ) { s->xmin = s->star[s->nstar].x; }
	if ( s->star[s->nstar].x > s->xmax ) { s->xmax = s->star[s->nstar].x; }
	if ( s->star[s->nstar].y < s->ymin ) { s->ymin = s->star[s->nstar].y; }
	if ( s->star[s->nstar].y > s->ymax ) { s->ymax = s->star[s->nstar].y; }

	s->nstar++;
    }

    if ( s->nstar < 3 ) {
      fprintf(stderr,"only %d row in table : %s\n", s->nstar, fname);
      exit(1);
    }


    return s;
}

 
int match(StarGrid sg, Stars s1, Stars s2
	, double dx, double dy
	, double MatchTol, double *rms, double *xc)
{
  int 	 i;
  int	 count = 0;
  double sum   = 0;

  for (i = 0; i < s1->nstar; i++) {
      s1->star[i].match = -1;
      s1->star[i].dist  = DBL_MAX;
  }

  *xc = 0;

  for (i = 0; i < s2->nstar; i++) {
      double dist;
      Star s;

      if ( s = stargrid_search(sg,  s2->star[i].x + dx,
				    s2->star[i].y + dy, MatchTol, &dist) ) {
	  if ( dist < s->dist ) {
	      s->dist  = dist;
	      s->match = i;
	      sum +=  (s->x - s2->star[i].x) * (s->x - s2->star[i].x)
		    + (s->y - s2->star[i].y) * (s->y - s2->star[i].y);

	      *xc += s->z * s2->star[i].z;
	      count++;
	  }
      }
  }

  *rms = sqrt(sum);

  return count;
}


void print_match(Stars s1, Stars s2, int rows)
{
    int i;

    if ( rows ) {
	fprintf(stdout, "x1	y1	x2	y2	row1	row2\n");
	fprintf(stdout, "--	--	--	--	----	----\n");
    } else {
	fprintf(stdout, "x1	y1	x2	y2\n");
	fprintf(stdout, "--	--	--	--\n");
    }

    for (i = 0; i < s1->nstar; i++) {
        if ( s1->star[i].match >= 0 ) {
	    fprintf(stdout, "%.8f	%.8f	%.8f	%.8f"
		    , s1->star[i].x0
		    , s1->star[i].y0
		    , s2->star[s1->star[i].match].x0
		    , s2->star[s1->star[i].match].y0);

	    if ( rows ) {
		fprintf(stdout, "	%d	%d", s1->star[i].row, s2->star[s1->star[i].match].row);
	    }

	    fprintf(stdout, "\n");
	}
    }
    return;
}


int main(int argc, char **argv)
{
  int ncols = 0;
  int cols[50];
  double chisqr;  
  int	 TX = 0;

  char *st1 = NULL;
  char *st2 = NULL;

  int	i;

  int rows = 0;

  double MatchTol = 10;
  double MatchOk  =  0.0;
  double MatchMin =  0.5;
  double MaxDist  =  0.0;

  Stars s1;
  Stars s2;

  int	once = 0;
  int	most = 0;

  StarGrid sg;

  for ( i = 1; i < argc; i++ ) {
   if ( argv[i][0] == '-' && argv[i][1] != '\0' ) {
    if ( !strcmp(argv[i], "-rows") ) { rows = 1; continue; }
    if ( !strcmp(argv[i], "-most") ) { most = 1; continue; }

    if ( !strcmp(argv[i], "-matchok")  ) { MatchOk	= atof(argv[++i]); continue; }
    if ( !strcmp(argv[i], "-maxdist")  ) { MaxDist	= atof(argv[++i]); continue; }
    if ( !strcmp(argv[i], "-matchtol") ) { MatchTol	= atof(argv[++i]); continue; }
    if ( !strcmp(argv[i], "-matchmin") ) { MatchMin	= atof(argv[++i]); continue; }
    if ( !strcmp(argv[i], "-once")     ) { once++; 			   continue; }
    if ( !strcmp(argv[i], "-Coeff")    ) { 
	TX = 1;
	A = atof(argv[++i]);
	B = atof(argv[++i]);
	C = atof(argv[++i]);
	D = atof(argv[++i]);
	E = atof(argv[++i]);
	F = atof(argv[++i]);
	continue;
    }

    fprintf(stderr,"usage: xyshift  list1  list2\n"
	 "		-matchmin <n> \n"
	 "		-matchok  <n> \n"
	 "		-matchtol <n> \n"
	 );
    exit(-1);
	
   } else {
       if ( st1 == NULL ) { st1 = argv[i]; continue; }
       if ( st2 == NULL ) { st2 = argv[i]; continue; }
   }
  }

  if ( !st2 ) {
      fprintf(stderr,"usage: xyshift  list1  list2\n"
	 "		-matchmin <n> \n"
	 "		-matchok  <n> \n"
	 "		-matchtol <n> \n"
	 );
      exit(-1);
  }

  s1 = read_file(st1,  0);
  s2 = read_file(st2, TX);

  MaxDist = MaxDist*MaxDist;

  {
    double A1,A2,A;
    int N1,N2,M1,M2;

	if ( !(A1 = (s1->xmax-s1->xmin)*(s1->ymax-s1->ymin)) ) {
	    A1 = 1;
	}
	if ( !(A2 = (s2->xmax-s2->xmin)*(s2->ymax-s2->ymin)) ) {
	    A2 = 1;
	}
	A  = MIN(A1,A2);
	N1 = s1->nstar; 
	N2 = s2->nstar;

    if ( MatchMin < 1.0) {
	M1 = (int) (MatchMin*(double)N1*A1/A);
	M2 = (int) (MatchMin*(double)N2*A2/A);

	MatchMin = MIN(M1,M2);
    }

    if ( MatchOk  < 1.0 && MatchOk > 0.0 ) {
	M1 = (int) (MatchOk*(double)N1*A1/A);
	M2 = (int) (MatchOk*(double)N2*A2/A);

	MatchOk = MIN(M1,M2);
    }
    if ( MatchOk == -1 ) {
	MatchOk = s1->nstar + 1;
    }
  }
  if ( MatchOk == 0 ) { MatchOk = MatchMin; }

  MatchOk = MAX(MatchMin, MatchOk);

    if ( !(sg = stargrid(s1, 30, 30)) ) {
        fprintf(stderr,"fatal error building star grid!!\n");
        exit(1);
    }


  {
    int i, j;

    int matched;

    int best = 0;
    double dx = 0.0;
    double dy = 0.0;

    double best_dx;
    double best_dy;

    double      rms;
    double best_rms = DBL_MAX;
    double	xc;
    double best_xc  = -1.0;

      if ( once ) {
	    matched = match(sg, s1, s2, dx, dy, MatchTol, &rms, &xc);

	    best     = matched;
	    best_xc  = xc;
	    best_dx  = dx;
	    best_dy  = dy;
	    best_rms = rms;
      } else {
	  for ( i = 0; i < s1->nstar && best < MatchOk; i++ ) {
	      for ( j = 0; j < s2->nstar && best < MatchOk; j++ ) {
		  dx = s1->star[i].x - s2->star[j].x;
		  dy = s1->star[i].y - s2->star[j].y;

		  if ( MaxDist != 0.0 && dx*dx + dy*dy > MaxDist ) { continue; }
		  
		  matched = match(sg, s1, s2, dx, dy, MatchTol, &rms, &xc);

		  if ( matched > MatchMin && ( most ? ( matched > best ) : ( xc > best_xc || ( xc == best_xc && best_rms > rms ))) ) {
		      best     = matched;
		      best_xc  = xc;
		      best_dx  = dx;
		      best_dy  = dy;
		      best_rms = rms;
		  }
	      }
	  }
  }

    if ( best < MatchMin ) {
	fprintf(stderr, "ERROR  didn't find match, best = %d\n", best);
	exit(-1);
    }

    matched = match(sg, s1, s2, best_dx, best_dy, MatchTol, &rms, &xc);
  }

  print_match(s1, s2, rows);
  
  return 0;
}
