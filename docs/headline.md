
NAME
====

`headline` - output the headline of a starbase data table.

SYNOPSYS
========

```
        headline [-i input] [-o output] [-t ~template~] [column]
```

DESCRIPTION
===========

`headline` prints the header line that names the columns of a starbase data table.
 
OPTIONS
=======

All of the options of the [column](column.html) program are also availabe with headline.
`headline` is exactally like running "`column` -hl".

{% include colstd-opts.md %}

EXAMPLES
========

```
    john@panic : headline < tab
    X       Y       Z
```

{% include column-seealso.md %}

