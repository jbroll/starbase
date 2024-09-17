

`cprodtable` -  compute the cartesian product of a list of input tables.

SYNOPSYS
--------

```
        cprodtable [-o ofile] table1 table2 [table3 ...]
```


DESCRIPTION
-----------

The cartesian product of two tables is a table with every row in one table
joined with every row in the other table.  `cprodtable` has the capability of
joining many tables at once.

{% include join-rename.md %}

EXAMPLES
--------

Here is the product of a small table of coordinates with itself:

```
    john@panic : cprodtable tab tab
    X_1     Y_1     Z_1     X_2     Y_2     Z_2
    ---     ---     ---     ---     ---     ---
    1       3       4       1       3       4
    1       3       4       8       9       0
    8       9       0       1       3       4
    8       9       0       8       9       0
```



SEE ALSO
--------


- [jointable](jointable.html)  - join two starbase data tables on a join column.
- [pastetable](pastetable.html) - paste the rows of starbase tables side by side.



- [index](index.html)      - index a starbase data table.
- [search](search.html)     - search a starbase data table using an index.


{% include starbase-seealso.md %}

