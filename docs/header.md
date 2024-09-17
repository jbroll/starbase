

`header` - output the header of a starbase data table

SYNOPSYS
--------

```
        header  [-i input] [-o output] [-t ~template~] [column]
```

DESCRIPTION
-----------

`header` prints the header portion of a starbase data table.
 

OPTIONS
-------

All of the options of the [column](column.html) program are also availabe with header.
`header` is exactally like running "`column` -h".

{% include colstd-opts.md %}

EXAMPLES
--------

```
    john@panic : header < tab
    Table 1

    ObjectNum   1
    Sample      2

    X       Y       Z
    -       -       -
```


SEE ALSO
--------

{% include column-seealso.md %}
