/* teeproc.c

 SYNOPSYS
	teeproc [-m] 'pipeline 1' ['pipeline 2' ['-']]

 DESCRIPTION

	Each argument to teeproc will be executed as a shell command using the
	popen library call.  Teeproc will copy its standard input to the
	several processes.  If a pipeline is specified as "-" teeproc will
	copy its standard input to its standard output.

 EXAMPLE

	To tee the output of the preliminary to the input to several 
	processing commands:

	> preliminary | teeproc 'further f1 f2' 'otheri o1 o2' 'yetmoreproc'


	Since the command arguments to teeproc are executed with popen, they
	may themselves be sh pipelines.  Here the same input data may be
	reduced using different arguments and reports generated directly to
	output files:

	> getdata | teeproc 'reduce type=1 | report > type1.out'  \
			    'reduce type=2 | report > type2.out'

 */

#include <signal.h>
#include <stdio.h>
#include <stdlib.h>

#define MAXPROCS	32
#define BUFFERSIZE	8192*64

int strcmp();


int main(argc, argv)
	int	 argc;
	char	*argv[];
{
		char	 data[BUFFERSIZE];
		FILE	*file[MAXPROCS];
		int	 size;

		int	 i;

	signal(SIGPIPE, SIG_IGN);

	if ( argc == 1 ) {
		fprintf(stderr, "teeproc: no processes to run\n");
		exit(1);
	}

	if ( argc-1 > MAXPROCS ) {
		fprintf(stderr, "teeproc: only %d process can be run\n"
			, MAXPROCS);
		exit(1);
	}

	for ( i = 1; i < argc; i++ ) {
	    if ( !strcmp(argv[i], "-") ) file[i-1] = stdout;
	    else
		if ( !(file[i-1] = popen(argv[i], "w")) ) {
			fprintf(stderr, "teeproc: can't popen pipeline, %s\n"
				, argv[i]);
			perror("teeproc");
			exit(1);
		};
	}

	while ( size = fread(data, 1, BUFFERSIZE, stdin) ) {
	    for ( i = 1; i < argc; i++ ) {
		fwrite(data, 1, size, file[i-1]);
	    }
	}

    {
	int code = 0;

	for ( i = 1; i < argc; i++ ) {
		if ( file[i-1] != stdout ) code |= pclose(file[i-1]);
	}

	exit(code);
    }
}
