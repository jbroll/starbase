
NAME
====

compute - compute column values for the rows of a starbase data table

SYNOPSYS
========

```
        compute 'statements' < tablefile
```

DESCRIPTION
===========

`compute` executes a list of awk statements on each row of a starbase data
table.  Each statement is separated from the next by a semi-colon or newline.
`compute` is very useful for computing values for new columns in a table.

{% include table-inc.md %}

{% include mawk-inc.md %}

OPTIONS
=======

{% include tabstd-opts.md %}

CONVFMT and OFMT
================

When computing numeric columns with more than 6 significant figures it may
be necessary to set the formatting values that compute uses to convert numeric
value back into text for printing.  The CONVFMT value specifies a printf spec
for converting a number to text interanlly to compute and OFMT specifies
a printf spec for converting a number text for output.  When CONVFMT is set
OFMT is also set to the same value.

This is usually done on the command line with the -v option:

```
    john@panic : compute -v CONVFMT="%.9g" < tab 'XY = X*Y'
```
        


EXAMPLES
========

`
    Assign the a new value to the "dist" column in a table.  Since compute
    does not create new columns to hold computed values, it is often used
    with "column -a *newcolumn*" as in this example.

```
    john@panic : column -a dist < tab | compute 'dist = sqrt(X*X+Y*Y)'
    dist    X       Y       Z
    ----    -       -       -
    3.16228 1       3       4
    12.0416 8       9       0
```

'

{% include table-env.md %}

{% include row-seealso.md %}

