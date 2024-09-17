
NAME
====

jointable - join two starbase data tables on matching key columns.

SYNOPSYS
========

```
        `jointable` [-a`#n]` [-v`#n]` [-j *column*] [-1 *column* -2 *column*] *table1* *table2*
```

OPTIONS
=======


- -j column    join on the column form both files.
- -1 column    join on the column from file 1
- -2 column    join on the column from file 2
- -a n in addition to the normal output, produce  a
        line  for  each  unpairable  line  in file n,
        where n is 1 or 2.
- -v n print only the unpairable lines from file n
- -n   Compare  according  to  arithmetic  value.  an  initial
        numeric  string  consisting of optional white space, an
        optional - sign, and zero or  more  digits,  optionally
        followed by a decimal point and zero or more digits.
- -h   Like -n except sexagesimal values are allowed (HH:MM:SSS.SS etc).
- -r   Reverse the result of comparison, so that  lines  with
        greater key values appear earlier in the output instead
        of later.
- -D   print out the intended arvg just before calling join.


DESCRIPTION
===========

`jointable` joins two sorted starbase data tables on a key column in each
table.  Two rows are joined when thier key column value is equil.  The tables
must be sorted in the same order.

{% include join-rename.md %}

EXAMPLES
========

  Join two tables with object name columns.  The tables must be sorted first.

```
        john@panic: sorttable Object < catalog.tab > tmp
        john@panic: mv tmp catalog.tab

        john@panic: sorttable Object < object.tab > tmp
        john@panic: mv tmp object.tab

        john@panic: jointable -j Object catalog.tab object.tab > joined.tab
```

  Join the same two tables on Object name from the catalog table and Alias from the
  object table.

```
        john@panic: sorttable Alias < object.tab > tmp
        john@panic: mv tmp object.tab

        john@panic: jointable -j1 Object -j2 Alais catalog.tab object.tab > joined.tab
```

NOTES
=====
   Care must to taken when printing the unpairable line.  The column 
   names can become misaligned when using the -a2 switch.

SEE ALSO
========


- [sorttable](sorttable.html)   - sort a starbase data table on specified columns.
- [pastetable](pastetable.html)  - join starbase data tables by row number.
- [cprodtable](cprodtable.html)  - compute the cartesian product of a list of input tables.



- [index](index.html)      - index a starbase data table.
- [search](search.html)     - search a starbase data table using an index.



{% include starbase-seealso.md %}

