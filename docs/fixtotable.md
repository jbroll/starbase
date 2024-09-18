
### fixtotable - convert a file of fix format columns to a starbase data table.

SYNOPSYS
--------

```
        `fixtotable` [-i *ifile*] [-o *ofile*] *column*[:*n*-[*m*]] [*column*[:*n*[-*m*]]] ...
```

DESCRIPTION
-----------

`fixtotable` converts fixed format files to starbase data tables.  The output
table contains the columns named on the command line.  The contents of each
column are the range of `character` columns indicated with each column name.

OPTIONS
-------


- -i *file* Read input from *file* instead of the standard input.
- -o *file* Write output to *file* instead of the standard output.
- -skip *n* Skip *n* lines before beginning to read data rows.
- -comment *char* Skip lines with leading comment characters.
- -copy Copy lines that are skipped with the -skip or -comment options
         to the new starbase header.


EXAMPLES
--------

Convert a fixed format file to a table with the following column definitions:
Object name from column 1-25, RA from column 27-39, Dec from column 40-52 and 
a trailing comment from column 55 to the end of line.

```
        john@panic: fixtotable Object:1-25 RA:27-39 Dec:40-52 Comment:55-
```


SEE ALSO
--------

- [fldtotable](fldtotable.html) - convert a file of space delimited fields to a starbase data table.


{% include starbase-seealso.md %}
