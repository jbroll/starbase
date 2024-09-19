/* table_search	- C interface to search functions.
 */
/* Starbase Data Tables - An Ascii Database for UNIX
 
   copyright 1991, 1993, 1995, 1999 John B. Roll jr.
 */



#define TABLE_INDEX_BUFFERSIZE	1024
#define LADDR_SIZE		1024*128

#include "table.h"
#include "tablesearch.h"
#include <filelist.h>

int xGetChar(file)
	File file;
{
	int ch;
	    
	ch = GetC(file);
	ch = ch == '\r' ? ' ' : ch;

	return ch;
}

#define GetChar xGetChar

/* Internal comparison operators
 */
#define OP_EQ		0
#define OP_LT		1
#define OP_LE		2
#define OP_GE		3
#define OP_GT		4
#define OP_MATCH	5

double strtod();
double SAOstrtod();

TableRow seekrow();
void FileListFree();

static off_t binsearch	PROTOTYPE((
	TableSearch	 s,
	File		 file,
	int		 icol,
	char		*value,
	int		 op,
	off_t		 start,
	off_t		 end));
static int scompare	PROTOTYPE((
	TableSearch	s,
        int     op,
        char    *v1,
        char    *v2));


static int ncompare(op, type, rev, d1, d2) 
	int	op, type, rev;
	double	d1;
	double	d2;
{
		int	comp;

	switch ( op ) {
	 case OP_LT:	comp = d1 <  d2; break;
	 case OP_LE:	comp = d1 <= d2; break;
	 case OP_GE:	comp = d1 >= d2; break;
	 case OP_GT:	comp = d1 >  d2; break;
	 case OP_EQ:	comp = d1 == d2; break;
	 default:	fprintf(stderr, "search : internal error\n");
			exit(1);
	}

	if ( rev )  return !comp;
	else	    return  comp;
}

typedef struct _BinRowCache {
	struct _CNode	cache;
	off_t		offset;
	int		length;
	off_t		star;
	off_t		ends;
	union { 
		char   *s;
		double	d;
	} value;
} *BinRowCache;


void BinRowCacheFree(cache, s)
	TableSearch	s;
	BinRowCache	cache;
{
	if ( !s->Numeric ) Free(cache->value.s);
}

int BinRowHashComp(c1, c2)
	BinRowCache	c1;
	BinRowCache	c2;
{
    return c1->cache.hash == c2->cache.hash
	&& c1->star == c2->star
	&& c1->ends == c2->ends;
}

int TableValCacheComp(c1, c2, s)
	TableValCacheNode	c1;
	TableValCacheNode	c2;
	TableSearch		s;
{
	if ( c1->cache.hash != c2->cache.hash ) return 0;

	return 1;
}

void TableValCacheFree(c1)
	TableValCacheNode	c1;
{
	free(c1->value);
}

int TableRangeCacheComp(c1, c2, s)
	TableRangeCacheNode	c1;
	TableRangeCacheNode	c2;
	TableSearch		s;
{
	if ( c1->cache.hash != c2->cache.hash ) return 0;
	if ( c1->value0 == NULL && c2->value0 != NULL ) return 0;
	if ( c1->value0 != NULL && c2->value0 == NULL ) return 0;
	if ( c1->value0 != NULL && c2->value0 != NULL
	     &&	!scompare(s, OP_EQ
			, c1->value0
			, c2->value0) ) 	return 0;

	if ( c1->value1 == NULL && c2->value1 != NULL ) return 0;
	if ( c1->value1 != NULL && c2->value1 == NULL ) return 0;
	if ( c1->value1 != NULL && c2->value1 != NULL
	     &&	!scompare(s, OP_EQ
			, c1->value0
			, c2->value0) ) 	return 0;

	return 1;
}

void TableRangeCacheFree(c1)
	TableRangeCacheNode	c1;
{
	free(c1->r);
}

TableSearch table_searchsetup(table)
	char	*table;
{
    	TableSearch search;
	char		*tbuffer;

    Malloc(search, sizeof(struct _TableSearch));

    /* Open the Table and look up the search column. 
     */
    if ( (search->Tabl = Open(table, "r")) == NULL ) {
	FPrint(Stderr, "table_search: can't open file: %s\n", table);
	Free(search);
	return NULL;
    }
    Malloc(tbuffer, 32768);
    SetBuffer(search->Tabl, _IOFBF, tbuffer, 32768);

    if ( (search->THead = table_header(search->Tabl, 0)) == NULL ) {
	FPrint(Stderr, "table_search: can't read table header from: %s\n", table);
	Free(search);
	return NULL;
    }

    search->Table  = strdup(table);
    search->Tstart = Seek(search->Tabl, 0, 1);
    search->Tend   = Seek(search->Tabl, 0, 2);
    Seek(search->Tabl, search->Tstart, 0);

    {   TableRow Row = table_rowloc(NULL, TABLE_LINE, TABLE_NCOL);
	TableRow row;
	int	i, tot      = 0;

      for ( i = 0; i < 15; i++ ) {
	    if ( !(row = table_rowget(search->Tabl, search->THead, Row, NULL, NULL, 0)) )
		break;
	    tot += row->nbuf;
      }

      table_rowfree(Row);

      if ( !tot || !i ) {
	    FPrint(Stderr, "table_search: can't read row from table file: %s\n", table);
	    exit(1);
      }
      if ( i == 1 ) i = 2;

      tot /= (i/2);
      if ( tot < 512 ) tot = 512;

      search->TIdx = OpenFd(FileFd(search->Tabl), "r");
      Malloc(tbuffer, tot);
      SetBuffer(search->TIdx, _IOFBF, tbuffer, tot);
    }

    search->Index   = NULL;
    search->Method  = 0;
    search->Compare = 0;
    search->Reverse = 0;
    search->modulus = 0.0;

    return search;
}

char *table_searchcheckindexpattern(search, directo, pattern, method)
    TableSearch search;
    char	*directo;
    char	*pattern;
    int		 method;
{
    char	*mpatt;
    char	*npatt;

    char	*name = NULL;
    char	 N, M;
    filelist	 list;
    int		 len;

    switch ( method & TSEARCH_METHOD ) {
     case 0			: mpatt = "[sbhir]";	break;
     case TSEARCH_HASH		: mpatt = "h";		break;
     case TSEARCH_BINARY	: mpatt = "b";		break;
     case TSEARCH_INDEX		: mpatt = "i";		break;
     case TSEARCH_SEQUENTIAL	: mpatt = "s";		break;
    }

    switch ( method & TSEARCH_NUMERIC ) {
     case 0			: npatt = "[-.+]";;	break;
     case TSEARCH_ASCII		: npatt = "-";		break;
     case TSEARCH_NUMBER	: npatt = "[.+]";	break;
     case TSEARCH_DECIMAL	: npatt = ".";		break;
     case TSEARCH_SEXAGAS	: npatt = "+";		break;
    }

    sprintf(pattern, "%s%s%s", pattern, npatt, mpatt);

    if ( (list = FileList(directo, pattern)) )  {
	if ( list->next ) {
	    FPrint(Stderr, "search: search method must be explicite when there is more than one index.\n");
	    for ( ; list; list = (filelist) list->next ) {
		FPrint(Stderr, "	found: %s\n", list->name);
	    }
	    exit(1);
	}

	len = strlen(list->name);

	N = list->name[len - 2];
	M = list->name[len - 1];

	search->Numeric = 0;
	search->Numeric = ( N == '-' ? 0 : ( N == '.' ? 1 : ( N == '+' ? 2 : -1 )));
	search->Method  = ( M == 's' ? TSEARCH_SEQUENTIAL
			: ( M == 'b' ? TSEARCH_BINARY
			: ( M == 'i' ? TSEARCH_INDEX
			: ( M == 'r' ? TSEARCH_ROW
			: ( M == 'h' ? TSEARCH_HASH : -1 )))));

	if ( search->Numeric == -1 || search->Method == -1 ) {
	    FPrint(Stderr, "table_search: can't decode index mode from index name: %s.\n"
			, list->name);
	    exit(1);
	}


	Malloc(name,  strlen(list->path) + strlen(list->name) + 2);
	sprintf(name, "%s/%s", list->path, list->name);
	FileListFree(list);
	return name;
    }

    return NULL;
}

void table_searchsetupindex(search, Index, column)
    TableSearch search;
    char	*Index;
    char	*column;
{
    char	*tbuffer;
    TableRow	 row;

    if ( (search->Indx = Open(Index, "r")) == NULL ) {
	FPrint(Stderr, "table_search: can't open index file: %s\n", Index);
	exit(1);
    }
    Malloc(tbuffer, TABLE_INDEX_BUFFERSIZE);
    SetBuffer(search->Indx, _IOFBF, tbuffer, TABLE_INDEX_BUFFERSIZE);

    if ( (search->IHead = table_header(search->Indx, TABLE_PARSE)) == NULL ) {
	FPrint(Stderr, "table_search: can't read table header from index: %s\n"
		, Index);
	exit(1);
    }
    search->Istart = Seek(search->Indx, 0, 1);

    if ( search->Method == TSEARCH_ROW
      || search->Method == TSEARCH_HASH ) {
	if ( (row = table_rowget(search->Indx, search->IHead, NULL, NULL, NULL, 0)) == NULL ) {
	    FPrint(Stderr, "table_search: can't read row from index file: %s\n", Index);
	    exit(1);
	}
	table_rowfree(row);
	search->Iwidth = Seek(search->Indx, 0, 1) - search->Istart;
	search->Irows   = (search->Iend - search->Istart) / search->Iwidth;
    }

    search->Iend   = Seek(search->Indx, 0, 2);
    Seek(search->Indx, search->Istart, 0);

    search->Ioffcol = table_colnum(search->IHead, "Offset");
    search->Ilencol = table_colnum(search->IHead, "Length");
    search->Indxcol = table_colnum(search->IHead, column);

    if ( search->IHead != NULL ) {
	search->Iindexd = atoffset(table_hdrget(search->IHead, "Index", 0, 1));
    } else {
	search->Iindexd = search->Tend;
    }
    { char *Col;

      if ( Col = table_hdrget(search->IHead, "Column", 1, 1) ) {
	  if ( Col = strchr(Col, '%') ) {
	      search->modulus = atof(++Col);
	  }
      }
    }
}

TableSearch table_searchsetupcache(search, filecache)
    TableSearch	search;
    TableFileRowCache filecache;
{

    search->cache	= cache_init(sizeof(struct _BinRowCache)
			    , 27905, 200000
			    , BinRowCacheFree
			    , search
			    , BinRowHashComp);

    Calloc(search->laddr, LADDR_SIZE * sizeof(struct _Laddr));
    search->lhit = 0;
    search->ltry = 0;

    search->filecache = filecache;

    search->rangecache = cache_init(sizeof(struct _TableRangeCacheNode)
			    , 15432, 10000
			    , TableRangeCacheFree
			    , search
			    , TableRangeCacheComp);

    search->valuecache = cache_init(sizeof(struct _TableValCacheNode)
			    , 15432, 10000
			    , TableValCacheFree
			    , search
			    , TableValCacheComp);

    return search;
}

TableSearch table_search(table, column, method, filecache)
	char	*table;
	char	*column;
	int	 method;
	TableFileRowCache filecache;
{
		char		*Index = NULL;
		int		 Output;

	    	char	*tbuffer;

	TableSearch search = table_searchsetup(table);

    if ( method & TSEARCH_MEMORY ) {
	;
    }

    if ( strcmp(column, "Row") ) {
	    char	buf[TABLE_TRIMBUF];
	    char	pattern[1024];
	    char	directo[1024];
	    char	*here;

	if ( (search->Tcolumn = table_colnum(search->THead, trim(column, buf))) == 0 ) {
	    FPrint(Stderr, "table_search: search column: %s not in file: %s\n"
		    , column, table);
	    Free(search);
	    return NULL;
	}

	search->Output  = ( method & TSEARCH_OUTONE | method & TSEARCH_OUTALL );

	strcpy(directo, table);
	if ( (here = strrchr(directo, '/')) ) {
	    *here = '\0';
	    here++;
	} else {
	    strcpy(directo, ".");
	    here = table;
	}
	
	sprintf(pattern, "%s.%s", here, trim(column, buf));
	search->Index = table_searchcheckindexpattern(search, directo, pattern, method);
    }

    if ( !search->Index || search->Method == TSEARCH_SEQUENTIAL ) {
	search->Method  = TSEARCH_SEQUENTIAL;
	switch ( method & TSEARCH_NUMERIC ) {
	   case 0 :
	   case TSEARCH_ASCII:
		search->Numeric = 0;	break;
	   case TSEARCH_DECIMAL:
		search->Numeric = 1;	break;
	   case TSEARCH_NUMBER:
	   case TSEARCH_SEXAGAS:
		search->Numeric = 2;	break;
   	} 
    } else {
	table_searchsetupindex(search, search->Index, column);
    }

    return table_searchsetupcache(search, filecache);
}


TableRow sequ_rowget(file, offset, r, saved, current)
	File		file;
	off_t		offset;
	TableRow	r;
	off_t		*saved;
	off_t		*current;
{
	int	ch;
	if ( offset == *saved ) return r;

	if ( offset != *current ) {
	    Seek(file, offset-1, 0);
	    while ( (ch = GetChar(file)) != '\n' && ch != EOF ) offset++;
	}

	r =  table_rowget(file, NULL, r, NULL, NULL, 0);

	if ( r ) {
	    *saved   = offset;
	    *current = offset + r->nbuf;
	}

	return r;
}

char *sequ_valget(cache, file, column, offset, r, saved, end, current)
	Cache		cache;
	File		file;
	int		column;
	off_t		offset;
	TableRow	*r;
	off_t		*saved;
	off_t		*end;
	off_t		*current;
{
	struct _TableValCacheNode	rx;
	TableValCacheNode		rr;
	char			    *value;

    rx.cache.hash = offset;

    if ( !(rr = (TableValCacheNode) cache_hashe(cache, (CNode) &rx, NULL)) ) {
	*r = sequ_rowget(file, offset, *r, saved, current);

        rr = (TableValCacheNode) cache_cache(cache, (CNode) &rx);
	rr->end   = *current;
	if ( *r ) {
	    value = rr->value = strdup(table_colval(*r, column));
	} else
	    value = rr->value = NULL;
    }

    *end = rr->end;
    value = rr->value;

    cache_nfree(cache, (CNode) rr);


    return value;
}

void *search_sequ(s, value0, value1, func, data, R, start)
	TableSearch	s;
	char		*value0;
	char		*value1;
	void	      *(*func)();
	void		*data;
	TableRow	 R;
	off_t		start;
{
		TableRow	r = R, x = NULL;
		off_t	end;
		char	*value;

		off_t	 saved   = 0;
		off_t	 current = 0;

	if ( start == -1 ) start = s->Tstart;

	while ( (value = sequ_valget(s->valuecache, s->Tabl, s->Tcolumn, start, &r, &saved, &end, &current)) ) {
	  x = r;

	  if ( value1 ?    scompare(s, s->Compare & TSEARCH_LT ? OP_LT : OP_LE
                        	, value0, value)
			&& scompare(s, s->Compare & TSEARCH_GT ? OP_GT : OP_GE
                        	, value1, value)
		      : (s->Compare & TSEARCH_MATCH)
			? scompare(s, OP_MATCH, value0, value)
			: scompare(s, OP_EQ,    value0, value) ) {

		if ( s->Output & TSEARCH_OUTALL ) {
		    if ( start != saved ) {
			r = sequ_rowget(s->Tabl, start, r, &saved, &current);
		    }

		    data = func(data, start, end, r);
		} else {
		    data = func(data, start, end, NULL);
		}

		r = R;
		x = NULL;
		if ( s->Output & TSEARCH_OUTONE ) break;
	  }
	  start = end;
	}

	if ( R == NULL && x != NULL ) table_rowfree(x);
	return data;
}

static void *search_rows(s, value0, value1, func, data, R) 
	TableSearch	s;
	char		*value0;
	char		*value1;
	void	      *(*func)();
	void		*data;
	TableRow         R;

{
		TableRow	r;
		TableRow	i;
		TableRow	I = table_rowloc(NULL, 0, 0);

		int	row0;
		int	row1;

		int		offs, j;

	row0 = strtod(value0, NULL);
	row1 = value1 ? ((int) strtod(value1, NULL)) : row0;

	row0 = Max(row0, 1);
	row1 = Min(row1, s->Irows);

	if ( s->Output & TSEARCH_OUTALL || s->Output & TSEARCH_OUTONE ) {
	  for ( j = row0; j <= row1; j++ ) {
	    Seek(s->Indx, (j + 1) * s->Iwidth, 0);
	    i    = table_rowget(s->Indx, s->IHead, I, NULL, NULL, 0);
	    offs = atoffset(table_colval(i, s->Ioffcol));
	
	    Seek(s->TIdx, offs, 0);
	    r = table_rowget(s->TIdx, s->THead, R, NULL, NULL, 0);

	    data = (*func)(data, offs, offs + R->nbuf, r);
	    if ( s->Output & TSEARCH_OUTONE ) break;
	  }
	} else {
		int	off0;

	    /* This branch optimnizes a range of rows.
	     */
	    Seek(s->Indx, (row0 + 1) * s->Iwidth, 0);
	    i    = table_rowget(s->Indx, s->IHead, I, NULL, NULL, 0);
	    off0 = atoffset(table_colval(i, s->Ioffcol));

	    Seek(s->Indx, (row1 + 1) * s->Iwidth, 0);
	    i    = table_rowget(s->Indx, s->IHead, I, NULL, NULL, 0);
	    offs = atoffset(table_colval(i, s->Ioffcol));

	    Seek(s->Tabl, offs, 0);
	    i    = table_rowget(s->Tabl, s->THead, I, NULL, NULL, 0);
	    data = (*func)(data, off0, offs + i->nbuf, NULL);
	}

	/* Supplimentary sequential search of unindexed data
	 */
	if ( s->Tend != s->Iindexd ) {
	    data = search_sequ(s, value0, value1, func, data, R, s->Iindexd);
	}

	table_rowfree(I);
	return data;
}

static void *search_hash(s, value0, value1, func, data, R)
	TableSearch	s;
	char		*value0;
	char		*value1;
	void	      *(*func)();
	void		*data;
	TableRow         R;
{
		TableRow	r = R, x = NULL;
		TableRow	i;
		TableRow	I = table_rowloc(NULL, 0, 0);
		int	hashed;
		int	offset;

	hashed = table_hash(value0, s->Irows);
	offset = ( hashed + 2 ) * s->Iwidth;

	Seek(s->Indx, offset, 0);
	i = table_rowget(s->Indx, NULL, I, NULL, NULL, 0);

	while ( (offset = atoffset(table_colval(i, s->Ioffcol))) ) {
		Seek(s->TIdx, offset, 0);
		r = table_rowget(s->TIdx, s->THead, r, NULL, NULL, 0);
		x = r;

		if ( scompare(s, OP_EQ
			   , value0
			   , table_colval(r, s->Tcolumn)) ) {
		    data = (*func)(data, offset, offset + r->nbuf, r);
		    r = R;
		    x = NULL;
	    	    if ( s->Output & TSEARCH_OUTONE ) break;
		}
		if ( ++hashed >= s->Irows ) {
			hashed = 0;
			Seek(s->Indx, s->Istart, 0);
		}

		i = table_rowget(s->Indx, NULL, I, NULL, NULL, 0);
	}

	/* Supplimentary sequential search of unindexed data
	 */
	if ( s->Tend != s->Iindexd ) {
	    data = search_sequ(s, value0, value1, func, data, R, s->Iindexd);
	}

	if ( R == NULL && x != NULL ) table_rowfree(x);
	table_rowfree(I);
	return data;
}

static void *search_indx(s, value0, value1, func, data, R)
	TableSearch	s;
	char		*value0;
	char		*value1;
	void	      *(*func)();
	void		*data;
	TableRow         R;
{
		TableRow	r = R;
		TableRow	i, I;
		off_t		start	= s->Istart;
		off_t		end	= s->Iend;
	    	off_t		offset;

	if ( R ) I = R;
	else	 I = table_rowloc(NULL, 0, 0);

	start = binsearch(s, s->Indx, s->Indxcol, value0
			, s->Compare & TSEARCH_GT ? OP_LE : OP_LT
			, start, end);

	if ( s->Compare & TSEARCH_MATCH ) {
	  Seek(s->Indx, start, 0);
	  while (  (i = table_rowget(s->Indx, s->IHead, I, NULL, NULL, 0))
		&& scompare(s, OP_MATCH, value0
                           , table_colval(i, s->Tcolumn)) ) {

	    offset = atoffset(table_colval(i, s->Ioffcol));

	    Seek(s->TIdx, offset, 0);

	    r = table_rowget(s->TIdx, s->THead, R, NULL, NULL, 0);
	    data = (*func)(data, offset, offset + r->nbuf, r);
	    r = R;
	    if ( s->Output & TSEARCH_OUTONE ) break;
	  }
	} else {
	  end   = binsearch(s, s->Indx, s->Indxcol, value1 ? value1 : value0
			  , s->Compare & TSEARCH_LT ? OP_LT : OP_LE
		          , s->Istart, end);

	  Seek(s->Indx, start, 0);

	  while ( start < end ) {
	    int	indinc;

	    i      = table_rowget(s->Indx, s->IHead, I, NULL, NULL, 0);

	    indinc = i->nbuf;

	    offset = atoffset(table_colval(i, s->Ioffcol));

	    if ( s->Ilencol && !(s->Output & TSEARCH_OUTALL) ) {
	        int length = atoi(table_colval(i, s->Ilencol));

		data = (*func)(data, offset, offset + length, NULL);
	    } else {
		Seek(s->TIdx, offset-1, 0);
		if ( GetChar(s->TIdx) == '\n' ) {
		    r = table_rowget(s->TIdx, s->THead, R, NULL, NULL, 0);

		    data = (*func)(data, offset, offset + r->nbuf, r);
		    r = R;
		}
	    }

	    if ( s->Output & TSEARCH_OUTONE ) break;
	    start += indinc;
	  }
	}

	/* Supplimentary sequential search of unindexed data
	 */
	if ( s->Tend != s->Iindexd ) {
	    data = search_sequ(s, value0, value1, func, data, R, s->Iindexd);
	}

	if ( R == NULL ) table_rowfree(I);
	return data;
}


static void *search_biny(s, value0, value1, func, data, R)
	TableSearch	s;
	char		*value0;
	char		*value1;
	void	      *(*func)();
	void		*data;
	TableRow	 R;
{
		TableRow	r;
		off_t		start;
		off_t		end;

	start = binsearch(s, s->TIdx, s->Tcolumn, value0
			, s->Compare & TSEARCH_GT ? OP_LE : OP_LT
			, s->Tstart, s->Iindexd);

	end   = binsearch(s, s->TIdx, s->Tcolumn, value1 ? value1 : value0
			, s->Compare & TSEARCH_LT ? OP_LT : OP_LE
		        , s->Tstart, s->Iindexd);

	Seek(s->Tabl, start, 0);

	if ( s->Output & TSEARCH_OUTALL || s->Output & TSEARCH_OUTONE ) {
	    while ( start < end ) {
		r = table_rowget(s->Tabl, s->THead, R, NULL, NULL, 0);
		data = (*func)(data, start, end, r);
		if ( s->Output & TSEARCH_OUTONE ) break;
		start += r->nbuf;
	    }
	} else
	    data = (*func)(data, start, end, NULL);

	/* Supplimentary sequential search of unindexed data
	 */
	if ( s->Tend != s->Iindexd ) {
	    data = search_sequ(s, value0, value1, func, data, R, s->Iindexd);
	}

	return data;
}

void *table_srange(s, value0, value1, func, data, R)
	TableSearch	s;
	char		*value0;
	char		*value1;
	void	      *(*func)();
	void		*data;
	TableRow	 R;
{
	struct _TableRangeCacheNode rx;
	TableRangeCacheNode	    rr;

	rr = &rx;
#if 0
    rx.cache.hash   = table_hash(value0, s->rangecache->width);
    rx.value0 = value0;
    rx.value1 = value1;

    if ( rr = (TableRangeCacheNode) cache_hashe(s->rangecache, &rx, NULL) ) {
	return range_copy(rr->r);
    }

    rr = (TableRangeCacheNode) cache_cache(s->rangecache, &rx, NULL);
    if ( value0 ) rr->value0 = strdup(value0);
    if ( value1 ) rr->value1 = strdup(value1);

    cache_nfree(s->rangecache, rr);
#endif

    switch ( s->Method ) {
     case TSEARCH_SEQUENTIAL
			 : return /* range_copy(rr->r = */ search_sequ(s, value0, value1, func, data, R, (off_t) -1)/* ) */;
     case TSEARCH_BINARY : return /* range_copy(rr->r = */ search_biny(s, value0, value1, func, data, R)/* ) */;
     case TSEARCH_INDEX	 : return /* range_copy(rr->r = */ search_indx(s, value0, value1, func, data, R)/* ) */;
     case TSEARCH_ROW	 : return /* range_copy(rr->r = */ search_rows(s, value0, value1, func, data, R)/* ) */;
     case TSEARCH_HASH	 : return /* range_copy(rr->r = */ search_hash(s, value0, value1, func, data, R)/* ) */;
    }

    return rr->r = NULL;
}

void *table_svalue(s, value, func, data, R)
	TableSearch	s;
	char		*value;
	void	      *(*func)();
	void		*data;
	TableRow	 R;
{
	return table_srange(s, value, NULL, func, data, R);
}


TableRow seekrow(file, row, here, start, end, Offset, Length)
	File		 file;
	TableRow	 row;
	off_t		 here;
	off_t		 start;
	off_t		 end;
	off_t		*Offset;
	int		*Length;
{
	off_t		target;
	TableRow	r;


    *Offset = end + 1;
    *Length = 2;

    while ( *Offset >= end ) {
		int	ch;

	here -= *Length;
	if ( here < start ) here = start;

	target = here;
	Seek(file, here-1, 0);

	while ( (ch = GetChar(file)) != '\n' && ch != EOF ) {
		target++;
	}

	if ( ch == EOF ) abort();

	*Offset = target;

	if ( *Offset >= end ) {
		if ( here == start ) {
			return NULL;
		}
		*Length += 2;
		*Length *= 2;
		continue;
	}

	r = table_rowget(file, NULL, row, NULL, NULL, 0);
    }

    *Length = r->nbuf;
    return r;
}

static off_t binsearch(s, file, icol, value, op, start, end)
	TableSearch	 s;
	File		 file;
	int		 icol;
	char		*value;
	int		 op;
	off_t		 start;
	off_t		 end;
{
	off_t	Offset;
	int	Length;
	char	*colval;

	int	 level = 0;

	TableRow R = table_rowloc(NULL, 0, 0);
	TableRow row;
	BinRowCache rcache;

	double	 dvalue;
	int	 cut, div;

    if ( s->Numeric ) dvalue = SAOstrtod(value, NULL);

#if 1
    if ( ( start == s->Tstart && end == s->Iindexd )
      || ( start == s->Istart && end == s->Iend ) ) {

	cut = LADDR_SIZE / 2;
	div = LADDR_SIZE / 2;

	/* First level of cache.  A binary Ladder Array.
	 */
	while ( start < end && div > 1 ) {

	    s->ltry++;

	    off_t here = start + (end - start) /2;
	    if ( s->laddr[cut].offset == 0 ) {
		if ( icol ) {
		    if ( !(row 
			= seekrow(file, R, here, start, end, &Offset, &Length))){
    			table_rowfree(R);
			return Offset;
		    }
		
		    colval = table_colval(row, icol);
		} else {
			int	ch = 0;
			int	foundend = 0;

		  /* Find a row in the index between start and end that has not
		     been deleted in the table
		   */
		  while ( 1 ) { 
			off_t	Toff;

		    if ( !(row 
			= seekrow(file, R, here, start, end, &Offset, &Length)) ) {
    			table_rowfree(R);
			return start;
		    }

		    do {
			Toff = atoffset(table_colval(row, s->Ioffcol));

			Seek(s->TIdx, Toff-1, 0);
			ch = GetChar(s->TIdx);

			if ( ch == '\n' ) break;
			if ( Offset + row->nbuf >= end ) break;

		    } while ( row = table_rowget(file, NULL, R, NULL, NULL, 0) );

		    Length = row ? row->nbuf : 0;

		    if ( ch == '\n' ) break;

		    if ( Offset <= start ) {
				start = Offset + Length;
		    }
		    if ( Offset + Length >= end ) {
				end = Offset;
		    }

		    if ( start >= end ) {
    				table_rowfree(R);
				return start;
		    }
		  }

		  row = table_rowget(s->TIdx, NULL, R, NULL, NULL, 0);
		  colval = table_colval(row, s->Tcolumn);
		}

		if ( row == NULL ) abort();

		s->laddr[cut].offset  = Offset;
		s->laddr[cut].length  = Length;

		if ( s->Numeric ) s->laddr[cut].value.d = SAOstrtod(colval, NULL);
		else	          s->laddr[cut].value.s = strdup(colval);
	    } else {
		s->lhit++;
	    }

	    div /= 2;

	    if ( s->Numeric )
		if ( ncompare(op, s->Numeric, s->Reverse, s->laddr[cut].value.d, dvalue) ) {
			start = s->laddr[cut].offset + s->laddr[cut].length;
			cut += div;
		} else {
			end   = s->laddr[cut].offset;
			cut -= div;
		}
	    else
		if ( scompare(s, op, s->laddr[cut].value.s, value) ) {
			start = s->laddr[cut].offset + s->laddr[cut].length;
			cut += div;
		} else {
			end   = s->laddr[cut].offset;
			cut -= div;
		}

	    level++;
	}
    }
#endif

    /* Second level of cache. A file offset LRU cache hash with a max # of
       entries.
     */
    while ( start < end ) {
	off_t	here = start + (end - start) /2;
	struct _BinRowCache	rx;

	rx.cache.hash = abs((int)here);
	rx.star = start;
	rx.ends = end;

	/* FPrint(Stderr, "Hash : %d\n", here); */

	if ( (rcache = (BinRowCache) cache_hashe(s->cache, (CNode) &rx, NULL)) == NULL ) {
		if ( icol ) {
		    if ( !(row 
			= seekrow(file, R, here, start, end, &Offset, &Length))) {
    			table_rowfree(R);
			return Offset;
		    }
		    colval = table_colval(row, icol);
		} else {
			int	ch = 0;

		  while ( 1 ) { 
			off_t	Toff;

		    if ( !(row 
			= seekrow(file, R, here, start, end, &Offset, &Length))){
    			table_rowfree(R);
			return Offset;
		    }

		    do {
			Toff = atoffset(table_colval(row, s->Ioffcol));

			Seek(s->TIdx, Toff-1, 0);
			ch = GetChar(s->TIdx);

			if ( ch == '\n' ) break;
			if ( Offset + row->nbuf >= end ) break;

		    } while ( row = table_rowget(file, NULL, R, NULL, NULL, 0) );

		    Length = row ? row->nbuf : 0;

		    if ( ch == '\n' ) break;

			if ( Offset <= start ) {
				start = Offset + Length;
			}
			if ( Offset + Length >= end ) {
				end = Offset;
			}
			if ( start >= end ) {
    				table_rowfree(R);
				return start;
			}
		  }
		  row = table_rowget(s->TIdx, NULL, R, NULL, NULL, 0);
		  colval = table_colval(row, s->Tcolumn);
		}

	    if ( row == NULL ) abort();

	    rcache = (BinRowCache) cache_cache(s->cache, (CNode) &rx);

	    rcache->offset  = Offset;
	    rcache->length  = Length;
	    rcache->star    = start;
	    rcache->ends    = end;

	    if ( s->Numeric ) rcache->value.d = SAOstrtod(colval, NULL);
	    else	      rcache->value.s = strdup(colval);
	}

	if ( s->Numeric )
	    if ( ncompare(op, s->Numeric, s->Reverse, rcache->value.d, dvalue) )
		    start = rcache->offset + rcache->length;
	    else    end   = rcache->offset;
	else
	    if ( scompare(s, op, rcache->value.s, value) ) 
		    start = rcache->offset + rcache->length;
	    else    end   = rcache->offset;

        cache_nfree(s->cache, (CNode) rcache);
    } 

    table_rowfree(R);

    return start;
}


static int scompare(s, op, v1, v2) 
	TableSearch s;
	int	op;
	char	*v1;
	char	*v2;
{
		int	type = s->Numeric;
		int	rev  = s->Reverse;
		int	comp;

	if ( v1 == NULL || v2 == NULL ) {
	    FPrint(Stderr, "Incorrect column value specified: internal error?\n");
	    abort();
	}

	if ( type ) {
		double d1 = SAOstrtod(v1, NULL);
		double d2 = SAOstrtod(v2, NULL);


	    switch ( op ) {
	     case OP_LT:	comp = d1 <  d2; break;
	     case OP_LE:	comp = d1 <= d2; break;
	     case OP_GE:	comp = d1 >= d2; break;
	     case OP_GT:	comp = d1 >  d2; break;
	     case OP_EQ:	comp = d1 == d2; break;
	     default:		fprintf(stderr, "search : internal error\n");
				exit(1);
	    }
	} else {
		char	*c1 = (char *) v1;
		char	*c2 = (char *) v2;
		int 	 set1 = 0;
		int 	 set2 = 0;
		char	 chr1 = ' ';
		char	 chr2 = ' ';

	    int len1;
	    int len2;


	    while ( *c1 && *c1 == ' ' ) { c1++; }
	    len1 = strlen(c1);
	    if ( len1 ) set1 = c1[len1-1] == ' ';
	    if ( set1 ) { while ( len1 && c1[len1-1] == ' ' ) len1--; }

	    if ( s->modulus && len1 > s->modulus ) {
		set1 = 1;
		len1 = s->modulus;
		chr1 = c1[len1];
	    }
	    if ( set1 ) c1[len1] = '\0';


	    while ( *c2 && *c2 == ' ' ) { c2++; }
	    len2 = strlen(c2);
	    if ( len2 ) set2 = c2[len2-1] == ' ';
	    if ( set2 ) { while ( len2 && c2[len2-1] == ' ' ) len2--; }


	    if ( s->modulus && len2 > s->modulus ) {
		set2 = 1;
		len2 = s->modulus;
		chr2 = c2[len2];
	    } 
	    if ( set2 ) c2[len2] = '\0';

	    v1 = c1;
	    v2 = c2;

	    while ( *c1 && *c2 ) {
		if ( *c1 != *c2 ) break;
		c1++;  c2++;
	    }	
	    switch ( op ) {
	     case OP_LT:	comp = *c1 <  *c2; break;
	     case OP_LE:	comp = *c1 <= *c2; break;
	     case OP_GE:	comp = *c1 >= *c2; break;
	     case OP_GT:	comp = *c1 >  *c2; break;
	     case OP_EQ:	comp = *c1 == *c2; break;
	     case OP_MATCH:
		if ( !*c1 ) comp = 1;
		else  	    comp = 0;
	     default:       comp = 0;
	    }

	    if ( set1 ) v1[len1] = chr1;
	    if ( set2 ) v2[len2] = chr2;
	}

	if ( rev )  return !comp;
	else	    return  comp;
}


void *search_ind2(s, v1, v2, v3, v4, func, data, R)
        TableSearch       s; 
        char             *v1, *v2, *v3, *v4;
        void            *(*func)();
        void             *data;
        TableRow          R;
{                       
	return data;
}

void *search_bin2(s, v1, v2, v3, v4, func, data, R)
        TableSearch       s; 
        char             *v1, *v2, *v3, *v4;
        void            *(*func)();
        void             *data;
        TableRow          R;
{                       
		TableRow	r;
		off_t	star1, start;
		off_t	end1,  end;

	double modulus = s->modulus;
	double xstart = SAOstrtod(v1, NULL);
	double vstart = ((int) (xstart / s->modulus)) * modulus;
	double vend   = SAOstrtod(v2, NULL);

    if ( xstart < 0 ) vstart -= modulus;

    for ( ; vstart < vend; vstart += modulus ) {
		char vstr1[64];
		char vstr2[64];

	sprintf(vstr1, "%.15g", vstart);
	sprintf(vstr2, "%.15g", vstart + modulus);

	star1 = binsearch(s, s->Tabl, s->Tcolumn, vstr1
			, s->Compare & TSEARCH_GT ? OP_LE : OP_LT
			, s->Tstart, s->Iindexd);

	end1  = binsearch(s, s->Tabl, s->Tcolumn, vstr2
		        , s->Compare & TSEARCH_LT ? OP_LT : OP_LE
		        , s->Tstart, s->Iindexd);

	start = binsearch(s, s->Tabl, s->Tcolum2, v3
			, s->Compare & TSEARCH_GT ? OP_LE : OP_LT
			, star1, end1);

	end   = binsearch(s, s->Tabl, s->Tcolum2, v4 ? v4 : v3
		        , s->Compare & TSEARCH_LT ? OP_LT : OP_LE
		        , star1, end1);

	Seek(s->Tabl, start, 0);

	while ( start < end ) {

	    r = table_rowget(s->Tabl, s->THead, R, NULL, NULL, 0);

	    if ( v2 ?    scompare(s, s->Compare & TSEARCH_LT ? OP_LT : OP_LE
				    , v1
				    , table_colval(r, s->Tcolumn))
		      && scompare(s, s->Compare & TSEARCH_GT ? OP_GT : OP_GE
				    , v2
				    , table_colval(r, s->Tcolumn))
		       : scompare(s, OP_EQ, v1
					, table_colval(r, s->Tcolumn)) ) {

		data = func(data, start, start + r->nbuf, r);

		if ( s->Output & TSEARCH_OUTONE ) break;
	    }
	    start += r->nbuf;
	}
    }

    return data;
}

void *table_srange2(s, v1, v2, v3, v4, func, data, row)
	TableSearch	  s;
	char		 *v1, *v2, *v3, *v4;
	void		*(*func)();
	void		 *data;
	TableRow	  row;
{
	switch ( s->Method ) {
	 case TSEARCH_BINARY : return search_bin2(s, v1, v2, v3, v4, func, data, row);
	 case TSEARCH_INDEX  : return search_ind2(s, v1, v2, v3, v4, func, data, row);
	}

	return data;
}
	
