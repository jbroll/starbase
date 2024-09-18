

`calc` - a command line calcutator


SYNOPSYS
--------

```
        calc    'expression'
```


DESCRIPTION
-----------

`calc` is a command line calculator.  It evaluates its arguments as a single
awk language expression and prints the result on the standard output.

The results of the last few expressions evaluated by calc are kept in a file
by default named ~/.calc.  The file is used as a stack.  The top value may be
accessed in a calc expression as "@@".  Other values on the stack may be 
accessed with an index on "@@".  for example "@@[2]" is the second value on the
stack.  The name of the stack file may be set with the -s <filename> option.

In csh it is convenient to alias calc to "=" to mimic the behavior of the IRAF
cl compute mode.  File name substitution can be turned off within
the alias so that "*" need not be escaped within an expression and parentheses
are escaped.

```
        alias = 'set noglob;calc `echo "\!*" | sed -e s/\(/\\\(/ -e s/\)/\\\)/`;unset noglob'
        = 6*5
        30
        = (4+10)*2
        28
```

OPTIONS
-------

    * -s *filename* Set the calc stack file name (default ~/.calc).
    * -D Print out the awk program to be executed before forking.


EXAMPLES
--------

`
Add a couple of number on the command line:

```
        johnpanic : calc 4 + 6
        10
```
Add a couple of number in hh:mm:ss format:
```
        johnpanic : calc 4:00:00 + 0:30:00
        4:30:00
```
Add a number to the previous answer:
```
        johnpanic : calc @ + 2:00:00
        6:30:00
```
'

{% include table-env.md %}

SEE ALSO
--------
    * [mawk](mawk.html) An implementation of the awk programming language by Mike Brennan.
    * [tawk](tawk.html) Awk language extensions implemented at SAO.
