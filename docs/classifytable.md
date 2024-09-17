

`classifytable` - group together rows in a table into similar classes.

SYNOPSIS
--------

```
classifytable *table* *search_expression*

DESCRIPTION
-----------

`classifytable` is used to classify or group the rows of a table into sets with
similar column values.  Each row in the output table is labeled with a column
"SetNumber" to indicate the class to which it belongs.  Rows can be classified
by the matching criteria supported in the [search](search.html) program.


EXAMPLES
--------

Place all target within 30 seconds of each other in the same class.

```
        john@panic: classifytable stars.tab -S2 RA Dec 0:00:30
```


DETAILS
-------

~classifytable~ is a shell script which combines search, sort and compute:

#!/bin/sh
#

   [search](search.html) -s $* < $1           \
 | [sorttable](sorttable.html) -u `headline < $1` \
 | [sorttable](sorttable.html) -n SetNumber      \
 | [reseqtable](reseqtable.html)

