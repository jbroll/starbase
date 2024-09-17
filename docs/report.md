
NAME
====

report - create a report from a table.

SYNOPSYS
========

```
    `report` *template* < *table*
```

DESCRIPTION
===========

`report` reads the template file and makes substitutions in it for each 
row in the input table.  The template report is an ASCII file containing the
boiler plate of the report and special replacement fields that are replaced
with the values from the input table.

```
    <column name>       Replace the text with the value of *column* .  
    <!command!>         Replace the text with the output of *command* .
    <`#filename##>`        Change the output file name to *filename* .    
```




SEE ALSO
========

{% include starbase-seealso.md %}
