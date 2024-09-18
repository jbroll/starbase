
### resample - resample numeric columns in a table

SYNOPSYS
--------

```
    `resample` *controltable* *keycol*[:*controlkeycol*] *col1* ... [*coln*] [< *input*]
```

DESCRIPTION
-----------

`resampletable` is used to resample numeric columns in a table to a set of
points specified by a column in a control table.  The resampling is done using
linear interpolation.  Both tables must be sorted on the key column before
calling this program. The keycol and col1...coln are written to the output.
The number of rows in the output table is equal to the number of rows in the
control table.  If the control value is outside the range of the key values in
the input file, the output value is linearly extrapolated using the end
values.

OPTIONS
-------

     * *controltable* - The name of the table that will control the resampling

     * *keycol* - The name of the column in the input table that specifies the
       original ordinate values.

     * *controlkeycol* - The name of the column in the control table that is
       used to specify the new ordinate values.  If controlkeycol is not
       specified it is assumed to be the same as keycol.

     * *col* ... - Names of columns in the input table that will be resampled.

EXAMPLE
-------

```
    % cat newxtab
    NEWX
    ----
    0
    3
    6.5
    11
     
    %cat oldxtab
    OLDX    Y1      Y2
    ----    --      --
    2   20      0
    4   40      40
    6   60      60
    8   80      80
    10  100     500

    % resampletable newxtab OLDX:NEWX  Y1 Y2 < oldxtab
    OLDX   OLDX    Y1      Y2
    ----    --      --
    0       0       -40
    3       30      20
    6.5     65      65
    11      110     710
```


SEE ALSO
--------

{% include starbase-seealso.md %}
