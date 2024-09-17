
NAME
====

`index` - index a starbase data table for fast access using the [search](search.html) command.

SYNOPSIS
========

```
    `index` -m[sbihr] [-n] [-h] [-r] [-L] *table* *column*
```


DESCRIPTION
===========

 `index` creates a sorted index file (or sorts the table itself) so that 
 the [search](search.html) command may be used to access rows of the table more quickly
 than the sequential access provided by `row.`

OPTIONS
=======


- -m[sbihr]    Search method to index for.  `Index` creates an index for use
                with a particular search method.


                * s     sequential (no op)
                * b     primary key index on column
                * i     secondary key index on column
                * h     hash index
                * r     row index

                Hash and Row indexing are untested and Do NOT work.
                

- -n           sort index in numeric order.  The default is dictionary order.
- -h           sort index in astronomical numeric order.  This option 
                enables the use of RA, Dec values formated in sexagesimal.
- -r           reverse the index ordering (search -r is broken, don't use this).
- -L           record the record locations in the index file.  This option
                allows faster access with index files.  The index file contains
                extra information and will be larger.


EXAMPLES
========


        Example of primary indexing.  Make a numeric primary index of file foo
        on column A.

```
        john@panic: index -mb -n foo.mmethod A
```

        Make three indicies for the file ZYX.tab.  The X column is the primary
        index and the file XYZ.tab will be sorted in X order by the index
        command.  The other two index commands make secondary of "indirect" 
        indexes.  They will create auxillery files that will be used by search.
```

        john@panic: index -mb -n XYZ.tab X
        john@panic: index -mi -n XYZ.tab Y
        john@panic: index -mi -n XYZ.tab Z
```
        Make indexes for RA Dec and Magnitude searching.  The -h option should
        be used for RA, Dec column if they are in HH MM SS.SSS format.

```
        john@panic: index -mb -h coords.tab RA
        john@panic: index -mi -h coords.tab Dec
        john@panic: index -mi -n coords.tab Mag
```
'

SEE ALSO
========

  * [search](search.html)     - quickly search an indexed starbase data table.
{% include starbase-seealso.md %}

