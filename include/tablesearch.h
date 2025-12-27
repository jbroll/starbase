/* table_search.h
 */

#include <xlist.h>
#include <range.h>
#include <cache.h>

#if _FILE_OFFSET_BITS == 64
#define atoffset(x) strtoll(x, NULL, 10)
#else
#define atoffset(x) atoi(x)
#endif


/* Search method search types
 */
#define TSEARCH_METHOD		0x000F
#define TSEARCH_INDEX		0x0001
#define TSEARCH_ROW		0x0002
#define TSEARCH_BINARY		0x0003
#define TSEARCH_HASH		0x0004
#define TSEARCH_SEQUENTIAL	0x0005
#define TSEARCH_MEMORY		0x0006

#define TSEARCH_NUMERIC		0x00F0
#define TSEARCH_ASCII		0x0010
#define TSEARCH_DECIMAL		0x0020
#define TSEARCH_SEXAGAS		0x0030
#define TSEARCH_NUMBER		0x0040

/* Search method compare types
 */
#define TSEARCH_COMPARE		0x0F00
#define TSEARCH_LT		0x0100
#define TSEARCH_GT		0x0200
#define TSEARCH_MATCH		0x0400

#define TSEARCH_LE		0x0	/* default */
#define TSEARCH_GE		0x0


#define TSEARCH_OUTONE		0x1000
#define TSEARCH_OUTALL		0x2000

#define TSEARCH_REVERSE		0x8000

#define TABLE_HASH_MOD	701

typedef struct _Laddr {
	union { 
		char   *s;
		double	d;
	} value;
	off_t	offset;
	int	length;
} *Laddr;

typedef struct _TableRangeCacheNode {
	struct _CNode	cache;
	char	*value0;
	char	*value1;
	Range	*r;
} *TableRangeCacheNode;

typedef struct _TableValCacheNode {
	struct _CNode	 cache;
	off_t		 end;
	char		*value;
} *TableValCacheNode;

typedef struct _TableFileRowCache {
	char	*filename;
	Cache	cache;
	int	n;
	Cache	seqcache;
	int	seqn;
} *TableFileRowCache;

typedef struct _TableSearch {
	char		*Table;
	File	 	 Tabl;
	File	 	 TIdx;
	TableHead	 THead;
	int		 Tcolumn;
	int		 Tcolum2;
	off_t	 	 Tstart;
	off_t	 	 Tend;

	char		*Index;
	File	 	 Indx;
	TableHead	 IHead;
	off_t	 	 Istart;
	off_t	 	 Iend;
	int	 	 Iwidth;
	int	 	 Irows;
	int	 	 Ioffcol;
	int	 	 Ilencol;
	int		 Indxcol;
	int		 Indxco2;
	off_t		 Iindexd;

	/* Search and Sort types
	 */
	int	 	 Method;
	int		 Numeric;
	int		 Compare;
	int	 	 Reverse;
	int		 Output;

	/* Index cache
	 */
	Cache		cache;
	Laddr		laddr;
	int		ltry;
	int		lhit;

	Cache		rangecache;
	Cache		valuecache;

	TableFileRowCache	filecache;

	/* Double index tables
	 */
	double		modulus;
} *TableSearch;


TableSearch table_searchsetup();


/* #table_search - set up to search a table using a fast access method

 */
TableSearch table_search	PROTOTYPE((
	  char *file		/* Name of a table to search.			*/
	, char *col		/* Name of the key column.			*/
	, int method		/* Suggested method.				*/
	, TableFileRowCache filecache
));

/* #table_srange - search a table for a range using a fast access method

 */
void *table_srange	PROTOTYPE((
	  TableSearch search	/* Search descriptor returned from @table_search */ 
	, char *value1		/* Lower limit of desired range. */ 
	, char *value2		/* Upper limit of desired range. */ 
	, void *(*func)()	/* Function to call with each selected row. */ 
	, void *data		/* Closure data for func() */
	, TableRow R		/* Row buffer.  
				   If a row buffer is not passed (NULL) a new
				   row will be allocated for each row selected */
));

/* #table_srange - search a table for a value using a fast access method

 */
void *table_svalue	PROTOTYPE((
	  TableSearch search	/* Search descriptor returned from @table_search */
	, char *value1 		/* Lower limit of desired range. */
	, void *(*func)() 	/* Function to call with each selected row. */
	, void *data		/* Closure data for func() */
	, TableRow R		/* Row buffer.
                                   If a row buffer is not passed (NULL) a new
                                   row will be allocated for each row selected */
));


void *table_rangerows	PROTOTYPE((
	File	 	 file,
	TableHead	 table,
	TableRow	 R,
	Range		*range,
	void    	*(*func)(),
	void		*data));

TableSearch table_searchsetup(char *table);
TableSearch table_searchsetupcache(TableSearch search, TableFileRowCache filecache);
void table_searchsetupindex(
    TableSearch search,
    char	*Index,
    char	*column
);

