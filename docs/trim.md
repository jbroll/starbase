

`trim` - removes extra white space from the columns of a table.

SYNOPSYS
--------

```
    `trim`  [-i *input*] [-o *output*] [*column*]
```

DESCRIPTION
-----------

`trim` removes extra white space that may have been placed in a table by
justify.  It trims white space from the beginning and ends of column values.
Any white space in the middle of a column value is unmodified.

OPTIONS
-------

All of the options of the [column](column.html) program are also availabe with `trim.`

{% include colstd-opts.md %}

EXAMPLES
--------

{% include column-seealso.md %}

