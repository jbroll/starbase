

 table_colarg  - parse a table program column argument.

SYNOPSIS
--------
 

```
#include <../tablelib/table.h>

 int table_colarg(TableHead table, char *column, char *flag, char *form[], TableColArg
 *val, int *check);
 

```
PARAMETERS
----------
  * `"TableHead` table" - table to lookup column in.
  * `"char` *column" - column argument.
  * `"char` *flag" - string of allowed flags
  * `"char` *form[]" - array of strings indicating flag types
  * `"TableColArg` *val" - array of returned flag values.
  * `"int` *check" - array of returned flag switched

DESCRIPTION
-----------
 `table_colarg` parses any flags from the end of a column name argument given
 to a table program on the program command line and calls `table_col` to look
 up the column in the table header. `table_arg` may also be used to parse
 column flags without looking up the column number. If table is NULL
 `table_colnum` is not called and the return value is 0.

RETURNS
-------
 `table_colarg` returns the column number of the column or 0 if the column
 is not in
 the table.
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
[table_loadva](table_loadva.html)
 ,
[table_mode](table_mode.html)
 ,
[table_ncol](table_ncol.html)
 ,
[table_ofs](table_ofs.html)
 ,
[table_ors](table_ors.html)
 
