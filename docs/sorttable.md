

`sorttable` - sort a starbase data table on specified columns.

SYNOPSYS
--------

```
        `sorttable` [options] < *table* [*keycolumn*[:flags]] [*keycolumn*[:flags]] ...
```

DESCRIPTION
-----------

   `sorttable` uses the unix sort program to sort a starbase data table.

   A pair of lines is compared as follows: if  any  key  fields
   have  been  specified, sort compares each pair of fields, in
   the order specified on the command line,  according  to  the
   associated  ordering options, until a difference is found or
   no fields are left.

   If any of the program options Mbdfhinr are given  but  no  key
   fields are specified, sort compares the entire lines according 
   to the global options.

OPTIONS
-------

Program Options
---------------


- -i *file     use *file~ as input instead of the standard input.
- -o *file     use *file~ as output instead of the standard output.
- -c   Check whether the given files are already  sorted:  if
        they  are  not  all  sorted, print an error message and
        exit with a status of 1.
- -u   For the default case or the -m option, only output the
        first  of  a sequence of lines that compare equal.  For
        the -c option, check that no pair of consecutive  lines
        compares equal.
- -A   Print all lines of those that have duplicates.
- -D   Print only the first line of lines that have duplicates.
- -U   Print only lines which have no duplicates.
- -m   merge sorted files.
- -C   Print the number of lines that compare equil in a new column
        named "Count".
- -M <m>       multiple all internal buffers by <m>.
- -Z <z>       zip temporaty file at zip factor <z>.
- -T <dir>     use temp dir <dir>.
- -X   print out the intended arvg just before calling sort.


Sort Options
------------


- -a   Compare using the ASCII collating sequence.
- -b   skip initial blank space (note that trailing whitespace is 
        always skipped in sorttable).
- -d   Sort in `phone directory' order: ignore all characters
        except letters, digits and blanks when sorting.
- -F   Fold lower case characters into the  equivalent  upper
        case  characters when sorting so that, for example, `b'
        is sorted the same way `B' is.
- -I   Ignore characters outside  the  ASCII  range  040-0176
        octal (inclusive) when sorting.
- -n   Compare  according  to  arithmetic  value.  an  initial
        numeric  string  consisting of optional white space, an
        optional - sign, and zero or  more  digits,  optionally
        followed by a decimal point and zero or more digits.
- -h   Like -n except sexagesimal values are allowed (HH:MM:SSS.SS etc).
- -q   Column values are compared as dates in any valid date format (see mjd().)
- -r   Reverse the result of comparison, so that  lines  with
        greater key values appear earlier in the output instead
        of later.
- -z<n>        Zone sort of numeric column divided by <n>.


COLUMN FLAGS
------------

   Any of the above sort options may be supplied for specific columns
   by suffixing the column with a sort flag.  Sort flags are introduced
   by the colon character (":").  Several flags may be given.  The flags
   affect only that column.

   In addition a specific subset of characters in a key column may be 
   selected with the column range modifier.

   To sort file on column XXX characters 3 through 5 inclusive.

```
        sorttable XXX+3-5 file
```

EXAMPLES
--------

`
  Sort a table by Object name
```
        john@panic: sorttable < catalog.tab Object > sorted.tab
```

  Sort a table by RA and Dec
```
        john@panic: sorttable -h < catalog.tab RA Dec > sorted.tab
```

  Sort a table by Object name and then RA position
```
        john@panic: sorttable < catalog.tab Object -h RA > sorted.tab
```
'

SEE ALSO
--------


- [jointable]( jointable.html) - join two starbase data tables on matching key columns.


{% include starbase-seealso.md %}

