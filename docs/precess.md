

precess - Precess the ra and dec columns from one equinox to another.

SYNOPSYS
--------

```
        `preces` *sys* *eq0* *eq1* < file
```

DESCRIPTION
-----------

 Precess the ra and dec columns of an starbase data table
 from an equinox in either FK4 (Bessel-Newcomb, 
 pre-IAU1976) or the FK5 (Fricke, post-IAU1976) 
 system as indicated with the sys parameter.

OPTIONS
-------

  * -i *file* Read input from *file* instead of the standard input.
  * -o *file* Write output to *file* instead of the standard output.

PARAMETERS
----------

  * sys

        The coordinate system for precession.  One of "FK4|fk4|FK5|fk5".

  * eq0

        The initial equinox.

  * eq1

        The final equinox.

  * rcol

        The column name of the ra column defaults to "ra".  The
        column should contain RA coordinate values in hours.

  * dcol

        The column name of the dec column defaults to "dec".  The
        column should contain Dec coordinate values in degrees.

NOTES
-----
  
 The epochs are Besselian if sys='FK4' and Julian if 'FK5'.
 For example, to precess coordinates in the old system from
 equinox 1900.0 to 1950.0 the command would be

```
         precess FK4 1900.0 1950.0 < targets
```
  
 This routine will not correctly convert between the old and
 the new systems - for example conversion from B1950 to J2000.
 For these purposes see [fk45z](fk45z.html) and [fk54z](fk54z.html) .

SEE ALSO
--------


- [fk45z](fk45z.html)     - convert ra, dec columns from fk4 to fk5 assuming zero proper
                  motion.
- [fk54z](fk54z.html)     - convert ra, dec columns from fk5 to fk4 assuming zero proper
                  motion.
- [fk425](fk425.html)     - convert ra, dec columns from fk4 to fk5 with proper motion,
                  paralax and recession velocity.
- [fk524](fk524.html)     - convert ra, dec columns from fk5 to fk4 with proper motion,
                  paralax and recession velocity.


{% include starbase-seealso.md %}


