
NAME
====

diffrtable - compute the difference of two starbase data table.

SYNOPSYS
========

```
        `diffrtable` *table1* *table2* [*columns*] [-o *ofile*]
```

DESCRIPTION
===========

`diffrtable` computes the difference of two tables over a set of columns.  It
subtracts the rows in table1 which are equal to rows in table2.  There is no
way to specify different column names for each of the two tables.

EXAMPLES
========

Difference of a table with itself is empty:

```
        john@panic: diffrtable tab tab
        X       Y       Z
        -       -       -
```

Make a shorter table called gab.  It has only one column.  The difference of
this tiny table and our original short table over the X column is one row
where X == X.  Order is important the output table only contains rows from 
table1.

```
        john@panic : cat gab
        X
        -
        1
        5

        john@panic : diffrtable tab gab X
        X       Y       Z
        -       -       -
        8       9       0

        john@panic : diffrtable gab tab
        X
        -
        5
```

SEE ALSO
========

- [intertable](intertable.html) - compute the intersection of two starbase data tables.
- [uniontable](uniontable.html) - compute the union of two starbase data tables.



- [sorttable](sorttable.html)   - sort a starbase data table on specified columns.


{% include starbase-seealso.md %}

