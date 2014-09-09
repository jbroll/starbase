/* xos.h
**/

#ifndef mathmac_h
#define os_h

#define Min(x, y)	(((x) < (y)) ? (x) : (y))
#define Max(x, y)	(((x) > (y)) ? (x) : (y))

#define Clip(min, max, val)	 (((val) < (min) ) ? (min) :		\
				  ((val) > (max) ) ? (max) : (val) )
#define Abs(xx)		(((xx) > 0) ? (xx) : -(xx) )

#define MIN_normal	0.99e-306
#define MAX_normal	0.99e306


/* Types.h
**
** Data type constants
**/

#ifndef INDEFS
#define INDEFS          (-32767)
#endif
#ifndef INDEFL
#define INDEFL          ((unsigned long) 0x80000001)
#endif
#ifndef INDEFI
#define INDEFI          INDEFL
#endif
#ifndef INDEFR
#define INDEFR          1.6e38
#endif
#ifndef INDEFD
#define INDEFD          1.6e308
#endif
#ifndef INDEF
#define INDEF           INDEFR
#endif


#define	TY_CHAR		0
#define	TY_SHORT	1
#define	TY_INT		2
#define	TY_LONG		3
#define	TY_REAL		4
#define	TY_DOUBLE	5

#define TY_UCHAR	6
#define TY_USHORT	7
#define TY_UINT		8
#define TY_ULONG	9


#define	SZ_CHAR		1
#define	SZ_SHORT	2
#define	SZ_INT		4
#define	SZ_LONG		4
#define	SZ_REAL		4
#define	SZ_DOUBLE	8

double SAOstrtod();
void   SAOconvert();
int	atoi();

#define X__PI	3.14159265358979323846
#define X_2PI	( 2 * X__PI )
#define X_R2D	(X_2PI / 360.0)
#define X_R2H	(X_2PI /  24.0)
#define X_H2D	(360.0 /  24.0)

#define r2h(r)	( (r) / X_R2H )
#define h2r(d)	( (d) * X_R2H )
#define r2d(r)	( (r) / X_R2D )
#define d2r(d)	( (d) * X_R2D )
#define h2d(r)	( (r) * X_H2D )
#define d2h(d)	( (d) / X_H2D )


#endif
/* end mathmac_h */
