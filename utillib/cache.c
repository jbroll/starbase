/* cache.c
 */
/* copyright 1991, 1993, 1995, 1999 John B. Roll jr.
 */


#include "xos.h"
#include "xfile.h"
#include "cache.h"

Cache	cache_init(
	int	 size,
	int	 width,
	int	 max,
	void   (*freenode)(),
	void	*data,
	int    (*hashcomp)())
{
	Cache	c;

	Calloc(c, sizeof(struct _Cache));
	Calloc(c->cache, width * sizeof(CNode));

	c->size		= size;
	c->width	= width;
	c->max 		= max;
	c->freenode 	= freenode;
	c->data		= data;
	c->n		= 0;
	c->hashcomp	= hashcomp;

	c->try		= 0;
	c->hit		= 0;

	lock_init(c->lock);

	return c;
}


CNode cache_hashe( Cache c, CNode hash, CNode *prev)
{
		unsigned x = hash->hash % c->width;
		CNode h;
		CNode p;

	lock_lock(c->lock);

	c->try++;

	if ( prev == NULL ) prev = &p;
	*prev = NULL;

	for ( h = c->cache[x]; h != NULL; *prev = h, h = h->next ) {
		/* FPrint(Stderr, "Check: %6d\n", hash); 	*/
		/* FPrint(Stderr, "     : %6d\n", h->hash); 	*/

	    if ( (*c->hashcomp)(h, hash, c->data) ) {
		c->hit++;
		h->refs++;

		if ( h != c->list ) {			/* There are at least 2 nodes	*/
		    h->prev->list = h->list;		/* Take h off the list		*/

		    if ( h == c->last ) {		/* If h is last move last	*/
			c->last = h->prev;
		    }
		    h->list = c->list; 			/* Link h in front		*/
		    c->list->prev = h;
		    h->prev = NULL;
		    c->list = h;
		}
		
		break;
	    }
	}

	lock_unlock(c->lock);
	return h;
}

CNode cache_cache(Cache c, CNode hash)
{
		CNode	 h = NULL;
		CNode	 p;
		unsigned x = hash->hash % c->width;
		int	i;

	if ( c->max && c->n >= c->max ) {
		unsigned y = c->last->hash % c->width;


	    if ( (h = cache_hashe(c, c->last, &p)) == NULL ) {
		fprintf(stderr, "Cannot hashe last!!\n");
		abort();
	    }

	    c->hit--;	/* This was an internal lookup */
	    c->try--;
	    h->refs--;

	    if ( p != NULL ) p->next 	   = h->next;	/* Unlink the hash list */
	    else	     c->cache[y]   = h->next;

	    c->list = c->list->list;
	    h->prev = NULL;
	    h->list = NULL;

	    if ( !--h->refs ) {
		if ( c->freenode != NULL )
		    (*c->freenode)(h, c->data);
	    } else {
		h = NULL;
	    }
	}

	if ( h == NULL ) {
	    Calloc(h, c->size);
	    c->n++;
	}

	h->hash = hash->hash;

	h->next = c->cache[x];				/* Link the hash list	*/
	c->cache[x] = h;

	h->list 	  = c->list;			/* Link the LRU list	*/

	if ( c->list )  c->list->prev = h;
	else		c->last       = h;

	c->list	  = h;
	h->refs	  = 2;

	lock_unlock(c->lock);
	return h;
}

void cache_iter(Cache c, void (*callback)(), void *data)
{
    	CNode	n = c->list;

    for ( ; n; n = n->next ) {
	(*callback)(n, data);
    }
}

void cache_nfree(Cache c, CNode h)
{
	lock_lock(c->lock);
	
	if ( !--h->refs ) {
	    if ( c->freenode != NULL )
		(*c->freenode)(h, c->data);
	}

	lock_unlock(c->lock);
}
