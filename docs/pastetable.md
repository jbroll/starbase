

`pastetable` - join starbase data tables by row number

SYNOPSYS
--------

```
        `pastetable` *table1* *table2* [*tableN*] ...
```

DESCRIPTION
-----------

`pastetable` joins all of the tables on its command line by row number.

{% include join-rename.md %}

EXAMPLES
--------

Paste a small table to itself

```
    john@panic : pastetable tab tab
    X_1     Y_1     Z_1     X_2     Y_2     Z_2
    ---     ---     ---     ---     ---     ---
    1       3       4       1       3       4
    8       9       0       8       9       0
```

SEE ALSO
--------


- [jointable]( jointable.html) - join two starbase data tables on matching key columns.
- [cprodtable]( cprodtable.html) - compute the cartesian product of a list of input tables.



- [index](index.html)      - index a starbase data table.
- [search](search.html)     - search a starbase data table using an index.


{% include starbase-seealso.md %}


