

check - check for column errors in a starbase database.

SYNOPSYS
--------

```
    `check` [-i *input*] [-o *output*] [-v]
```

DESCRIPTION
-----------

`check` reads in a table file and checks that the number of columns in each
row is correct.  `check` exits with a non-zero status at the first error
it detects.

OPTIONS
-------

- -v Verbose listing of any errors found to stdandard error


EXAMPLES
--------

Here is an ok table.  Nothing is printed.
```
        john@panic : check < tab
```

Here is a table with a bad row:
```
    john@panic : check -v -i tab
    check : table ? row 1 has 4 columns (3 in header)
```

{% include column-seealso.md %}
