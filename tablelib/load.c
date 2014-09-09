/* table_load.c
 */
/* Starbase Data Tables - An Ascii Database for UNIX
 
   copyright 1991, 1993, 1995, 1999 John B. Roll jr.
 */



#include "table.h"
#ifdef __STDC__
#include <stdarg.h>
#else
#include <varargs.h>
#endif

static int table_sizeof(type)
	int	type;
{
	switch ( type & TblType ) {
	 case TblChr:   return sizeof(char);
	 case TblSht:   return sizeof(short);
	 case TblInt:	return sizeof(int);
	 case TblFlt:	return sizeof(float);
	 case TblDbl:	return sizeof(double);
	 case TblStr:	return sizeof(char *);
	 case TblDup:	return sizeof(char *);
	}

	return 0;
}

/* Construct a table descriptor from a varargs list.

 convert varargs of 

 TblTyp [| TblDef], <name>, <address>, [<data>]
 TblArr, <name>, <address>,  <length>, [<data>]
 */
static TblDescriptor table_descrip(args)
	va_list args;

{
		TblDescriptor T = NULL;
		int	type;

		int	ndesc = 0;
		int	adesc = 0;

	while ( (type = va_arg(args, int)) ) {
		if ( ndesc >= adesc ) {
			adesc += 10;
			ReAlloc(T, adesc * sizeof(TblRowDescrip));
		}

		T[ndesc].type = type;
		T[ndesc].name = va_arg(args, char *);

		if ( type & TblHdr )
			T[ndesc].cols = va_arg(args, int);

		T[ndesc].offs = va_arg(args, int);

		if ( type & TblUsr ) {
			T[ndesc].proc = va_arg(args, tblvect);
		} else {
			T[ndesc].proc = NULL;
			T[ndesc].size = table_sizeof(type);
		}

		if ( (type & TblType) == TblBuf ) {
			T[ndesc].size = va_arg(args, int);
		}

		if ( type & TblDef )
			T[ndesc].data = va_arg(args, void *);
		else	
			T[ndesc].data = NULL;

		ndesc++;
	}
	
	if ( ++ndesc > adesc ) {
		ReAlloc(T, ndesc * sizeof(TblRowDescrip));
	}

	T[ndesc-1].type = 0;
	return T;
}
/* Create a table descriptor */

TblDescriptor table_create_descrip(size_t *nrecs,...)
{

	va_list	args;

	TblDescriptor T;
	TableHead     t;

#ifdef __STDC__
	va_start(args, nrecs);
#else
	File	 ifile;
	TableHead	*head;
	void	*hbase;
	TableRow        **rows;
	void	*tbase;
	int	 tsize;
	size_t	*nrecs;

	va_start(args);

	nrecs	= va_arg(args, size_t *);
#endif

	T = table_descrip(args);
	return(T);
}




/* Load a table from ifile, using a varargs table descritpor list.
 */
#ifdef __STDC__
void *table_loadva(File ifile
		,  TableHead 	    *head, void *hbase
		,  TableRow        **rows, void *tbase
		,  size_t tsize, size_t *nrecs, ...)
#else
void *table_loadva(va_alist)
	va_dcl
#endif
{
	va_list	args;

	TblDescriptor T;
	TableHead     t;

#ifdef __STDC__
	va_start(args, nrecs);
#else
	File	 ifile;
	TableHead	*head;
	void	*hbase;
	TableRow        **rows;
	void	*tbase;
	size_t	 tsize;
	size_t	*nrecs;

	va_start(args);

	ifile	= va_arg(args, File);
	head    = va_arg(args, TableHead *);
	hbase 	= va_arg(args, void *);
	rows    = va_arg(args, TableRow **);
	tbase 	= va_arg(args, void *);
	tsize 	= va_arg(args, size_t);
	nrecs	= va_arg(args, size_t *);
#endif

	T = table_descrip(args);
	if ( head && *head ) t = *head;
	else 		     t = table_header(ifile, TABLE_PARSE);

	if ( t == NULL || T == NULL )
		return NULL;

	tbase = table_load(ifile, t, T, hbase, rows, tbase, tsize, nrecs);
	Free(T);

	if ( head ) {
	  if ( !*head )  { *head = t; }
	} else 		    table_hdrfree(t);

	return tbase;
}



int table_loadmap(t, T)
	TableHead	 t;
	TblDescriptor	 T;
{
		int	i;

	/* Look up column #'s for requested data.
	 */
	for ( i = 0; T[i].type; i++ ) {
	    if ( T[i].type & TblHdr ) continue;
	    if ( !(T[i].cols = table_colnum(t, T[i].name))
		&& !(T[i].type & TblDef) ) {
		    fprintf(stderr, "table_load: missing column: %s\n", T[i].name);
		    return 0;
	    }
	}

	return 1;
}

void table_loadhdr(t, T, rec)
	TableHead	t;
	TblDescriptor	T;
	void		*rec;
{
	int	i;

	for ( i = 0; T[i].type; i++ ) {
			char	*base = (char *) rec;
			long	 offs = (long) T[i].offs;
			void	*here = (void *)(base + offs);
			int	 col  = T[i].cols;
			char	*name = T[i].name;

	    if ( !(T[i].type & TblHdr) ) continue;

	    if ( T[i].type & TblBak ) {
		switch ( T[i].type & TblType ) {
		 case TblChr:
			*((char   *)here) = *((char   *) T[i].data);
			continue;
		 case TblSht:
			*((short  *)here) = *((short  *) T[i].data);
			continue;
		 case TblInt:
		    if ( *((long    *) T[i].data) != INDEFI ) {
			*((int    *)here) = *((int    *) T[i].data);
			continue;
		    }
		 case TblFlt:
		    if ( *((float  *) T[i].data) != INDEFR ) {
			*((float  *)here) = *((float  *) T[i].data);
			continue;
		    }
		 case TblDbl:
		    if ( *((double *) T[i].data) != INDEFD ) {
			*((double *)here) = *((double *) T[i].data);
			continue;
		    }
		 case TblStr:
		    if ( strcmp(*((char  **) T[i].data), "INDEF") ) {
			strcpy(*((char  **) here), *((char  **) T[i].data));
			continue;
		    }
		}
	    }

	    if ( table_hdrget(t, T[i].name, 0, col) != NULL ) {
	       switch ( T[i].type & TblType ) {
		case TblInt:
		    *((int    *)here) = table_hdrgeti(t, name, 1, col);	break;
		case TblFlt:
		    *((float  *)here) = table_hdrgetd(t, name, 1, col);	break;
		case TblDbl:
		    *((double *)here) = table_hdrgetd(t, name, 1, col);	break;
		case TblStr:
		    *((char  **)here) = table_hdrgets(t, name, 1, col);	break;
	      }
	    } else {
	       switch ( T[i].type & TblType ) {
		case TblInt:
			*((int    *)here) = *((int    *) T[i].data);
			break;
		case TblFlt:
			*((float  *)here) = *((float  *) T[i].data);
			break;
		case TblDbl:
			*((double *)here) = *((double *) T[i].data);
			break;
		case TblStr:
			strcpy(*((char  **) here), *((char  **) T[i].data));
			break;
	      }
	    }
	}
}

int table_loadrow(ifile, t, r, T, rec)
	File		ifile;
	TableHead	t;
	TableRow	r;
	TblDescriptor	T;
	void		*rec;
{
	int	i;

	for ( i = 0; T[i].type; i++ ) {
			char	*base = (char *) rec;
			long	 offs = (long) T[i].offs;
			void	*here = (void *)(base + offs);
			int	 col  = T[i].cols;
			int	size  = T[i].size;

	    if ( T[i].type & TblHdr ) continue;

	    if ( T[i].proc ) {
	      if ( !col ) {
		if ( !(*T[i].proc)(t, T[i].data, NULL                , here, rec) )
		    break;
	      } else {
		if ( !(*T[i].proc)(t, T[i].data, table_colval(r, col), here, rec) )
		    break;
	      }
	    } else {
	      if ( col ) {
	       switch ( T[i].type & TblType ) {
		case TblChr: {
			char	*s = table_colval(r, col);

		    while ( *s == ' ' && *s != '\0' );

		    *((char   *)here) = *s;	break;
		 }
		case TblByt:
		    *((unsigned char *)here) = table_colvali(r, col); 	break;
		case TblSht:
		    *((short  *)here) = table_colvali(r, col); 	break;
		case TblInt:
		    *((int    *)here) = table_colvali(r, col); 	break;
		case TblFlt:
		    *((float  *)here) = table_colvalr(r, col); 	break;
		case TblDbl:
		    *((double *)here) = table_colvald(r, col); 	break;
		case TblStr:
		    *((char  **)here) = table_colval(r, col);	break;
		case TblDup:
		    *((char  **)here) = strdup(table_colval(r, col));break;
		case TblBuf:
		    strncpy((char  *)here, table_colval(r, col), size);
		    break;
	       }
	      } else {
	       if ( T[i].data )  
	       switch ( T[i].type & TblType ) {
		case TblChr:   
		    *((char   *)here) = *((char   *) T[i].data);	break;
		case TblByt:   
		    *((unsigned char *)here) = *((unsigned char  *) T[i].data);	break;
		case TblSht:   
		    *((short  *)here) = *((short  *) T[i].data);	break;
		case TblInt:   
		    *((int    *)here) = *((int    *) T[i].data);	break;
		case TblFlt:
		    *((float  *)here) = *((float  *) T[i].data);	break;
		case TblDbl:   
		    *((double *)here) = *((double *) T[i].data);	break;
		case TblStr:   
		    *((char  **)here) =   (char   *) T[i].data;		break;
		case TblDup:
		    *((char  **)here) = strdup((char *) T[i].data);	break;
		case TblBuf:
		    strncpy((char  *)here, (char  *) T[i].data, size);
		    break;
	       }
	      }
	    }
	}

	if ( T[i].type ) return 0;
	else		 return 1;
}


/* Load a table from ifile, using a table descriptor.
 */
void *table_load(ifile, t, T, hbase, rows, tbase, tsize, nrecs)
	File		 ifile;
	TableHead	 t;
	TblDescriptor	 T;
	void		*hbase;
	TableRow	**rows;
	void		*tbase;
	size_t		 tsize;
	size_t		*nrecs;
{
	TableRow	 R = NULL;
	TableRow	 r;
	size_t		 n, a;

	size_t		 arecs, arows = 0;

	table_loadhdr(t, T, hbase);

	if ( tbase ) {
		a      = *nrecs;
	    	arecs  = 0;
	} else {
		a      = 0;
		*nrecs = 0;
		arecs  = 1;
	}

	if ( rows )
	    if ( *rows )	arows = 0;
	    else		arows = 1;

	if ( !table_loadmap(t, T) ) return NULL;

	if ( rows == NULL ) R = table_rowloc(NULL, 0, 0);

	/* Read the table.
	 */
	for ( n = 0; (r = table_rowget(ifile, t, R, NULL, NULL, 0)); n++ ) {
	    *nrecs = n+1;

	    if ( n >= a ) {
			int x = a;

		if ( !arecs ) return tbase;

		a += 100; 
		a *= 2;
		ReAlloc(tbase, a * tsize);
		memset((char *) tbase + (x * tsize), 0, (a - x) * tsize);
		if ( arows ) {
		    ReAlloc(*rows, a * sizeof(TableRow));
		    memset(&(*rows)[x], 0, (a - x) * sizeof(TableRow));
		}
	    }
	    if ( !table_loadrow(ifile, t, r
			, T, ((char *) tbase) + n * tsize) ) {
		n--;
		continue;
	    }

	    if ( rows ) {
		(*rows)[n] = r;
	    }
	}

	if ( rows == NULL ) table_rowfree(R);
	return tbase;
}
