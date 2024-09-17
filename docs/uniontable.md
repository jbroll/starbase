
NAME
====

uniontable - compute the union of starbase data tables.

SYNOPSYS
========

```
        `uniontable` [-o *ofile] *table1* *table2* [*tablen~] ...
```

DESCRIPTION
===========

`uniontable` computes the union of two or more tables.  It concatenates the
rows of all the tables into one larger table.  All the rows and all the
columns of all the tables in the union appear in the output table.

EXAMPLES
========

Here is the union of three tables showing how columns are aligned bwtween 
tables and rows are concatenated.

```
        john@panic : cat foo
        X       Name    Y
        -       ----    -
        4.5     Star1   5000
        435     Star2_bk        4.3
        2300    Star3   2.04

        john@panic : cat gab
        X
        -
        1
        5

        john@panic : cat tab
        X       Y       Z
        -       -       -
        1       3       4
        8       9       0

        john@panic : diffrtable gab tab
        X
        -
        5

        john@panic : uniontable tab gab foo
        X       Y       Z       Name
        -       -       -       ----
        1       3       4
        8       9       0
        1
        5
        4.5     5000            Star1
        435     4.3             Star2_bk
        2300    2.04            Star3
```

SEE ALSO
========

- [diffrtable](diffrtable.html) - compute the difference of two starbase data tables.
- [intertable](intertable.html) - compute the intersection of two starbase data tables.



- [sorttable](sorttable.html)   - sort a starbase data table on specified columns.


{% include starbase-seealso.md %}

