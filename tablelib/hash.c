/* hash.c
 */
/* Starbase Data Tables - An Ascii Database for UNIX
 
   copyright 1991, 1993, 1995, 1999 John B. Roll jr.
 */

int table_hash(key, n)
	char	*key;
	int	 n;
{
		int	h = 0;

	while ( *key ) h += h + (*key++) * 13;

	return h % n;
}

