

 table_hdrval  - access a header value.

SYNOPSIS
--------
 

```
#include <../tablelib/table.h>

 char *table_hdrval(TableHead table, char *name, int i);
 

```
PARAMETERS
----------
  * `"TableHead` table" - table header.
  * `"char` *name" - name of header value.
  * `"int` i" - array index of value.

DESCRIPTION
-----------
 Access the header value `name` as an integer, double or string. If the value
 is an array the i'th value in the array is returned.
 
 `table_hdrval` returns a pointer to the actual header buffer. It should not
 be modified in any way.

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
[table_hdrvaln](table_hdrvaln.html)
 ,
[table_hdrvali](table_hdrvali.html)
 ,
[table_hdrvald](table_hdrvald.html)
 ,
[table_hdrvals](table_hdrvals.html)
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
 
