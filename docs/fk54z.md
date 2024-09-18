
### `fk54z` - convert coordinates from FK5 to FK4

SYNOPSYS
--------

```
        `fk54z` [-i *ifile*] [-o *ofile*] *rcol* *dcol*
```

DESCRIPTION
-----------

  Convert a J2000.0 FK5 star position from the starbase data table 
  read from the standard input to B1950.0 FK4 assuming
  zero proper motion and parallax.

  Star positions are converted from the new, IAU 1976,
  FK5, Fricke system to the old, Bessel-Newcomb, FK4 system.

  See [slaFk54z]( slalib/slaFk54z.3.html)

OPTIONS
-------

  * -i *file* Read input from *file* instead of the standard input.
  * -o *file* Write output to *file* instead of the standard output.

PARAMETERS
----------

  * rcol

        The column name of the ra column defaults to "ra".  The
        column should contain RA coordinate values in hours.
  * dcol

        The column name of the dec column defaults to "dec".  The
        column should contain Dec coordinate values in degrees.

  * bepoch
        The converted position in the B1950
        reference frame but at Besselian epoch bepoch.  For
        comparison with catalogues the bepoch argument will
        frequently be 1950.0 and is the default.

PARAMETERS
----------

  * rcol

        The column name of the ra column defaults to "ra".  The column should
        contain RA coordinate values in hours.

  * dcol

        The column name of the dec column defaults to "dec".  The column
        should contain Dec coordinate values in degrees.

  * bepoch

        The initial position in the B1950 reference frame but at Besselian
        epoch bepoch.  For comparison with catalogues the bepoch argument will
        frequently be 1950.0 and is the default.

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
- [precess](precess.html)     - Precess the ra and dec columns from one equinox to another.


{% include starbase-seealso.md %}

