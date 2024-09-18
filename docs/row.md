
### row - select rows from a starbase data table

SYNOPSYS
--------

```
    `row` 'selection expression' < *tablefile*
    `select` 'selection expression' < *tablefile*
```

DESCRIPTION
-----------

`row` reads a starbase data table from its standard input and prints the rows
for which the selection expression evaluates to true on the standard output.
The expression should be a valid awk language expression and may contain
references to column names and header keyword values.


{% include table-inc.md %}

{% include mawk-inc.md %}

OPTIONS
-------

{% include tabstd-opts.md %}

EXAMPLES
--------

Select all the rows that are for which the magnitude is fainter than 19:

```
        row 'Magnitude > 19' < catalog.tab
        ObjectNumber    Magnitude
        ------------    ---------
                   1         19.1
                   2         21.1
                   3         20.1
```


{% include table-env.md %}

{% include row-seealso.md %}

