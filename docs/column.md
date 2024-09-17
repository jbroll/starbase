
NAME
====

`column` - project columns from a [starbase](starbase.html) data table

SYNOPSYS
========

```
column  [-i input] [-o output] [options] [column] ...
project [-i input] [-o output] [options] [column] ...
```

DESCRIPTION
===========

`column` is used to create, delete, format and choose columns in a table.

`column` reads a starbase data table from its standard input and prints a 
table with the columns given as arguments on its standard output.  If
a given column does not exist it is created in the output.  If no column
are given as arguments all the columns in the table are output.

`column` and `project` are the same program.

OPTIONS
=======

{% include column-opts.md %}

EXAMPLES
========

`
    Ask for all the column from the standard input
 
```
    john@panic: column <  input
    X       Y
    -       -
    0       1
    0       1
```
    Ask for all the columns from a file specified with the -i switch
    put the output in a file specified with the -o switch.

```
    john@panic: column -i input -o output
```

    Ask for the X column
 
```
    john@panic: column X < input
    X
    -
    0
    0
```

    Number the rows

```
    john@panic: column -n < input
    Number  X       Y
    ------  -       -
    1       0       1
    2       0       1
    3       0       1
```
    Rename a column.
     
```
    john@panic: column X=K < input
    K       Y
    -       -
    0       1
    0       1
    0       1
```
    Output only the header without the rows

```
    john@panic: $ column -h < input
    Value Table

    X1      1
    Y1      1       2       3

    X       Y
    -       -
```
    Output the rows wo/ the header.
 
```
    john@panic: $ column -b < input
    0       1
    1       2
```
    Output the extended header part.
  
```
    john@panic: column -hv < input
    Value Table

    X1      1
    Y1      1       2       3
```
     
    Output the header values as a value table

```
    john@panic: $ column -hV < val $
    Name    Value
    ----    -----
    X1      1
    Y1      1       2       3
```
    Output the headline or the dashline.

```
    john@panic: $ column -hl < input
    X       Y
    john@panic: $ column -hd < input
    -       -
```
    Add a column to the table using the -a all/add option.
     
```
    john@panic: $ column -a Z < input
    Z       X       Y
    -       -       -
            0       1
            0       1
            0       1
```

    Justify one column with the default justifications pass the others through.

```
    john@panic: $ column AAAAAA:d B C D E < U.tab $
    AAAAAA  B       C       D       E
    ------  -       -       -       -
    1.00    112.00  3       4.00    5
    1      1113    4.00    5.0     6
    1      1113    4.00    5.0     6
```
     
    Justify all column by default but pass one column through using the column
    :p flag.

```
    john@panic: $ column -d AAAAAA B C:p D E < U.tab $
    AAAAAA  B       C       D       E
    ------  ------- -       ------- -
      1.00   112.00 3          4.00 5
      1     1113    4.00       5.0  6
```
'

Column Name Pattern Selection and Renaming
==========================================

  Columns can be selected by explicit name or by pattern matching.  This can be effectivly used
  to select columns that have similar prefix or suffix charateers.  Column name patterns use
  the same file selection meta chatacters that are used by the shell (*?[]).  When selecting
  column for renaming (with the = option) the "matched" part of the column name can be used
  in the new name with the '?' character.

Examples
========

  After a search -j operation, select all of the column from the keytable:

```
    john@panic: column '*_1' < search_results
```

 On the same table rename the columns back to thier original names:

```
    john@panic: column '*_1=?' < search_results
```

{% include column-seealso.md %}
