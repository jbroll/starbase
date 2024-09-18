

###  table_rowtrim  - trim blanks out of a row structure.

SYNOPSIS
--------
 

```
#include <../tablelib/table.h>

 void table_rowtrim(TableRow row, int *width, int *precision, int ncol, int *justify,
 int *selection, int nsel);
 

```
PARAMETERS
----------
  * `"TableRow` row" - the row to trim.
  * `"int` *width" - width of each column.
  * `"int` *precision" - precision of each column.
  * `"int` ncol" - number of entries in width and precision
  * `"int` *justify" - justification options for each col
  * `"int` *selection" - selection of columns
  * `"int` nsel" - number of selections

DESCRIPTION
-----------
 Blank spaces in the values of the row are trimmed out inplace.
 The widths and numeric precision of the values in the row are
 recorded in the width and precision arrays. `table_trim` is a
 low level routine invoked by `table_row` on each row of the
 table read when the table header mode was `TABLE_PARSE.`

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
[table_header](table_header.html)
 ,
[table_rowput](table_rowput.html)
 ,
[table_hdrput](table_hdrput.html)
 ,
[table_rowget](table_rowget.html)
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
 
