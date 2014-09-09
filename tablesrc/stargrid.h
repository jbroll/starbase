
typedef struct _Star {
  double x;
  double y;
  double z;

  double x0;
  double y0;

  struct _Star *next; /* pointer to next star in chain */
  int	match;
  double dist;

  int row;
} *Star;


typedef struct _Stars {
  int astar;
  int nstar;
  Star star;

  double xmin,xmax,ymin,ymax;
} *Stars;


typedef struct _StarGrid {
  double x0, y0, dx, dy;  /* center of box (nx,ny) is at (x0+dx*nx,y0+dy*ny)*/
  int nx,ny;              /* indeces go from 0,0 to nx-1,ny-1 */
  Star **sgrid;    	  /* points to vector of pointers; each element
			     points to a vector of pointers to a 
			     star_link (sgrid[i][j] is a pointer to 
			     a star struct) */
} *StarGrid;


#define NINT(x) ((x)>=0 ? (int)((x)+0.5) : -(int)(-(x)+0.5) ) 

/* macros to convert indeces to positions */
#define X_INDEX_TO_X(sg,x_index) (sg->x0 + sg->dx*(x_index))
#define Y_INDEX_TO_Y(sg,y_index) (sg->y0 + sg->dy*(y_index))

/* macros to convert positions to indeces */
#define X_TO_X_INDEX(sg,x) (NINT((x - sg->x0)/sg->dx))
#define Y_TO_Y_INDEX(sg,y) (NINT((y - sg->y0)/sg->dy))  
  
#define MIN(a,b) (((a)<(b)) ? (a) : (b))
#define MAX(a,b) (((a)>(b)) ? (a) : (b))

#define SQUARE(x)  ((x)*(x))
#define DIST_SQR(x1,y1,x2,y2) (SQUARE(x1-x2) + SQUARE(y1-y2))
#define DIST(x1,y1,x2,y2) sqrt(DIST_SQUARED(x1,y1,x2,y2))

/****************************************************************/

int stargrid_insert(StarGrid sg, Star s);

StarGrid stargrid(Stars s, int xbins, int ybins);

Star stargrid_search(StarGrid sg, double x, double y, double tol, double *dist);

