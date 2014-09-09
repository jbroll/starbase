/* bcopy.c
 */
/* copyright 1991, 1993, 1995, 1999 John B. Roll jr.
 */


bcopy(s1, s2, n)
    char           *s1;
    char           *s2;
    int             n;
{
    while (n--) {
	*s2++ = *s1++;
    }
}
