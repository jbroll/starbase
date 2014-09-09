
#include "xos.h"
#include "xfile.h"

char **filelines(file, nfile) 
	char	*file;
	int	*nfile;
{
	int	n;
	char   *data = readfile(file);
	char   *here = data;
	char **list;

	if ( !data ) return NULL;

	n = 1;
	while ( *here ) if ( *here++ == '\n' ) n++;
	here--;
	if ( *here == '\n' ) {
		n--;
		*here = '\0';
	}


	list = malloc(sizeof(char *) * n);

	n = 0;
	list[n++] = here = data;
	
	for ( ; *here; here++ ) {
	    if ( *here == '\n' ) {
		*here++   = '\0';
		list[n++] = here;
	    }
	}

	*nfile = n;
	return list;
}

char *readfile(file)
			char	*file;
{
		char	*text = NULL;
		off_t	 size;
		File	 fd;
		char mode[2];

		mode[0] = 'r';
		mode[1] = '\0';

	if ( ( fd = xOpen(file, "r")) == NULL ) {
	    	return NULL;
	}
	if ( size = filesize(fd) ) {
		Malloc(text, size + 1);
		if ( Read(fd, text, 1, size) != size ) {
	    		return NULL;
		}

		text[size] = '\0';
	}

	Close(fd);

	return text;
}

off_t filesize(file)
			File	file;
{
    off_t here = Seek(file, 0, SEEK_CUR);
    off_t size;

    Seek(file, 0,    SEEK_END);
    size = Tell(file);
    Seek(file, here, SEEK_SET);

    return size;
}	
