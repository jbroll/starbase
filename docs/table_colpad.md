

###  table_colpad  - pad column output for justification.

SYNOPSIS
--------
 

```
#include <../tablelib/table.h>

 void table_colpad(File file, char *string, int mwidth, int owidth, int precision,
 int type);
 

```
PARAMETERS
----------
  * `"File` file" - output file.
  * `"char` *string" - string to pad.
  * `"int` mwidth" - width of output in characters.
  * `"int` owidth" - width of output in characters.
  * `"int` precision" - precision of numeric output.
  * `"int` type" - justification padding type.

DESCRIPTION
-----------
 Output string to the file in a column of width characters. If precision
 is
 non-zero, the output is right justified as a number with the "." precision
 characters from the right edge of the column. Otherwise string is left
 justified. The type may override the default justification with one of the
 following \`defined` values:
 

- TAB_DEFAULT
- TAB_LEFT
- TAB_RIGHT
- TAB_CENTER
- TAB_SKIP
 
 This is a low level routine to output one table column at a time with the
 proper justification. User programs should call `table_put.`


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
[table_loadva](table_loadva.html)
 ,
[table_mode](table_mode.html)
 ,
[table_ncol](table_ncol.html)
 ,
[table_ofs](table_ofs.html)
 ,
[table_ors](table_ors.html)
 
