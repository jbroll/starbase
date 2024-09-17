

fldtotable - convert a whitespace delimited file of columns to an starbase data table.

```
        `fldtotable` [-i *ifile*] [-o *ofile*] *column*[:*n*[-*m*]] ...
```

DESCRIPTION
-----------

 `fldtotable` reads an ASCII whitespace (space or tab) delimited file of
 column data selecting the specified fields, adds a header and converts 
 the column delimiters to tabs.

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

Convert a file of space delimited column to a table.  Object is in column 1
RA is in column 2 Dec is in column 3 and a trailing comment is in column 6 to the 
end of line.

Note that the column numbers default to 1, 2 and 3 for Object, RA and Dec but the
column selection for Comment is given.

```
        john@panic: fldtotable Object RA Dec Comment:6-
```

If the RA and Dec are given in space delimited sexagesimal notation ("hh mm ss.sss") 
then this command might have been used for the same file:

```
        john@panic: fldtotable Object RA:2-4 Dec:5-7 Comment:10-
```

SEE ALSO
--------

- [fixtotable](fixtotable.html) - convert a file of fix format columns to a starbase data table.


{% include starbase-seealso.md %}

