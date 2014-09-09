/* msdos.c
 */
/* copyright 1991, 1993, 1995, 1999 John B. Roll jr.
 */





#include <time.h>

long            SAOSleep(x)
    int             x;
{
    time_t          then;
    time_t          here;

    time(&here);
    then = here + x;

    while (here < then)
	time(&here);
}


set_termio()
{

}
