/* xfile memory class
 */

#include "xos.h"
#include "xfile.h"

xmemm_write(f, b, n, s)
	File	 f;
	char	*b;
	int	 n;
	int	 s;
{
}

xmemm_read(f, b, n, s)
	File	 f;
	char	*b;
	int	 n;
	int	 s;
{
}

xmemm_seek(f, o, n)
	File	 f;
	int	 o;
	int	 n;
{
}

xmemm_flush(f)
	File	 f;
{
}

xmemm_close(f)
	File	f;
{
	xFileDone(f);
}

xmemm_getc(f)
	File	 f;
{
}

xmemm_putc(f, c)
	File	 f;
	char	 c;
{
}

xmemm_ungetc(f, c)
	File	 f;
	char	 c;
{
}

xmemm_print(f)
	File f;
{
}

xmemm_setbuffer(f, t, b, s)
	File	 f;
{
}

xmemm_left(f)
	File	 f;
{
}


struct _xFileClass xFileMemm = {
	/* ivector write;	*/	  xmemm_write
	/* ivector read;	*/	, xmemm_read
	/* ivector seek;	*/	, xmemm_seek
	/* ivector flush;	*/	, xmemm_flush
	/* ivector close;	*/	, xmemm_close
	/* ivector getc;	*/	, xmemm_getc
	/* ivector putc;	*/	, xmemm_putc
	/* ivector ungetc;	*/	, xmemm_ungetc
	/* ivector print;	*/	, xmemm_print
	/* ivector setbuffer;	*/	, xmemm_setbuffer
	/* ivector left;	*/	, xmemm_left
	/* void	 *data;		*/	, NULL
};
