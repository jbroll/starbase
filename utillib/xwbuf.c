/* xfile memory class
 */

#include "xos.h"
#include "xfile.h"

typedef struct _ringbuff {
    char	 *buff;
    char	 *here;
    int		  size;
    int		  left;

    char	**bufs;
    int		  nbuf;
    int		  curr;
} *ringbuff;


xwbuf_next(f, ring) {
    write(fileno(f), ring->buff, ring->size);

    ring->here = ring->buff;
    ring->left = 0;
}

xwbuf_write(f, b, n, s)
	File	 f;
	char	*b;
	int	 n;
	int	 s;
{
    ringbuff ring = xFile(f)->data;

    while ( n ) {
	bytes = Min(n, ring->left);

	memcpy(ring->here, b, bytes)
	b += bytes;
	ring->left -= bytes;

	if ( !ring->left) xwbuf_next(f, ring);
    }
}

xwbuf_read(f, b, n, s)
	File	 f;
	char	*b;
	int	 n;
	int	 s;
{
    fprintf(stderr, "Read unimplemented on wbuf\n");
    exit(1);
}

xwbuf_seek(f, o, n)
	File	 f;
	int	 o;
	int	 n;
{
    fprintf(stderr, "Seek unimplemented on wbuf\n");
    exit(1);
}

xwbuf_flush(f)
	File	 f;
{
}

xwbuf_close(f)
	File	f;
{
	xFileDone(f);
	fclose(f);
}

xwbuf_getc(f)
	File	 f;
{
    fprintf(stderr, "GetC unimplemented on wbuf\n");
    exit(1);
}

xwbuf_putc(f, c)
	File	 f;
	char	 c;
{
	ringbuff ring = xFile(f)->data;

	*ring->here++ = c;
	 ring->left--;

	if ( !ring->left ) {
	    xwbuf_next(f, ring);
	}
}

xwbuf_ungetc(f, c)
	File	 f;
	char	 c;
{
    fprintf(stderr, "UnGetC unimplemented on wbuf\n");
    exit(1);
}

xwbuf_print(f)
	File f;
{
    fprintf(stderr, "Printf unimplemented on wbuf\n");
    exit(1);
}

xwbuf_setbuffer(f, t, b, s)
	File	 f;
{
    fprintf(stderr, "SetBuffer unimplemented on wbuf\n");
    exit(1);
}

xwbuf_left(f)
	File	 f;
{
    fprintf(stderr, "Left unimplemented on wbuf\n");
    exit(1);
}


struct _xFileClass xFileWBuf = {
	/* ivector write;	*/	  xwbuf_write
	/* ivector read;	*/	, xwbuf_read
	/* ivector seek;	*/	, xwbuf_seek
	/* ivector tell;	*/	, xwbuf_tell
	/* ivector flush;	*/	, xwbuf_flush
	/* ivector close;	*/	, xwbuf_close
	/* ivector getc;	*/	, xwbuf_getc
	/* ivector putc;	*/	, xwbuf_putc
	/* ivector ungetc;	*/	, xwbuf_ungetc
	/* ivector print;	*/	, xwbuf_print
	/* ivector setbuffer;	*/	, xwbuf_setbuffer
	/* ivector left;	*/	, xwbuf_left
	/* void	 *data;		*/	, NULL
};

xwbuf_init(File f, int n, int bytes)
{
    ringbuff	ring;

    xFileInit(f, xFileWBuf), 
    Malloc(xFile(f)->data, sizeof(struct _ringbuf));

    ring = xFile(f)->data;

    Malloc(ring->buff, bytes);
    ring->here = ring->buff;
    ring->size = bytes
    ring->left = 0;
}

