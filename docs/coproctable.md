
NAME
====

`coproctable` - run a coprocess on the rows of a table.


SYNOPSYS
========

```
        coproctable [-h] pipeline [COL[=COL] COL[=COL]]
```

DESCRIPTION
===========

`coprocess` table is `ALPHA` software.

`coprocess` table allows some columns of a table to be passed to a coprocess
and read back from the coprocess while other column are simply coppied from
standard input to standard output.

EXAMPLES
========

Here the X, Y coordinate columns of a table are passed to an iraf coprocess
to be converted from X, Y to RA and Dec using the HST t_metric task.

```
    #!/data/mmti/bin/ksh
    #
            . $ENVIRONMENT
     
            export PDEFAULTS
            PDEFAULTS="
    image,s,a,,,,Image that x/y positions are from
    reference,s,h,,,,HST reference hhh image
    xcol,s,h,,,,X column name
    ycol,s,h,,,,Y column name
    mode,s,h,ql,,,
    "
            parameters image reference xcol ycol
     
      ( print "# hstxytoskytable"
        print "#"
        print "Reference $reference"
        print ""
        cat )                                               \
      | column -a RA2000 Dec2000 RA1950 Dec1950             \
      | coproctable -w -r 'headoff '$xcol' '$ycol'          \
         | wcsctran STDIN STDOUT image='$image'             \
                inwcs=log outwcs=phy                        \
         | t_metric '$reference'                            \
                x="STDIN 1" y=2                             \
         | deletecomment                                    \
         | fldtotable RA:9 Dec:10                           \
         | headoff RA Dec           \
         | skyctran STDIN STDOUT fk5 fk4                    \
                transform=no lngcol=1 latcol=2              \
         | deletecomment                                    \
         | fldtotable RA2000 Dec2000 RA1950 Dec1950'        \
       RA2000 Dec2000 RA1950 Dec1950
```


SEE ALSO
========

{% include starbase-seealso.md %}

