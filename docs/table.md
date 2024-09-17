

table - run an awk program with a starbase data table as input.

SYNOPSYS
--------

```
        table   'program' < tablefile
```

DESCRIPTION
-----------

`table` allows a complete awk language program to be run on a starbase data
table input file.  The program may be processed for $column name
substitutions, header value indexing, and undefined function inclusion but is
otherwise unaltered.  Column name substitution and header value indexing are
controlled with the -h switch.  Undefined function inclusion is always done
but may be disabled by unsetting the TABLEFUNCTIONS environment variable.

OPTIONS
-------

- -r Read in a starbase data table header.  The header is read from the
   standard input or from a reference file given with the -H option.
- -p Print the starbase data table header on the standard output before 
   executing the awk program.  The -p option implies -r.
- -h Use the header read in to make $column substitutions while translating
   the awk program.  The -h option implies -r.
- -i *file* Read input from *file*.
- -o *file* Write output to *file*.
- -D Print out the awk program to be executed before forking.
- -H Specify an alternate file for the starbase data table header.
   When the -H option is given the input file should not have a header.


DETAILS
-------

`table` is the base program for table row manipulation.  [row](row.html), [select](select.html), [calc](calc.html),
@reckon, [compute](compute.html) and [validate](validate.html) are all implemented as links to `table.`

{% include table-env.md %}
{% include row-seealso.md %}

