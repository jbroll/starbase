
Starbase Data Table File Format
-------------------------------

DESCRIPTION
-----------

`starbase` uses an ascii file format used to store relational data base tables
with a few magic characters deliniating columns and tables.  When the table
file format is combined with the starbase filter programs and the i/o library
a convient data base with editable ascii files is the result.

  *     The tab character delimits columns in a row.
  *     The newline character delimits rows in a table.
  *     The linfeed character delimits tables in a file.

A table contains three parts.

  1.  An optional keyword/ value header.
  2.  Column definition/ width lines.
  3.  Data columns.

The optional keyword/ value portion of the header consists of any number
of free text line containing comments and keyword/ value lines.  If a table
contains an optional keyword/ value portion the first line of the header
names the table.  This name may be used with the `-t` option of the [column](column.html)
program to extract and process this table by name.

A keyword value line consists of

```
        keyword [tab] value
```
A keyword may contain may have an array of values and is written
```
        keyword [tab] value [tab] value ...
```

Header lines which contain no tabs or whose initial keyword portions are not
valid keyword identifiers are comments.  Header values can be accessed by name
in expressions passed to the `starbase` programs.

Example
-------
```
    Table1

    This table is named Table1

    This is a text comment in the header of a table.  This portion may ramble
    on and on but should not contain any line with ONLY dash (-) and tab
    characters.  The dash line is the indication that the data table is about
    to begin.

    Key 1
    Keys        1       2       3

    There are two header values above.  Key is a scalar value, Keys is an 
    array.

    RA  Dec
    --  ---
    0:0:0       0:0:0
    ^L
    Table2

    This is another table it is in the same file as Table1 and is delimited
    from the first by a ^L (form feed) character.  A tables name is given by
    the first  line in the table.  This table is named Table2.


    RA  Dec     Comment
    --  ---     -------
    1:00:00     2:00:00 This as an RA DEC pair in sexagesimal format.
```
