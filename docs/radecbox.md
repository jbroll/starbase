

`radecbox` - Search an starbase data table catalog for targets in an ra dec
            specified box.

SYNOPSYS
--------

```
        `radecbox` *file* *ra* *dec* *width*
```

PARAMETERS
----------

  * file

        Input catalog to search, this file must have ra and dec columns.
        If the search parameter not set to "row" the input file must be 
        be a real file (not a pipe) and be numericly indexed on the ra 
        column.  The ra column should be in units
        of hours and the dec column should be in units of degrees.
  * ra

        Right Ascension of the center of the search box in hours.

  * dec
        Declination of the center of the search box in degrees.

  * width

        Width of the search box in degrees.

  * height
        Height of the search box in hours.  This parameter is optional and the 
        height is computed from the width for create a square box on the sky if 
        the height is omitted.

  * rcol

        The column name of the ra column defaults to "ra".

  * dcol

        The column name of the dec column defaults to "dec".

  * search

        The search method to use.  Three search methods are
        implemented.   The default method is "fast"

    * row       The search is done sequentially with a single "row" command.
    * fast      The an initial search command is executed for the range of 
                ras.  The ra column of the input file must be indexed.  The
                results of this search are passed to row and filtered on the
                dec range.  This method is excellent when retrieving a small
                number of rows (<1000) from large datasets.

  * debug

        Control debug output.  The default is debug=0.  When debug=1
        the commands that will be executec are printed out on the 
        standard error instead.  When debug=2 the commands are printed 
        and executed.

EXAMPLES
--------

Search a for all the stars withing a 3 arc minute square box at 2:30:03.5
hours RA and 34:45:00 deg Dec.  If the catalog is indexes the search will
be done using the fast index program [search.](search..html)

```
        john@@panic: [index](index.html) -mb -h catalog.tab RA
        john@@panic: [index](index.html) -mi -h catalog.tab Dec
        john@@panic: radecbox catalog.tab ra=2:30:03.5 dec=34:45:00 width=0:03:00
```

SEE ALSO
--------


- [radeccirc](radeccirc.html) - Search an starbase data table catalog for targets in an
                 ra dec specified circle.


{% include starbase-seealso.md %}
