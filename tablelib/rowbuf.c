/* table_row.c
 */
/* Starbase Data Tables - An Ascii Database for UNIX
 
   copyright 1991, 1993, 1995, 1999 John B. Roll jr.
 */

#include "table.h"


typedef struct ParsBuffCtrl {
	char	*buff;
	char	*here;
	int	 size;
	int	 left;
	int	 read;
} *ParsBuffCntl;

int  xfile_rowbufclos(file)
	File	file;
{
    	ParsBuffCntl pars = xFile(file)->data;

	Free(pars->buff);
	Free(pars);
	Free(xFile(file));
	fclose(file);

	return 0;
}


off_t xfile_rowbufseek(file, offs, from)
	File	file;
	int	offs;
	int	from;
{
    	ParsBuffCntl pars = xFile(file)->data;

	pars->left = 0;
	pars->read = 1;

	fseek(file, offs, from);
	return ftell(file);
}


struct _xFileClass xFileRowBuf = {
	/* ivector write;	*/	  NULL
	/* ivector read;	*/	, NULL
	/* ivector seek;	*/	, xfile_rowbufseek
	/* ivector tell;	*/	, NULL
	/* ivector flush;	*/	, NULL
	/* ivector close;	*/	, xfile_rowbufclos
	/* ivector getc;	*/	, NULL
	/* ivector putc;	*/	, NULL
	/* ivector ungetc;	*/	, NULL
	/* ivector print;	*/	, NULL
	/* ivector setbuffer;	*/	, NULL
	/* ivector left;	*/	, NULL
	/* void	 *data;		*/	, NULL
};

#define ROWBUFFSIZE	128*1000

void table_rowbuf_open(file, table)
	File	  file;
	TableHead table;
{
    	ParsBuffCntl pars;

    table_mode(table, TABLE_PARSBUFF, 1);

    xFileInit(file, &xFileRowBuf);

    Malloc(xFile(file)->data, sizeof(struct ParsBuffCtrl));
    pars = xFile(file)->data;
    Malloc(pars->buff, ROWBUFFSIZE + 1);

    pars->here = pars->buff;
    pars->size = ROWBUFFSIZE;
    pars->left = 0;
    pars->read = 1;
}

TableRow table_parsbuff(file, row)
     File	 file;	/* file descriptor to read from  */
     TableRow   row;
{
        TableRow     xrow = row;
    	ParsBuffCntl pars;
	char        *buff;
        int     ch;    /* The last character read      */
	int	n;


    pars = xFile(file)->data;

    again:
	if ( pars->left < pars->size ) {
	    if ( pars->read ) {
		memmove(pars->buff, pars->here, pars->left);
		pars->read = Read(file, pars->buff + pars->left, 1, pars->size - pars->left);

		pars->left = pars->left + pars->read;
		pars->here = pars->buff;
	    }

	    if ( pars->left && !pars->read ) {
		if ( pars->here[pars->left-1] != '\n' ) {
		    pars->here[pars->left++] = '\n';
		}
	    }

	    if ( pars->left == 0 ) {
		return NULL;
	    }
	}

        if ( xrow == NULL ) xrow = table_rowloc(NULL, TABLE_LINE, TABLE_NCOL);

	xrow->buffer = pars->here;
	xrow->abuf   = -1;

	buff         = xrow->buffer;
 
        if ( row == NULL && buff == NULL )
                return xrow;
 
        /* Parse up the columns of the file
        */
        xrow->column[0] = "";

        for ( n = 0, xrow->ncol = 1; ; xrow->ncol++ ) {
	    if ( xrow->ncol >= xrow->acol )     /* col pointers bigger? */
		    table_rowloc(xrow, xrow->abuf, xrow->acol * 2);

	    xrow->column[xrow->ncol] = &buff[n];

	    for ( ; ; n++ ) {
		if ( n >= pars->left ) { 	/* char buffer bigger? */
		    int offs   = pars->here - pars->buff;

		    pars->size = pars->size * 1.5;

		    if ( pars->size < 0 ) return NULL;

		    ReAlloc(pars->buff, pars->size + 1);
		    pars->here = pars->buff + offs;

		    goto again;
		}

		ch = buff[n];

		if (   ch == '\0' ) { n++; break; }     /* Col delimiter      	*/
		if (   ch == '\t' ) {             	/* Col delimiter      	*/
	    	    buff[n++] = '\0';
		    break;
		}

		if (   ch == '\n'		 /* Row delimiter	*/
		    || ch == '\f' ) goto done;
	    }
        }

    done:
 
        if ( !n && ( ch == '\f' || ch == EOF ) ) {                /* EOF  */
                if ( row == NULL )
                	table_rowfree(xrow);
                return NULL;
        }
 
	buff[n++]   = '\0';
	xrow->nbuf  = n;
	pars->left -= n;
	pars->here += n;

	xrow->column[xrow->ncol+1] = &xrow->buffer[xrow->nbuf];
        return xrow;
}
