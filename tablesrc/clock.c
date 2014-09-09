
#include <stdio.h>
#include <sys/time.h>

double msg_clock()
{
    struct timeval date;
    struct timezone tz;
 
    gettimeofday(&date, &tz);
    return date.tv_sec + date.tv_usec / 1000000.0;
}

double mjd();

int main(argc, argv)
	int	argc;
	char	*argv[];
{
	double clock = msg_clock();

	if ( argc == 2 ) {
		clock = mjd(argv[1], 0);
	}

	printf("%.3f\n", clock);
}

