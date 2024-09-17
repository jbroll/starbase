

`wctable` -   use the wc utility on an starbase data table.

SYNOPSYS
--------

```
    `wctable` [-cClw] [*table* ... ]
```


DESCRIPTION
-----------

When name is specified on the command line,  the  names  are
printed along with the counts.
If no option is specified the default is -lwc (count  lines,
words, and bytes.)

OPTIONS 
-------

  * -c  Count bytes.
  * -C  Count characters.
  * -l  Count lines.
  * -w  Count words delimited by white space characters or new
        line  characters.

SEE ALSO
--------

{% include starbase-seealso.md %}
