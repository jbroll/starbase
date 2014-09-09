
#include <stdio.h>

#include <sys/ipc.h>
#include <sys/shm.h>

usage(i) {
        fprintf(stderr, "usage:\n");
        fprintf(stderr, "       shmcat < file id\n");
        fprintf(stderr, "       shmcat > file id\n");
	exit(1);
}

main(argc, argv)
        int      argc;
        char    *argv[];
{
    if ( isatty(0) ) {
        if ( isatty(1) ) 	      usage(1);
        if ( argc <= 1 || argc >= 4 ) usage(1);
        {

	    int size;
            int shkey;
            char *mm;
	    struct shmid_ds buf;
	    
	    shkey = shmget(strtol(argv[1], NULL, 0), 0, 0);
            if ( argc <= 2 ) {
		shmctl(shkey, IPC_STAT, &buf);
		size = buf.shm_segsz;
	    } else {
                size  = atoi(argv[2]);
	    }

            shkey = shmget(strtol(argv[1], NULL, 0), size, 0400);
	    if ( shkey == -1 ) {
		perror("shmcat");
		exit(1);
	    }

            mm  = shmat(shkey, 0, 0);
	    if ( mm == (char *) -1 || mm == NULL ) {
		perror("shmcat");
		exit(1);
	    }

            write(1, mm, size);
        }
    } else {
        if ( isatty(0) ) 	      usage(1);
        if ( argc <= 1 || argc >= 3 ) usage(1);

        {
            int size  = filesize(0);
            int shkey;
            char *mm;

/*            shkey = shmget(strtol(argv[1], NULL, 0), size, 0666 | IPC_CREAT); */
            shkey = shmget(strtol(argv[1], NULL, 0), size, 0660 | IPC_CREAT);
	    if ( shkey == -1 ) {
		perror("shmcat");
		exit(1);
	    }

            mm  = shmat(shkey, 0, 0);
	    if ( mm == (char *) -1 || mm == NULL ) {
		perror("shmcat");
		exit(1);
	    }
            read(0, mm, size);
        }
    }
}

#include <sys/types.h>
#include <sys/stat.h>

typedef struct stat FileInfo;

#define FileStat(f, info)       fstat(f, info)
#define FileInfoSize(info)      info.st_size


int filesize(file)
              int    file;
{
        FileInfo info;

        if ( FileStat(file, &info) < 0 )        return 0;
        else                                    return FileInfoSize(info);
}

