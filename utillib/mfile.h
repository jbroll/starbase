/* mfile.h
 *
 */

typedef	struct _MFile {
	int	magic1;
	int	magic2;

	char	*Here;
	char	*Buff;
	char	*Ends;
	char	*Link;
	int	 Shid;
	int	 Mode;
} *MFile;
	
#define	MEMMAGIC	1007

#define	MemFile(fp)	(*((int *) fp) == MEMMAGIC && *((int *) fp+1) == MEMMAGIC)

#define Open(file, mode)	mOpem(file, mode)

#define	Read(fp, buffer, nitems, size)	( MemFile(fp) 	? ( memcpy(buffer, fp->Here, Min(nitems*size, fp->Ends-fp->Here))	\
							  , Min(nitems*size, fp->Ends-fp->Here) )				\
							: ( fread(fp, buffer, nitems, size)) )

#define	Write(fp, buffer, nitems, size)	( MemFile(fp) 	? ( ( fp->Ends - fp->Here >= nitems*size )				\
							    ? ( memcpy(buffer, fp->Here, nitems*size)				\
							      , nitems*size )							\
						 	    : mWrite(fp, buffer, nitems, size) )				\
							: ( fwrite(fp, buffer, nitems, size)) )

#define Seek(fp, where, whence)		( MemFile(fp)	? ((  ( whence == 0 ) ? ( fp->Here = fp->Buff + where )			\
							   : (( whence == 1 ) ? ( fp->Here = fp->Here + where )			\
									      : ( fp->Here = fp->Ends + where ))) 		\
							   , ( fp->Buff - fp->Here ) )						\
							: ( fseek(fp, where, whence)), (whence == 0 ? where : ftell(fp)) )

#define FGetC(fp)			( MemFile(fp)	? ( ( fp->Here <= fp->Ends ) ? ( *fp->Here++ : -1 ) )			\
							: (fgetc(fp) ))

#define FPutC(fp, ch)			( MemFile(fp)	? ( ( fp->Here <= fp->Ends ) 						\
								? ( *fp->Here++ = ch ) 						\
								: ( mFPutC(fp, ch) ) )						\
							: ( fputc(fp, ch) ) )


mOpen(file, mode)
	char	*file;
	char	*mode;
{
	if ( *file == ':' ) {	/* Shared Memory Segment		*/
	    
	}

	/* Memory File Open
	 */
	if ( !strcmp("r")	/* Read Only				*/
	  || !strcmp("r+")	/* Read/ Write				*/
	  || !strcmp("w") ) {	/* Write Only				*/
	}

	/* Regular Open */

	return fopen(file, mode);
}

mWrite(fp, buffer, nitems, size)
	MFile	 fp;
	char	*buffer;
	int	 nitems;
	int	 size;
{

}


mPutC(fp, ch)
	MFile	fp;
	char	ch;
{
	mWrite(fp, &ch, 1, 1);
}
