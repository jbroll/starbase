/* routines to handle a grid of stars */


char **ft_make2d(ptr, size, xstar, ystar, xsize, ysize)
    char    *ptr;
    int      size;
    int      xstar;
    int      ystar;
    int      xsize;
    int      ysize;
{
    char **ptr2d;

    if ( ptr == NULL )          return NULL;

    Malloc(ptr2d, ysize * sizeof(void *));

    if ( ptr2d == NULL )        return NULL;

    for (; ysize--;) {
	ptr2d[ysize] = ptr + xsize * ysize * size - xstar * size;
    }
    return ptr2d - ystar;
}

void *ft_alloc2d(size, xstar, ystar, xsize, ysize)
    int      size;
    int      xstar;
    int      ystar;
    int      xsize;
    int      ysize;
{
    void *ptr;

    Malloc(ptr, xsize * ysize * size);
    return ft_make2d(ptr, size, xstar, ystar, xsize, ysize);
}


/* insert a star contained in a star_link into a stargrid, returning
   0 if successful and 1 if out of bounds */
int stargrid_insert(StarGrid sg, Star s)
{
    int xindex = X_TO_X_INDEX(sg, s->x);
    int yindex = Y_TO_Y_INDEX(sg, s->y);

    if ( xindex >= sg->nx || yindex >= sg->ny || xindex<0 || yindex<0 ) {
        return 1;
    }

    s->next = sg->sgrid[xindex][yindex];
              sg->sgrid[xindex][yindex] = s;

    return 0;
}


/* given an array of stars extending with maximum index n-1, put them
   all into a star_grid, and return the star_grid; the range of x and
   y is taken from the stars themselves */
StarGrid stargrid(Stars s, int xbins, int ybins)
{
    double xmin,xmax,ymin,ymax;
    int i,j;

    StarGrid sg;

    Malloc(sg, sizeof(struct _StarGrid));

    sg->sgrid = ft_alloc2d(sizeof(Star), 0, 0, xbins, ybins);

    for (i = 0; i < xbins; i++)
  	for (j = 0; j < ybins; j++)
	    sg->sgrid[i][j] = NULL;

    /* Expand the boundaries by 0.1% just to make sure everything falls
       into the grid, even with roundoff
     */
    xmin = s->xmin - (s->xmax - s->xmin)*0.001;
    xmax = s->xmax + (s->xmax - s->xmin)*0.001;
    ymin = s->ymin - (s->ymax - s->ymin)*0.001;
    ymax = s->ymax + (s->ymax - s->ymin)*0.001;

    if ( !xmin && ! xmax ) {
	xmin = -0.001;
	xmax =  0.001;
    }
    if ( !ymin && ! ymax ) {
	ymin = -0.001;
	ymax =  0.001;
    }



    sg->nx = xbins;
    sg->ny = ybins;
    sg->dx = (xmax-xmin)/(double)xbins;
    sg->dy = (ymax-ymin)/(double)ybins;
    sg->x0 = xmin+sg->dx/2.0;		/* because x0 is the CENTER of the 0th box */
    sg->y0 = ymin+sg->dy/2.0;

    for ( i = 0; i < s->nstar; i++) {
        if ( stargrid_insert(sg, &s->star[i]) ) {
	    return NULL;
	}
    }

    return sg;
}



/* find the closest star to x and y in a grid, returning the distance
   to that star.  If no star is closer than max_dist, then 
   return 0, else return 1; a pointer to the best match star is returned
   in **spp, and the closest distance is returned in *dist_ptr */

Star stargrid_search(StarGrid sg, double x, double y, double tol, double *dist)
{
    int i, j;

    int min_x_bin =  X_TO_X_INDEX(sg,x-tol);
    int max_x_bin =  X_TO_X_INDEX(sg,x+tol);
    int min_y_bin =  Y_TO_Y_INDEX(sg,y-tol);
    int max_y_bin =  Y_TO_Y_INDEX(sg,y+tol);

    Star best = NULL;
    double r = tol*tol;
  
    if ( max_x_bin < 0 || min_x_bin >= sg->nx || 	/* return if we are off the map entirely */
         max_y_bin < 0 || min_y_bin >= sg->ny ) {
        return NULL;
    }

    min_x_bin = Max(0, min_x_bin);
    max_x_bin =    Min(max_x_bin, sg->nx-1);
    min_y_bin = Max(0, min_y_bin);
    max_y_bin =    Min(max_y_bin, sg->ny-1);

    for ( i = min_x_bin; i <= max_x_bin; i++ ) {
        for ( j = min_y_bin; j <= max_y_bin; j++ ) {
	    Star s;
	    for ( s = sg->sgrid[i][j];  s; s = s->next ) {
		double rr = (s->x - x)*(s->x - x) + (s->y - y)*(s->y - y) ;

		if ( rr <= r ) {
		    r = rr; 
		    best = s;
		}
	    }
	}
    }
  
    *dist = sqrt(r);
    return best;
}

