/* xawk.c
 */

#include "mawk.h"
#include "symtype.h"
#include "parse.h"
#include "init.h"
#include "memory.h"

struct anode ;
typedef struct {struct anode *slink, *ilink ;} DUAL_LINK ;

typedef struct anode {
   struct anode *slink ;
   struct anode  *ilink ;
   STRING *sval ;
   unsigned hval ;
   Int     ival ;
   CELL    cell ;
} ANODE ;


ANODE *find_by_ival();
ANODE *find_by_sval();

void xawk_register(fbp)
	FBLOCK	*fbp;
{
  	register SYMTAB *q;
	q = insert(fbp->name);
	q->type = ST_FUNCT;
	q->stval.fbp = fbp;
}

void *xawk_arg(sp, nargs, n)
	void *sp;
	int	nargs;
	int	n;
{
	CELL *cp = sp;
	return ( (cp - nargs) + n );
}

double xawk_num(cp)
	void	*cp;
{
	CELL *T = cp;
	cast1_to_d(T);
	return T->dval;
}

char *xawk_str(cp)
	void *cp;
{
	CELL *T = cp;
	cast1_to_s(T);
	return string(T)->str;
}

double xawk_numarg(sp, nargs, n)
        void *sp;               
        int     nargs;          
        int     n;              
{
	return xawk_num(xawk_arg(sp, nargs, n));
}

char *xawk_strarg(sp, nargs, n)
        void *sp;               
        int     nargs;          
        int     n;              
{
	return xawk_str(xawk_arg(sp, nargs, n));
}


void *xawk_stridx(ap, idx)
	void	*ap;
	char	*idx;
{
	CELL *xp = ap;

	STRING *s = new_STRING(idx);
	ANODE  *a;

	a = find_by_sval(xp->ptr, s, 1);
	free_STRING(s);

	return &a->cell;
}

void *xawk_numidx(ap, idx)
	void	*ap;
	int	idx;
{
	CELL *xp = ap;
	ANODE *a = find_by_ival(xp->ptr, idx, 1);
	return &a->cell;
}

void xawk_setnum(cell, d)
	void	*cell;
	double	d;
{
	CELL *c = cell;

	cell_destroy(c);
	c->type = C_DOUBLE;
	c->dval = d;
}

void xawk_setstr(cell, str)
	void	*cell;
	char	*str;
{
	CELL *c = cell;

	cell_destroy(c);
	c->type = C_STRING;
	c->ptr = new_STRING(str);
}

void *xawk_pshnum(sp, d, format)
	void	*sp;
	double	d;
	int	format;
{
	CELL	*xp = sp;

	xp++;
	xp->type = C_DOUBLE;
	xp->vcnt = format;
	xp->dval = d;

	return xp;
}

void *xawk_pshstr(sp, str)
	void	 *sp;
	char	*str;
{
	CELL	*xp = sp;

	xp++;
	xp->type = C_STRING;
	xp->ptr = new_STRING(str);

	return xp;
}
