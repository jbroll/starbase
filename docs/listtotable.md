

`listtotable` - convert a /rdb compatible list to a table.

SYNOPSYS
--------

```
    `listtotable` [-i *ifile*] [-o *ofile*]
```

DESCRIPTION
-----------

`listtotable` read a table from its standard input and writes a list on its output.

OPTIONS
-------

  * -i *file* Read input from *file* instead of the standard input.
  * -o *file* Write output to *file* instead of the standard output.
  * -l Treat the input as a list even though it doesn't begin with
        an initial newline.
  * -t Print each list record as a separate form feed delimited table.

SEE ALSO
--------


- [tabletolist](tabletolist.html) - convert a table to a /rdb compatible list.


{% include starbase-seealso.md %}
