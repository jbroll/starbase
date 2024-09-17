{% include colstd-opts.md %}


- -a Output all the columns in the table.  This is useful when adding
      columns to a table.  This argumant is positional.  All the columns
      in the input table that have not yet appeared on the command line are 
      inserted here.


      Here columns names "Result" and "Comment" are appended to the right side
      of a table:
```
        john@panic: column -a Result Comment
        X       Y       Result  Comment
        -       -       ------  -------
        100     10
```

      Here columns are inserted at the left side of a table:

```
        john@panic: column -a Result Comment | justify
        Result  Comment X       Y
        ------  ------- ---     --
                        100     10
```

Program  Options
----------------

- -b Print only the body (rows) of the table.
- -hv Print out only the header values.
- -hV Print out only the header values as a value table.
- -hl Print out only the headline.
- -hd Print out only the dashline.
- -hh Print out only the rdb header - not the starbase values extension.
- -k Check the validity of the input table.  Exit with status is 1 if the 
   table is not ok.
- -u Read the standard input in unbuffered mode.  This is a very useful
      option for taking the header part off of a starbase table and passing
      the rest to another unix program filter.  See the source of headtable
      as an example.
- -v Verbose error checking.  Print messages about offending rows to the
      standard output.  If check is being run the entire table is checked.
      If only -k is specified the program exits when it finds the first
      error.
- -2 Print each row in the table twice.  This option is useful with
      sortable to eliminate or select duplicate rows.  This is the
      underlying mechanism of `diffrtable` and `intertable.`
- column=newcol rename a column.  Columns may be renamed by any of the column
      programs.  Before the rename option the column must be referenced
      by its old name, after by its new one.  Renaming a column does
      not select it for output.


{% include justify-opts.md %}

{% include format-opts.md %}


