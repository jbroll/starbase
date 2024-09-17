
NAME
====

validate - validate and fix column values of a starbase data table

SYNOPSYS
========

```
        validate 'statements' < tablefile
```

DESCRIPTION
===========

`validate` executes a list of awk pattern statement pairs on each row of a
starbase data table.  Each statement is separated from the next by a
semi-colon or newline.  `validate` is very useful for checking and fixing
values of columns in a table.

{% include table-inc.md %}

{% include mawk-inc.md %}

OPTIONS
=======

{% include tabstd-opts.md %}

EXAMPLES
========

`
    Check the rows of a table for consistancy.  X must be less than 7 and Y
    must be less than 4.

```
    john@panic : validate 'X > 7 && Y < 4 { print "Warning bad position at line " NR }'
```

    Validate programs can be quite complex and are very powerful.
    [{FastFixes}]({FastFixes}.html) used validate to check and fix the initial versions of the
    FAST database.

'

{% include table-env.md %}

{% include row-seealso.md %}

