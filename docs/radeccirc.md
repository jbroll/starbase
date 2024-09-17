
NAME
====

`radeccirc` - Search an starbase data table catalog for targets in an ra dec
             specified circle.  radeccirc file ra dec radius

SYNOPSYS
========

```
        `radecbox` *file* *ra* *dec* *radius*
```

PARAMETERS
==========

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

  * radius

                The radius of the circle to search in degrees.

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
========

Search a for all the stars withing a 3 arc minute circle radius at 2:30:03.5
hours RA and 34:45:00 deg Dec.  If the catalog is indexes the search will be
done using the fast index program [search.](search..html)

Then search at the same point with a 3 arc minute circle diameter circle.

```
        john@@panic: @index -mb -h catalog.tab RA
        john@@panic: @index -mi -h catalog.tab Dec
        john@@panic: radeccirc catalog.tab ra=2:30:03.5 dec=34:45:00 radius=0:03:00

        john@@panic: radeccirc catalog.tab ra=2:30:03.5 dec=34:45:00 width=0:03:00
```

SEE ALSO
========


- [radecbox](radecbox.html) - Search an starbase data table catalog for targets
               in an ra dec specified box.


{% include starbase-seealso.md %}
