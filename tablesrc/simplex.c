/* simplex.c
 */

#define HAVE_MEMCPY

#include <xos.h>
#include <xfile.h>

extern FILE *logfile;

#include "simplex.h"

double fabs();

/* Coefficients used to modify the simplex
 */
static double	a = 1.0;	/* Reflection	*/
static double	b = 0.5;	/* Contraction	*/
static double	g = 2.0;	/* Expansion	*/

void simplex_coeffs(alpha, beta, gamma)
     double	alpha;
     double	beta;
     double	gamma;
{
  a = alpha;  b = beta;  g = gamma;
}


double  simplex_step();

/* Find and return the highest, Second highest (q) and Lowest indexes
 */
void simplex_index(n, y, h, q, l, yh, yq, yl)
     int	 n;
     double	*y;
     int	*h;
     int	*q;
     int	*l;
     double	*yh;
     double	*yq;
     double	*yl;
{
  int		i;

  /* Find the highest and lowest indexes
   */
  *l = 0;
  *q = 0;
  *h = 1;

  *yl = *yq = y[*l];
        *yh = y[*h];

  if ( *yl > *yh ) {
    *h  =  0;  *q  =  1;  *l  =  1;
    *yq = *yh; *yh = *yl; *yl = *yq;
  }

  for ( i = 2; i < n; i++ ) {
    double yt;

    yt = y[i];

    if ( yt <= *yl ) {
      *l = i;  *yl = yt;
    }

    if ( yt > *yh ) {
      *q = *h;  *yq = *yh;
      *h =  i;  *yh =  yt;
    } else
      if ( yt > *yq ) {
	*q = i;  *yq = yt;
      }
  }
}


void simplex_display(display, s, iterations, str, yh, yq, yl, utol)
     int	*display;
     Simplex	*s;
     int	iterations;
     char	*str;
     double	 yh;
     double	 yq;
     double	 yl;
     double	utol;

{
  static int init = 0;
  int	i;

if ( *display == 0 ) return;	/* Nothing	*/

if ( *display >= 3 || *display == 1 ) {		/* FULL Display */
 FILE *out = stderr;

 if ( *str != 'K' ) {
  if ( !init ) {
    Termcap();
    TC_clear(); 
    init = 1;
  }

  TC_move(0,0); 
 }

  if ( str )	TC_print("+%-20s %5d\n\n", str, iterations);
  else		TC_print("\n\n");

  if ( *display == 1 ) return;

  TC_print("High %2d % 17.15f\n", s->h, s->y[s->h]);    
  TC_print("Next %2d % 17.15f\n", s->q, s->y[s->q]); 
  TC_print("Low  %2d % 17.15f\n", s->l, s->y[s->l]);
  TC_print("Best    % 17.15f\n", s->best[0]);
  TC_print("RTol    % 17.15f\n", 2 * (fabs(s->y[s->h] - s->y[s->l])));
  TC_print("UTol    % 17.15f\n\n", utol);

  for ( i = 1; i < s->npoints; i++ ) {
	TC_print("% 9.5f\t", s->best[i]);
  }
  TC_print("% 9.5f\n", s->best[i]);

  TC_flush();

#if 0
 if ( *display >= 4 ) {
  dad(s->y, s->npoints, 1); 
  dapd(s->simplex, s->npoints + 1, s->npoints);
 }
#endif
}
}


/* N.B. !!
 *
 * There is a tricky interaction between simplex_search
 * code and simplex_step code .  Be Aware!
 *
 *	The simplex high vector and high index are
 *	passed free and loose.  It is imporatant to 
 *	watch them carefully.
 */

double *simplex_search(s, tolerence, iterations, extras
			, funct, fdata
			, monit, mdata)     
     Simplex	 *s;
     double	  tolerence;
     int	 *iterations;
     int	  extras;
     double	(*funct)();
     void	 *fdata;
     void	(*monit)();
     void	 *mdata;
{
  double *ptry	= malloc((s->npoints - 1) * sizeof(double));
  double *psum	= malloc((s->npoints - 1) * sizeof(double));

  double	y0, yh, yq, yl;

  int		i, j;		/* iterators			*/

  int		iterasdone;	/* termination counters		*/
  int		extrasdone;

  char		*message;

  /* Compute the sum of the simplex points on each axis
   */
  for ( j = 0; j < s->npoints; j++ ) {
    psum[j] = 0;
    for ( i = 0; i < s->npoints + 1; i++ )
      psum[j] += s->simplex[i][j];
  }

  message = "Simplex Stepper";

  iterasdone = extrasdone = 0;
  for ( ;; ) {
    if (           iterasdone >= *iterations )	break;
    if ( extras && extrasdone >= extras )	break;

    simplex_index(   s->npoints + 1, s->y
		  , &s->h, &s->q, &s->l
		  , &yh, &yq, &yl);

    if ( monit )
      (*monit)(mdata, s, iterasdone, message, yh, yq, yl, tolerence);

    /* Reflection.
     * To begin this iteration reflect the highest point 
     * over the center of the simplex.
     */
    message = "Reflection";

    y0 = simplex_step(s, -a, psum, funct, fdata, ptry, &yh);

    /* Compare the try againt the low point
     */
    if ( y0 < yl ) {
      /* Expansion.
       * If the new value is better than the low value push
       * the point farther in that direction.
       */
      message = "Expansion";

      /* The high value of the simplex has been secretly set
       * to y0 (better than the previous low).  This call to
       * simplex_step will only accept the try if the expansion
       * gives back an even better value.
       */
      (void) simplex_step(s, g, psum, funct, fdata, ptry, &yh);
      if ( y0 != yh ) message = "Expansion Taken";

    } else
      /* Compare the try againts the next to highest point
       */
      if ( y0 >= yq ) {
	double save = yh;		/* Save the real high value, because
					 * the high value will be set if 
					 * this point is accepted
					 */
	/* Try half leg expansion 
	 */
	if ( simplex_step(s, b, psum, funct, fdata, ptry, &yh)
	    >= save ) {
	  double **simplex = s->simplex;
	  int	   npoints = s->npoints;
	  int	   i, j;
	  int	   h = s->h;
	  int	   l = s->l;

	  /* New point is still the worst.
	   * Full contraction.
	   * Shrink the whole simplex towards the best point and start over.
	   */
	  message = "Full Contract";

	  for ( i = 0; i < npoints + 1; i++ )
	    if ( i != l ) {
	      for ( j = 0; j < npoints; j++ ) {
		psum[j] -= simplex[i][j];
		simplex[i][j] = (simplex[i][j] + simplex[l][j]) * 0.5;
		psum[j] += simplex[i][j];
	      }

	      s->y[i] = funct(fdata, npoints, simplex[i]);

	      if ( s->y[i] < s->best[0] ) {
		s->best[0] = s->y[i];
		bcopy(s->simplex[i], &s->best[1],
	      s->npoints * sizeof(double)); }

	      if ( monit )
		(*monit)(mdata, s, iterasdone, message, yh, yq, yl, tolerence);
	    }
	} else 	message = "Contract 1";
      }


    /* Check tolerence -
     */
    if ( tolerence > 2 * (fabs(s->y[s->h] - s->y[s->l])) ) {
      extrasdone++;
    } else { 
      extrasdone = 0;
    }

    iterasdone++;
  }

  *iterations -= iterasdone;

  return s->best;
}

/* This routine should be considered part simplex_search
 */
double simplex_step(  s, factor, psum, funct, fdata, ptry, yh)
     Simplex	*s;
     double	 factor;
     double	*psum;
     double    (*funct)();
     void	*fdata;
     double	*ptry;
     double	*yh;
{
  int	 i;

  double fac1 = ( 1.0 - factor ) / s->npoints;
  double fac2 =  fac1 - factor;

  double y0, yan;

  for ( i = 0; i < s->npoints; i++ )
    ptry[i] = psum[i] * fac1 - s->simplex[s->h][i] * fac2;

  y0 = funct(fdata, s->npoints, ptry);

  /* If the point is best save it.
   */
  if ( y0 < s->best[0] ) {
    s->best[0] = y0;
    bcopy(ptry, &s->best[1], s->npoints * sizeof(double));
  }

  yan = y0;
  
  if ( yan < *yh ) {
    s->y[s->h] = y0;
    *yh = yan;

    for ( i = 0; i < s->npoints; i++ ) {
      psum[i] -= s->simplex[s->h][i];
      psum[i] += ptry[i];

      s->simplex[s->h][i] = ptry[i];
    }
  }

  return yan;
}



void simplex_free(s)
     Simplex	*s;
{
  ft_free2d(s->simplex, s->npoints + 1, 0, 0);
  Free(s->y);
  Free(s->best);
  free(s);
}




Simplex *simplex_create(  npoints, y0, start, sdelta, limit, iterations
			, funct, fdata, monit, mdata)
     int	npoints;		/* Number of points in start	*/
     double	y0;			/* FOM at the start vector	*/
     double    *start;			/* Initial point		*/
     double    *sdelta;			/* Initial delta		*/
     double    **limit;			/* Search limits		*/
     int        *iterations;		/* Max times to call funct	*/
     double    (*funct)();		/* FOM evaluation function	*/
     void       *fdata;			/* Data for funct		*/
     void      (*monit)();		/* monitor function		*/
     void       *mdata;			/* Data for monit		*/
{
  int	i, j;

  Simplex	 *s 		= (Simplex *) malloc(sizeof(Simplex));
  double	**simplex	= ft_alloc2d(sizeof(double), 0, 0, npoints, npoints + 1);
  double	 *y 		= malloc(npoints * sizeof(double));
  double	 *best		= malloc(npoints * sizeof(double));

  double	  yh, yq, yl;

  double	 sdelt;

  s->npoints	= npoints;
  s->simplex	= simplex;
  s->best	= best;
  s->y		= y;


  /* Create the simplex and compute initial values
   */
  for ( i = 0; i < npoints + 1; i++ ) {
    for ( j = 0; j < npoints; j++ ) {
      if ( !sdelta ) {
	sdelt = start[j] * 0.1;
      } else {
	sdelt = sdelta[j];
      }
      if ( i - 1 == j )	simplex[i][j] = start[j] + sdelt;
      else		simplex[i][j] = start[j];
    }

    y[i] = (*funct)(fdata, npoints, simplex[i]);
    if ( monit )
      (*monit)(mdata, s, *iterations, "Construct Simplex", 0.0, 0.0, 0.0, 0,0);
  }

  /* Initialize the best point
   */
  simplex_index(s->npoints + 1, s->y, &s->h, &s->q, &s->l, &yh, &yq, &yl);
  s->best[0] = s->y[s->l];
  bcopy(s->simplex[s->l], &s->best[1], s->npoints * sizeof(double));

  if ( monit )
    (*monit)(mdata, s, *iterations, "Construct Simplex", 0.0, 0.0, 0.0, 0,0);

  return s;
}
