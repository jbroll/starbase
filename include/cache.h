/* cache.h
 */

#include <xlock.h>

typedef struct _CNode {
	struct	_CNode *next;
	int		hash;
	struct	_CNode *list;
	struct	_CNode *prev;
	int		refs;
} *CNode;

typedef struct _Cache {
	CNode  *cache;
	int	size;
	int	width;
	int	n;
	int	max;
	void  (*freenode)();
	void   *data;
	CNode	list;
	CNode	last;
	int   (*hashcomp)();
	int	try;
	int	hit;

	lock_decl(lock)
} *Cache;


Cache cache_init(int size, int width, int max, void (*freenode)(), void *data, int (*hashcomp)());
CNode cache_hashe(Cache c, CNode hash, CNode *prev);
CNode cache_cache(Cache c, CNode hash);
void cache_nfree(Cache	c, CNode h);

