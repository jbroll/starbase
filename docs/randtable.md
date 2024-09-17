
NAME
====

`randtable`      - pick random rows from an starbase data table .

SYNOPSYS
========

```
    `randtable` [-i *ifile*] [-o *ofile*] [n=*nlines*]
```

DESCRIPTION
===========

`randtable` outputs n random rows from its input table (or tables).  If the
optional parameter `n` is not given `rand` outputs all of the input rows in a
random order.


OPTIONS
=======

  * -i *file* Read input from *file* instead of the standard input.
  * -o *file* Write output to *file* instead of the standard output.

SEE ALSO
========

{% include starbase-seealso.md %}
