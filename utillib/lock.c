/* lock.c
 */
/* copyright 1991, 1993, 1995, 1999 John B. Roll jr.
 */

Lock(fd, type, size)
	int	fd, type, size;
{
#ifdef BERKLEY
	flock(fd, type);
#else
	lockf(fd, type, size);
#endif
}

