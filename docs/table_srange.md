

NAME
====
 table_srange  - search a table for a range using a fast access method

SYNOPSIS
========
 

```
#include <../tablelib/table.h>

 void *table_srange(TableSearch search, char *value1, char *value2, void *(*func)(void),
 void *data, TableRow R);
 

```
PARAMETERS
==========
  * `"TableSearch` search" - Search descriptor returned from [table_search](table_search.html)
  * `"char` *value1" - Lower limit of desired range.
  * `"char` *value2" - Upper limit of desired range.
  * `"void` *(*func)(void)" - Function to call with each selected row.
  * `"void` *data" - Closure data for func()
  * `"TableRow` R" - Row buffer. If a row buffer is not passed (NULL)
      a new row will be allocated for each row selected

DESCRIPTION
===========
 search a table for a range using a fast access method

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
[table_search](table_search.html)
 ,
[table_svalue](table_svalue.html)
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
[table_hdrval](table_hdrval.html)
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
 
