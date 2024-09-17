

`dashline` - output the dashline of a starbase data table.

SYNOPSYS
--------

```
        dashline [-i input] [-o output] [-t ~template~] [column]
```

DESCRIPTION
-----------

`dashline` prints the dashed line that separates the header and data portions
of a starbase data table.
 
OPTIONS
-------

All of the options of the [column](column.html) program are also availabe with `dashline.`
`dashline` is exactally like running "`column` -hd".

{% include colstd-opts.md %}

EXAMPLES
--------

```
    john@panic : dashline < tab
    -       -       -
```

{% include column-seealso.md %}

