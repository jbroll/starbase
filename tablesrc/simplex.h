/* simplex.h
**
**/

struct _Simplex {
  int	       npoints;

  double       **simplex;
  double        *best;
  double        *y;
  int	       h, q, l;		/* high and low indexes	*/
};

typedef struct _Simplex Simplex;

Simplex *simplex_create();

