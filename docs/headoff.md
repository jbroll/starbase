

`headoff` - remove the header of a starbase data table and output the data part.

SYNOPSYS
--------

```
        `headoff`  [-i *input*] [-o *output*] [-t *template*] [*column*]
```

DESCRIPTION
-----------

`headoff` prints the data portion of a starbase data table.
 

OPTIONS
-------

All of the options of the [column](column.html) program are also availabe with header.
`header` is exactally like running "@column -b".

{% include colstd-opts.md %}

EXAMPLES
--------

```
        john@panic : headoff < tab
        1       3       4
        8       9       0
```


SEE ALSO
--------

{% include column-seealso.md %}
