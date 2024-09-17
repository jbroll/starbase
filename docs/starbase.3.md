
Starbase C Interface
--------------------


- [Alphabetical listing]( starbase.3.ABC.html)


Header Routines
---------------

- [table_header]( table_header.html) - read the table header from a file.
- [table_hdrput]( table_hdrput.html) - output a table header.
- [table_hdrfree]( table_hdrfree.html) - release memory of the table header structures when done.



- [table_hdrnth]( table_hdrnth.html) - returns the nth header keyword.
- [table_hdrval]( table_hdrval.html) - access a header value.
- [table_hdrvald]( table_hdrvald.html) - return the header value as a double.
- [table_hdrvali]( table_hdrvali.html) - return the header value an integer.
- [table_hdrvaln]( table_hdrvaln.html) - return the number of values in this header keyword.
- [table_hdrvals]( table_hdrvals.html) - return a copy of the column value made with with
- [table_hdrfind]( table_hdrfind.html) - return the row structure that holds the header value



- [table_mode]( table_mode.html) - set the table i/o mode.
- [table_ofs]( table_ofs.html) - change the output field separator.
- [table_ors]( table_ors.html) - change the output record separator.


Column Routines
---------------

- [table_coladd]( table_coladd.html) - add a column to the table.
- [table_colarg]( table_colarg.html) - parse a table program column argument.
- [table_colnam]( table_colnam.html) - returns the name of the column number.
- [table_colnum]( table_colnum.html) - look up a column in a table header.



- [table_colval]( table_colval.html) - get the value from the column.
- [table_colvald]( table_colvald.html) - get the value from the column.
- [table_colvali]( table_colvali.html) - get the value from the column.
- [table_colvals]( table_colvals.html) - get the value from the column.


Row Routines
------------

- [table_rowfree]( table_rowfree.html) - release memory of the row structures when done.
- [table_rowget]( table_rowget.html) - read the next row from a table file.
- [table_rowput]( table_rowput.html) - output a table row.
- [table_ncol]( table_ncol.html) - return the number of column in a table row structure.



Fast Searching
--------------

- [table_search]( table_search.html) - set up to search a table using a fast access method
- [table_srange]( table_srange.html) - search a table for a range using a fast access method
- [table_svalue]( table_svalue.html) - search a table for a value using a fast access method


Loading data into an array of user data structures
--------------------------------------------------

- [table_load]( table_load.html) - load a table directly into a data structure.
- [table_loadva]( table_loadva.html) - load a table directly into a data structure from varargs table


Low Level Routines
------------------

- [table_parsline]( table_parsline.html) - parse a line from a table file.
- [table_rowtrim]( table_rowtrim.html) - trim blanks out of a row structure.
- [table_colpad]( table_colpad.html) - pad column output for justification.
- [table_rowloc]( table_rowloc.html) - low level table row descriptor allocation.


Other
-----

- [table_extract]( table_extract.html) - extract a table from a multi table file.


SEE ALSO
--------

{% include starbase-seealso.md %}
