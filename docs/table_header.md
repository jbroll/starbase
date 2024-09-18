

###  table_header  - read the table header from a file.

SYNOPSIS
--------
 

```
#include <../tablelib/table.h>

 TableHead table_header(File file, int mode);
 

```
PARAMETERS
----------
  * `"File` file" - file to read header from.
  * `"int` mode" - parse mode.

DESCRIPTION
-----------
 table_header reads in a header from file and allocates the header data
 structures. The mode parameter indicates how the table rows will be
 treated in later operations. TAB_PARSE will cause table_row() to
 trim extra spaces and record width and numeric precision of rows
 as they are read. TAB_JUSTIFY will cause table_put() to output the
 rows with column justifications.
 
 

- mode == 0
 
 the rows of a table will pass through a program calling
 table_row() and table_put() without modification. This is the
 behavoir of column with no argumants.
 
- mode == TAB_PARSE
 
 the rows of a table will pass through a program calling
 table_row() and table_put() with all extra spaces trimmed.
 This is the behavoir ot trim with no arguments.
 
- mode == TAB_PARSE | TAB_JUSTIFY
 
 the rows of a table will pass through a program calling
 table_row() and table_put() with default justifications. This
 is the behavoir of justify with no arguments.


RETURNS
-------
 `table_header` returns a pointer to a table header data structure.
SEE ALSO
--------
[table_colval](table_colval.html)
 ,
[table_colvals](table_colvals.html)
 ,
[table_colvali](table_colvali.html)
 ,
[table_colvald](table_colvald.html)
 ,
[table_rowloc](table_rowloc.html)
 ,
[table_parsline](table_parsline.html)
 ,
[table_colpad](table_colpad.html)
 ,
[table_coladd](table_coladd.html)
 ,
[table_colarg](table_colarg.html)
 ,
[table_colnum](table_colnum.html)
 ,
[table_colnam](table_colnam.html)
 ,
[table_hdrfree](table_hdrfree.html)
 ,
[table_hdrnth](table_hdrnth.html)
 ,
[table_rowfree](table_rowfree.html)
 ,
[table_rowput](table_rowput.html)
 ,
[table_hdrput](table_hdrput.html)
 ,
[table_rowget](table_rowget.html)
 ,
[table_rowtrim](table_rowtrim.html)
 ,
[table_hdrget](table_hdrget.html)
 ,
[table_hdrgetn](table_hdrgetn.html)
 ,
[table_hdrgeti](table_hdrgeti.html)
 ,
[table_hdrgetd](table_hdrgetd.html)
 ,
[table_hdrgets](table_hdrgets.html)
 ,
[table_hdrfind](table_hdrfind.html)
 ,
[table_extract](table_extract.html)
 ,
[table_load](table_load.html)
 ,
[table_loadva](table_loadva.html)
 ,
[table_mode](table_mode.html)
 ,
[table_ncol](table_ncol.html)
 ,
[table_ofs](table_ofs.html)
 ,
[table_ors](table_ors.html)
 
