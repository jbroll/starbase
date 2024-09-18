

`svdfittable` - linear coefficient fit of a set of basis functions to a dependent column.

SYNOPSYS
--------

```
     `svdfittable` [-m '*model* *spec*'] *column* [*col* ...]
```

DESCRIPTION
-----------

    `svdfittable` fits a set of model functions to the data in *column*.  The
    model is a comma separated list of basis functions.  `svdfittable` computes
    a linear coefficient for each basis function to provide the best fit sum to
    the dependent variable column.  Each basis function in the model is
    evaluated as an awk expression which may contain table column names and
    header values.  The columns of the table used in the basis function
    expressions are the independent variables of the fit.  More than one
    dependent variable column may be given and an independent fit will be
    performed for each.  See [slaSvd]( slalib/slaSvd.3.html) [{slaSvdcov]({slaSvdcov.html),
    slalib/slaSvdcov.3.html} [slaSvdsol]( slalib/slaSvdsol.3.html)


OPTIONS
-------

    * -m *model* *spec*

        Provide a model to fit.  A comma separated list of basis functions.


EXAMPLE
-------

`
 A simple linear fit of measured data in column Y to actual position column X.
 The first line of the example  uses [jottable](jottable.html) to creates some data to fit.  The
 second line fits the data with `svdfittable` and then neatens up the output
 table with `justify`

```
    john@@panic : [jottable](jottable.html) X 5 | [column](column.html) -a Y | [compute](compute.html) 'Y = X + gresid(.1)' > foo.tab
    john@@panic  : svdfittable < foo.tab -m '1, X' Y | [justify](justify.html)
    svdfittable
            
    Model            1, X
            
    RMS for each column fit:
    RMS_Y        0.012060
            
    Coefficients for each column fit:
    C_Y        -0.0646272       0.991883
            
           Y           Fit_Y            Res_Y
    --------        --------        ---------
    0.941286        0.927256        -0.014030
    1.907160        1.919138         0.011978
    2.895200        2.911021         0.015821
    3.914360        3.902904        -0.011456
    4.897100        4.894787        -0.002313
```
'

SEE ALSO
--------


- [xyfittable]( xyfittable.html)   - Fit one pair of X, Y columns to another pair with a linear model of translation, scale and optional shear.
- [simplexfittable]( simplexfittable.html) - coefficient fit of a an expression to a dependent column.


{% include starbase-seealso.md %}
