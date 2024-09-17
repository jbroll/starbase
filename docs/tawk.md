

tawk - extension to the awk programming language.

DESCRIPTION
-----------

`tawk` is an extended version of the awk programming langauge.  It is based on 
the [mawk](mawk.html) implementation.  The extensions are intended to make processing
data files in astronomy easier.

EXTENSIONS
----------


- Sexagesimal Notation


   In the text of a program or expression run by [table](table.html), [compute](compute.html) or [calc](calc.html) (and
   others) a number may be represented in sexagesimal notation.  In this context
   the number must form a single lexical token ie it cannot contain spaces.  In
   a data table the degrees mininutes and seconds portions of the number may be
   separated by spaces.


```
                DDD:MM:SS.SSS
                 HH:MM:SS.SSS

                000d00m00.000
                 00h00m00.000

                    00:00.000
                    00m00.000
```

   The sexagesimal number notation is supported in the printf
   function with a new format specifier "@".

```
        john@panic: [reckon](reckon.html) 'printf("%2.2@\n", 21.5)'
        21:30:00
```

   In addition to allowing sexagesimal notation the ASCII representation base
   of a number is tracked as an expression is evaluated.  The output
   representation of an expression is that of the right most term in the 
   expression.  This feature is very handy in maintaining the representation 
   of data files as they are operated on.  It can also be used to convert data
   from one ASCII representation to another by adding 0 in a specific
   representation to the data to be converted.

Representation  Zero
--------------  ----
Decimal         0
Sexagesimal     0:00:00.00
Hexadecimal     0x0000
Octal           0o000
Binary          0b000

   An example of converting an RA,Dec in decimal to an RA in sexagesimal
   representation.

```

   john@panic: [calc](calc.html) 12.5+0:00:00
   12:30:00.000
```


- Additional functions


  Units conversions

   * degrees = r2d(radians)
   * radians = d2r(degrees)
   * hours   = r2h(radians)
   * radians = h2r(hours)
   * hours   = d2h(degrees)
   * degrees = h2d(hours)

  Astronomical Functions

   * Airmass

```
        mass = airmass(zd)
```

        The airmass at a specific zenith distance is returned.  Zenith distance
        is given in degrees.  See [slaAirmas](slalib/slaAirmas.3.html).

   * Coordinate Precession

```
        [ra, dec] = precess(system, epoch0, epoch1, ra0, dec0)
```

        The ra, dec of ra0, dec0 precessed from epoch0 to epoch1 is returned.
        The system may be one of "FK4" or "FK5".  RA values are given and returned
        in hours.  Dec values are given and returned in degrees.  See
        [slaPreces](slalib/slaPreces.3.html).

   * Standard Coordinate Systems Conversion

```
        
        [r2000, d2000] = fk45z(r1950, d1950, bepoch)
        [r2000, d2000, dr2000, dd2000, p2000, v2000] = fk425(r1950, d1950, dr1950, dd1950, p1950, v1950)

        [r1950, d1950, dr1950, dd1950] = fk54z(r2000, d2000, bepoch)
        [r1950, d1950, dr1950, dd1950, p1950, v1950] = fk524(r2000, d2000, dr2000, dd2000, p2000, v2000)
```

        With `fk425` and `fk45z` the Besellian r1950, d1950 coordinates in hours
        and degrees are converted to Julian r2000, d2000 coordiantes.  Catalog
        data for proper motion, paralax and velocity may optionally be
        provided and will be returned.  See
        [slaFk425](slalib/slaFk425.3.html) and [{slaFk45z]({slaFk45z.html),slalib/slaFk45z.3.html}.

        With `fk524` and `fk54z` the Julian r2000, d2000 coordinates in hours
        and degrees are converted to Besellian r1950, d1950 coordiantes.
        Catalog data for proper motion, paralax and velocity may optionally be
        provided and will be returned.  See
        [slaFk524](slalib/slaFk524.3.html) and [{slaFk54z]({slaFk54z.html),slalib/slaFk54z.3.html}.

   * Tangent Plane Coordinates

```
        [r, d] = tp2s(x, y, rzero, dzero, scale, rot)
        [x, y] = s2tp(r, d, rzero, dzero, scale, rot)
```

        Conversion bewteen spherical and tangent plane coordiantes.  `tp2s`
        converts from tangent plane to sphreical coordinates with `s2tp`
        converting back.  Each routine passes r and d as hours and degress.
        The additional parameters *rzero*, *dzero*, *scale*, *rot* define the
        zero point, scale and rotaiton of the tangent plane.  *rzero*, *dzero*
        are given in hours and degrees.  See
        [slaDtp2s](slalib/slaDtp2s.3.html) and
        [slaDs2tp](slalib/slaDs2tp.3.html)

   * Separation and Bearing Angles

```
        angle = sep(ra0, dec0, ra1, dec1)
        angle = bear(ra0, dec0, ra1, dec1)
```

        `Bear` returns the bearing angle in degrees between to two ras and decs
        given in hours and degrees.

        `Sep` returns the separation angle in degrees between to two ras and
        decs given in hours and degrees.

   * Modified Julain Date

```
        daynumber = mjd(datestring)
        daynumber = mjd(month, day, year)
        daynumber = mjd(month, day, year, hour, min, sec)
```

        Convert from a calendar date to a modified julian day number with
        fractional time of day.  All input dates are considered to be in GMT
        unless specifically overridden.  This is only possible using a date
        string input.  The syntax for converting the date string is explained
        here [getdate](getdate.html) .

   * UNIX Time

        seconds = unx(datestring)

        Convert from a calendar date to the number of seconds since the UNIX
        epoch (1970).  All input dates are considered to be in GMT
        unless specifically overridden.  This is only possible using a date
        string input.  The syntax for converting the date string is explained
        here [getdate](getdate.html) .

   * Calendar date string creating

```
        datestring = cal(mjd)
        datestring = cal(mjd, format)
```

        Convert from a modified julain date to a calendar date string.  An optional
        format string may be provided.  The format string is passed directly 
        to the [strftime](strftime.html) function.  Since strftime is a UNIX function it
        may not work for all formats before the UNIX epoch.  Specifically it
        has been found that the day of the week is formatted as a "?" for these
        dates

   * Observing Station Parameters

```
        [name, westlong, lat, elevation] = obs(place)
```

        Return earth position parameters of an observing station.
        See [slaObs](slalib/slaObs.3.html).  Here is the current table
        of station data.


ID              Station                                 WestLong        Lat     Elevation
----------      ------------------------------------    --------        ------  ---------
AAT             Anglo-Australian 3.9m Telescope           -2.602        -0.546   1164.000
LPO4.2          William Herschel 4.2m Telescope            0.312         0.502   2332.000
LPO2.5          Isaac Newton 2.5m Telescope                0.312         0.502   2336.000
LPO1            Jacobus Kapteyn 1m Telescope               0.312         0.502   2364.000
LICK120         Lick 120 inch                              2.123         0.652   1290.000
MMT             MMT, Mt Hopkins                            1.935         0.553   2608.000
VICBC           Victoria B.C. 1.85 metre                   2.154         0.847    238.000
DUPONT          Du Pont 2.5m Telescope, Las Campanas       1.234        -0.506   2280.000
MTHOP1.5        Mt Hopkins 1.5 metre                       1.935         0.553   2344.000
STROMLO74       Mount Stromlo 74 inch                     -2.601        -0.616    767.000
ANU2.3          Siding Spring 2.3 metre                   -2.602        -0.546   1149.000
GBVA140         Greenbank 140 foot                         1.393         0.671    881.000
TOLOLO4M        Cerro Tololo 4 metre                       1.236        -0.526   2235.000
TOLOLO1.5M      Cerro Tololo 1.5 metre                     1.236        -0.526   2225.000
TIDBINBLA       Tidbinbilla 64 metre                      -2.600        -0.618    670.000
BLOEMF          Bloemfontein 1.52 metre                   -0.461        -0.507   1387.000
BOSQALEGRE      Bosque Alegre 1.54 metre                   1.127        -0.551   1250.000
FLAGSTF61       USNO 61 inch astrograph, Flagstaff         1.950         0.614   2316.000
LOWELL72        Perkins 72 inch, Lowell                    1.947        -0.613   2198.000
HARVARD         Harvard College Observatory 1.55m          1.249         0.742    185.000
OKAYAMA         Okayama 1.88 metre                        -2.332         0.603    372.000
KPNO158         Kitt Peak 158 inch                         1.948         0.558   2120.000
KPNO90          Kitt Peak 90 inch                          1.948         0.558   2071.000
KPNO84          Kitt Peak 84 inch                          1.948         0.558   2096.000
KPNO36FT        Kitt Peak 36 foot                          1.948         0.558   1939.000
KOTTAMIA        Kottamia 74 inch                          -0.555         0.522    476.000
ESO3.6          ESO 3.6 metre                              1.234        -0.511   2428.000
MAUNAK88        Mauna Kea 88 inch                          2.714         0.346   4215.000
UKIRT           UK Infra Red Telescope                     2.713         0.346   4200.000
QUEBEC1.6       Quebec 1.6 metre                           1.242         0.793   1114.000
MTEKAR          Mt Ekar 1.82 metre                        -0.202         0.800   1365.000
MTLEMMON60      Mt Lemmon 60 inch                          1.932         0.566   2790.000
MCDONLD2.7      McDonald 2.7 metre                         1.816         0.535   2075.000
MCDONLD2.1      McDonald 2.1 metre                         1.816         0.535   2075.000
PALOMAR200      Palomar 200 inch                           2.040         0.582   1706.000
PALOMAR60       Palomar 60 inch                            2.040         0.582   1706.000
DUNLAP74        David Dunlap 74 inch                       1.386         0.766    244.000
HPROV1.93       Haute Provence 1.93 metre                 -0.100         0.767    665.000
HPROV1.52       Haute Provence 1.52 metre                 -0.100         0.767    667.000
SANPM83         San Pedro Martir 83 inch                   2.015         0.542   2830.000
SAAO74          Sutherland 74 inch                        -0.363        -0.565   1771.000
TAUTNBG         Tautenburg 2 metre                        -0.204         0.890    331.000
CATALINA61      Catalina 61 inch                           1.933         0.566   2510.000
STEWARD90       Steward 90 inch                            1.948         0.558   2071.000
USSR6           USSR 6 metre                              -0.723         0.762   2100.000
ARECIBO         Arecibo 1000 foot                          1.165         0.320    496.000
CAMB5KM         Cambridge 5km                             -0.001         0.911     17.000
CAMB1MILE       Cambridge 1 mile                          -0.001         0.910     17.000
EFFELSBERG      Effelsberg 100 metre                      -0.120         0.882    366.000
GBVA300         Greenbank 300 foot                         1.394         0.671    894.000
JODRELL1        Jodrell Bank 250 foot                      0.040         0.929     78.000
PARKES          Parkes 64 metre                           -2.588        -0.576    392.000
VLA             Very Large Array                           1.878         0.595   2124.000
SUGARGROVE      Sugar Grove 150 foot                       1.384         0.672    705.000
USSR600         USSR 600 foot                             -0.726         0.765    973.000
NOBEYAMA        Nobeyama 45 metre                         -2.417         0.627   1350.000
JCMT            JCMT 15 metre                              2.714         0.346   4111.000
ESONTT          ESO 3.5 metre NTT                          1.234        -0.511   2377.000
ST.ANDREWS      St Andrews                                 0.049         0.983     30.000
APO3.5          Apache Point 3.5m                          1.847         0.572   2809.000
KECK1           Keck 10m Telescope `1`                      2.714         0.346   4160.000
KECK1           ?                                          2.714         0.346   4160.000


   * Siderial Time

```
        stime = st(place, date)
```

        Return the siderial time at place for the given datestring.
        See  [slaGmst](slaGmst.3.html), [{slaEqeqx]({slaEqeqx.html),slalib/slaEqeqx.3.html}.

   * Alt Az pointing direction

```
        [alt, az] = altax(place, ra, dec, datestring)
        [alt, az] = altax(place, ra, dec, datestring, epoch)
```

        Return the altitude and azimuth for pointing a telescope at the ra,
        dec, given in hours and degrees.  The place is given as the ID of an
        observing station.  See [slaMappa](slalib/slaMappa.3.html),
        [slaAoppa](slalib/slaAoppa.3.html),
        [slaMapqkz](slalib/slaMapqkz.3.html),
        [slaAopqk](slalib/slaAopqk.3.html).


   * Gausian Residual

```
        x = gresid(s)
```

        The results of many calls to this routine will be normally distributed
        with mean zero and standard deviation s.

        The Box-Muller algorithm is used.  This is described in Numerical
        Recipes, Section 7.2.  See [slaGresid]( slalib/slaGresid.3.html)



- Multiple function value return and assignment lists:


        To support returning multiple values as is convient in the above
        functions, the awk syntax has been extended to allow returning a list
        of values.

```
        [ x, y, z ] = funct()
```

        This works for user defined functions as well:

```
                function squares(x, y) {
                        return [ x * x, y * y ]
                }
```
        The bracket notation in the return statement allows a list of expressions
        to be returned from the function.  When the function is called it should
        be called from an assignment list statment.
```
                [ xsq, ysq ] = squares(x, y)
```
        Here the bracket notation to the left of an assignment allows the multiple
        values returning from the function to be assigned to variables.
 

        Multiple assignment is also supported.
```
        [ x, y, z ] = [ z, y, x ]
```
