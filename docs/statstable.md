
### `statstable` - compute basic stats about the columns of a table.

SYNOPSYS
--------

```
    `statstable` < *table*

    `meantable`  < *table*
    `medtable`   < *table*
    `mintable`   < *table*
    `maxtable`   < *table*
    `rmstable`   < *table*
    `stdtable`   < *table*
    `sumtable`   < *table*
    `tottable`   < *table*
```

DESCRIPTION
-----------

`statstable` read the input file from the standard input and outputs a 
table of basic statistics on the standard output.  It computes the 
sum, min, max, mean, median, standard devaition and range of a column.

The output is a table of computed values one column for each column in 
the input table and an extra column named "Stat" to identify the 
statistic in each row prepended to the table.

Thses short scripts call statstable to compute a single
statistic.


- meantable    - compute the mean of each column of a table.
- mintable     - compute the max of each column of a table.
- maxtable     - compute the min of each column of a table.
- medtable     - compute the median of each column of a table.
- rngtable     - compute the range of each column of a table.
- rmstable     - compute the rms of each column of a table.
- stdtable     - compute the standard deviation of each column of a table.
- sumtable     - compute the sum of each column of a table.
- tottable     - compute the total of each column of a table.


EXAMPLES
--------

```
    john@panic: statstable < data.tab
    Stat    X               Y               Z        
    ------  ---------       ---------       ---------
    Sum     12.000000       12.000000       22.000000
    Mean     3.000000        3.000000        5.500000
    Min      1.000000        0.000000        2.000000
    Max      5.000000        6.000000        9.000000
    Median   3.000000        3.000000        5.500000
    RMS      2.309401        2.581989        3.511885
    Stdev    2.309401        2.581989        3.511885
    Range    4.000000        6.000000        7.000000
```

SEE ALSO
--------

{% include starbase-seealso.md %}
