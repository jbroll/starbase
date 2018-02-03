/* slafit.f -- translated by f2c (version 20100827).
   You must link the resulting object file with libf2c:
	on Microsoft Windows system, link with libf2c.lib;
	on Linux or Unix systems, link with .../path/to/libf2c.a -lm
	or, if you install libf2c.a in a standard place, with -lf2c -lm
	-- in that order, at the end of the command line, as in
		cc *.o -lf2c -lm
	Source for libf2c is in /netlib/f2c/libf2c.zip, e.g.,

		http://www.netlib.org/f2c/libf2c.zip
*/

#include "f2c.h"

/* Table of constant values */

static integer c__3 = 3;
static integer c__4 = 4;

/* Subroutine */ int sla_svd__(integer *m, integer *n, integer *mp, integer *
	np, doublereal *a, doublereal *w, doublereal *v, doublereal *work, 
	integer *jstat)
{
    /* System generated locals */
    integer a_dim1, a_offset, v_dim1, v_offset, i__1, i__2, i__3;
    doublereal d__1, d__2, d__3, d__4;

    /* Builtin functions */
    double sqrt(doublereal), d_sign(doublereal *, doublereal *);

    /* Local variables */
    static doublereal c__, f, g, h__;
    static integer i__, j, k, l;
    static doublereal s, x, y, z__;
    static integer i1, k1, l1;
    static doublereal an;
    static integer its;
    static doublereal scale;
    static logical cancel;

/* + */
/*     - - - - */
/*      S V D */
/*     - - - - */

/*  Singular value decomposition  (double precision) */

/*  This routine expresses a given matrix A as the product of */
/*  three matrices U, W, V: */

/*     A = U x W x VT */

/*  Where: */

/*     A   is any M (rows) x N (columns) matrix, where M.GE.N */
/*     U   is an M x N column-orthogonal matrix */
/*     W   is an N x N diagonal matrix with W(I,I).GE.0 */
/*     VT  is the transpose of an N x N orthogonal matrix */

/*     Note that M and N, above, are the LOGICAL dimensions of the */
/*     matrices and vectors concerned, which can be located in */
/*     arrays of larger PHYSICAL dimensions, given by MP and NP. */

/*  Given: */
/*     M,N    i         numbers of rows and columns in matrix A */
/*     MP,NP  i         physical dimensions of array containing matrix A */
/*     A      d(MP,NP)  array containing MxN matrix A */

/*  Returned: */
/*     A      d(MP,NP)  array containing MxN column-orthogonal matrix U */
/*     W      d(N)      NxN diagonal matrix W (diagonal elements only) */
/*     V      d(NP,NP)  array containing NxN orthogonal matrix V */
/*     WORK   d(N)      workspace */
/*     JSTAT  i         0 = OK, -1 = A wrong shape, >0 = index of W */
/*                      for which convergence failed.  See note 2, below. */

/*   Notes: */

/*   1)  V contains matrix V, not the transpose of matrix V. */

/*   2)  If the status JSTAT is greater than zero, this need not */
/*       necessarily be treated as a failure.  It means that, due to */
/*       chance properties of the matrix A, the QR transformation */
/*       phase of the routine did not fully converge in a predefined */
/*       number of iterations, something that very seldom occurs. */
/*       When this condition does arise, it is possible that the */
/*       elements of the diagonal matrix W have not been correctly */
/*       found.  However, in practice the results are likely to */
/*       be trustworthy.  Applications should report the condition */
/*       as a warning, but then proceed normally. */

/*  References: */
/*     The algorithm is an adaptation of the routine SVD in the EISPACK */
/*     library (Garbow et al 1977, EISPACK Guide Extension, Springer */
/*     Verlag), which is a FORTRAN 66 implementation of the Algol */
/*     routine SVD of Wilkinson & Reinsch 1971 (Handbook for Automatic */
/*     Computation, vol 2, ed Bauer et al, Springer Verlag).  These */
/*     references give full details of the algorithm used here.  A good */
/*     account of the use of SVD in least squares problems is given in */
/*     Numerical Recipes (Press et al 1986, Cambridge University Press), */
/*     which includes another variant of the EISPACK code. */

/*  Last revision:   8 September 2005 */

/*  Copyright P.T.Wallace.  All rights reserved. */

/*  License: */
/*    This program is free software; you can redistribute it and/or modify */
/*    it under the terms of the GNU General Public License as published by */
/*    the Free Software Foundation; either version 2 of the License, or */
/*    (at your option) any later version. */

/*    This program is distributed in the hope that it will be useful, */
/*    but WITHOUT ANY WARRANTY; without even the implied warranty of */
/*    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the */
/*    GNU General Public License for more details. */

/*    You should have received a copy of the GNU General Public License */
/*    along with this program (see SLA_CONDITIONS); if not, write to the */
/*    Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor, */
/*    Boston, MA  02110-1301  USA */

/* - */
/*  Maximum number of iterations in QR phase */
/*  Variable initializations to avoid compiler warnings. */
    /* Parameter adjustments */
    --work;
    --w;
    v_dim1 = *np;
    v_offset = 1 + v_dim1;
    v -= v_offset;
    a_dim1 = *mp;
    a_offset = 1 + a_dim1;
    a -= a_offset;

    /* Function Body */
    l = 0;
    l1 = 0;
/*  Check that the matrix is the right shape */
    if (*m < *n) {
/*     No:  error status */
	*jstat = -1;
    } else {
/*     Yes:  preset the status to OK */
	*jstat = 0;

/*     Householder reduction to bidiagonal form */
/*     ---------------------------------------- */
	g = 0.;
	scale = 0.;
	an = 0.;
	i__1 = *n;
	for (i__ = 1; i__ <= i__1; ++i__) {
	    l = i__ + 1;
	    work[i__] = scale * g;
	    g = 0.;
	    s = 0.;
	    scale = 0.;
	    if (i__ <= *m) {
		i__2 = *m;
		for (k = i__; k <= i__2; ++k) {
		    scale += (d__1 = a[k + i__ * a_dim1], abs(d__1));
		}
		if (scale != 0.) {
		    i__2 = *m;
		    for (k = i__; k <= i__2; ++k) {
			x = a[k + i__ * a_dim1] / scale;
			a[k + i__ * a_dim1] = x;
			s += x * x;
		    }
		    f = a[i__ + i__ * a_dim1];
		    d__1 = sqrt(s);
		    g = -d_sign(&d__1, &f);
		    h__ = f * g - s;
		    a[i__ + i__ * a_dim1] = f - g;
		    if (i__ != *n) {
			i__2 = *n;
			for (j = l; j <= i__2; ++j) {
			    s = 0.;
			    i__3 = *m;
			    for (k = i__; k <= i__3; ++k) {
				s += a[k + i__ * a_dim1] * a[k + j * a_dim1];
			    }
			    f = s / h__;
			    i__3 = *m;
			    for (k = i__; k <= i__3; ++k) {
				a[k + j * a_dim1] += f * a[k + i__ * a_dim1];
			    }
			}
		    }
		    i__2 = *m;
		    for (k = i__; k <= i__2; ++k) {
			a[k + i__ * a_dim1] = scale * a[k + i__ * a_dim1];
		    }
		}
	    }
	    w[i__] = scale * g;
	    g = 0.;
	    s = 0.;
	    scale = 0.;
	    if (i__ <= *m && i__ != *n) {
		i__2 = *n;
		for (k = l; k <= i__2; ++k) {
		    scale += (d__1 = a[i__ + k * a_dim1], abs(d__1));
		}
		if (scale != 0.) {
		    i__2 = *n;
		    for (k = l; k <= i__2; ++k) {
			x = a[i__ + k * a_dim1] / scale;
			a[i__ + k * a_dim1] = x;
			s += x * x;
		    }
		    f = a[i__ + l * a_dim1];
		    d__1 = sqrt(s);
		    g = -d_sign(&d__1, &f);
		    h__ = f * g - s;
		    a[i__ + l * a_dim1] = f - g;
		    i__2 = *n;
		    for (k = l; k <= i__2; ++k) {
			work[k] = a[i__ + k * a_dim1] / h__;
		    }
		    if (i__ != *m) {
			i__2 = *m;
			for (j = l; j <= i__2; ++j) {
			    s = 0.;
			    i__3 = *n;
			    for (k = l; k <= i__3; ++k) {
				s += a[j + k * a_dim1] * a[i__ + k * a_dim1];
			    }
			    i__3 = *n;
			    for (k = l; k <= i__3; ++k) {
				a[j + k * a_dim1] += s * work[k];
			    }
			}
		    }
		    i__2 = *n;
		    for (k = l; k <= i__2; ++k) {
			a[i__ + k * a_dim1] = scale * a[i__ + k * a_dim1];
		    }
		}
	    }
/*        Overestimate of largest column norm for convergence test */
/* Computing MAX */
	    d__3 = an, d__4 = (d__1 = w[i__], abs(d__1)) + (d__2 = work[i__], 
		    abs(d__2));
	    an = max(d__3,d__4);
	}

/*     Accumulation of right-hand transformations */
/*     ------------------------------------------ */
	for (i__ = *n; i__ >= 1; --i__) {
	    if (i__ != *n) {
		if (g != 0.) {
		    i__1 = *n;
		    for (j = l; j <= i__1; ++j) {
			v[j + i__ * v_dim1] = a[i__ + j * a_dim1] / a[i__ + l 
				* a_dim1] / g;
		    }
		    i__1 = *n;
		    for (j = l; j <= i__1; ++j) {
			s = 0.;
			i__2 = *n;
			for (k = l; k <= i__2; ++k) {
			    s += a[i__ + k * a_dim1] * v[k + j * v_dim1];
			}
			i__2 = *n;
			for (k = l; k <= i__2; ++k) {
			    v[k + j * v_dim1] += s * v[k + i__ * v_dim1];
			}
		    }
		}
		i__1 = *n;
		for (j = l; j <= i__1; ++j) {
		    v[i__ + j * v_dim1] = 0.;
		    v[j + i__ * v_dim1] = 0.;
		}
	    }
	    v[i__ + i__ * v_dim1] = 1.;
	    g = work[i__];
	    l = i__;
	}

/*     Accumulation of left-hand transformations */
/*     ----------------------------------------- */
	for (i__ = *n; i__ >= 1; --i__) {
	    l = i__ + 1;
	    g = w[i__];
	    if (i__ != *n) {
		i__1 = *n;
		for (j = l; j <= i__1; ++j) {
		    a[i__ + j * a_dim1] = 0.;
		}
	    }
	    if (g != 0.) {
		if (i__ != *n) {
		    i__1 = *n;
		    for (j = l; j <= i__1; ++j) {
			s = 0.;
			i__2 = *m;
			for (k = l; k <= i__2; ++k) {
			    s += a[k + i__ * a_dim1] * a[k + j * a_dim1];
			}
			f = s / a[i__ + i__ * a_dim1] / g;
			i__2 = *m;
			for (k = i__; k <= i__2; ++k) {
			    a[k + j * a_dim1] += f * a[k + i__ * a_dim1];
			}
		    }
		}
		i__1 = *m;
		for (j = i__; j <= i__1; ++j) {
		    a[j + i__ * a_dim1] /= g;
		}
	    } else {
		i__1 = *m;
		for (j = i__; j <= i__1; ++j) {
		    a[j + i__ * a_dim1] = 0.;
		}
	    }
	    a[i__ + i__ * a_dim1] += 1.;
	}

/*     Diagonalisation of the bidiagonal form */
/*     -------------------------------------- */
	for (k = *n; k >= 1; --k) {
	    k1 = k - 1;
/*        Iterate until converged */
	    its = 0;
	    while(its < 30) {
		++its;
/*           Test for splitting into submatrices */
		cancel = TRUE_;
		for (l = k; l >= 1; --l) {
		    l1 = l - 1;
		    if (an + (d__1 = work[l], abs(d__1)) == an) {
			cancel = FALSE_;
			goto L10;
		    }
/*              (Following never attempted for L=1 because WORK(1) is zero) */
		    if (an + (d__1 = w[l1], abs(d__1)) == an) {
			goto L10;
		    }
		}
L10:
/*           Cancellation of WORK(L) if L>1 */
		if (cancel) {
		    c__ = 0.;
		    s = 1.;
		    i__1 = k;
		    for (i__ = l; i__ <= i__1; ++i__) {
			f = s * work[i__];
			if (an + abs(f) == an) {
			    goto L20;
			}
			g = w[i__];
			h__ = sqrt(f * f + g * g);
			w[i__] = h__;
			c__ = g / h__;
			s = -f / h__;
			i__2 = *m;
			for (j = 1; j <= i__2; ++j) {
			    y = a[j + l1 * a_dim1];
			    z__ = a[j + i__ * a_dim1];
			    a[j + l1 * a_dim1] = y * c__ + z__ * s;
			    a[j + i__ * a_dim1] = -y * s + z__ * c__;
			}
		    }
L20:
		    ;
		}
/*           Converged? */
		z__ = w[k];
		if (l == k) {
/*              Yes:  stop iterating */
		    its = 30;
/*              Ensure singular values non-negative */
		    if (z__ < 0.) {
			w[k] = -z__;
			i__1 = *n;
			for (j = 1; j <= i__1; ++j) {
			    v[j + k * v_dim1] = -v[j + k * v_dim1];
			}
		    }
		} else {
/*              Not converged yet:  set status if iteration limit reached */
		    if (its == 30) {
			*jstat = k;
		    }
/*              Shift from bottom 2x2 minor */
		    x = w[l];
		    y = w[k1];
		    g = work[k1];
		    h__ = work[k];
		    f = ((y - z__) * (y + z__) + (g - h__) * (g + h__)) / (
			    h__ * 2. * y);
		    if (abs(f) <= 1e15) {
			g = sqrt(f * f + 1.);
		    } else {
			g = abs(f);
		    }
		    f = ((x - z__) * (x + z__) + h__ * (y / (f + d_sign(&g, &
			    f)) - h__)) / x;
/*              Next QR transformation */
		    c__ = 1.;
		    s = 1.;
		    i__1 = k1;
		    for (i1 = l; i1 <= i__1; ++i1) {
			i__ = i1 + 1;
			g = work[i__];
			y = w[i__];
			h__ = s * g;
			g = c__ * g;
			z__ = sqrt(f * f + h__ * h__);
			work[i1] = z__;
			if (z__ != 0.) {
			    c__ = f / z__;
			    s = h__ / z__;
			} else {
			    c__ = 1.;
			    s = 0.;
			}
			f = x * c__ + g * s;
			g = -x * s + g * c__;
			h__ = y * s;
			y *= c__;
			i__2 = *n;
			for (j = 1; j <= i__2; ++j) {
			    x = v[j + i1 * v_dim1];
			    z__ = v[j + i__ * v_dim1];
			    v[j + i1 * v_dim1] = x * c__ + z__ * s;
			    v[j + i__ * v_dim1] = -x * s + z__ * c__;
			}
			z__ = sqrt(f * f + h__ * h__);
			w[i1] = z__;
			if (z__ != 0.) {
			    c__ = f / z__;
			    s = h__ / z__;
			}
			f = c__ * g + s * y;
			x = -s * g + c__ * y;
			i__2 = *m;
			for (j = 1; j <= i__2; ++j) {
			    y = a[j + i1 * a_dim1];
			    z__ = a[j + i__ * a_dim1];
			    a[j + i1 * a_dim1] = y * c__ + z__ * s;
			    a[j + i__ * a_dim1] = -y * s + z__ * c__;
			}
		    }
		    work[l] = 0.;
		    work[k] = f;
		    w[k] = x;
		}
	    }
	}
    }
    return 0;
} /* sla_svd__ */

/* Subroutine */ int sla_svdcov__(integer *n, integer *np, integer *nc, 
	doublereal *w, doublereal *v, doublereal *work, doublereal *cvm)
{
    /* System generated locals */
    integer v_dim1, v_offset, cvm_dim1, cvm_offset, i__1, i__2, i__3;

    /* Local variables */
    static integer i__, j, k;
    static doublereal s;

/* + */
/*     - - - - - - - */
/*      S V D C O V */
/*     - - - - - - - */

/*  From the W and V matrices from the SVD factorisation of a matrix */
/*  (as obtained from the sla_SVD routine), obtain the covariance matrix. */

/*  (double precision) */

/*  Given: */
/*     N      i         number of rows and columns in matrices W and V */
/*     NP     i         first dimension of array containing matrix V */
/*     NC     i         first dimension of array to receive CVM */
/*     W      d(N)      NxN diagonal matrix W (diagonal elements only) */
/*     V      d(NP,NP)  array containing NxN orthogonal matrix V */

/*  Returned: */
/*     WORK   d(N)      workspace */
/*     CVM    d(NC,NC)  array to receive covariance matrix */

/*  Reference: */
/*     Numerical Recipes, section 14.3. */

/*  P.T.Wallace   Starlink   December 1988 */

/*  Copyright (C) 1995 Rutherford Appleton Laboratory */

/*  License: */
/*    This program is free software; you can redistribute it and/or modify */
/*    it under the terms of the GNU General Public License as published by */
/*    the Free Software Foundation; either version 2 of the License, or */
/*    (at your option) any later version. */

/*    This program is distributed in the hope that it will be useful, */
/*    but WITHOUT ANY WARRANTY; without even the implied warranty of */
/*    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the */
/*    GNU General Public License for more details. */

/*    You should have received a copy of the GNU General Public License */
/*    along with this program (see SLA_CONDITIONS); if not, write to the */
/*    Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor, */
/*    Boston, MA  02110-1301  USA */

/* - */
    /* Parameter adjustments */
    --work;
    --w;
    v_dim1 = *np;
    v_offset = 1 + v_dim1;
    v -= v_offset;
    cvm_dim1 = *nc;
    cvm_offset = 1 + cvm_dim1;
    cvm -= cvm_offset;

    /* Function Body */
    i__1 = *n;
    for (i__ = 1; i__ <= i__1; ++i__) {
	s = w[i__];
	if (s != 0.) {
	    work[i__] = 1. / (s * s);
	} else {
	    work[i__] = 0.;
	}
    }
    i__1 = *n;
    for (i__ = 1; i__ <= i__1; ++i__) {
	i__2 = i__;
	for (j = 1; j <= i__2; ++j) {
	    s = 0.;
	    i__3 = *n;
	    for (k = 1; k <= i__3; ++k) {
		s += v[i__ + k * v_dim1] * v[j + k * v_dim1] * work[k];
	    }
	    cvm[i__ + j * cvm_dim1] = s;
	    cvm[j + i__ * cvm_dim1] = s;
	}
    }
    return 0;
} /* sla_svdcov__ */

/* Subroutine */ int sla_svdsol__(integer *m, integer *n, integer *mp, 
	integer *np, doublereal *b, doublereal *u, doublereal *w, doublereal *
	v, doublereal *work, doublereal *x)
{
    /* System generated locals */
    integer u_dim1, u_offset, v_dim1, v_offset, i__1, i__2;

    /* Local variables */
    static integer i__, j;
    static doublereal s;
    static integer jj;

/* + */
/*     - - - - - - - */
/*      S V D S O L */
/*     - - - - - - - */

/*  From a given vector and the SVD of a matrix (as obtained from */
/*  the SVD routine), obtain the solution vector (double precision) */

/*  This routine solves the equation: */

/*     A . x = b */

/*  where: */

/*     A   is a given M (rows) x N (columns) matrix, where M.GE.N */
/*     x   is the N-vector we wish to find */
/*     b   is a given M-vector */

/*  by means of the Singular Value Decomposition method (SVD).  In */
/*  this method, the matrix A is first factorised (for example by */
/*  the routine sla_SVD) into the following components: */

/*     A = U x W x VT */

/*  where: */

/*     A   is the M (rows) x N (columns) matrix */
/*     U   is an M x N column-orthogonal matrix */
/*     W   is an N x N diagonal matrix with W(I,I).GE.0 */
/*     VT  is the transpose of an NxN orthogonal matrix */

/*     Note that M and N, above, are the LOGICAL dimensions of the */
/*     matrices and vectors concerned, which can be located in */
/*     arrays of larger PHYSICAL dimensions MP and NP. */

/*  The solution is found from the expression: */

/*     x = V . [diag(1/Wj)] . (transpose(U) . b) */

/*  Notes: */

/*  1)  If matrix A is square, and if the diagonal matrix W is not */
/*      adjusted, the method is equivalent to conventional solution */
/*      of simultaneous equations. */

/*  2)  If M>N, the result is a least-squares fit. */

/*  3)  If the solution is poorly determined, this shows up in the */
/*      SVD factorisation as very small or zero Wj values.  Where */
/*      a Wj value is small but non-zero it can be set to zero to */
/*      avoid ill effects.  The present routine detects such zero */
/*      Wj values and produces a sensible solution, with highly */
/*      correlated terms kept under control rather than being allowed */
/*      to elope to infinity, and with meaningful values for the */
/*      other terms. */

/*  Given: */
/*     M,N    i         numbers of rows and columns in matrix A */
/*     MP,NP  i         physical dimensions of array containing matrix A */
/*     B      d(M)      known vector b */
/*     U      d(MP,NP)  array containing MxN matrix U */
/*     W      d(N)      NxN diagonal matrix W (diagonal elements only) */
/*     V      d(NP,NP)  array containing NxN orthogonal matrix V */

/*  Returned: */
/*     WORK   d(N)      workspace */
/*     X      d(N)      unknown vector x */

/*  Reference: */
/*     Numerical Recipes, section 2.9. */

/*  P.T.Wallace   Starlink   29 October 1993 */

/*  Copyright (C) 1995 Rutherford Appleton Laboratory */

/*  License: */
/*    This program is free software; you can redistribute it and/or modify */
/*    it under the terms of the GNU General Public License as published by */
/*    the Free Software Foundation; either version 2 of the License, or */
/*    (at your option) any later version. */

/*    This program is distributed in the hope that it will be useful, */
/*    but WITHOUT ANY WARRANTY; without even the implied warranty of */
/*    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the */
/*    GNU General Public License for more details. */

/*    You should have received a copy of the GNU General Public License */
/*    along with this program (see SLA_CONDITIONS); if not, write to the */
/*    Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor, */
/*    Boston, MA  02110-1301  USA */

/* - */
/*  Calculate [diag(1/Wj)] . transpose(U) . b (or zero for zero Wj) */
    /* Parameter adjustments */
    --b;
    --x;
    --work;
    --w;
    v_dim1 = *np;
    v_offset = 1 + v_dim1;
    v -= v_offset;
    u_dim1 = *mp;
    u_offset = 1 + u_dim1;
    u -= u_offset;

    /* Function Body */
    i__1 = *n;
    for (j = 1; j <= i__1; ++j) {
	s = 0.;
	if (w[j] != 0.) {
	    i__2 = *m;
	    for (i__ = 1; i__ <= i__2; ++i__) {
		s += u[i__ + j * u_dim1] * b[i__];
	    }
	    s /= w[j];
	}
	work[j] = s;
    }
/*  Multiply by matrix V to get result */
    i__1 = *n;
    for (j = 1; j <= i__1; ++j) {
	s = 0.;
	i__2 = *n;
	for (jj = 1; jj <= i__2; ++jj) {
	    s += v[j + jj * v_dim1] * work[jj];
	}
	x[j] = s;
    }
    return 0;
} /* sla_svdsol__ */

/* Subroutine */ int sla_fitxy__(integer *itype, integer *np, doublereal *xye,
	 doublereal *xym, doublereal *coeffs, integer *j)
{
    /* System generated locals */
    integer i__1;

    /* Local variables */
    extern /* Subroutine */ int sla_dmat__(integer *, doublereal *, 
	    doublereal *, doublereal *, integer *, integer *), sla_dmxv__(
	    doublereal *, doublereal *, doublereal *);
    static doublereal a, b, c__, d__;
    static integer i__;
    static doublereal p, v[4], xe, ye;
    static integer iw[4];
    static doublereal xm, ym, xr, yr, dm3[9]	/* was [3][3] */, dm4[16]	
	    /* was [4][4] */, det, sgn, sxe, sye, sxm, sym, sdr2, aold, bold, 
	    cold, dold, sold;
    static integer nsol;
    static doublereal sx2y2;
    static integer jstat;
    static doublereal sxexm, sxeym, syeym, syexm, sxmxm, sxmym, symym, sxxyy, 
	    sxyyx;

/* + */
/*     - - - - - - */
/*      F I T X Y */
/*     - - - - - - */

/*  Fit a linear model to relate two sets of [X,Y] coordinates. */

/*  Given: */
/*     ITYPE    i        type of model: 4 or 6 (note 1) */
/*     NP       i        number of samples (note 2) */
/*     XYE     d(2,np)   expected [X,Y] for each sample */
/*     XYM     d(2,np)   measured [X,Y] for each sample */

/*  Returned: */
/*     COEFFS  d(6)      coefficients of model (note 3) */
/*     J        i        status:  0 = OK */
/*                               -1 = illegal ITYPE */
/*                               -2 = insufficient data */
/*                               -3 = no solution */

/*  Notes: */

/*  1)  ITYPE, which must be either 4 or 6, selects the type of model */
/*      fitted.  Both allowed ITYPE values produce a model COEFFS which */
/*      consists of six coefficients, namely the zero points and, for */
/*      each of XE and YE, the coefficient of XM and YM.  For ITYPE=6, */
/*      all six coefficients are independent, modelling squash and shear */
/*      as well as origin, scale, and orientation.  However, ITYPE=4 */
/*      selects the "solid body rotation" option;  the model COEFFS */
/*      still consists of the same six coefficients, but now two of */
/*      them are used twice (appropriately signed).  Origin, scale */
/*      and orientation are still modelled, but not squash or shear - */
/*      the units of X and Y have to be the same. */

/*  2)  For NC=4, NP must be at least 2.  For NC=6, NP must be at */
/*      least 3. */

/*  3)  The model is returned in the array COEFFS.  Naming the */
/*      elements of COEFFS as follows: */

/*                  COEFFS(1) = A */
/*                  COEFFS(2) = B */
/*                  COEFFS(3) = C */
/*                  COEFFS(4) = D */
/*                  COEFFS(5) = E */
/*                  COEFFS(6) = F */

/*      the model is: */

/*            XE = A + B*XM + C*YM */
/*            YE = D + E*XM + F*YM */

/*      For the "solid body rotation" option (ITYPE=4), the */
/*      magnitudes of B and F, and of C and E, are equal.  The */
/*      signs of these coefficients depend on whether there is a */
/*      sign reversal between XE,YE and XM,YM;  fits are performed */
/*      with and without a sign reversal and the best one chosen. */

/*  4)  Error status values J=-1 and -2 leave COEFFS unchanged; */
/*      if J=-3 COEFFS may have been changed. */

/*  See also sla_PXY, sla_INVF, sla_XY2XY, sla_DCMPF */

/*  Called:  sla_DMAT, sla_DMXV */

/*  Last revision:   8 September 2005 */

/*  Copyright P.T.Wallace.  All rights reserved. */

/*  License: */
/*    This program is free software; you can redistribute it and/or modify */
/*    it under the terms of the GNU General Public License as published by */
/*    the Free Software Foundation; either version 2 of the License, or */
/*    (at your option) any later version. */

/*    This program is distributed in the hope that it will be useful, */
/*    but WITHOUT ANY WARRANTY; without even the implied warranty of */
/*    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the */
/*    GNU General Public License for more details. */

/*    You should have received a copy of the GNU General Public License */
/*    along with this program (see SLA_CONDITIONS); if not, write to the */
/*    Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor, */
/*    Boston, MA  02110-1301  USA */

/* - */
/*  Preset the status */
    /* Parameter adjustments */
    xym -= 3;
    xye -= 3;
    --coeffs;

    /* Function Body */
    *j = 0;
/*  Variable initializations to avoid compiler warnings */
    a = 0.;
    b = 0.;
    c__ = 0.;
    d__ = 0.;
    aold = 0.;
    bold = 0.;
    cold = 0.;
    dold = 0.;
    sold = 0.;
/*  Float the number of samples */
    p = (doublereal) (*np);
/*  Check ITYPE */
    if (*itype == 6) {

/*     Six-coefficient linear model */
/*     ---------------------------- */
/*     Check enough samples */
	if (*np >= 3) {
/*     Form summations */
	    sxe = 0.;
	    sxexm = 0.;
	    sxeym = 0.;
	    sye = 0.;
	    syeym = 0.;
	    syexm = 0.;
	    sxm = 0.;
	    sym = 0.;
	    sxmxm = 0.;
	    sxmym = 0.;
	    symym = 0.;
	    i__1 = *np;
	    for (i__ = 1; i__ <= i__1; ++i__) {
		xe = xye[(i__ << 1) + 1];
		ye = xye[(i__ << 1) + 2];
		xm = xym[(i__ << 1) + 1];
		ym = xym[(i__ << 1) + 2];
		sxe += xe;
		sxexm += xe * xm;
		sxeym += xe * ym;
		sye += ye;
		syeym += ye * ym;
		syexm += ye * xm;
		sxm += xm;
		sym += ym;
		sxmxm += xm * xm;
		sxmym += xm * ym;
		symym += ym * ym;
	    }
/*        Solve for A,B,C in  XE = A + B*XM + C*YM */
	    v[0] = sxe;
	    v[1] = sxexm;
	    v[2] = sxeym;
	    dm3[0] = p;
	    dm3[3] = sxm;
	    dm3[6] = sym;
	    dm3[1] = sxm;
	    dm3[4] = sxmxm;
	    dm3[7] = sxmym;
	    dm3[2] = sym;
	    dm3[5] = sxmym;
	    dm3[8] = symym;
	    sla_dmat__(&c__3, dm3, v, &det, &jstat, iw);
	    if (jstat == 0) {
		for (i__ = 1; i__ <= 3; ++i__) {
		    coeffs[i__] = v[i__ - 1];
		}
/*           Solve for D,E,F in  YE = D + E*XM + F*YM */
		v[0] = sye;
		v[1] = syexm;
		v[2] = syeym;
		sla_dmxv__(dm3, v, &coeffs[4]);
	    } else {
/*           No 6-coefficient solution possible */
		*j = -3;
	    }
	} else {
/*        Insufficient data for 6-coefficient fit */
	    *j = -2;
	}
    } else if (*itype == 4) {

/*     Four-coefficient solid body rotation model */
/*     ------------------------------------------ */
/*     Check enough samples */
	if (*np >= 2) {
/*        Try two solutions, first without then with flip in X */
	    for (nsol = 1; nsol <= 2; ++nsol) {
		if (nsol == 1) {
		    sgn = 1.;
		} else {
		    sgn = -1.;
		}
/*           Form summations */
		sxe = 0.;
		sxxyy = 0.;
		sxyyx = 0.;
		sye = 0.;
		sxm = 0.;
		sym = 0.;
		sx2y2 = 0.;
		i__1 = *np;
		for (i__ = 1; i__ <= i__1; ++i__) {
		    xe = xye[(i__ << 1) + 1] * sgn;
		    ye = xye[(i__ << 1) + 2];
		    xm = xym[(i__ << 1) + 1];
		    ym = xym[(i__ << 1) + 2];
		    sxe += xe;
		    sxxyy = sxxyy + xe * xm + ye * ym;
		    sxyyx = sxyyx + xe * ym - ye * xm;
		    sye += ye;
		    sxm += xm;
		    sym += ym;
		    sx2y2 = sx2y2 + xm * xm + ym * ym;
		}

/*           Solve for A,B,C,D in:  +/- XE = A + B*XM - C*YM */
/*                                    + YE = D + C*XM + B*YM */
		v[0] = sxe;
		v[1] = sxxyy;
		v[2] = sxyyx;
		v[3] = sye;
		dm4[0] = p;
		dm4[4] = sxm;
		dm4[8] = -sym;
		dm4[12] = 0.;
		dm4[1] = sxm;
		dm4[5] = sx2y2;
		dm4[9] = 0.;
		dm4[13] = sym;
		dm4[2] = sym;
		dm4[6] = 0.;
		dm4[10] = -sx2y2;
		dm4[14] = -sxm;
		dm4[3] = 0.;
		dm4[7] = sym;
		dm4[11] = sxm;
		dm4[15] = p;
		sla_dmat__(&c__4, dm4, v, &det, &jstat, iw);
		if (jstat == 0) {
		    a = v[0];
		    b = v[1];
		    c__ = v[2];
		    d__ = v[3];
/*              Determine sum of radial errors squared */
		    sdr2 = 0.;
		    i__1 = *np;
		    for (i__ = 1; i__ <= i__1; ++i__) {
			xm = xym[(i__ << 1) + 1];
			ym = xym[(i__ << 1) + 2];
			xr = a + b * xm - c__ * ym - xye[(i__ << 1) + 1] * 
				sgn;
			yr = d__ + c__ * xm + b * ym - xye[(i__ << 1) + 2];
			sdr2 = sdr2 + xr * xr + yr * yr;
		    }
		} else {
/*              Singular: set flag */
		    sdr2 = -1.;
		}
/*           If first pass and non-singular, save variables */
		if (nsol == 1 && jstat == 0) {
		    aold = a;
		    bold = b;
		    cold = c__;
		    dold = d__;
		    sold = sdr2;
		}
	    }
/*        Pick the best of the two solutions */
	    if (sold >= 0. && (sold <= sdr2 || *np == 2)) {
		coeffs[1] = aold;
		coeffs[2] = bold;
		coeffs[3] = -cold;
		coeffs[4] = dold;
		coeffs[5] = cold;
		coeffs[6] = bold;
	    } else if (jstat == 0) {
		coeffs[1] = -a;
		coeffs[2] = -b;
		coeffs[3] = c__;
		coeffs[4] = d__;
		coeffs[5] = c__;
		coeffs[6] = b;
	    } else {
/*           No 4-coefficient fit possible */
		*j = -3;
	    }
	} else {
/*        Insufficient data for 4-coefficient fit */
	    *j = -2;
	}
    } else {
/*     Illegal ITYPE - not 4 or 6 */
	*j = -1;
    }
    return 0;
} /* sla_fitxy__ */

/* Subroutine */ int sla_xy2xy__(doublereal *x1, doublereal *y1, doublereal *
	coeffs, doublereal *x2, doublereal *y2)
{
/* + */
/*     - - - - - - */
/*      X Y 2 X Y */
/*     - - - - - - */

/*  Transform one [X,Y] into another using a linear model of the type */
/*  produced by the sla_FITXY routine. */

/*  Given: */
/*     X1       d        x-coordinate */
/*     Y1       d        y-coordinate */
/*     COEFFS  d(6)      transformation coefficients (see note) */

/*  Returned: */
/*     X2       d        x-coordinate */
/*     Y2       d        y-coordinate */

/*  The model relates two sets of [X,Y] coordinates as follows. */
/*  Naming the elements of COEFFS: */

/*     COEFFS(1) = A */
/*     COEFFS(2) = B */
/*     COEFFS(3) = C */
/*     COEFFS(4) = D */
/*     COEFFS(5) = E */
/*     COEFFS(6) = F */

/*  the present routine performs the transformation: */

/*     X2 = A + B*X1 + C*Y1 */
/*     Y2 = D + E*X1 + F*Y1 */

/*  See also sla_FITXY, sla_PXY, sla_INVF, sla_DCMPF */

/*  P.T.Wallace   Starlink   5 December 1994 */

/*  Copyright (C) 1995 Rutherford Appleton Laboratory */

/*  License: */
/*    This program is free software; you can redistribute it and/or modify */
/*    it under the terms of the GNU General Public License as published by */
/*    the Free Software Foundation; either version 2 of the License, or */
/*    (at your option) any later version. */

/*    This program is distributed in the hope that it will be useful, */
/*    but WITHOUT ANY WARRANTY; without even the implied warranty of */
/*    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the */
/*    GNU General Public License for more details. */

/*    You should have received a copy of the GNU General Public License */
/*    along with this program (see SLA_CONDITIONS); if not, write to the */
/*    Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor, */
/*    Boston, MA  02110-1301  USA */

/* - */
    /* Parameter adjustments */
    --coeffs;

    /* Function Body */
    *x2 = coeffs[1] + coeffs[2] * *x1 + coeffs[3] * *y1;
    *y2 = coeffs[4] + coeffs[5] * *x1 + coeffs[6] * *y1;
    return 0;
} /* sla_xy2xy__ */

/* Subroutine */ int sla_dmxv__(doublereal *dm, doublereal *va, doublereal *
	vb)
{
    static integer i__, j;
    static doublereal w, vw[3];

/* + */
/*     - - - - - */
/*      D M X V */
/*     - - - - - */

/*  Performs the 3-D forward unitary transformation: */

/*     vector VB = matrix DM * vector VA */

/*  (double precision) */

/*  Given: */
/*     DM       dp(3,3)    matrix */
/*     VA       dp(3)      vector */

/*  Returned: */
/*     VB       dp(3)      result vector */

/*  To comply with the ANSI Fortran 77 standard, VA and VB must be */
/*  different arrays.  However, the routine is coded so as to work */
/*  properly on many platforms even if this rule is violated. */

/*  Last revision:   26 December 2004 */

/*  Copyright P.T.Wallace.  All rights reserved. */

/*  License: */
/*    This program is free software; you can redistribute it and/or modify */
/*    it under the terms of the GNU General Public License as published by */
/*    the Free Software Foundation; either version 2 of the License, or */
/*    (at your option) any later version. */

/*    This program is distributed in the hope that it will be useful, */
/*    but WITHOUT ANY WARRANTY; without even the implied warranty of */
/*    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the */
/*    GNU General Public License for more details. */

/*    You should have received a copy of the GNU General Public License */
/*    along with this program (see SLA_CONDITIONS); if not, write to the */
/*    Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor, */
/*    Boston, MA  02110-1301  USA */

/* - */
/*  Matrix DM * vector VA -> vector VW */
    /* Parameter adjustments */
    --vb;
    --va;
    dm -= 4;

    /* Function Body */
    for (j = 1; j <= 3; ++j) {
	w = 0.;
	for (i__ = 1; i__ <= 3; ++i__) {
	    w += dm[j + i__ * 3] * va[i__];
	}
	vw[j - 1] = w;
    }
/*  Vector VW -> vector VB */
    for (j = 1; j <= 3; ++j) {
	vb[j] = vw[j - 1];
    }
    return 0;
} /* sla_dmxv__ */

/* Subroutine */ int sla_dmat__(integer *n, doublereal *a, doublereal *y, 
	doublereal *d__, integer *jf, integer *iw)
{
    /* System generated locals */
    integer a_dim1, a_offset, i__1, i__2, i__3;
    doublereal d__1;

    /* Local variables */
    static integer i__, j, k;
    static doublereal t;
    static integer ki;
    static doublereal yk, aik, akk, amx;
    static integer imx, np1mk;

/* + */
/*     - - - - - */
/*      D M A T */
/*     - - - - - */

/*  Matrix inversion & solution of simultaneous equations */
/*  (double precision) */

/*  For the set of n simultaneous equations in n unknowns: */
/*     A.Y = X */

/*  where: */
/*     A is a non-singular N x N matrix */
/*     Y is the vector of N unknowns */
/*     X is the known vector */

/*  DMATRX computes: */
/*     the inverse of matrix A */
/*     the determinant of matrix A */
/*     the vector of N unknowns */

/*  Arguments: */

/*     symbol  type   dimension           before              after */

/*       N      i                    no. of unknowns       unchanged */
/*       A      d      (N,N)             matrix             inverse */
/*       Y      d       (N)            known vector      solution vector */
/*       D      d                           -             determinant */
/*     * JF     i                           -           singularity flag */
/*       IW     i       (N)                 -              workspace */

/*  * JF is the singularity flag.  If the matrix is non-singular, JF=0 */
/*    is returned.  If the matrix is singular, JF=-1 & D=0D0 are */
/*    returned.  In the latter case, the contents of array A on return */
/*    are undefined. */

/*  Algorithm: */
/*     Gaussian elimination with partial pivoting. */

/*  Speed: */
/*     Very fast. */

/*  Accuracy: */
/*     Fairly accurate - errors 1 to 4 times those of routines optimized */
/*     for accuracy. */

/*  P.T.Wallace   Starlink   4 December 2001 */

/*  Copyright (C) 2001 Rutherford Appleton Laboratory */

/*  License: */
/*    This program is free software; you can redistribute it and/or modify */
/*    it under the terms of the GNU General Public License as published by */
/*    the Free Software Foundation; either version 2 of the License, or */
/*    (at your option) any later version. */

/*    This program is distributed in the hope that it will be useful, */
/*    but WITHOUT ANY WARRANTY; without even the implied warranty of */
/*    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the */
/*    GNU General Public License for more details. */

/*    You should have received a copy of the GNU General Public License */
/*    along with this program (see SLA_CONDITIONS); if not, write to the */
/*    Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor, */
/*    Boston, MA  02110-1301  USA */

/* - */
    /* Parameter adjustments */
    --iw;
    --y;
    a_dim1 = *n;
    a_offset = 1 + a_dim1;
    a -= a_offset;

    /* Function Body */
    *jf = 0;
    *d__ = 1.;
    i__1 = *n;
    for (k = 1; k <= i__1; ++k) {
	amx = (d__1 = a[k + k * a_dim1], abs(d__1));
	imx = k;
	if (k != *n) {
	    i__2 = *n;
	    for (i__ = k + 1; i__ <= i__2; ++i__) {
		t = (d__1 = a[i__ + k * a_dim1], abs(d__1));
		if (t > amx) {
		    amx = t;
		    imx = i__;
		}
	    }
	}
	if (amx < 1e-20) {
	    *jf = -1;
	} else {
	    if (imx != k) {
		i__2 = *n;
		for (j = 1; j <= i__2; ++j) {
		    t = a[k + j * a_dim1];
		    a[k + j * a_dim1] = a[imx + j * a_dim1];
		    a[imx + j * a_dim1] = t;
		}
		t = y[k];
		y[k] = y[imx];
		y[imx] = t;
		*d__ = -(*d__);
	    }
	    iw[k] = imx;
	    akk = a[k + k * a_dim1];
	    *d__ *= akk;
	    if (abs(*d__) < 1e-20) {
		*jf = -1;
	    } else {
		akk = 1. / akk;
		a[k + k * a_dim1] = akk;
		i__2 = *n;
		for (j = 1; j <= i__2; ++j) {
		    if (j != k) {
			a[k + j * a_dim1] *= akk;
		    }
		}
		yk = y[k] * akk;
		y[k] = yk;
		i__2 = *n;
		for (i__ = 1; i__ <= i__2; ++i__) {
		    aik = a[i__ + k * a_dim1];
		    if (i__ != k) {
			i__3 = *n;
			for (j = 1; j <= i__3; ++j) {
			    if (j != k) {
				a[i__ + j * a_dim1] -= aik * a[k + j * a_dim1]
					;
			    }
			}
			y[i__] -= aik * yk;
		    }
		}
		i__2 = *n;
		for (i__ = 1; i__ <= i__2; ++i__) {
		    if (i__ != k) {
			a[i__ + k * a_dim1] = -a[i__ + k * a_dim1] * akk;
		    }
		}
	    }
	}
    }
    if (*jf != 0) {
	*d__ = 0.;
    } else {
	i__1 = *n;
	for (k = 1; k <= i__1; ++k) {
	    np1mk = *n + 1 - k;
	    ki = iw[np1mk];
	    if (np1mk != ki) {
		i__2 = *n;
		for (i__ = 1; i__ <= i__2; ++i__) {
		    t = a[i__ + np1mk * a_dim1];
		    a[i__ + np1mk * a_dim1] = a[i__ + ki * a_dim1];
		    a[i__ + ki * a_dim1] = t;
		}
	    }
	}
    }
    return 0;
} /* sla_dmat__ */

/* Subroutine */ int sla_pxy__(integer *np, doublereal *xye, doublereal *xym, 
	doublereal *coeffs, doublereal *xyp, doublereal *xrms, doublereal *
	yrms, doublereal *rrms)
{
    /* System generated locals */
    integer i__1;
    doublereal d__1, d__2;

    /* Builtin functions */
    double sqrt(doublereal);

    /* Local variables */
    static integer i__;
    static doublereal p;
    extern /* Subroutine */ int sla_xy2xy__(doublereal *, doublereal *, 
	    doublereal *, doublereal *, doublereal *);
    static doublereal dx, dy, xp, yp, dx2, dy2, sdx2, sdy2;

/* + */
/*     - - - - */
/*      P X Y */
/*     - - - - */

/*  Given arrays of "expected" and "measured" [X,Y] coordinates, and a */
/*  linear model relating them (as produced by sla_FITXY), compute */
/*  the array of "predicted" coordinates and the RMS residuals. */

/*  Given: */
/*     NP       i        number of samples */
/*     XYE     d(2,np)   expected [X,Y] for each sample */
/*     XYM     d(2,np)   measured [X,Y] for each sample */
/*     COEFFS  d(6)      coefficients of model (see below) */

/*  Returned: */
/*     XYP     d(2,np)   predicted [X,Y] for each sample */
/*     XRMS     d        RMS in X */
/*     YRMS     d        RMS in Y */
/*     RRMS     d        total RMS (vector sum of XRMS and YRMS) */

/*  The model is supplied in the array COEFFS.  Naming the */
/*  elements of COEFF as follows: */

/*     COEFFS(1) = A */
/*     COEFFS(2) = B */
/*     COEFFS(3) = C */
/*     COEFFS(4) = D */
/*     COEFFS(5) = E */
/*     COEFFS(6) = F */

/*  the model is applied thus: */

/*     XP = A + B*XM + C*YM */
/*     YP = D + E*XM + F*YM */

/*  The residuals are (XP-XE) and (YP-YE). */

/*  If NP is less than or equal to zero, no coordinates are */
/*  transformed, and the RMS residuals are all zero. */

/*  See also sla_FITXY, sla_INVF, sla_XY2XY, sla_DCMPF */

/*  Called:  sla_XY2XY */

/*  P.T.Wallace   Starlink   22 May 1996 */

/*  Copyright (C) 1996 Rutherford Appleton Laboratory */

/*  License: */
/*    This program is free software; you can redistribute it and/or modify */
/*    it under the terms of the GNU General Public License as published by */
/*    the Free Software Foundation; either version 2 of the License, or */
/*    (at your option) any later version. */

/*    This program is distributed in the hope that it will be useful, */
/*    but WITHOUT ANY WARRANTY; without even the implied warranty of */
/*    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the */
/*    GNU General Public License for more details. */

/*    You should have received a copy of the GNU General Public License */
/*    along with this program (see SLA_CONDITIONS); if not, write to the */
/*    Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor, */
/*    Boston, MA  02110-1301  USA */

/* - */
/*  Initialize summations */
    /* Parameter adjustments */
    xyp -= 3;
    xym -= 3;
    xye -= 3;
    --coeffs;

    /* Function Body */
    sdx2 = 0.;
    sdy2 = 0.;
/*  Loop by sample */
    i__1 = *np;
    for (i__ = 1; i__ <= i__1; ++i__) {
/*     Transform "measured" [X,Y] to "predicted" [X,Y] */
	sla_xy2xy__(&xym[(i__ << 1) + 1], &xym[(i__ << 1) + 2], &coeffs[1], &
		xp, &yp);
	xyp[(i__ << 1) + 1] = xp;
	xyp[(i__ << 1) + 2] = yp;
/*     Compute residuals in X and Y, and update summations */
	dx = xye[(i__ << 1) + 1] - xp;
	dy = xye[(i__ << 1) + 2] - yp;
	dx2 = dx * dx;
	dy2 = dy * dy;
	sdx2 += dx2;
	sdy2 += dy2;
/*     Next sample */
    }
/*  Compute RMS values */
/* Computing MAX */
    d__1 = 1., d__2 = (doublereal) (*np);
    p = max(d__1,d__2);
    *xrms = sqrt(sdx2 / p);
    *yrms = sqrt(sdy2 / p);
    *rrms = sqrt(*xrms * *xrms + *yrms * *yrms);
    return 0;
} /* sla_pxy__ */

/* Subroutine */ int sla_dcmpf__(doublereal *coeffs, doublereal *xz, 
	doublereal *yz, doublereal *xs, doublereal *ys, doublereal *perp, 
	doublereal *orient)
{
    /* System generated locals */
    doublereal d__1;

    /* Builtin functions */
    double sqrt(doublereal), atan2(doublereal, doublereal), sin(doublereal), 
	    cos(doublereal);

    /* Local variables */
    static doublereal a, b, c__, d__, e, f, p, p1, p2, x0, y0, hp, wc, or, ws;
    extern doublereal sla_drange__(doublereal *);
    static doublereal chp, det, cor, shp, xsc, ysc, sor, rb2e2, rc2f2;

/* + */
/*     - - - - - - */
/*      D C M P F */
/*     - - - - - - */

/*  Decompose an [X,Y] linear fit into its constituent parameters: */
/*  zero points, scales, nonperpendicularity and orientation. */

/*  Given: */
/*     COEFFS  d(6)      transformation coefficients (see note) */

/*  Returned: */
/*     XZ       d        x zero point */
/*     YZ       d        y zero point */
/*     XS       d        x scale */
/*     YS       d        y scale */
/*     PERP     d        nonperpendicularity (radians) */
/*     ORIENT   d        orientation (radians) */

/*  Called:  sla_DRANGE */

/*  The model relates two sets of [X,Y] coordinates as follows. */
/*  Naming the elements of COEFFS: */

/*     COEFFS(1) = A */
/*     COEFFS(2) = B */
/*     COEFFS(3) = C */
/*     COEFFS(4) = D */
/*     COEFFS(5) = E */
/*     COEFFS(6) = F */

/*  the model transforms coordinates [X1,Y1] into coordinates */
/*  [X2,Y2] as follows: */

/*     X2 = A + B*X1 + C*Y1 */
/*     Y2 = D + E*X1 + F*Y1 */

/*  The transformation can be decomposed into four steps: */

/*     1)  Zero points: */

/*             x' = XZ + X1 */
/*             y' = YZ + Y1 */

/*     2)  Scales: */

/*             x'' = XS*x' */
/*             y'' = YS*y' */

/*     3)  Nonperpendicularity: */

/*             x''' = cos(PERP/2)*x'' + sin(PERP/2)*y'' */
/*             y''' = sin(PERP/2)*x'' + cos(PERP/2)*y'' */

/*     4)  Orientation: */

/*             X2 = cos(ORIENT)*x''' + sin(ORIENT)*y''' */
/*             Y2 =-sin(ORIENT)*y''' + cos(ORIENT)*y''' */

/*  See also sla_FITXY, sla_PXY, sla_INVF, sla_XY2XY */

/*  P.T.Wallace   Starlink   19 December 2001 */

/*  Copyright (C) 2001 Rutherford Appleton Laboratory */

/*  License: */
/*    This program is free software; you can redistribute it and/or modify */
/*    it under the terms of the GNU General Public License as published by */
/*    the Free Software Foundation; either version 2 of the License, or */
/*    (at your option) any later version. */

/*    This program is distributed in the hope that it will be useful, */
/*    but WITHOUT ANY WARRANTY; without even the implied warranty of */
/*    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the */
/*    GNU General Public License for more details. */

/*    You should have received a copy of the GNU General Public License */
/*    along with this program (see SLA_CONDITIONS); if not, write to the */
/*    Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor, */
/*    Boston, MA  02110-1301  USA */

/* - */
/*  Copy the six coefficients. */
    /* Parameter adjustments */
    --coeffs;

    /* Function Body */
    a = coeffs[1];
    b = coeffs[2];
    c__ = coeffs[3];
    d__ = coeffs[4];
    e = coeffs[5];
    f = coeffs[6];
/*  Scales. */
    rb2e2 = sqrt(b * b + e * e);
    rc2f2 = sqrt(c__ * c__ + f * f);
    if (b * f - c__ * e >= 0.) {
	xsc = rb2e2;
    } else {
	b = -b;
	e = -e;
	xsc = -rb2e2;
    }
    ysc = rc2f2;
/*  Non-perpendicularity. */
    if (c__ != 0. || f != 0.) {
	p1 = atan2(c__, f);
    } else {
	p1 = 0.;
    }
    if (e != 0. || b != 0.) {
	p2 = atan2(e, b);
    } else {
	p2 = 0.;
    }
    d__1 = p1 + p2;
    p = sla_drange__(&d__1);
/*  Orientation. */
    ws = c__ * rb2e2 - e * rc2f2;
    wc = b * rc2f2 + f * rb2e2;
    if (ws != 0. || wc != 0.) {
	or = atan2(ws, wc);
    } else {
	or = 0.;
    }
/*  Zero points. */
    hp = p / 2.;
    shp = sin(hp);
    chp = cos(hp);
    sor = sin(or);
    cor = cos(or);
    det = xsc * ysc * (chp + shp) * (chp - shp);
    if (abs(det) > 0.) {
	x0 = ysc * (a * (chp * cor - shp * sor) - d__ * (chp * sor + shp * 
		cor)) / det;
	y0 = xsc * (a * (chp * sor - shp * cor) + d__ * (chp * cor + shp * 
		sor)) / det;
    } else {
	x0 = 0.;
	y0 = 0.;
    }
/*  Results. */
    *xz = x0;
    *yz = y0;
    *xs = xsc;
    *ys = ysc;
    *perp = p;
    *orient = or;
    return 0;
} /* sla_dcmpf__ */

/* Subroutine */ int sla_invf__(doublereal *fwds, doublereal *bkwds, integer *
	j)
{
    static doublereal a, b, c__, d__, e, f, det;

/* + */
/*     - - - - - */
/*      I N V F */
/*     - - - - - */

/*  Invert a linear model of the type produced by the sla_FITXY routine. */

/*  Given: */
/*     FWDS    d(6)      model coefficients */

/*  Returned: */
/*     BKWDS   d(6)      inverse model */
/*     J        i        status:  0 = OK, -1 = no inverse */

/*  The models relate two sets of [X,Y] coordinates as follows. */
/*  Naming the elements of FWDS: */

/*     FWDS(1) = A */
/*     FWDS(2) = B */
/*     FWDS(3) = C */
/*     FWDS(4) = D */
/*     FWDS(5) = E */
/*     FWDS(6) = F */

/*  where two sets of coordinates [X1,Y1] and [X2,Y1] are related */
/*  thus: */

/*     X2 = A + B*X1 + C*Y1 */
/*     Y2 = D + E*X1 + F*Y1 */

/*  the present routine generates a new set of coefficients: */

/*     BKWDS(1) = P */
/*     BKWDS(2) = Q */
/*     BKWDS(3) = R */
/*     BKWDS(4) = S */
/*     BKWDS(5) = T */
/*     BKWDS(6) = U */

/*  such that: */

/*     X1 = P + Q*X2 + R*Y2 */
/*     Y1 = S + T*X2 + U*Y2 */

/*  Two successive calls to sla_INVF will thus deliver a set */
/*  of coefficients equal to the starting values. */

/*  To comply with the ANSI Fortran standard, FWDS and BKWDS must */
/*  not be the same array, even though the routine is coded to */
/*  work on many platforms even if this rule is violated. */

/*  See also sla_FITXY, sla_PXY, sla_XY2XY, sla_DCMPF */

/*  Last revision:   26 December 2004 */

/*  Copyright P.T.Wallace.  All rights reserved. */

/*  License: */
/*    This program is free software; you can redistribute it and/or modify */
/*    it under the terms of the GNU General Public License as published by */
/*    the Free Software Foundation; either version 2 of the License, or */
/*    (at your option) any later version. */

/*    This program is distributed in the hope that it will be useful, */
/*    but WITHOUT ANY WARRANTY; without even the implied warranty of */
/*    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the */
/*    GNU General Public License for more details. */

/*    You should have received a copy of the GNU General Public License */
/*    along with this program (see SLA_CONDITIONS); if not, write to the */
/*    Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor, */
/*    Boston, MA  02110-1301  USA */

/* - */
    /* Parameter adjustments */
    --bkwds;
    --fwds;

    /* Function Body */
    a = fwds[1];
    b = fwds[2];
    c__ = fwds[3];
    d__ = fwds[4];
    e = fwds[5];
    f = fwds[6];
    det = b * f - c__ * e;
    if (det != 0.) {
	bkwds[1] = (c__ * d__ - a * f) / det;
	bkwds[2] = f / det;
	bkwds[3] = -c__ / det;
	bkwds[4] = (a * e - b * d__) / det;
	bkwds[5] = -e / det;
	bkwds[6] = b / det;
	*j = 0;
    } else {
	*j = -1;
    }
    return 0;
} /* sla_invf__ */

