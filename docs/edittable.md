

edittable - Fast indexed editing of a starbase data table.

SYNOPSYS
--------
```
edittable.c

        edittable [options] table keycolumn [editcolumns...] [< input]

                keycolumn               - column in table and input to search on.
                keycolumn:keycolumn     - column in table and column in input to search on.
                keycolumn:keycolumn0:keycolumn1
                                        - column in table and range columns to search in input.
                keycolumn=value         - column value in table to search, no input.
                keycolumn=value0:value1 - column range in table to search, no input.

                editcolumn              - column in table to edit from input.
                editcolumn:editcolumn   - column in table to edit from column in input.
                editcolumn=value        - column in table to edit with value, no input.

         Options:
                -x              - verify commands before editing.
                -v              - print # of edits at completion.
                -V              - verify each row edit.
```
