
###     `histtable` - compute the histogram of a column in a table.

SYNOPSYS
--------

```
`histtable` [-min *min*] [-max *max*] [-n *bins*] *column*
```

DESCRIPTION
-----------

`histtable` computes the histogram of values for *column*.  The output is
a table containing the columns "left", "right", "center" and "count".
These columns defind the left, right, center and count in each histogram
bin.


EXAMPLE
-------

```
    john@panic : < foo ./histtable X
    left    center  right   count
    ------  -----   ------  -----
    0.5     1       1.5     5
    1.5     2       2.5     3
    2.5     3       3.5     2
```

SEE ALSO
--------

 It is also possible to compute a histogram of non-numeric values with
 the "-C" option to [sorttable](sorttable.html) .

{% include starbase-seealso.md %}


