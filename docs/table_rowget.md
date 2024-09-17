

NAME
====
 table_rowget  - read the next row from a table file.

SYNOPSIS
========
 

```
#include <../tablelib/table.h>

 TableRow table_rowget(File file, TableHead table, TableRow row, int *justify,
 int *select, int nsel);
 

```
PARAMETERS
==========
  * `"File` file" - file to read from.
  * `"TableHead` table" - table header.
  * `"TableRow` row" - row structure.
  * `"int` *justify" - Optional justification flags for each column.
  * `"int` *select" - Optional selection vector for each column.
  * `"int` nsel" - Not Documented.

DESCRIPTION
===========
 A row is read in from file. If the row argumant is NULL a new
 row structure is allocated for the row.
 
 Returns a TableRow.

SEE ALSO
========
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
 
