

###  table_loadva  - load a table directly into a data structure from varargs table
 description.

SYNOPSIS
--------
 

```
#include <../tablelib/table.h>

 void *table_loadva(File ifile, TableHead *t, void *hbase, TableRow **rows, void
 *tbase, int size, int *nrecs, ...);
 

```
PARAMETERS
----------
  * `"File` ifile" - File pointer to read table from.
  * `"TableHead` *t" - Table header if already read in
  * `"void` *hbase" - Base pointer for header values.
  * `"TableRow` **rows" - Returned row array.
  * `"void` *tbase" - Base pointer for data structure array.
  * `"int` size" - Size of one data structure.
  * `"int` *nrecs" - Number of records read.
  * `"..."` - Not Documented.

DESCRIPTION
-----------
 `table_loadva` constructs a TblDescriptor from its arguments and calls
 `table_load.`

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
[table_mode](table_mode.html)
 ,
[table_ncol](table_ncol.html)
 ,
[table_ofs](table_ofs.html)
 ,
[table_ors](table_ors.html)
 
