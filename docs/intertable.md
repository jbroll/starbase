

intertable - compute the intersection of two starbase data table.

SYNOPSYS
--------

```
        `intertable` *table1* *table2* [*columns*] [-o *ofile*]
```

DESCRIPTION
-----------

`intertable` computes the intersection of two tables over a set of columns.  ie
only rows that are equal over the selected columns will appear in the output
table.  There is no way to specify different column names for each of the two
tables.

EXAMPLES
--------

Intersection of a table with itself is itself:

```
        john@panic: intertable tab tab
        X       Y       Z
        -       -       -
        1       3       4
        8       9       0
```

Make a shorter table called gab.  It has only one column.  The intersetion of
this tiny table and our original short table over the X column is one row
where X == X.

```
        john@panic : cat gab
        X
        -
        1
        5

        john@panic : intertable tab gab X
        X       Y       Z
        -       -       -
        1       3       4
```

SEE ALSO
--------

- [diffrtable](diffrtable.html) - compute the difference of two starbase data tables.
- [uniontable](uniontable.html) - compute the union of two starbase data tables.



- [sorttable](sorttable.html)   - sort a starbase data table on specified columns.


{% include starbase-seealso.md %}

