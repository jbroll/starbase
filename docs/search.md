

`search` - search a table using a fast access method.


SYNOPSIS
--------

```
    `search` *table* [options] < *keytable*
```

DESCRIPTION
-----------

The `search` program may be used in combination with the [index](index.html) program to find
data rows in large tables very quickly.  First the table must be indexed.
Index may sort the data table or create an auxillery index file depending on
how it is run.  Search will use the created index to find rows in the table
that match the given options.

OPTIONS
-------

Program Options:
----------------

- -i *ifile*   Read input from file instead of the standard input.
- -o *ofile*   Write output to file instead of the standard output.
- -h           Do not output a table header.
- -j           Join the keytable rows with the matching rows in the searched
                table.
- -s           List the set number for matching rows in the searched table.
- -a           The set number option will usually force all the rows in each
                set to be unique, even when a row might normally belong to several
                key row results.  The -a option allows the output of all matches
                in each set.
- -c           Verbose display of dots .... as the program runs.
- -X           Tells search to "cross" match the table against itself.  No 
                input on stdin is expected.  The -s option is set and the 
                search will skip input rows that have been previously matched.
                If the -a option is also given the search proceeds but the
                optimization is defeated.


Search Specification Options:
-----------------------------

        C2 is a column from the table to be searched.  This is generally the
        indexed table.  C1 is a column from the table of input keys.  These
        are the values to be searched for.  The examples are given along with
        the specification of each type of search option.


- <C2>[:<C1>]


        Search for the exact value (string of number) given in the 
        key column.

        Search table.tab for rows with column XPixel equal to the column XPixel
        in key.tab.
```
        john@panic: search table.tab XPixel < key.tab
```
        Search table.tab for rows with column XPixel equal to the column
        XPosition in key.tab.
```
        john@panic: search table.tab XPixel:XPosition < key.tab
```
        Listing several column will cause an exact match for each column.
        Search table.tab for rows with column XPixel equal to the column
        XPosition in key.tab and matching OBJECT columns.
```
        john@panic: search table.tab XPixel:XPosition OBJECT < key.tab
```


- -C[123] <C2>[:<C1>] [<C2>[:<C1>] [<C2>[:<C1>]]]


    Search for a range of values in a one, two or three dimensional Cartesian
    coordinate system.  The dimension is specified by a 1, 2 or 3 immedietly
    following the 'C' option character.  The units of all columns specifying
    the coordinates must match.  If The column name in the key table (C1) is
    missing from the coordinate column specification the columns in each table
    are assumed to have the same name.

       * -C1 Searches a range around a point on a line.
       * -C2 Searches a circle around a point on a plane.
       * -C3 Searches a sphere around a point in 3 space.

    This option uses the input key pair of column coordinate and returns the
    rows in the searched table that are within the specified range (distance)
    of the input point.

    Search for all the targets within 4 pixels of X,Y from the key table.

```
    john@panic: search object.tab -C2 X:XPixel Y:YPixel 4 < key.tab
```

    Search in a cartesian coordinate system sphere of 1500 units raduis.
- 
    john@panic: search XYZ.tab -C3 X Y Z 1500 < XYZ.key

    X       Y       Z       Data    Mag     RA              Dec
    ------- ------- ------- ----    ---     ---------       ---------
    2875.33 3921.35 4284.41 Obj7    22       0:43:37.32     41.245383
    3368.52 3523.19 4311.07 Obj5    21       0:43:15.42     41.190227
```


- -I
 
   Invert the search, output all rows that do not match.




- -S[12][units] <C2>[:<C1>] [<C2>[:<C1>]] <range>


    Search for a range in a one or two dimensional spherical coordinate
    system.  The dimension is specified by a 1 or 2 immedietly following
    the 'S' option character.  A two dimensional coordinate system is the
    default.  The units of each column in a two dimensional system may be
    optionally indicated by three following characters.  A 'h' indicated
    hours, a 'd' indicated degrees and a 'r' indicated radians.  The units
    indicated are in order, RA column, Dec column and the range value
    units.  The default units are 'hdd'. The units specification for a one
    dimensional coordinate system only determines where axis wraps over to
    zero, no units conversion is done between the column and range
    values.  If The column name in the key table (C1) is missing from the
    coordinate column specification the columns in each table are assumed
    to have the same name.  In the examples below the range value is given 
    in decimal degrees,  it may also be convienient to use sexagesimal
    notataion to specify ranges (0:00:30 for 30 seconds).
        

    Search in a spherical coordinate system box of 1 degree square.  The
    units for a one dimensional spherical coordinate system should be explicitly
    set.  The default units for the first axis in a spherical search is hours
    (RA).  #Search uses the units of a spherical axis to compute the axis wrap
    point.
 
```
    john@panic: search coords.tab -S1d RA 0.1 -S1d Dec 0.1 < range.key

    X       Y       Z       Data    Mag     RA              Dec
    ------- ------- ------- ----    ---     ---------       ---------
    3476.18 3935.77 3500.07 Obj8    18      10.794499       41.247551
    3992.21 3500.07 4854.75 Obj4    18      10.699151       41.187092
```
 
    Search in a spherical coordinate system circle of 0.1 degree raduis.  Here
    the units of RA are in hours (the default) while the units of Dec are
    degrees (the default).  The spherical coordinate system option knows how
    to combine columns of a spherical system with different units.

```
    john@panic: search coords.tab -S2 RA Dec 0.1 < range.key

    X       Y       Z       Data    Mag     RA              Dec
    ------- ------- ------- ----    ---     ---------       ---------
    3476.18 3935.77 3500.07 Obj8    18       0:43:10.68     41.247551
    3992.21 3500.07 4854.75 Obj4    18       0:42:47.80     41.187092
    4365.01 4509.67 5888.78 ObjC    18       0:42:31.26     41.327306
```
    Search in a spherical coordinate system circle of 0.16 degree raduis.
    This command sets the spherical coordinates system units to degrees for
    each axis and the search radius.  The RA, Dec and the search range will be
    in degrees.

```
    john@panic: search coords.tab -S2ddd RA Dec 0.16 < range.key

    X       Y       Z       Data    Mag     RA              Dec
    ------- ------- ------- ----    ---     ---------       ---------
    2875.33 3921.35 4284.41 Obj7    22      10.905480       41.245383
    3368.52 3523.19 4311.07 Obj5    21      10.814261       41.190227
    3390.06 3035.48 3476.18 Obj2    18      10.810160       41.122493
```


- -A[12][units] <C2>[:<C1>] [<C2>[:<C1>]] <inner> <outer>


    Search for an anular range in a one or two dimensional spherical coordinate
    system.   This is identical to the -S option above but allows an additional 
    range to specify an inner radius of the annulus.


- -V <C2> <value>      Search for an exact value (string or number).



- -R <C2> <lower> <upper>      Search for a range specified by upper and lower limit
        values on the command line.


    Search a spherical coordinate system circle of 0.16 degrees raduis, and
    limit the output with an absolute range of magnitude.

```
    john@panic: search coords.tab -S2ddd RA Dec 0.16 -R Mag 18 20 < range.key

    X       Y       Z       Data    Mag     RA              Dec
    ------- ------- ------- ----    ---     ---------       ---------
    3390.06 3035.48 3476.18 Obj2    18      10.810160       41.122493
    3476.18 3935.77 3500.07 Obj8    18      10.794499       41.247551
    3992.21 3500.07 4854.75 Obj4    18      10.699151       41.187092
```



- -Z <C2> <lower C1> <upper C1>   Search for a range of values in the search
                                column where the range is determined by lower
                                and upper limits in the input key table.
                                determined This option is stuck here to allows
                                access to the "column range" search functions
                                but it will be integrated into the -R option
                                is a future release.


SEE ALSO
--------

  * [index](index.html)      - index a starbase data table for fast access using the search command.
{% include starbase-seealso.md %}
