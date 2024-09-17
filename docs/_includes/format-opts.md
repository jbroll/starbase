Formatting Options
------------------

Column will accept column formatting options to reformat the ASCII
representation of numeric columns.  These options are invoked by following the
column name with a printf style % specification.

`
 Format some columns:

```
 john@panic : column X%7.2f Y%7.2f RA%12.3@ Dec%11.2@
 X       Y       RA      Dec
 -       -       --      ---
 512.00  512.00           12:30:34.501           34:00:54.45
```
'

