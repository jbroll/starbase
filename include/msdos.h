/* dos.h
**
	Fix up some stuff that dos leaves out.
**/

#define fcntl(fd, cmd, arg)		-1

#define _TIME_T_DEFINED
typedef long    time_t;


#define sleep(x)


/* Open call.
 */
#define O_SYNC		0
#define O_NOCTTY	0
#define O_NDELAY	0

/* Access call.
 */
#define F_OK            0		    /* does file exist */
#define X_OK            1		    /* is it executable by caller */
#define W_OK            2		    /* writable by caller */
#define R_OK            4		    /* readable by caller */

extern int      errno;

#define	_FOPEN		(-1)		    /* from sys/file.h, kernel use
					     * only */
#define	_FREAD		0x0001		    /* read enabled */
#define	_FWRITE		0x0002		    /* write enabled */
#define	_FNDELAY	0x0004		    /* non blocking I/O (4.2 style) */
#define	_FAPPEND	0x0008		    /* append (writes guaranteed at
					     * the end) */
#define	_FMARK		0x0010		    /* internal; mark during gc() */
#define	_FDEFER		0x0020		    /* internal; defer for next gc
					     * pass */
#define	_FASYNC		0x0040		    /* signal pgrp when data ready */
#define	_FSHLOCK	0x0080		    /* BSD flock() shared lock
					     * present */
#define	_FEXLOCK	0x0100		    /* BSD flock() exclusive lock
					     * present */
#define	_FCREAT		0x0200		    /* open with file create */
#define	_FTRUNC		0x0400		    /* open with truncation */
#define	_FEXCL		0x0800		    /* error on open if file exists */
#define	_FNBIO		0x1000		    /* non blocking I/O (sys5 style) */
#define	_FSYNC		0x2000		    /* do all writes synchronously */
#define	_FNONBLOCK	0x4000		    /* non blocking I/O (POSIX style) */
#define	_FNOCTTY	0x8000		    /* don't assign a ctty on this
					     * open */

/*
 * Flags that work for fcntl(fd, F_SETFL, FXXXX)
 */
#define	FAPPEND		_FAPPEND
#define	FSYNC		_FSYNC
#define	FASYNC		_FASYNC
#define	FNBIO		_FNBIO
#define	FNONBIO		_FNONBLOCK	    /* XXX fix to be NONBLOCK
					     * everywhere */
#define	FNDELAY		_FNDELAY

/*
 * Flags that are disallowed for fcntl's (FCNTLCANT);
 * used for opens, internal state, or locking.
 */
#define	FREAD		_FREAD
#define	FWRITE		_FWRITE
#define	FMARK		_FMARK
#define	FDEFER		_FDEFER
#define	FSHLOCK		_FSHLOCK
#define	FEXLOCK		_FEXLOCK

/*
 * The rest of the flags, used only for opens
 */
#define	FOPEN		_FOPEN
#define	FCREAT		_FCREAT
#define	FTRUNC		_FTRUNC
#define	FEXCL		_FEXCL
#define	FNOCTTY		_FNOCTTY

/*
 * Flags that work for fcntl(fd, F_SETFL, FXXXX)
 */
#define	FAPPEND		_FAPPEND
#define	FSYNC		_FSYNC
#define	FASYNC		_FASYNC
#define	FNBIO		_FNBIO
#define	FNONBIO		_FNONBLOCK	    /* XXX fix to be NONBLOCK
					     * everywhere */
#define	FNDELAY		_FNDELAY

/*
 * Flags that are disallowed for fcntl's (FCNTLCANT);
 * used for opens, internal state, or locking.
 */
#define	FREAD		_FREAD
#define	FWRITE		_FWRITE
#define	FMARK		_FMARK
#define	FDEFER		_FDEFER
#define	FSHLOCK		_FSHLOCK
#define	FEXLOCK		_FEXLOCK

/*
 * The rest of the flags, used only for opens
 */
#define	FOPEN		_FOPEN
#define	FCREAT		_FCREAT
#define	FTRUNC		_FTRUNC
#define	FEXCL		_FEXCL
#define	FNOCTTY		_FNOCTTY
