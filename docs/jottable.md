
NAME
====

jottable - print a table of sequential or random data

SYNOPSYS
========

```
    `jottable` *column* [-cnr] [-s] [-p] [-w *string*] [*reps* [*begin* [*end* [*s*]]]]
```

DESCRIPTION
===========

`jottable` is used to print out increasing, decreasing, random, or redundant
data column of data, usually numbers.

The following options are available:

     * -r
            Generate random data instead of the default sequential data.

     * -b *word* 
            Just print *word* repetitively.

     * -w *word*

            Print *word* with the generated data appended to it.  Octal,
            hexadecimal, exponential, `ASCII` , zero padded, and right-adjusted
            representations are possible by using the appropriate `printf(3)`
            conversion specification inside *word* , in which case the data
            are inserted rather than appended.

     * -c

            This is an abbreviation for "-w %c".

     * -p *precision*

            Print only as many digits or characters of the data as indicated
            by the integer *precision*.  In the absence of -p, the precision
            is the greater of the precisions of *begin* and *end*.  The -p
            option is overridden by whatever appears in a `printf(3)` conversion
            following -w.

    The last four arguments indicate, respectively, the number of data, the
    lower bound, the upper bound, and the step size or, for random data, the
    seed.  While at least one of them must appear, any of the other three may
    be omitted, and will be considered as such if given as "".

    Any three of these arguments determines the fourth.  If four are specified
    and the given and computed values of *reps* conflict, the lower value is
    used.  If fewer than three are specified, defaults are assigned left to
    right, except for *s* , which assumes its default unless both *begin* and
    *end* are given.

    Defaults for the four arguments are, respectively, 100, 1, 100, and 1,
    except that when random data are requested, the seed, *s* , is picked
    randomly.  *Reps* is expected to be an unsigned integer, and if given as
    zero is taken to be infinite.  *Begin* and *end* may be given as real
    numbers or as characters representing the corresponding value in `ASCII` .
    The last argument must be a real number.

    Random numbers are obtained through `random(3)` .  The name `jot` derives in
    part from `iota` , a function in APL.


EXAMPLES
========

  The command
```
    jottable X 21 -1 1.00
```

 prints 21 evenly spaced numbers increasing from -1 to 1 in column X.

 The `ASCII` character set is generated with
```
    jottable ASCII -c 128 0
```

and the strings xaa through xaz with


```
    jottable TmpFile -w xa%c 26 a
```

while 20 random 8-letter strings are produced with

```
    jottable TmpFile -r -c 160 a z | rs -g 0 8"
```

Infinitely many `yes's` may be obtained through

```
    jottable Yes -b yes 0
```

and thirty ed substitution commands applying to lines 2, 7, 12, etc. is
the result of

```
    jottable ed -w %ds/old/new/ 30 2 - 5
```

The stuttering sequence 9, 9, 8, 8, 7, etc. can be
produced by suitable choice of precision and step size,
as in

```
    jottable Sequence 9 - -.5
```

Copyright
=========
```

- Copyright (c) 1993
-      The Regents of the University of California.  All rights reserved.
 *
- Redistribution and use in source and binary forms, with or without
- modification, are permitted provided that the following conditions  
- are met:
- 1. Redistributions of source code must retain the above copyright
-    notice, this list of conditions and the following disclaimer.
- 2. Redistributions in binary form must reproduce the above copyright
-    notice, this list of conditions and the following disclaimer in the
-    documentation and/or other materials provided with the distribution.
- 3. All advertising materials mentioning features or use of this software
-    must display the following acknowledgement:
-      This product includes software developed by the University of
-      California, Berkeley and its contributors.
- 4. Neither the name of the University nor the names of its contributors
-    may be used to endorse or promote products derived from this software
-    without specific prior written permission.
 *
- THIS SOFTWARE IS PROVIDED BY THE REGENTS AND CONTRIBUTORS ``AS IS'' AND
- ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
- IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
- ARE DISCLAIMED.  IN NO EVENT SHALL THE REGENTS OR CONTRIBUTORS BE LIABLE
- FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
- DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS
- OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
- HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
- LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY
- OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF
- SUCH DAMAGE.
```


SEE ALSO
========

{% include starbase-seealso.md %}

