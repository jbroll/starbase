

`xyfittable` - Fit two pair of columns with a linear model of translation, scale, rotation and optional shear.


SYNOPSYS
--------

```
    `xyfittable` [-<0|2|3|4>] *X* *Y* *U* *V*
```

DESCRIPTION
-----------

`xyfittable` fits a linear model to relate two pair of columns in a table.  The
*X*, *Y* column pair are the indepentant or expected data.  The *U*, *V* pair
are the dependant or measured data.

The model contains 0, 2, 3, 4 or 6 free coefficients.  The option selects
the number of coefficients.  The full 6 coefficient model is the default.

The model is:

```
      xe = a + b*xm + c*ym
      ye = d + e*xm + f*ym
```

The default is to fit all six coefficients are independently, modelling squash
and shear as well as origin, scale, and orientation.

The four coefficient option selects a "solid body rotation" model.  The model
still consists of the same six coefficients, but now two of them are used
twice (appropriately signed).  The magnitudes of b and f, and of c and e, are
equal.  Origin, scale and orientation are still modelled, but not squash or
shear - the units of x and y have to be the same.  The signs of these
coefficients depend on whether there is a sign reversal between xe,ye and
xm,ym.  Fits are performed with and without a sign reversal and the best one
chosen.  The three coefficient option forces unity scale, fitting origin and
rotation.  The two coefficient option fits only origin.

With the zero coefficient option the RMS and residuals are computed between 
two pairs of columns.  This is convienient to allow other transformations on
columns to be compared with fits using the same plotting scripts.

For three and four coefficient model the number of rows must be at least 2.
For six coefficient model the number of rows must be at least 3.

See
[slaFitxy]( slalib/slaFitxy.3.html),
[slaInvf]( slalib/slaInvf.3.html),
[slaXy2xy]( slalib/slaXy2xy.3.html),
[slaDcmpf]( slalib/slaDcmpf.3.html)

OPTIONS
-------

  * -4 - select solid body rotation model
  * -6 - select six coefficient model.

SEE ALSO
--------


- [svdfittable]( svdfittable.html) - Fit any set of columns to with linear coefficients on arbitrary basis functions.
- [simplexfittable]( simplexfittable.html) - coefficient fit of a an expression to a dependent column.


{% include starbase-seealso.md %}
